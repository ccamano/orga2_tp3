// clang-format off
#include "map.h"

#define FREE_SPACE '.'
#define WALL 'P'
#define WATER 'L'
#define BROKEN_WALL 'X'
#define BRIDGE '+'

#define LEMMING_A_DISPLAY_CHAR 'A'
#define LEMMING_A_DISPLAY_ATTRS (C_BG_RED | C_FG_WHITE)
#define LEMMING_B_DISPLAY_CHAR 'B'
#define LEMMING_B_DISPLAY_ATTRS (C_BG_BLUE | C_FG_WHITE)

const char mapa_original[40][80] = {
"........PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL....",
"........LLLLLLLLL..................................................LLLLLLLLL....",
"........PPPPPPPPP.PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP.PPPPPPPPP....",
"........LLLLLLLLL.LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........LLLLLLLLL.LLLL........................................LLLL.LLLLLLLLL....",
"........PPPPPPPPP.PPPP.PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP.PPPP.PPPPPPPPP....",
"........LLLLLLLLL.LLLL.LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLL.LLLLLLLLL....",
"........LLLLLLLLL.LLLL.LLLL..............................LLLL.LLLL.LLLLLLLLL....",
"........PPPPPPPPP.PPPP.PPPP..............................PPPP.PPPP.PPPPPPPPP....",
"........LLLLLLLLL.LLLL.LLLL..............................LLLL.LLLL.LLLLLLLLL....",
"........LLLLLLLLL.LLLL.LLLL..............................LLLL.LLLL.LLLLLLLLL....",
"........PPPPPPPPP.PPPP.PPPP..............................PPPP.PPPP.PPPPPPPPP....",
"........LLLLLLLLL.LLLL.LLLLLLLLLLLLLLLLL.LLLLLLLLLLLLLLLLLLLL.LLLL.LLLLLLLLL....",
"........LLLLLLLLL.LLLL.LLLLLLLLLLLLLLLLL.LLLLLLLLLLLLLLLLLLLL.LLLL.LLLLLLLLL....",
"........PPPPPPPPP.PPPP...................PPPPPPPPPPPPPPPPPPPP.PPPP.PPPPPPPPP....",
"........LLLLLLLLL.LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLL.LLLLLLLLL....",
"........LLLLLLLLL.LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLL.LLLLLLLLL....",
"........PPPPPPPPP.............................................PPPP.PPPPPPPPP....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........PPPPPPPPPPP................................................PPPPPPPPP....",
"........LLLLLLLLLLL.LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL....",
"........LLLLLLLLLLL.LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL....",
"........PPPPPPPPPPP.PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP....",
"........LLLLLLLLLLL.LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL....",
"........LLLLLLLLLLL................................................LLLLLLLLL....",
"........PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP.PPPPPPPPP....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP.PPPPPPPPP....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP.PPPPPPPPP....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL.LLLLLLLLL....",
"...................................................................PPPPPPPPP....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL....",
"........LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL....",
"........PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP....",


};

/**
 * Mapa del juego
 */
char map[MAP_ROWS][MAP_COLS];

/**
 * Imprime un espacio libre
 */
void map_print_free_space(position_t pos) {
  print_char(' ', pos.col, pos.row, C_BG_GREEN | C_FG_GREEN);
}

/**
 * Imprime una pared
 */
void map_print_wall(position_t pos) {
  print_char('P', pos.col, pos.row, C_BG_RED | C_FG_LIGHT_RED);
}

/**
 * Imprime un lago
 */
void map_print_water(position_t pos) {
  print_char(' ', pos.col, pos.row, C_BG_BLUE | C_FG_BLUE);
}

/**
 * Imprime un puente
 */
void map_print_bridge(position_t pos) {
  print_char('+', pos.col, pos.row, C_BG_BLUE | C_FG_WHITE);
}

/**
 * Imprime una pared rota
 */
void map_print_broken_wall(position_t pos) {
  print_char('X', pos.col, pos.row, C_BG_RED | C_FG_GREEN);
}

