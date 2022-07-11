/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "../common/types.h"
#include "../common/defines.h"
#include "../common/i386.h"
#include "../common/utility.h"
#include "../memory/gdt.h"
#include "tss.h"

/**
 * Se encarga de realizar el cambio de tarea
 * a la tarea idle.
 */
extern void switch_to_idle();

/**
 * Inicializa las estructuras del scheduler
 */
void sched_init();

/**
 * Calcula la proxima tarea que debe ser ejecutada
 * (despues de la que se esta ejecutando actualmente)
 *
 * @returns el selector de TSS de la proxima tarea que tiene que ser
 * ejecutada segun la logica de scheduling
 */
tss_selector_t sched_next_task();

/**
 * Crea una nueva tarea lemming para el equipo indicado
 *
 * @returns el id de la tarea lemming creada
 */
task_id_t sched_init_task(lemming_team_e team);

/**
 * @returns el id de la tarea lemming que se esta ejecutando
 */
task_id_t get_running_lemming_task_id();

/**
 * Se encarga de desalojar del scheduler la tarea con el id indicado.
 */
void sched_kill_task(task_id_t task_id);

/**
 * Se encarga de desalojar del scheduler la tarea actual
 * y pasar a la tarea idle
 *
 * ! Todo el codigo luego de llamar a esta funcion no se va a ejecutar
 * ! porque hay un cambio de tareas y esta tarea no se va a volver a ejecutar
 */
void sched_kill_running_task();

/**
 * Se encarga de desalojar todas las tareas del scheduler
 * y pasar a la tarea idle
 */
void sched_kill_all();

#endif //  __SCHED_H__
