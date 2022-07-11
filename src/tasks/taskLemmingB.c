#include "../syscall.h"

#define CHECK_EQ(A, B)                 \
  do {                                 \
    if ((A) != (B)) {                  \
      __asm volatile("xchg %bx, %bx"); \
    }                                  \
  }                                    \
  while (0)

// Hacer que el Lemming produzca una excepcion.
#define SEPPUKKU(...) do { __asm volatile("ud2"); } while(0)

void task(void) {
  while (true)
    syscall_move(DIR_RIGHT);
}

