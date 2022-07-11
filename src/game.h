/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas asociadas al juego.
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "common/types.h"
#include "common/defines.h"
#include "common/utility.h"
#include "tasks/sched.h"
#include "map.h"

#define LEMMING_CREATION_TIME 401
#define LEMMING_LIFE_SPAN 2001

/**
 * @returns true si el juego esta en modo debug
 */
bool game_in_debug_mode();

/**
 * Activa o desactiva el modo debug
 */
void game_toggle_debug_mode();

/**
 * Funcion para informarle al juego que hubo una excepcion
 */
void game_inform_exception(bool pagefault);

/**
 * Funcion para indicarle al juego que una tarea intento acceder a una
 * posicion invalida del area de memoria disponible
 */
void game_invalid_shared_memory_access();

/**
 * Logica para manejar los ciclos dentro del juego
 *
 * @returns el selector de TSS de la proxima tarea lemming (o idle) que
 * debe ejecutarse.
 */
tss_selector_t game_tick();

/**
 * Inicializa las estructuras del juego
 */
void game_init();

/**
 * Implementacion de syscall_move
 *
 * Mueve al lemming ejecutandose en la direccion dir
 *
 * @returns el resultado de la operacion move segun
 * especificado en el enunciado
 */
move_result_e move(direction_e dir);

/**
 * Implementacion de syscall_explode
 *
 * El lemming en ejecucion se autodestruye, rompe
 * las paredes cercanas, y lemmings a su alrededor
 */
void explode();

/**
 * Implementacion de syscall_bridge
 *
 * Crea un puente en la posicion mas cercana
 * al lemming en ejecucion en la direccion dir
 */
void bridge(direction_e dir);


#endif //  __GAME_H__
