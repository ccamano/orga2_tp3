/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

/**
 * Indica si el juego esta en modo debug
 */
bool debug = false;

/**
 * Indica si hubo una excepcion
 */
bool exception_interrupted = false;

/**
 * Indica si el juego termino
 */
bool game_finished = false;

/**
 * Posiciones de los lemmings en el mapa
 */
position_t lemmings[MAX_LEMMINGS];

/**
 * Posicion de inicio de lemmings tipo a
 */
const position_t lemming_A_start = {.col = 0, .row = 39};
/**
 * Posicion de inicio de lemmings tipo b
 */
const position_t lemming_B_start = {.col = 79, .row = 0};

/**
* Contadores de:
* ticks de reloj
* cantidad de lemmings A/B inicializados en toda la duraciion del juego
* Cantidad de lemmings A/B destruidos en toda la duracion del juego
*/
size_t ticks = 0;
size_t count_spawned_a = 0;
size_t count_spawned_b = 0;
size_t count_killed_a = 0;
size_t count_killed_b = 0;

/**
* Contadores de lemmings A/B vivos actualmente
*/
size_t alive_lemming_A = 0;
size_t alive_lemming_B = 0;

//orden en que se fueron creando los lemmings de cada equipo
//0 indica que esta muerto/no hay lemming
//el minimo indica que es el mas reciente, maximo es el mas viejo
uint8_t order_spawned_a[MAX_LEMMINGS_TEAM];
uint8_t order_spawned_b[MAX_LEMMINGS_TEAM];


bool game_in_debug_mode() {
  return debug;
}

void game_toggle_debug_mode() {
  if (exception_interrupted && debug) {
    // volver a la pantalla del juego original
    map_print();
    for (task_id_t i = 0; i < MAX_LEMMINGS; i++) {
      map_spawn_lemming(lemming_team(i), lemmings[i]);
    }

  } else {
    debug = !debug;
  }
  exception_interrupted = false;

  print_debug_mode(debug);
}

/**
 * @returns el id de la tarea lemming A que
 * lleva activa mas tiempo
 */
task_id_t get_oldest_lemming_A_id() {
  return pos_max(order_spawned_a, MAX_LEMMINGS_TEAM);
}

/**
 * @returns el id de la tarea lemming B que
 * lleva activa mas tiempo
 */
task_id_t get_oldest_lemming_B_id() {
  return pos_max(order_spawned_b, MAX_LEMMINGS_TEAM) + MAX_LEMMINGS_TEAM;
}

/**
 * para A chequea su pos de spawn, idem B
 * ocupada = hay un lemming en el spawn
 *
 * @returns true si la posicion de spawn esta ocupada
 */
bool spawn_position_occupied(lemming_team_e team, task_id_t except) {
  position_t spawn_position = team == LEMMING_A ? lemming_A_start : lemming_B_start;
  for (size_t i = 0; i < MAX_LEMMINGS; i++) {
    if (i != except && lemmings[i].col == spawn_position.col && lemmings[i].row == spawn_position.row) {
      return true;
    }
  }

  return false;
}

/**
 * Actualiza el arreglo con el orden de creacion para los lemmings
 * cuando se crea uno nuevo
 */
void update_order_spawned_on_new(task_id_t new_lemming_id) {
  // le sumamos a todos 1 en order_spawned_a/b
  lemming_team_e team = lemming_team(new_lemming_id);

  uint8_t* order = team == LEMMING_A ? (uint8_t*)&order_spawned_a : (uint8_t*)&order_spawned_b;

  //sumamos 1 a los del equipo si su orden no es 0
  for (uint8_t i = 0; i < MAX_LEMMINGS_TEAM; i++) {
    if (i != (new_lemming_id % MAX_LEMMINGS_TEAM) && order[i] > 0) {
      order[i]++;
    }
  }
  //seteamos orden = 1 al nuevo lemming spawneado
  order[new_lemming_id % MAX_LEMMINGS_TEAM] = 1;
}

/**
 * @returns true si el lemming con id lemming_id llego al borde opuesto
 */
bool game_won(task_id_t lemming_id) {
  lemming_team_e team = lemming_team(lemming_id);
  //lemming amalin llega a columna de los betarotes
  bool team_A_win = team == LEMMING_A && lemmings[lemming_id].col == lemming_B_start.col;
  //lemming betarote llega a columna de los amalin
  bool team_B_win = team == LEMMING_B && lemmings[lemming_id].col == lemming_A_start.col;

  return team_A_win || team_B_win;
}

