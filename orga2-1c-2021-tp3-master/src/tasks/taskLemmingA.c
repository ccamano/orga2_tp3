#include "../syscall.h"
#include "../common/i386.h"

static direction_e opposite_dir(direction_e dir);

// Chequear que el resultado de una operacion sea esperado.
// En caso de que falle, se generara un breakpoint.
#define CHECK_EQ(A, B)                 \
  do {                                 \
    if ((A) != (B)) {                  \
      __asm volatile("xchg %bx, %bx"); \
    }                                  \
  }                                    \
  while (0)

// Hacer que el Lemming produzca una excepcion.
#define SEPPUKKU(...) do { __builtin_trap(); } while(0)

void task(void) {
  // Walk up to the start of the labirinth
  CHECK_EQ(syscall_move(DIR_UP), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_UP), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_UP), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);
  CHECK_EQ(syscall_move(DIR_RIGHT), MOVE_RESULT_SUCCESS);


  direction_e dirs[] = {DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT};
  direction_e last_dir = DIR_DOWN; // last successfull direction.
  size_t i = 1;
  while (true) {
    direction_e next_dir;

    do {
      i = (i + 1) % 4;
      next_dir = dirs[i];
    } while (next_dir == opposite_dir(last_dir)); // never look back.

    while (syscall_move(next_dir) == MOVE_RESULT_SUCCESS) {
      last_dir = next_dir;
    };

    // Si llegamos hasta aca y nos pudimos mover ok
    // last_dir == next_dir.
    // Si fallo el primer movimiento, last_dir esta igual que antes.
  }
}

static direction_e opposite_dir(direction_e dir) {
  switch (dir) {
    case DIR_UP:
      return DIR_DOWN;
    case DIR_DOWN:
      return DIR_UP;
    case DIR_LEFT:
      return DIR_RIGHT;
    case DIR_RIGHT:
      return DIR_LEFT;
    default:
      __asm volatile("xchg %bx, %bx");
  }
  SEPPUKKU();
}
