#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/// The memory size.
#define MEMORY_SIZE 32

/// Represents the kind of the opcode.
typedef enum {
  OpLoadi,   // Loadi rx l1
  OpAddi,    // Addi rx ra l1
  OpCompare, // Compare rx ra rb
  OpJump,    // Jump l1
  OpBranch,  // Branch ra l1
  OpExit     // Exit
} opcode_kind_t;

typedef uint8_t reg_t;
typedef int64_t lit_t;

typedef struct {
  reg_t rx;
  lit_t l1;
} loadi_t;

typedef struct {
  reg_t rx;
  reg_t ra;
  lit_t l1;
} addi_t;

typedef struct {
  reg_t rx;
  reg_t ra;
  reg_t rb;
} compare_t;

typedef struct {
  lit_t l1;
} jump_t;

typedef struct {
  reg_t ra;
  lit_t l1;
} branch_t;

/// Represents an opcode with 3 operands.
typedef struct {
  opcode_kind_t kind;
  void *operation;
} opcode_t;

/// Entry point.
int main() {
  // Program memory (registers)
  int memory[MEMORY_SIZE] = {0};

  const loadi_t first_load = {.rx = 0, .l1 = 100000000};
  const loadi_t second_load = {.rx = 1, .l1 = 0};
  const compare_t compare = {.rx = 2, .ra = 0, .rb = 1};
  const branch_t branch = {.ra = 2, .l1 = 2};
  const addi_t add = {.rx = 1, .ra = 1, .l1 = 1};
  const jump_t jump = {.l1 = -4};

  // Program code to execute.
  const opcode_t code[] = {
      {.kind = OpLoadi, .operation = (void *)&first_load},  // r0 = 100000000;
      {.kind = OpLoadi, .operation = (void *)&second_load}, // r1 = 0;
      {.kind = OpCompare, .operation = (void *)&compare},   // r2 = r0 == r1;
      {.kind = OpBranch, .operation = (void *)&branch}, // if (r2 == 0) goto +2;
      {.kind = OpAddi, .operation = (void *)&add},      // r1 = r1 + 1;
      {.kind = OpJump, .operation = (void *)&jump},     // goto -4;
      {.kind = OpExit}};
  // Program Counter.
  size_t pc = 0;
  // The VM itself.
  for (;;) {
    const opcode_t *op = &code[pc];

    switch (op->kind) {
    case OpLoadi: {
      const loadi_t *load = op->operation;
      memory[load->rx] = load->l1;
      break;
    }
    case OpAddi: {
      const addi_t *add = op->operation;
      memory[add->rx] = memory[add->ra] + add->l1;
      break;
    }
    case OpCompare: {
      const compare_t *compare = op->operation;
      memory[compare->rx] = memory[compare->ra] == memory[compare->rb];
      break;
    }
    case OpJump: {
      const jump_t *jump = op->operation;
      pc += jump->l1;
      break;
    }
    case OpBranch: {
      const branch_t *branch = op->operation;
      if (memory[branch->ra] != 0)
        pc += branch->l1;
      break;
    }
    case OpExit: {
      goto end;
    }
    }
    pc++;
  }
end:
  return EXIT_SUCCESS;
}
