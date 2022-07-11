/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss_t tss_initial;
tss_t tss_idle;
tss_t tss_lemmings[MAX_LEMMINGS];

void tss_init() {
  tss_initial = (tss_t){0};

  // Completamos tareas de privilegio nivel 3
  for (size_t i = 0; i < MAX_LEMMINGS; i++) {
    tss_lemmings[i] = (tss_t){0};
  }
}

void tss_init_idle() {

  // pila nivel 0
  tss_idle.ss0 = GDT_DATA0_SEGSEL;
  tss_idle.esp0 = KERNEL_STACK;

  tss_idle.eip = TASK_IDLE_START;           // direccion inicial del codigo de la tarea idle
  tss_idle.cr3 = KERNEL_PAGE_DIR;           // cr3 igual al del kernel
  tss_idle.esp = KERNEL_STACK;              // misma pila del kernel
  tss_idle.ebp = KERNEL_STACK;              // misma pila del kernel
  tss_idle.cs = GDT_CODE0_SEGSEL;         // segmento código de nivel 0
  tss_idle.ds = GDT_DATA0_SEGSEL;         // segmento datos nivel 0
  tss_idle.ss = GDT_DATA0_SEGSEL;         // segmento datos nivel 0
  tss_idle.fs = GDT_DATA0_SEGSEL;         // segmento datos nivel 0
  tss_idle.gs = GDT_DATA0_SEGSEL;         // segmento datos nivel 0
  tss_idle.es = GDT_DATA0_SEGSEL;         // segmento datos nivel 0
  tss_idle.eflags = TSS_DEFAULT_EFLAGS;     // interrupciones activadas

  // completamos la base del tss descriptor de la tarea idle
  gdt[GDT_IDX_TSS_IDLE_DESC].base_15_0 = (uint16_t)(((uint32_t)(&tss_idle)) & 0xffff);
  gdt[GDT_IDX_TSS_IDLE_DESC].base_23_16 = (uint8_t)((((uint32_t)(&tss_idle)) >> 16) & 0xff);
  gdt[GDT_IDX_TSS_IDLE_DESC].base_31_24 = (uint8_t)((((uint32_t)(&tss_idle)) >> 24) & 0xff);
}

void tss_init_lemming_task(task_id_t lemming_id) {

  paddr_t phy_code_start = lemming_team(lemming_id) == LEMMING_A ? TASK_LEMMING_A_CODE_START : TASK_LEMMING_B_CODE_START;

  tss_lemmings[lemming_id].cr3 = mmu_init_task_dir(phy_code_start);
  // mapeamos la pila de nivel 0
  // pedimos una pagina libre del kernel y ponemos el esp al final de la pagina recibida
  tss_lemmings[lemming_id].esp0 = mmu_next_free_kernel_page() + PAGE_SIZE;

  // inicializamos el eip, stack y flags para la nueva tarea
  tss_lemmings[lemming_id].eip = TASK_CODE_VIRTUAL;
  tss_lemmings[lemming_id].ebp = TASK_STACK_BASE;
  tss_lemmings[lemming_id].esp = TASK_STACK_BASE;
  tss_lemmings[lemming_id].eflags = TSS_DEFAULT_EFLAGS;
  tss_lemmings[lemming_id].cs = GDT_CODE3_SEGSEL;
  tss_lemmings[lemming_id].ds = GDT_DATA3_SEGSEL;
  tss_lemmings[lemming_id].ss = GDT_DATA3_SEGSEL;
  tss_lemmings[lemming_id].fs = GDT_DATA3_SEGSEL;
  tss_lemmings[lemming_id].gs = GDT_DATA3_SEGSEL;
  tss_lemmings[lemming_id].es = GDT_DATA3_SEGSEL;
  tss_lemmings[lemming_id].ss0 = GDT_DATA0_SEGSEL;
}

/**
 * @returns la direccion fisica en la que esta mapeada la direccion virtual addr
 * para la tarea con id task_id en ejecucion o NULL_ADDR si no esta mapeada.
 */
paddr_t mapped_addr(uint8_t task_id, vaddr_t addr) {
  //buscamos en la gdt
  uint8_t gdt_idx = GDT_IDX_TSS_FIRST_LEMMING_DESC + task_id;
  // vemos si la tarea no esta corriendo
  if (gdt[gdt_idx].p == 0) {
    return NULL_ADDR;
  }
  // la tarea esta corriendo
  // chequeamos si la tarea  esta en la tabla de paginas 
  page_directory_entry* cr3_task = (page_directory_entry*)tss_lemmings[task_id].cr3;
  page_directory_entry pde = cr3_task[get_pde_idx(addr)];
  if (pde.present == 0) {
    return NULL_ADDR;
  }
  //esta en la tabla de paginas, vemos si esta corriendo
  page_table_entry* page_table = (page_table_entry*)(pde.page_table_base << 12);
  page_table_entry pte = page_table[get_pte_idx(addr)];

  return pte.present ? (pte.physical_address_base << 12) : NULL_ADDR;
}

/**
 * @returns la direccion fisica en la que esta mapeada la direccion virtual addr
 * para el equipo de la tarea en ejecucion o NULL_ADDR si no esta mapeada.
 */
paddr_t tss_team_mapped_addr(vaddr_t addr) {
  //chequea si el address esta mapeado en el equipo del lemming actual
  task_id_t lemmimg_id = get_running_lemming_task_id();
  lemming_team_e team = lemming_team(lemmimg_id);
  uint8_t offset = (team == LEMMING_A ? 0 : MAX_LEMMINGS_TEAM);
  for (uint8_t i = offset; i < MAX_LEMMINGS_TEAM + offset; i++) {
    task_id_t task_id = i;
    if (task_id == lemmimg_id) {
      continue;
    }
    paddr_t phy = mapped_addr(task_id, addr);
    if (phy != NULL_ADDR) return phy;
  }

  return NULL_ADDR;
}

void tss_map_shared_page(vaddr_t virt) {
  task_id_t lemmimg_id = get_running_lemming_task_id();
  paddr_t phy = tss_team_mapped_addr(virt);
  phy = phy == NULL_ADDR ? mmu_next_free_user_page() : phy;

  mmu_map_page((page_directory_entry*)tss_lemmings[lemmimg_id].cr3, virt, phy, PRESENT_WRITE_USER_ATTRS);
}
