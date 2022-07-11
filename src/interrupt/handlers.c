/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Implementación de interrupciones especiales.
*/

#include "isr.h"

void handle_exception(uint8_t type, exception_stack_t exception_stack) {

  if (game_in_debug_mode()) {
    print_exception(from_exception_stack(type, exception_stack, get_running_lemming_task_id()));
  }

  game_inform_exception(false);
}

void handle_pagefault(exception_stack_t exception_stack) {

  if (game_in_debug_mode()) {
    print_exception(from_exception_stack(14, exception_stack, get_running_lemming_task_id()));
  }

  game_inform_exception(true);
  vaddr_t cr2 = rcr2(); //direccion virtual del page fault
  // verificamos que la direccion es valida
  if (cr2 < TASK_SHARED_MEMORY_INIT || cr2 > TASK_SHARED_MEMORY_END) {
    // la direccion que la tarea esta intentando acceder es invalida
    game_invalid_shared_memory_access();
  }
  // la direccion que esta intentando acceder es valida

  tss_map_shared_page(cr2);
}

void handle_keyboard_int(uint8_t scan_code) {
  if (scan_code == DEBUG_MODE_KEY_SCAN_CODE) {
    game_toggle_debug_mode();
  }

  // ejercicio 3c
  // https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
  // 1: 0x02, 2: 0x03, ..., 0: 0x0b
  if (scan_code >= 2 && scan_code < 12) {
    // es un numero
    int digit;
    if (scan_code == 0x0b) {
      digit = 0;
    } else {
      digit = scan_code - 1;
    }
    print_dec(digit, 1, 79, 0, C_BG_BLACK | C_FG_RED);
  }
}
