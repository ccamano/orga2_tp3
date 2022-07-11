/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas de atencion de interrupciones
*/

#ifndef __ISR_H__
#define __ISR_H__

#include "../common/i386.h"
#include "../memory/mmu.h"
#include "../tasks/tss.h"
#include "../screen.h"
#include "../game.h"

void _isr0();
void _isr1();
void _isr2();
void _isr3();
void _isr4();
void _isr5();
void _isr6();
void _isr7();
void _isr8();
void _isr9();
void _isr10();
void _isr11();
void _isr12();
void _isr13();
void _isr14();
void _isr15();
void _isr16();
void _isr17();
void _isr18();
void _isr19();

void _isr32();
void _isr33();

void _isr88();
void _isr98();
void _isr108();

/**
 * Logica de la rutina de atencion de teclado relacionada al sistema
 */
void handle_keyboard_int(uint8_t scan_code);

/**
 * Logica de manejo de la excepcion Pagefault (14)
 */
void handle_pagefault(exception_stack_t exception_stack);

/**
 * Logica de manejo de excepciones
 */
void handle_exception(uint8_t type, exception_stack_t excepction_stack);


#endif //  __ISR_H__
