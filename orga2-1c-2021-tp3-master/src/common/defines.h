/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__
/* MMU */
/* -------------------------------------------------------------------------- */

#define MMU_P 1 //   1b
#define MMU_W 1 //  10b
#define MMU_U 1 // 100b

#define PRESENT_WRITE_0_ATTRS 0x3 // 011b -> PRESENT, WRITE, user
#define PRESENT_READ_USER_ATTRS 0x5 // 101b -> PRESENT, READ, USER
#define PRESENT_WRITE_USER_ATTRS 0x7 // 111b -> PRESENT, WRITE, USER

#define KERNEL_FREE_AREA_INIT 0x100000
#define TASK_FREE_AREA_INIT 0x400000
#define PAGE_SIZE 4096

/* Misc */
/* -------------------------------------------------------------------------- */
// Y Filas
#define SIZE_N 40
#define MAP_ROWS   SIZE_N

// X Columnas
#define SIZE_M 80
#define MAP_COLS   SIZE_M

#define MAX_LEMMINGS 10
#define MAX_LEMMINGS_TEAM (MAX_LEMMINGS / 2)

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC 0
#define GDT_IDX_CODE0_DESC 8
#define GDT_IDX_DATA0_DESC 9
#define GDT_IDX_CODE3_DESC 10
#define GDT_IDX_DATA3_DESC 11
#define GDT_IDX_VIDEO_DESC 12
#define GDT_IDX_TSS_INIT_DESC 13
#define GDT_IDX_TSS_IDLE_DESC 14
#define GDT_IDX_TSS_FIRST_LEMMING_DESC 15
#define GDT_COUNT         35


/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_CODE0_SEGSEL (GDT_IDX_CODE0_DESC << 3)
#define GDT_DATA0_SEGSEL (GDT_IDX_DATA0_DESC << 3)
#define GDT_CODE3_SEGSEL ((GDT_IDX_CODE3_DESC << 3) | 0x3)
#define GDT_DATA3_SEGSEL ((GDT_IDX_DATA3_DESC << 3) | 0x3)
#define GDT_TSS_IDLE_SEGSEL (GDT_IDX_TSS_IDLE_DESC << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */

// direccion fisica de comienzo del bootsector (copiado)
#define BOOTSECTOR 0x00001000
// direccion fisica de comienzo del kernel
#define KERNEL 0x00001200
// direccion fisica del buffer de video
#define VIDEO 0x000B8000

#define NULL_ADDR (0x00000000)

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
// direccion virtual del codigo
#define TASK_CODE_VIRTUAL 0x08000000
#define TASK_CODE_PAGES   2
#define TASK_STACK_BASE   0x08003000
#define TASK_STACK0_BASE   0x08004000

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<X>_PHY_START.
 */
#define USER_TASK_SIZE (PAGE_SIZE * 2)

#define TASK_LEMMING_A_CODE_START (0x00018000)
#define TASK_LEMMING_B_CODE_START (0x0001A000)
#define TASK_IDLE_START           (0x0001C000)

#define TASK_SHARED_MEMORY_INIT (0x400000)
#define TASK_SHARED_MEMORY_END  (0x13FFFFF)

 /* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
 /* -------------------------------------------------------------------------- */
#define KERNEL_PAGE_DIR     (0x00025000)
#define KERNEL_PAGE_TABLE_0 (0x00026000)
#define KERNEL_STACK        (0x00025000)

// flags con interrupciones activadas
#define TSS_DEFAULT_EFLAGS 0x202

#define DEBUG_MODE_KEY_SCAN_CODE 0x15

#endif //  __DEFINES_H__
