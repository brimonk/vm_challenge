#include "common.h"

typedef struct VM {
    i16 regs[8];
    i16 *memory;
    i16 *stack;
    i16 *ip;
    i32 run;
} VM;

#define INSTRUCTION_HALT    0x00
#define INSTRUCTION_SET     0x01
#define INSTRUCTION_PUSH    0x02
#define INSTRUCTION_POP     0x03
#define INSTRUCTION_EQ      0x04
#define INSTRUCTION_GT      0x05
#define INSTRUCTION_JMP     0x06
#define INSTRUCTION_JT      0x07
#define INSTRUCTION_JF      0x08
#define INSTRUCTION_ADD     0x09
#define INSTRUCTION_MULT    0x0a
#define INSTRUCTION_MOD     0x0b
#define INSTRUCTION_AND     0x0c
#define INSTRUCTION_OR      0x0d
#define INSTRUCTION_NOT     0x0e
#define INSTRUCTION_RMEM    0x0f
#define INSTRUCTION_WMEM    0x10
#define INSTRUCTION_CALL    0x11
#define INSTRUCTION_RET     0x12
#define INSTRUCTION_OUT     0x13
#define INSTRUCTION_IN      0x14
#define INSTRUCTION_NOOP    0x15

#define USAGE    "%s <instructions.bin>\n"
#define ILLEGAL  "ILLEGAL %s ENCOUNTERED! %hd\n"

VM VMInitialize(void *instructions)
{
    VM vm = { 0 };
    vm.memory = instructions;
    vm.stack = NULL;
    vm.ip = vm.memory;
    vm.run = true;
    return vm;
}

i16 GetVMInstruction(VM *vm)
{
    return *vm->ip;
}

i16 GetVMValue(VM *vm, i16 v)
{
    if (0 <= v && v <= 32767) {
        return v;
    } else if (32768 <= v && v <= 32775) {
        return vm->regs[v - 32768];
    } else {
        fprintf(stderr, ILLEGAL, "VALUE", v);
        exit(1);
        return -1;
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, USAGE, argv[0]);
        exit(1);
    }

    // load the file first

    FILE *fp = fopen(argv[1], "rb");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    autofree void *buf = calloc(1, size);

    fread(buf, 1, size, fp);
    fclose(fp);

    // run the challenge.bin

    VM vm = VMInitialize(buf);

    for (; vm.run; vm.ip++) {
    EDDIE_VAN_HALEN:
        switch (GetVMInstruction(&vm)) {
            case INSTRUCTION_HALT: {
                vm.run = false;
                break;
            }

            // case INSTRUCTION_SET:
            // case INSTRUCTION_PUSH:
            // case INSTRUCTION_POP:
            // case INSTRUCTION_EQ:
            // case INSTRUCTION_GT:

            case INSTRUCTION_JMP: {
                vm.ip++;
                vm.ip = GetVMValue(&vm, *vm.ip) + vm.memory;
                goto EDDIE_VAN_HALEN;
                break;
            }

            case INSTRUCTION_JT: {
                i16 a = GetVMValue(&vm, *++vm.ip);
                i16 b = GetVMValue(&vm, *++vm.ip);

                if (a != 0) {
                    vm.ip = b + vm.memory;
                    goto EDDIE_VAN_HALEN;
                }

                break;
            }

            // case INSTRUCTION_JF:
            // case INSTRUCTION_ADD:
            // case INSTRUCTION_MUL:
            // case INSTRUCTION_MOD:
            // case INSTRUCTION_AND:
            // case INSTRUCTION_OR:
            // case INSTRUCTION_NOT:
            // case INSTRUCTION_RMEM:
            // case INSTRUCTION_WMEM:
            // case INSTRUCTION_CALL:
            // case INSTRUCTION_RET:

            case INSTRUCTION_OUT: {
                vm.ip++;
                putchar(GetVMValue(&vm, *vm.ip));
                break;
            }

            // case INSTRUCTION_IN: {
            //     break;
            // }

            case INSTRUCTION_NOOP: {
                printf("NOOP\n");
                break;
            }

            default: {
                fprintf(stderr, "ILLEGAL INSTRUCTION ENCOUNTERED! %hd\n", GetVMInstruction(&vm));
                exit(1);
            }
        }
    }
}
