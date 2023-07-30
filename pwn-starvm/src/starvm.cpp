#include<vector>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>
#include<iostream>
#include<malloc.h>



using namespace std;


/*
PUSH POP CALL   1 op
RET NOP  END    0 op

*/


typedef enum {
    VM_MOV,     VM_CMP,     VM_ADD,     VM_SUB,     
    VM_JMP,     VM_XOR,     VM_LOAD,    VM_UNLOAD,
    VM_AND,     VM_OR,      VM_MOVINT,
    VM_PUSH,
    VM_POP,     VM_CALL,    VM_RET,     VM_NOP,     VM_END
} vm_opcode_t;


typedef enum {
    BYTE,WORD
} vm_word_size;

#define read_sysreg(r) ({                    \
    unsigned long long __val;                        \
    asm volatile("mrs %0, " __stringify(r) : "=r" (__val));    \
    __val;                            \
})

typedef struct{
    char instr_code;
    int instr_num;
}instr;

unsigned long long val;

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


void dummy(){
    __asm__("xor %rdx, %rdx; ret");
    __asm__("pop %rax; ret");
    __asm__("syscall");
}


starvm* init_vm(){
    starvm* vm = (starvm*)malloc(sizeof(starvm));
    return vm;
}

bool init_code(starvm* vmp){
    fflush(stdout);
    puts("your command:");
    char tmp_code[0x1000];
    read(0,tmp_code,0x1000);
    char *p = strtok(tmp_code," ");
    // unsigned long long* backdoor = (unsigned long long*)malloc(0x8);
    char space = ' ';
    while(p != NULL){
        int cmd = atoi((const char*)p);
        if(cmd<VM_MOV || cmd > VM_END){
            printf("bad format\n");
            abort();
        }
        if(*p == space){
            p += 1;
            continue;
        }
        if(cmd == VM_END){
            return true;
        }
        instr* new_instr = new instr;
        new_instr->instr_code = cmd;
        if(new_instr->instr_code == VM_PUSH || new_instr->instr_code == VM_POP || new_instr->instr_code == VM_CALL){
            new_instr->instr_num  = 1;
        }
        else if(new_instr->instr_code == VM_RET || new_instr->instr_code == VM_END || new_instr->instr_code == VM_NOP){
            new_instr->instr_num  = 0;
        }
        else{
            new_instr->instr_num = 2;
        }
        vmp->codestack.push_back(new_instr);
        p = strtok(NULL," ");
        continue;
    }

    // __asm__("movq val, %rsp");
    // *backdoor = val;
    printf("%llu\n",val);
    return false;
}

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

void bad(){
    printf("learn reverse first\n");
    abort();
}

void execute(starvm* vmp){
    vmp->ip = vmp->codestack.begin();
    int data_cnt = 0;
    int op1 = 0;
    int op2 = 0;
    int res = 0;
    int meml=0;
    while(vmp->ip != vmp->codestack.end()){
        switch ((*(vmp->ip))->instr_code){
            case VM_MOV:
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op1>14 || op2>14){
                    bad();
                }
                vmp->reg[op1] = vmp->reg[op2];
                vmp->ip++;
                continue;
            case VM_CMP:
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op1>14 || op2>14){
                    bad();
                }
                if(vmp->reg[op1]>vmp->reg[op2]){
                    vmp->eflags = 1;
                }
                else if(vmp->reg[op1] == vmp->reg[op2]){
                    vmp->eflags = 0;
                }
                else{
                    vmp->eflags = 2;
                }
                vmp->ip++;
                continue;
            case VM_ADD:
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op1>14 || op2>14){
                    bad();
                }
                res = vmp->reg[op1]+vmp->reg[op2];
                vmp->reg[op1] = res;
                vmp->ip++;
                continue;
            case VM_SUB:
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op1>14 || op2>14){
                    bad();
                }
                res = vmp->reg[op1] - vmp->reg[op2];
                vmp->reg[op1] = res;
                vmp->ip++;
                continue;
            case VM_AND:
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op1>14 || op2>14){
                    bad();
                }
                res = vmp->reg[op1] & vmp->reg[op2];
                vmp->reg[op1] = res;
                vmp->ip++;
                continue;
            case VM_OR:
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op1>14 || op2>14){
                    bad();
                }
                res = vmp->reg[op1] & vmp->reg[op2];
                vmp->reg[op1] = res;
                vmp->ip++;
                continue;
            case VM_XOR:
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op1>14 || op2>14){
                    bad();
                }
                res = vmp->reg[op1] & vmp->reg[op2];
                vmp->reg[op1] = res;
                vmp->ip++;
                continue;
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

            case VM_PUSH:
                // push num
                op1 = vmp->datastack[data_cnt++];
                vmp->datastack.push_back(vmp->reg[op1]);
                vmp->ip++;
                continue;
            case VM_POP:
                // pop num
                op1 = vmp->datastack[data_cnt++];
                vmp->reg[op1] = vmp->datastack.back();
                vmp->datastack.pop_back();
                vmp->ip++;
                continue;

            case VM_MOVINT:
                // MOVINT reg, int
                op1 = vmp->datastack[data_cnt++];
                vmp->reg[op1] = vmp->datastack[data_cnt++];
                vmp->ip++;
                continue;

            case VM_JMP:
                // jmp step, minus
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                if(op2>0){
                    vmp->ip+=op1;
                }
                else{
                    vmp->ip-=op1;
                }
                continue;

            case VM_CALL:
                // call fp
                op1 = vmp->datastack[data_cnt++];
                op2 = vmp->datastack[data_cnt++];
                vmp->datastack.push_back(vmp->ip-vmp->codestack.begin());
                if(op2>0){
                    vmp->ip+=op1;
                }
                else{
                    vmp->ip-=op1;
                }
                continue;
            case VM_NOP:
                
            default:
                vmp->ip++;
                continue;

        }
    }
    return ;
}

__attribute__((constructor))
void setup(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  // alarm(180);
}


int main(){
    starvm* vmp = init_vm();
    printf("your vm starts at %llx\n", &vmp);
    // getchar();
    bool cond = init_code(vmp);
    if(cond != true){
        printf("learn reverse first\n");
        abort();
    }
    bool cond2 = init_data(vmp);
    if(cond2 != true){
        printf("learn reverse first\n");
        abort();
    }
    
    execute(vmp);
}