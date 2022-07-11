/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"

unsigned int next_free_kernel_page;
unsigned int next_free_user_page;

void mmu_init() {
  next_free_kernel_page = KERNEL_FREE_AREA_INIT;
  next_free_user_page = TASK_FREE_AREA_INIT;
}

paddr_t mmu_next_free_kernel_page() {
  unsigned int free_page = next_free_kernel_page;
  next_free_kernel_page += PAGE_SIZE;
  return free_page;
}

paddr_t mmu_next_free_user_page(void) {
  unsigned int free_page = next_free_user_page;
  next_free_user_page += PAGE_SIZE;
  return free_page;
}

paddr_t mmu_init_kernel_dir() {
  page_directory_entry* pd = (page_directory_entry*)KERNEL_PAGE_DIR;
  page_table_entry* pt_0 = (page_table_entry*)KERNEL_PAGE_TABLE_0; // primeros 4 megas identity mapping

  //Inicializamos en 0
  for (int i = 0; i < 1024; ++i) {
    pd[i] = (page_directory_entry){0};

    pt_0[i] = (page_table_entry){0};
    pt_0[i].present = MMU_P;
    pt_0[i].read_write = MMU_W;
    pt_0[i].user_supervisor = 0;
    pt_0[i].physical_address_base = i; // esto asegura el identity mapping
  }

  pd[0].present = MMU_P;
  pd[0].read_write = MMU_W;
  pd[0].user_supervisor = 0;
  pd[0].page_table_base = ((paddr_t)pt_0) >> 12;

  return (paddr_t)pd; // en cr3 cargamos 0x00025000 -> los primeros 12 bits van en 0, y la dirección al directorio es la 0x25000
}

uint32_t get_pde_idx(vaddr_t virt) {
  return virt >> 22;
}

uint32_t get_pte_idx(vaddr_t virt) {
  return (virt >> 12) & 0x3FF;
}

void mmu_map_page(page_directory_entry* cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
  uint32_t pde_id = get_pde_idx(virt);
  uint32_t pte_id = get_pte_idx(virt);

  if (cr3[pde_id].present != 1) {

    page_table_entry* next_page_table = (page_table_entry*)mmu_next_free_kernel_page();
    for (int i = 0; i < 1024; ++i) {
      next_page_table[i] = (page_table_entry){0};
    }

    cr3[pde_id].present = (attrs >> 0) & 0x1;
    cr3[pde_id].user_supervisor = (attrs >> 2) & 0x1;
    cr3[pde_id].page_table_base = ((paddr_t)next_page_table) >> 12;
  }

  // pisamos el permiso de escritura en pde
  // puede crearse primero una página sin permiso de escritura y despues otra con
  // para poder escribir en la segunda necesitamos que la PDE tenga el bit de R/W prendido
  cr3[pde_id].read_write = (attrs >> 1) & 0x1;

  page_table_entry* page_table = (page_table_entry*)(cr3[pde_id].page_table_base << 12);

  page_table[pte_id].present = (attrs >> 0) & 0x1;
  page_table[pte_id].read_write = (attrs >> 1) & 0x1;
  page_table[pte_id].user_supervisor = (attrs >> 2) & 0x1;
  page_table[pte_id].physical_address_base = (phy >> 12);

  tlbflush();
}

void mmu_unmap_page(page_directory_entry* cr3, vaddr_t virt) {
  uint32_t pde_id = get_pde_idx(virt);
  // si la tabla de pagina no esta presente no hay nada que hacer
  if (cr3[pde_id].present) {
    uint32_t pte_id = get_pte_idx(virt);
    page_table_entry* page_table = (page_table_entry*)(cr3[pde_id].page_table_base << 12);

    page_table[pte_id].present = 0;
    page_table[pte_id].read_write = 0;
    page_table[pte_id].user_supervisor = 0;
    page_table[pte_id].physical_address_base = 0;

    tlbflush();
  }
}

paddr_t mmu_init_task_dir(paddr_t phy_start) {
  // creamos un nuevo page directory para la tarea
  page_directory_entry* pd = (page_directory_entry*)mmu_next_free_kernel_page();

  // inicializamos todo el page directory en 0
  for (int i = 0; i < 1024; ++i) {
    pd[i] = (page_directory_entry){0};
  }

  // mapeamos con identity mapping los primeros 4Mb
  // de memoria
  // kernel y el area libre del kernel
  for (int i = 0; i < 1024; i++) {
    // shifteando i 12 lugares a la derecha obtenemos el comienzo
    // de la direccion 4Kb * i
    vaddr_t id_v = i * PAGE_SIZE;
    // como es identity mapping, la direccion virtual
    // coincide con la fisica
    paddr_t id_p = id_v;
    mmu_map_page(pd, id_v, id_p, PRESENT_WRITE_0_ATTRS);
  }

  // mapeamos las paginas de codigo de la tarea a la direccion fisica que recibimos
  for (int i = 0; i < TASK_CODE_PAGES; i++) {
    vaddr_t virtual = TASK_CODE_VIRTUAL + (i * PAGE_SIZE);
    paddr_t phy = phy_start + (i * PAGE_SIZE);
    mmu_map_page(pd, virtual, phy, PRESENT_READ_USER_ATTRS);
  }

  // mapeamos el stack de la tarea a una pagina libre del
  // espacio disponible para las tareas
  vaddr_t virtual_stack_init = TASK_STACK_BASE - PAGE_SIZE;
  mmu_map_page(pd, virtual_stack_init, mmu_next_free_user_page(), PRESENT_WRITE_USER_ATTRS);

  return (paddr_t)pd;
}


