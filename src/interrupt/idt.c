/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"

idt_entry_t idt[255] = {0};

idt_descriptor_t IDT_DESC = {sizeof(idt) - 1, (uint32_t)&idt};

#define IDT_ENTRY(numero)                                                                     \
  idt[numero].offset_15_0 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t)0xFFFF);      \
  idt[numero].segsel = (uint16_t)GDT_CODE0_SEGSEL;                                          \
  idt[numero].attr = (uint16_t)0x8E00;                                                        \
  idt[numero].offset_31_16 = (uint16_t)((uint32_t)(&_isr ## numero) >> 16 & (uint32_t)0xFFFF);

#define IDT_ENTRY_DPL_3(numero)                                                               \
  idt[numero].offset_15_0 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t)0xFFFF);      \
  idt[numero].segsel = (uint16_t)GDT_CODE0_SEGSEL;                                          \
  idt[numero].attr = (uint16_t)0xEE00;                                                        \
  idt[numero].offset_31_16 = (uint16_t)((uint32_t)(&_isr ## numero) >> 16 & (uint32_t)0xFFFF);
// attrs : P = 1 | DPL = 11 | 01110 | 000 | 00000
//                            interrupt gate de nivel 3

void idt_init() {
  // Excepciones
  IDT_ENTRY(0);
  IDT_ENTRY(1);
  IDT_ENTRY(2);
  IDT_ENTRY(3);
  IDT_ENTRY(4);
  IDT_ENTRY(5);
  IDT_ENTRY(6);
  IDT_ENTRY(7);
  IDT_ENTRY(8);
  IDT_ENTRY(9);
  IDT_ENTRY(10);
  IDT_ENTRY(11);
  IDT_ENTRY(12);
  IDT_ENTRY(13);
  IDT_ENTRY(14);
  IDT_ENTRY(15);
  IDT_ENTRY(16);
  IDT_ENTRY(17);
  IDT_ENTRY(18);
  IDT_ENTRY(19);

  // Rutina de atencion de reloj
  IDT_ENTRY(32);
  // Rutina de atencion de teclado
  IDT_ENTRY(33);

  // =====================================
  // Servicios del sistema
  // =====================================
  // syscall_move
  IDT_ENTRY_DPL_3(88);
  // syscall_explode
  IDT_ENTRY_DPL_3(98);
  // syscall_bridge
  IDT_ENTRY_DPL_3(108);
}