/**
 * Logica de finalizacion del juego
 */
void game_end(task_id_t winner) {
  print_game_end(lemming_team(winner), count_spawned_a, count_spawned_b);
  game_finished = true;
  sched_kill_all();
}

/**
 * Invalida la posicion en el mapa del lemming
 * con id lemming_id
 *
 * Una posicion invalida indica que el lemming no esta activo
 */
void invalidate_lemming_position(task_id_t lemming_id) {
  lemmings[lemming_id].col = MAP_COLS;
  lemmings[lemming_id].row = MAP_ROWS;
}

/**
 * Se encarga de matar el lemming con id lemming_id
 */
void kill_lemming(task_id_t lemming_id) {
  lemming_team_e team = lemming_team(lemming_id);
  if (team == LEMMING_A) {
    count_killed_a++;
    alive_lemming_A--;
    order_spawned_a[lemming_id] = 0;
  } else {
    count_killed_b++;
    alive_lemming_B--;
    order_spawned_b[lemming_id % MAX_LEMMINGS_TEAM] = 0;
  }

  map_kill_lemming(lemmings[lemming_id]);
  screen_print_lemming_status(lemming_id, false);

  invalidate_lemming_position(lemming_id);
  if (lemming_id == get_running_lemming_task_id()) {
    sched_kill_running_task();
  } else {
    sched_kill_task(lemming_id);
  }
}

/**
 * Crea un lemming para el equipo team si hay lugar para su creacion
 */
void spawn_lemming(lemming_team_e team) {
  //si la posicion de spawn esta libre, crea lemmings
  if (!spawn_position_occupied(team, MAX_LEMMINGS)) {
    task_id_t new_lemming_id = sched_init_task(team);

    //pone su posicion inicial 
    //actualizamos la cantidad de spawned de cada equipo
    if (team == LEMMING_A) {
      alive_lemming_A++;
      count_spawned_a++;
      lemmings[new_lemming_id] = lemming_A_start;
      screen_update_spawned(LEMMING_A, count_spawned_a);
    } else {
      alive_lemming_B++;
      count_spawned_b++;
      lemmings[new_lemming_id] = lemming_B_start;
      screen_update_spawned(LEMMING_B, count_spawned_b);
    }

    //actualizamos el orden de su equipo
    update_order_spawned_on_new(new_lemming_id);
    //actualizamos el mapa con el nuevo lemming spawneado
    map_spawn_lemming(team, lemmings[new_lemming_id]);
    screen_print_lemming_status(new_lemming_id, true);
  }
  //si esta aca, pos de spawn esta ocupada
}

/**
* Destruye el lemming y crea otro en su mismo id
* No desaloja la tarea del lemming original, sino que es reemplazada.
*/
void respawn_lemming(task_id_t task_id) {
  lemming_team_e team = lemming_team(task_id);
  if (!spawn_position_occupied(team, task_id)) {
    position_t old = lemmings[task_id];
    // limpia la tss, la proxima vez que se ejecute la tarea, va 
    // a levantar una TSS limpia
    tss_init_lemming_task(task_id);

    //pone su posicion inicial 
    //actualizamos la cantidad de spawned de cada equipo
    //actualizamos la cantidad de lemmings matados
    //count_alive no hace falta actualizar porque siguen siendo 5 vivos
    if (team == LEMMING_A) {
      count_spawned_a++;
      count_killed_a++;
      lemmings[task_id] = lemming_A_start;
      screen_update_spawned(LEMMING_A, count_spawned_a);
    } else {
      count_spawned_b++;
      count_killed_b++;
      lemmings[task_id] = lemming_B_start;
      screen_update_spawned(LEMMING_B, count_spawned_b);
    }

    //actualizamos el orden de su equipo
    update_order_spawned_on_new(task_id);


    //teletransporta el lemming viejo/nuevo a la posicion inicial
    map_move_lemming(team, old, lemmings[task_id]);
  }
}

void game_inform_exception(bool pagefault) {
  exception_interrupted = true;
  if (!pagefault) {
    kill_lemming(get_running_lemming_task_id());
  }
}

void game_invalid_shared_memory_access() {
  kill_lemming(get_running_lemming_task_id());
}

