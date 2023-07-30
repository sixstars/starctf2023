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

Since there are no output function, we have to calculate and store the address all inside the starvm. There are four steps in my exploit

1. load  heapaddr to register

2. vmload to save a copy of addr, and calculate stack address

3. rewrite vm->mem to tcache, control the tcache control block

4. control block in ret address and start ROP

The exploit is in `solve.py`

## feedback

Actually this binary in compiled with `partial relro`. So it's also possible to hijack `delete` GOT to get a shell. However, this method may not work well when attacking the remote, it needs some small adjustment. Thanks for `emmm`from `Spirit`.



