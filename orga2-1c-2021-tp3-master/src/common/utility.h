/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  Declaracion de funciones generales
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "types.h"
#include "defines.h"
#include "i386.h"

/**
 * @returns el equipo del lemming con id lemming_id
 */
lemming_team_e lemming_team(task_id_t lemming_id);

/**
 * @returns la proxima posicion en la direccion dir
 * a partir de la posicion from
 */
position_t next(position_t from, direction_e dir);

/**
 * @returns true si las posiciones estan a distancia 1 (Manhattan)
 */
bool close(position_t pos1, position_t pos2);

/**
 * @returns el indice del valor mas grande en v
 */
uint8_t pos_max(uint8_t v[], uint8_t size);

/**
 * @returns true si la direccion dir es valida
 */
bool valid_direction(uint32_t dir);

/**
 * Traduce el struct exception stack a un struct con toda la informacion necesaria
 * para mostrar la informacion de debug
 *
 * @returns informacion de debug luego de una excepcion
 */
exception_debug_info_t from_exception_stack(uint8_t type, exception_stack_t exception_stack, task_id_t task_id);


#endif //  __UTILITY_H__
