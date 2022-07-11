/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de estructuras para administrar tareas
*/

#ifndef __TSS_H__
#define __TSS_H__

#include "../common/types.h"
#include "../common/defines.h"
#include "../common/kassert.h"
#include "../memory/gdt.h"
#include "../memory/mmu.h"
#include "sched.h"

typedef struct str_tss {
  uint16_t ptl;
  uint16_t unused0;
  uint32_t esp0;
  uint16_t ss0;
  uint16_t unused1;
  uint32_t esp1;
  uint16_t ss1;
  uint16_t unused2;
  uint32_t esp2;
  uint16_t ss2;
  uint16_t unused3;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint16_t es;
  uint16_t unused4;
  uint16_t cs;
  uint16_t unused5;
  uint16_t ss;
  uint16_t unused6;
  uint16_t ds;
  uint16_t unused7;
  uint16_t fs;
  uint16_t unused8;
  uint16_t gs;
  uint16_t unused9;
  uint16_t ldt;
  uint16_t unused10;
  uint16_t dtrap;
  uint16_t iomap;
} __attribute__((__packed__, aligned(8))) tss_t;

/**
 * TSS de la tareas inicial
 */
extern tss_t tss_initial;

/**
 * TSS de la tarea idle
 */
extern tss_t tss_idle;

/**
 * Arreglo con las TSS de cada tarea Lemming, indexado por task_id/lemming_id
 *
 * Las primeras MAX_LEMMINGS_TEAM (5) posiciones corresponden a tareas lemming A,
 * las siguientes para tareas lemming B
 */
extern tss_t tss_lemmings[MAX_LEMMINGS];

/**
 * Inicializa las estructuras para manejar tareas
 */
void tss_init();

/**
 * Inicializa la TSS de la tarea idle
 */
void tss_init_idle();

/**
 * Inicializa la TSS de la tarea lemming correspondiente
 */
void tss_init_lemming_task(task_id_t lemming_id);

/**
 * Se encarga de mapear a la tarea lemming que esta corriendo
 * la pagina de memoria compartida que le corresponde a la
 * direccion virtual virt
 */
void tss_map_shared_page(vaddr_t virt);

#endif //  __TSS_H__