tss_selector_t game_tick() {
  //si entra a modo debug, no crea nuevos lemmings
  if (game_finished || (debug && exception_interrupted)) {
    // devolvemos la tarea idle para que ejecute
    return GDT_TSS_IDLE_SEGSEL;
  }

  //cada 401 ciclos de reloj crea nuevos lemmings si algun equipo tiene < 5 lemmings
  if (ticks % LEMMING_CREATION_TIME == 0) {
    if (alive_lemming_A < MAX_LEMMINGS_TEAM) {
      // nuevo amalin
      spawn_lemming(LEMMING_A);
    }
    if (alive_lemming_B < MAX_LEMMINGS_TEAM) {
      spawn_lemming(LEMMING_B);
    }
  }

  //cada 2001 ciclos de relo, si algun euipo tiene 5 lemmings, mata al mas antiguo
  //y crea uno nuevo
  if (ticks % LEMMING_LIFE_SPAN == 0) {
    if (alive_lemming_A == MAX_LEMMINGS_TEAM) {
      //"matar" lemming viejo y crea uno nuevo 
      respawn_lemming(get_oldest_lemming_A_id());
    }
    if (alive_lemming_B == MAX_LEMMINGS_TEAM) {
      respawn_lemming(get_oldest_lemming_B_id());
    }
  }

  ticks++;

  // actualizamos el reloj de la tarea lemming que esta corriendo
  task_id_t running = get_running_lemming_task_id();
  screen_print_lemming_status(running, in_range(lemmings[running]));

  // el juego se encarga de devolver la proxima tarea
  return sched_next_task();
}

void game_init() {
  map_reset();

  map_print();

  screen_init_layout();

  // inicializamos el arreglo con las posiciones
  // de los lemmings en posiciones invalidas
  for (uint8_t i = 0; i < MAX_LEMMINGS; i++) {
    invalidate_lemming_position(i);
  }

  for (uint8_t i = 0; i < MAX_LEMMINGS_TEAM; i++) {
    order_spawned_a[i] = 0;
    order_spawned_b[i] = 0;
  }

  print_debug_mode(debug);
}

/**
 * @returns la proxima posicion a la que va a parar el lemming
 * si se mueve en la direccion dir
 */
position_t get_next_position(uint8_t lemming_id, direction_e dir) {
  position_t lemming_position = lemmings[lemming_id];

  return next(lemming_position, dir);
}


move_result_e move(direction_e dir) {
  //chequea que el parametro sea valido
  //si no lo es, mata a la tarea
  if (!valid_direction(dir)) {
    kill_lemming(get_running_lemming_task_id());
  }

  uint8_t lemming_id = get_running_lemming_task_id();
  position_t next_position = get_next_position(lemming_id, dir);

  //chequea que hay en la prox posicion
  if (next_position.col >= MAP_COLS || next_position.row >= MAP_ROWS) {
    return MOVE_RESULT_BORDER;
  }

  if (is_wall(next_position)) {
    return MOVE_RESULT_WALL;
  }

  if (is_water(next_position)) {
    return MOVE_RESULT_LAKE;
  }

  for (size_t i = 0; i < MAX_LEMMINGS; i++) {
    if (i != lemming_id && lemmings[i].row == next_position.row && lemmings[i].col == next_position.col) {
      return MOVE_RESULT_LEMMING;
    }
  }

  // si esta aca, se puede mover y actualiza la posicion del lemming
  position_t old = lemmings[lemming_id];
  lemmings[lemming_id] = next_position;

  map_move_lemming(lemming_team(lemming_id), old, next_position);
  //finalizacion del juego solo puede pasar en un move
  //termina el juego = pasa a idle e imprime en pantalla el ganador
  if (game_won(lemming_id)) {
    game_end(lemming_id);
  }

  return MOVE_RESULT_SUCCESS;
}

void explode() {
  uint8_t lemming_id = get_running_lemming_task_id(); //lemming actual que explota
  position_t lemming_position = lemmings[lemming_id];
  for (uint8_t i = 0; i < MAX_LEMMINGS; i++) {//buscamos lemmings cercanos
    if (i == lemming_id) continue;
    if (close(lemming_position, lemmings[i])) {   //para lemmings contiguos,invalida la pos y desaloja la tarea
      kill_lemming(i);
    }
  }
  map_explosion(lemming_position);
  kill_lemming(lemming_id);
}

void bridge(direction_e dir) {
  uint8_t lemming_id = get_running_lemming_task_id(); //id del lemming actual que explota
  if (!valid_direction(dir)) {
    kill_lemming(lemming_id);
  }
  position_t bridge_position = get_next_position(lemming_id, dir);
  map_bridge(bridge_position);

  kill_lemming(lemming_id);
}
