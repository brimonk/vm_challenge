#include "common.h"

typedef struct VM {
    i16 regs[8];
    void *memory;
    void *stack;
    size_t stack_size;
    i16 *ip;
} VM;

#define INSTRUCTION_HALT    0x00
#define INSTRUCTION_SET     0x01
#define INSTRUCTION_PUSH    0x02
#define INSTRUCTION_POP     0x03
#define INSTRUCTION_EQ      0x03
#define INSTRUCTION_GT      0x04
#define INSTRUCTION_JMP     0x05
#define INSTRUCTION_JT      0x06
#define INSTRUCTION_JF      0x07
#define INSTRUCTION_ADD     0x08
#define INSTRUCTION_MUL     0x09
#define INSTRUCTION_MOD     0x0a
#define INSTRUCTION_AND     0x0b
#define INSTRUCTION_OR      0x0c
#define INSTRUCTION_NOT     0x0d
#define INSTRUCTION_RMEM    0x0e
#define INSTRUCTION_WMEM    0x0f
#define INSTRUCTION_CALL    0x10
#define INSTRUCTION_RET     0x11
#define INSTRUCTION_OUT     0x12
#define INSTRUCTION_IN      0x13
#define INSTRUCTION_NOOP    0x14

#define USAGE "%s <instructions.bin>\n"

VM VMInitialize(void *instructions)
{
    VM vm = { 0 };
    vm.memory = instructions;
    vm.ip = vm.memory;
    return vm;
}

i16 GetVMInstruction(VM *vm)
{
    return *vm->ip;
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

    switch (GetVMInstruction(&vm)) {
        // case INSTRUCTION_HALT:
        // case INSTRUCTION_SET:
        // case INSTRUCTION_PUSH:
        // case INSTRUCTION_POP:
        // case INSTRUCTION_EQ:
        // case INSTRUCTION_GT:
        // case INSTRUCTION_JMP:
        // case INSTRUCTION_JT:
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
        // case INSTRUCTION_OUT:
        // case INSTRUCTION_IN:
        // case INSTRUCTION_NOOP:
        default: {
            fprintf(stderr, "ILLEGAL INSTRUCTION ENCOUNTERED! %hd\n", GetVMInstruction(&vm));
            exit(1);
        }
    }
}
