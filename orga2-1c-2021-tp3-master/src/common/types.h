/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de tipos comunes en el kernel.
*/

#ifndef __TYPES_H__
#define __TYPES_H__

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

typedef uint32_t vaddr_t; // direccion virtual.
typedef uint32_t paddr_t; // direccion fisica.

typedef uint8_t task_id_t; // tipo task_id

typedef uint16_t tss_selector_t; // tipo selector de tss

/**
 * Estructura para indicar la posicion en el mapa del juego
 */
typedef struct position {
  uint32_t col;
  uint32_t row;
} position_t;

/**
 * Estructura para leer el stack luego de caer en una excepcion
 */
typedef struct exception_stack {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp0;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t stack1;
  uint32_t stack2;
  uint32_t stack3;
  uint32_t stack4;
  uint32_t stack5;
  uint32_t stack6;
  uint32_t stack7;
} __attribute__((__packed__, aligned(8))) exception_stack_t;

/**
 * Estructura con toda la informacion para el modo debug
 */
typedef struct exception_debug_info {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t cs;
  uint32_t ds;
  uint32_t es;
  uint32_t fs;
  uint32_t gs;
  uint32_t ss;
  uint32_t cr0;
  uint32_t cr2;
  uint32_t cr3;
  uint32_t cr4;
  uint32_t flags;
  uint32_t eip;
  uint32_t* stack;
  // max 3 valores en el stack
  uint8_t stack_size;
  uint32_t err_code;
  bool with_err_code;
  uint8_t type;
  task_id_t task_id;
  char* label;
} __attribute__((__packed__, aligned(8))) exception_debug_info_t;


typedef enum direction_e {
  DIR_UP = 0,
  DIR_RIGHT = 1,
  DIR_DOWN = 2,
  DIR_LEFT = 3
} direction_e;

typedef enum move_result_e {
  MOVE_RESULT_SUCCESS = 0,
  MOVE_RESULT_WALL = 1,
  MOVE_RESULT_LAKE = 2,
  MOVE_RESULT_BORDER = 3,
  MOVE_RESULT_LEMMING = 4
} move_result_e;

/**
 * Equipos de los lemmings
 */
typedef enum lemming_team_e {
  LEMMING_A = 0,
  LEMMING_B = 1
} lemming_team_e;

#endif //  __TYPES_H__
