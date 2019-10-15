#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/// The memory size.
#define MEMORY_SIZE 32

/// Represents the kind of the opcode.
typedef enum {
    Loadi, // Loadi rx l1
    Addi, // Addi rx ra l1
    Compare, // Compare rx ra rb
    Jump, // Jump l1
    Branch, // Branch ra l1
    Exit // Exit
} opcode_kind_t;

/// Represents an opcode with 3 operands.
typedef struct {
    opcode_kind_t kind;
    int64_t op1;
    int64_t op2;
    int64_t op3;
} opcode_t;

/// Entry point.
int main() {
    // Program memory (registers)
    int memory[MEMORY_SIZE] = { 0 };
    // Program code to execute.
    const opcode_t code[] = {
        { .kind = Loadi, .op1 = 0, .op2 = 20 }, // r0 = 20;
        { .kind = Loadi, .op1 = 1, .op2 = 0 }, // r1 = 0;
        { .kind = Compare, .op1 = 2, .op2 = 0, .op3 = 1 }, // r2 = r0 == r1;
        { .kind = Branch, .op1 = 2, .op2 = 2 }, // if (r2 == 0) goto +2;
        { .kind = Addi, .op1 = 1, .op2 = 1, .op3 = 1 }, // r0 = r0 + 1;
        { .kind = Jump, .op1 = -4 }, // goto -4;
        { .kind = Exit }
    };
    // Program Counter.
    size_t pc = 0;
    // The VM itself.
    for (;;) {
        const opcode_t op = code[pc];
        printf("(%u) Kind = %u\n\tr0=%d\tr1=%d\tr2=%d\n", pc, op.kind, memory[0], memory[1], memory[2]);
        switch (op.kind) {
            case Loadi: { memory[op.op1] = op.op2; break; }
            case Addi: { memory[op.op1] = memory[op.op2] + op.op3; break; }
            case Compare: { memory[op.op1] = memory[op.op2] == memory[op.op3]; break; }
            case Jump: { pc += op.op1; break; }
            case Branch: { if (memory[op.op1] != 0) pc += op.op1; break; }
            case Exit: { goto end; }
        }
        pc++;
    }
    end: printf("Finished!");
    return EXIT_SUCCESS;
}
