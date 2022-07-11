/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas de atencion de interrupciones
*/

#ifndef __IDT_H__
#define __IDT_H__

#include "../common/types.h"
#include "../common/defines.h"
#include "../common/i386.h"
#include "../screen.h"
#include "isr.h"

/* Struct de descriptor de IDT */
typedef struct str_idt_descriptor {
  uint16_t idt_length;
  uint32_t idt_addr;
} __attribute__((__packed__)) idt_descriptor_t;

/* Struct de una entrada de la IDT */
typedef struct str_idt_entry_fld {
  uint16_t offset_15_0;
  uint16_t segsel;
  uint16_t attr;
  uint16_t offset_31_16;
} __attribute__((__packed__, aligned(8))) idt_entry_t;

/**
 * Arreglo con todas las entradas de la IDT
 */
extern idt_entry_t idt[];

/**
 * Descriptor de la IDT
 */
extern idt_descriptor_t IDT_DESC;

/**
 * Carga en el arreglo idt[] las entradas correspondientes
 * para manejar las interrupciones (excepciones, reloj/teclado y syscalls)
 */
void idt_init();

#endif //  __IDT_H__
