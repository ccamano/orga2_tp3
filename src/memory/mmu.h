/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "../common/types.h"
#include "../common/i386.h"
#include "../common/kassert.h"

typedef struct str_page_directory_entry {
  uint8_t present : 1;
  uint8_t read_write : 1;
  uint8_t user_supervisor : 1;
  uint8_t page_write_through : 1;
  uint8_t page_cache_disable : 1;
  uint8_t accessed : 1;
  uint8_t x : 1;
  uint8_t page_size : 1;
  uint8_t ignored : 1;
  uint8_t available : 3;
  uint32_t page_table_base : 20;
} __attribute__((__packed__)) page_directory_entry;

typedef struct str_page_table_entry {
  uint8_t present : 1;
  uint8_t read_write : 1;
  uint8_t user_supervisor : 1;
  uint8_t page_write_through : 1;
  uint8_t page_cache_disable : 1;
  uint8_t accessed : 1;
  uint8_t dirty : 1;
  uint8_t x : 1;
  uint8_t global : 1;
  uint8_t available : 3;
  uint32_t physical_address_base : 20;
} __attribute__((__packed__)) page_table_entry;

/**
 * Inicializa las estructuras para administrar la paginacion
 */
void mmu_init();

/**
 * Reserva una pagina libre del kernel
 *
 * @returns la direccion fisica en la que empieza
 */
paddr_t mmu_next_free_kernel_page();

/**
 * Reserva una pagina libre del kernel
 *
 * @returns la direccion fisica en la que empieza
 */
paddr_t mmu_next_free_user_page();

/**
 * Mapea la direccion virtal virt a la direccion fisica phy con atributos attrs
 * utilizando cr3
 */
void mmu_map_page(page_directory_entry* cr3, vaddr_t virt, paddr_t phy, uint32_t attrs);

/**
 * Elimina el mapeo creado en la direccion virtual virt usando el cr3
 */
void mmu_unmap_page(page_directory_entry* cr3, vaddr_t virt);

/**
 * Inicializa el directorio de tablas de paginas del kernel y
 * mapea con identity mapping los primeros 4 megas de memoria (area del kernel)
 *
 * @returns la direccion fisica del directorio de tablas de pagina,
 *  lista para ser cargada en el cr3
 */
paddr_t mmu_init_kernel_dir();

/**
 * Inicializa el directorio de tablas de paginas para una tarea.
 *
 * Mapea con identity mapping los primeros 4 megas de memoria (area del kernel).
 *
 * Mapea la direccion virtual pre-establecida para el codigo a la direccion
 * fisica phy_start
 *
 * Mapea la direccion virtual pre-establecida para la pila (de nivel usuario)
 * a una pagina libre para usuarios
 *
 * @returns la direccion fisica del directorio de tablas de pagina,
 *  lista para ser cargada en el cr3
 */
paddr_t mmu_init_task_dir(paddr_t phy_start);

/**
 * @returns el indice del directorio de tablas
 * de paginas que le corresponde a la direccion virtual virt
 */
uint32_t get_pde_idx(vaddr_t virt);

/**
 * @returns el indice de la tabla de pagina
 *  que le corresponde a la direccion virtual virt
 */
uint32_t get_pte_idx(vaddr_t virt);

#endif //  __MMU_H__
