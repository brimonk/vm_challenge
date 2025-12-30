#include "common.h"

#include <unistd.h>

/*
 * If you find yourself trapped in an alternate dimension with nothing but a
 * hand-held teleporter, you will need to extract the confirmation algorithm,
 * reimplement it on more powerful hardware, and optimize it.  This should, at the
 * very least, allow you to determine the value of the eighth register which would
 * have been accepted by the teleporter's confirmation mechanism.
 *
 * Then, set the eighth register to this value, activate the teleporter, and
 * bypass the confirmation mechanism.  If the eighth register is set correctly, no
 * anomalies should be experienced, but beware - if it is set incorrectly, the
 * now-bypassed confirmation mechanism will not protect you!
 */

typedef struct VM {
    u16 regs[8];
    u16 *memory;
    size_t memory_size;
    u16 *stack;
    u16 *ip;
    i32 run;
    i32 disassemble;
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

#define REGISTER_BASE    32768
#define IS_REGISTER(r)   (32768 <= (r) && (r) <= 32775)

char *INSTRUCTION_STRINGS[] = {
    "HLT",
    "SET",
    "PUSH",
    "POP",
    "EQ",
    "GT",
    "JMP",
    "JT",
    "JF",
    "ADD",
    "MULT",
    "MOD",
    "AND",
    "OR",
    "NOT",
    "RMEM",
    "WMEM",
    "CALL",
    "RET",
    "OUT",
    "IN",
    "NOOP"
};

i32 INSTRUCTION_ARGS[] = {
    0, 2, 1, 1, 3, 3, 1, 2, 2, 3, 3, 3, 3, 3, 2, 2, 2, 1, 0, 1, 1, 0
};

VM VMInitialize(void *instructions, size_t instructions_size)
{
    VM vm = { 0 };
    vm.memory = instructions;
    vm.memory_size = instructions_size;
    vm.stack = NULL;
    vm.ip = vm.memory;
    vm.run = true;
    return vm;
}

u16 GetVMInstruction(VM *vm)
{
    return *vm->ip;
}

u16 GetVMValue(VM *vm, u16 v)
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

// this function purposefully named wrong - it just gets the destination
u16 *GetVMRegister(VM *vm, u16 r)
{
    return IS_REGISTER(r) ? &vm->regs[r - 32768] : &vm->memory[r];
}

u16 *GetVMMemory(VM *vm, u16 mem)
{
    return &vm->memory[IS_REGISTER(mem) ? vm->regs[mem - 32768] : mem];
}

void DumpMemory(VM *vm, char *path, i32 start)
{
    FILE *fp = fopen(path, "wa");
    u16 *ip = vm->memory;
    for (i32 i = start; i < vm->memory_size; i += INSTRUCTION_ARGS[*ip] + 1) {
        ip = vm->memory + i;
        if (!(INSTRUCTION_HALT <= *ip && *ip <= INSTRUCTION_NOOP))
            break;
        fprintf(fp, "%06d %s", i, INSTRUCTION_STRINGS[*ip]);
        for (i32 j = 1; j <= INSTRUCTION_ARGS[*ip]; j++) {
            fprintf(fp, "%.*s%s%hu", 2, " ", IS_REGISTER(*(ip + j)) ? "R" : "", *(ip + j) % REGISTER_BASE);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    assert(ARRSIZE(INSTRUCTION_ARGS) == ARRSIZE(INSTRUCTION_STRINGS));

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

    VM vm = VMInitialize(buf, size);

    vm.disassemble = true;

    unlink("disassembly.asm");

    DumpMemory(&vm, "disassembly.asm", 6049);
    DumpMemory(&vm, "disassembly.asm", 5476);

#define DISASSEMBLE(args)  if (vm.disassemble) { \
    fprintf(stderr, "> %06ld %s", vm.ip - vm.memory, INSTRUCTION_STRINGS[GetVMInstruction(&vm)]); \
    for (int disassemble_args_idx__ = 1; disassemble_args_idx__ <= args; disassemble_args_idx__++) \
        fprintf(stderr, "%.*s%s%hu", 2, " ", IS_REGISTER(*(vm.ip + disassemble_args_idx__)) ? "R" : "", *(vm.ip + disassemble_args_idx__) % REGISTER_BASE); \
    fprintf(stderr, "\n"); \
    } else {}

    for (; vm.run; vm.ip++) {
    EDDIE_VAN_HALEN:
        switch (GetVMInstruction(&vm)) {
            NOT_FREDDIE_MERCURY:
            case INSTRUCTION_HALT: {
                DISASSEMBLE(0);
                vm.run = false;
                break;
            }

            case INSTRUCTION_SET: {
                DISASSEMBLE(2);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                *a = b;
                break;
            }

            case INSTRUCTION_PUSH: {
                DISASSEMBLE(1);
                u16 v = GetVMValue(&vm, *++vm.ip);
                arrput(vm.stack, v);
                break;
            }

            case INSTRUCTION_POP: {
                DISASSEMBLE(1);
                assert(arrlen(vm.stack) > 0);
                u16 v = arrpop(vm.stack);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                *a = v;
                break;
            }

            case INSTRUCTION_EQ: {
                DISASSEMBLE(3);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                u16 c = GetVMValue(&vm, *++vm.ip);

                *a = b == c;

                break;
            }

            case INSTRUCTION_GT: {
                DISASSEMBLE(3);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                u16 c = GetVMValue(&vm, *++vm.ip);

                *a = b > c;

                break;
            }

            case INSTRUCTION_JMP: {
                DISASSEMBLE(1);
                vm.ip++;
                vm.ip = GetVMValue(&vm, *vm.ip) + vm.memory;
                goto EDDIE_VAN_HALEN;
                break;
            }

            case INSTRUCTION_JT: {
                DISASSEMBLE(2);
                u16 a = GetVMValue(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);

                if (a != 0) {
                    vm.ip = b + vm.memory;
                    goto EDDIE_VAN_HALEN;
                }

                break;
            }

            case INSTRUCTION_JF: {
                DISASSEMBLE(2);
                u16 a = GetVMValue(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);

                if (a == 0) {
                    vm.ip = b + vm.memory;
                    goto EDDIE_VAN_HALEN;
                }

                break;
            }

            case INSTRUCTION_ADD: {
                DISASSEMBLE(3);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                u16 c = GetVMValue(&vm, *++vm.ip);

                *a = b + c;
                *a %= REGISTER_BASE;

                break;
            }

            case INSTRUCTION_MULT: {
                DISASSEMBLE(3);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                u16 c = GetVMValue(&vm, *++vm.ip);

                // sarah made me scared... :(
                // *a = (u32)b * (u32)c;
                // *a %= REGISTER_BASE;
                *a = ((u32)b * (u32)c) % REGISTER_BASE;

                break;
            }

            case INSTRUCTION_MOD: {
                DISASSEMBLE(3);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                u16 c = GetVMValue(&vm, *++vm.ip);

                *a = b % c;

                break;
            }

            case INSTRUCTION_AND: {
                DISASSEMBLE(3);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                u16 c = GetVMValue(&vm, *++vm.ip);

                *a = b & c;

                break;
            }

            case INSTRUCTION_OR: {
                DISASSEMBLE(3);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                u16 c = GetVMValue(&vm, *++vm.ip);

                *a = b | c;

                break;
            }

            case INSTRUCTION_NOT: {
                DISASSEMBLE(2);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);

                *a = (~0x8000) & (~b);

                break;
            }

            case INSTRUCTION_RMEM: {
                DISASSEMBLE(2);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                u16 *b = GetVMMemory(&vm, *++vm.ip);
                *a = *b;
                break;
            }

            case INSTRUCTION_WMEM: {
                DISASSEMBLE(2);
                u16 *a = GetVMMemory(&vm, *++vm.ip);
                u16 b = GetVMValue(&vm, *++vm.ip);
                *a = b;
                break;
            }

            case INSTRUCTION_CALL: {
                DISASSEMBLE(1);
                u16 a = GetVMValue(&vm, *++vm.ip);
                u16 next = vm.ip + 1 - vm.memory;
                arrput(vm.stack, next);
                vm.ip = a + vm.memory;
                goto EDDIE_VAN_HALEN;
                break;
            }

            case INSTRUCTION_RET: {
                DISASSEMBLE(0);
                if (arrlen(vm.stack) == 0) {
                    goto NOT_FREDDIE_MERCURY;
                }
                u16 popped = arrpop(vm.stack);
                vm.ip = popped + vm.memory;
                goto EDDIE_VAN_HALEN;
                break;
            }

            case INSTRUCTION_OUT: {
                DISASSEMBLE(1);
                vm.ip++;
                putchar(GetVMValue(&vm, *vm.ip));
                break;
            }

            case INSTRUCTION_IN: {
                DISASSEMBLE(1);
                u16 *a = GetVMRegister(&vm, *++vm.ip);
                int c = getchar() & 0xff;
                *a = (u16)c;
                break;
            }

            case INSTRUCTION_NOOP: {
                DISASSEMBLE(0); // ????
                break;
            }

            default: {
                fprintf(stderr, "ILLEGAL INSTRUCTION ENCOUNTERED! %hd\n", GetVMInstruction(&vm));
                exit(1);
            }
        }
    }
}
