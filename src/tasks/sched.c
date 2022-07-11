/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"

/**
 * id de la tarea que se esta ejecutando
 *
 * 0 a 4 -> tarea lemming a
 * 5 a 9 -> tarea lemming b
 */
task_id_t running_task_id;
/**
 * id de la ultima tarea lemming a ejecutada
 */
task_id_t last_executed_A;
/**
 * id de la ultima tarea lemming b ejecutada
 */
task_id_t last_executed_B;
/**
 * arreglo con las tareas lemming a vivas (true)
 */
bool lemming_A_tasks[MAX_LEMMINGS_TEAM];
/**
 * arreglo con las tareas lemming b vivas (true)
 */
bool lemming_B_tasks[MAX_LEMMINGS_TEAM];

task_id_t get_running_lemming_task_id() {
  return running_task_id;
}

void sched_init() {
  // inicializamos para que el sched_next_task empiece con
  // la primera tarea lemming a
  running_task_id = 9;
  last_executed_A = 4;
  last_executed_B = 4;

  // iniciamos los lemmings (0 lemmings vivos al principio)
  for (uint8_t i = 0; i < MAX_LEMMINGS_TEAM; i++) {
    lemming_A_tasks[i] = false;
    lemming_B_tasks[i] = false;
  }
}

/**
 * @returns el proximo id libre para crear un lemming del equipo indicado
 */
task_id_t sched_next_free_task(lemming_team_e team) {
  bool* team_lemmings = team == LEMMING_A ? (bool*)&lemming_A_tasks : (bool*)&lemming_B_tasks;
  task_id_t i = 0;
  while (i < MAX_LEMMINGS_TEAM) {
    if (!team_lemmings[i]) {
      return i + (team == LEMMING_A ? 0 : MAX_LEMMINGS_TEAM);
    }
    i++;
  }

  // si llegamos aca, no hay lugar para crear un lemming
  return MAX_LEMMINGS;
}

task_id_t sched_init_task(lemming_team_e team) {
  task_id_t new_lemming_id = sched_next_free_task(team);
  if (new_lemming_id < MAX_LEMMINGS) { // si ya esta al limite de lemmings
    tss_init_lemming_task(new_lemming_id);
    if (team == LEMMING_A) {
      lemming_A_tasks[new_lemming_id] = true;
    } else {
      lemming_B_tasks[new_lemming_id % MAX_LEMMINGS_TEAM] = true;
    }

    uint8_t gdt_idx = GDT_IDX_TSS_FIRST_LEMMING_DESC + new_lemming_id;

    gdt[gdt_idx].p = 0x1;
    gdt[gdt_idx].base_15_0 = (uint16_t)(((uint32_t)(&tss_lemmings[new_lemming_id])) & 0xffff);
    gdt[gdt_idx].base_23_16 = (uint8_t)((((uint32_t)(&tss_lemmings[new_lemming_id])) >> 16) & 0xff);
    gdt[gdt_idx].base_31_24 = (uint8_t)((((uint32_t)(&tss_lemmings[new_lemming_id])) >> 24) & 0xff);
  }

  return new_lemming_id;
}

/**
 * @returns true si todos los lemmings estan muertos
 */
bool all_dead() {
  uint8_t i = 0;
  while (i < MAX_LEMMINGS_TEAM && !lemming_A_tasks[i] && !lemming_B_tasks[i]) {
    i++;
  }

  return i == MAX_LEMMINGS_TEAM;
}

/**
 * @returns el id del proximo lemming a ejecutar en un equipo
 */
task_id_t get_next_in_team(lemming_team_e team) {
  bool* team_lemmings = team == LEMMING_A ? (bool*)&lemming_A_tasks : (bool*)&lemming_B_tasks;
  //buscamos el lemming proximo segun su equipo
  uint8_t next = (team == LEMMING_A ? (last_executed_A + 1) : (last_executed_B + 1)) % MAX_LEMMINGS_TEAM;

  // Buscamos el proximo lemming vivo en el equipo
  // ciclando de esta forma, si el ultimo lemming ejecutado es el
  // unico vivo al salir del ciclo next = last_executed_X
  int i = 0;
  while (i < (MAX_LEMMINGS_TEAM - 1)) {
    //pregunta si el proximo a ejecutar esta vivo
    if (team_lemmings[next]) {
      return next + (team == LEMMING_A ? 0 : MAX_LEMMINGS_TEAM); //si esta vivo, devuelve su id
    }
    next = (next + 1) % MAX_LEMMINGS_TEAM;
    i++;
  }
  //si no esta vivo el proximo, se devuelve el ultimo lemming ejecutado
  return next + (team == LEMMING_A ? 0 : MAX_LEMMINGS_TEAM);
}

/**
 * @returns el id del proximo lemming a ejecutar
 */
task_id_t get_next_lemming_id() {
  task_id_t lemming_id;

  if (all_dead()) {
    return MAX_LEMMINGS;
  }

  //buscamos el proximo lemming/tarea a ejecutar
  lemming_team_e running_lemming_team = lemming_team(running_task_id);
  if (running_lemming_team == LEMMING_A) {
    //si el ultimo ejecutado es Amalin, busca el proximo de Betarote
    lemming_id = get_next_in_team(LEMMING_B);

    //chequea si esta vivo, si esta muerto, ejecuta el proximo de amalin
    if (!lemming_B_tasks[lemming_id % MAX_LEMMINGS_TEAM]) {
      lemming_id = get_next_in_team(running_lemming_team);
    }
  } else { //lo mismo si el actual es betarote
    lemming_id = get_next_in_team(LEMMING_A);
    if (!lemming_A_tasks[lemming_id]) {
      lemming_id = get_next_in_team(running_lemming_team);
    }
  }
  //devuelve el id del lemming proximo a ejecutar
  return lemming_id;
}

tss_selector_t sched_next_task() {
  task_id_t lemming_id = get_next_lemming_id();
  if (lemming_id == MAX_LEMMINGS) {
    return 0;
  }

  // actualizamos el ultimo lemming ejecutado 
  running_task_id = lemming_id;
  if (lemming_team(running_task_id) == LEMMING_A) {
    last_executed_A = running_task_id;
  } else {
    last_executed_B = running_task_id % MAX_LEMMINGS_TEAM;
  }

  // devolvemos el selector de TSS de la tarea lemming que sigue
  uint8_t gdt_idx = GDT_IDX_TSS_FIRST_LEMMING_DESC + lemming_id;
  return (gdt_idx << 3);
}

void sched_kill_running_task() {
  sched_kill_task(running_task_id);
  // pasa a la tarea idle
  switch_to_idle();
}

void sched_kill_task(task_id_t task_id) {
  uint8_t gdt_idx = GDT_IDX_TSS_FIRST_LEMMING_DESC + task_id;
  // seteamos el bit presente de la TSS de la tarea en 0
  gdt[gdt_idx].p = 0x0;

  // matamos el lemming en el arreglo de su equipo
  lemming_team_e team = lemming_team(task_id);
  if (team == LEMMING_A) {
    lemming_A_tasks[task_id] = false;
  } else {
    lemming_B_tasks[task_id % MAX_LEMMINGS_TEAM] = false;
  }
}

void sched_kill_all() {
  // desalojamos todas las tareas excepto la actual
  for (task_id_t i = 0; i < MAX_LEMMINGS; i++) {
    if (i != running_task_id) {
      sched_kill_task(i);
    }
  }

  // desalojamos la tarea actual y pasamos a idle
  sched_kill_running_task();
}

