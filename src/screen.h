
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "common/colors.h"
#include "common/defines.h"
#include "common/types.h"
#include "common/i386.h"
#include "common/utility.h"
#include "stdint.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
  uint8_t c;
  uint8_t a;
} ca;

/**
 * Imprime un texto en pantalla
 *
 * @returns el largo del texto
 */
uint32_t print(const char* text, uint32_t x, uint32_t y, uint16_t attr);

/**
 * Imprime un unico caracter en pantalla
 */
void print_char(char c, uint32_t col, uint32_t row, uint16_t attr);

/**
 * Imprime un entero en decimal en pantalla
 */
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y, uint16_t attr);

/**
 * Imprime un entero en hexa en pantalla
 */
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y, uint16_t attr);

/**
 * Imprime las libretas de los integrantes del grupo en pantalla
 */
void print_libretas();

/**
 * Imprime una caja con el mismo carcater (y atributos)
 */
void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize, uint32_t cSize, uint8_t character, uint8_t attr);

/**
 * Inicializa el layout con la informacion del juego
 */
void screen_init_layout();

/**
 * Actualiza el estado de la tarea lemming_id
 */
void screen_print_lemming_status(task_id_t lemming_id, bool alive);

/**
 * Actualiza el contador de lemmings creados para un equipo
 */
void screen_update_spawned(lemming_team_e team, size_t spawned);

/**
 * Imprime la informacion de debug en pantalla
 */
void print_exception(exception_debug_info_t debug_info);

/**
 * Imprime pantalla un indicador si el juego esta en modo debug
 */
void print_debug_mode(bool debug_mode);

/**
 * Imprime la pantalla de finalización del juego.
 * Imprime al equipo ganador y la cantidad de lemmings iniciados por equipo.
 */
void print_game_end(lemming_team_e winner, size_t spawned_a, size_t spawned_b);

#endif //  __SCREEN_H__