/**
 * Imprime en la posición recibida el caracter que le corresponde
 */
void map_print_tile(position_t pos) {
  if (is_free_space(pos)) {
    map_print_free_space(pos);
  } else if (is_wall(pos)) {
    map_print_wall(pos);
  } else if (is_water(pos)) {
    map_print_water(pos);
  } else if (is_bridge(pos)) {
    map_print_bridge(pos);
  } else if (is_broken_wall(pos)) {
    map_print_broken_wall(pos);
  }
}

/**
 * Imprime un lemming en la posicion indicada
 * segun su equipo
 */
void map_print_lemming(lemming_team_e team, position_t pos) {
  char tile_char = team == LEMMING_A ? LEMMING_A_DISPLAY_CHAR : LEMMING_B_DISPLAY_CHAR;
  uint16_t attrs = team == LEMMING_A ? LEMMING_A_DISPLAY_ATTRS : LEMMING_B_DISPLAY_ATTRS;

  print_char(tile_char, pos.col, pos.row, attrs);
}

void map_reset() {
  for (size_t i = 0; i < MAP_ROWS; i++) {
    for (size_t j = 0; j < MAP_COLS; j++) {
      map[i][j] = mapa_original[i][j];
    }
  }
}


void map_print() {
  for (uint32_t i = 0; i < MAP_ROWS; i++) {
    for (uint32_t j = 0; j < MAP_COLS; j++) {
      position_t pos;
      pos.col = j;
      pos.row = i;
      map_print_tile(pos);
    }
  }
}

bool in_range(position_t p) {
  return (p.col < MAP_COLS) && p.row < MAP_ROWS;
}

/**
 * Rompe la pared en la posicion p
 * si p es una posicion valida y hay una
 * pared en p.
 * Actualiza el mapa en caso de explotar
 */
void break_wall(position_t p) {
  if (in_range(p) && is_wall(p)) {
    map[p.row][p.col] = BROKEN_WALL;
    // actualizamos el mapa
    map_print_broken_wall(p);
  }
}

void map_explosion(position_t explosion_position) {
  break_wall(explosion_position);//explosion del tile original
  explosion_position.col++;
  break_wall(explosion_position); //explosion tile a la derecha
  explosion_position.col -= 2;
  break_wall(explosion_position); //explosion tile a la izquierda
  explosion_position.col++;
  explosion_position.row++;
  break_wall(explosion_position); //explosion tile abajo
  explosion_position.row -= 2;
  break_wall(explosion_position); // explosion tile arriba
}

void map_bridge(position_t bridge_position) {
  if (in_range(bridge_position) && is_water(bridge_position)) {
    map[bridge_position.row][bridge_position.col] = BRIDGE;
    // actualizamos el mapa
    map_print_bridge(bridge_position);
  }
}

void map_spawn_lemming(lemming_team_e team, position_t spawn_position) {
  if (in_range(spawn_position)) {
    map_print_lemming(team, spawn_position);
  }
}

void map_move_lemming(lemming_team_e team, position_t from, position_t to) {
  if (in_range(from)) {
    // restauramos la posicion del mapa en el se encontraba el lemming
    map_print_tile(from);
  }

  if (in_range(to)) {
    // imprimimos el lemming en la nueva posicion
    map_print_lemming(team, to);
  }
}

void map_kill_lemming(position_t pos) {
  // restauramos la posicion del mapa en el se encontraba el lemming
  if (in_range(pos)) {
    map_print_tile(pos);
  }
}

bool is_free_space(position_t p) {
  char tile = map[p.row][p.col];
  return tile == FREE_SPACE;
}

bool is_wall(position_t p) {
  char tile = map[p.row][p.col];
  return tile == WALL;
}

bool is_water(position_t p) {
  char tile = map[p.row][p.col];
  return tile == WATER;
}

bool is_bridge(position_t p) {
  char tile = map[p.row][p.col];
  return tile == BRIDGE;
}

bool is_broken_wall(position_t p) {
  char tile = map[p.row][p.col];
  return tile == BROKEN_WALL;
}
