/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion del mapa
*/

#ifndef __MAPA_H__
#define __MAPA_H__

#include "common/types.h"
#include "common/defines.h"
#include "screen.h"

extern const char mapa_original[40][80];

/**
 * Copia el mapa original a map
 */
void map_reset();

/**
 * Imprime todo el mapa en pantalla
 */
void map_print();

/**
 * Se encarga de actualizar el mapa
 * teniendo en cuenta que hubo una explosion
 * en la posicion recibida
 */
void map_explosion(position_t explosion_position);

/**
 * Se encarga de actualizar el mapa
 * cuando un lemming intenta crear un puente
 * en la posicion recibida
 */
void map_bridge(position_t bridge_position);

/**
 * Se encarga de actualizar el mapa
 * cuando se crea un lemming en la posicion
 */
void map_spawn_lemming(lemming_team_e team, position_t spawn_position);

/**
 * Se encarga de actualizar el mapa
 * cuando un lemming se movio en el mapa
 */
void map_move_lemming(lemming_team_e team, position_t from, position_t to);

/**
 * Se encarga de actualizar el mapa
 * cuando un lemming muere en esa posicion
 */
void map_kill_lemming(position_t pos);

/**
 * @returns true si la posicion recibida
 * se encuentra dentro del mapa
 */
bool in_range(position_t p);

/**
 * @returns true si la posicion esta libre
 */
bool is_free_space(position_t p);

/**
 * @returns true si hay una pared en la posicion
 */
bool is_wall(position_t p);

/**
 * @returns true si hay agua en la posicion
 */
bool is_water(position_t p);

/**
 * @returns true si hay un puente en la posicion
 */
bool is_bridge(position_t p);

/**
 * @returns true si la posicion tiene una pared rota
 */
bool is_broken_wall(position_t p);


#endif //  __MAPA_H__
