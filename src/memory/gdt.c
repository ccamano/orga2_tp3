/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de la tabla de descriptores globales
*/

#include "gdt.h"

#define tss_desc_lemmings { \
.base_15_0 = 0x0000,\
.base_23_16 = 0x00,\
.base_31_24 = 0x00,\
.limit_15_0 = sizeof(tss_t) - 1,\
.limit_19_16 = 0x00,\
.dpl = 0x0,\
.type = 0x9,\
.s = 0x0,\
.p = 0x0,\
.avl = 0x0,\
.l = 0x0,\
.db = 0x0,\
.g = 0x0,\
}

gdt_entry_t gdt[GDT_COUNT] = {
  /* Descriptor nulo*/
  /* Offset = 0x00 */
  [GDT_IDX_NULL_DESC] = {
    .limit_15_0 = 0x0000,
    .base_15_0 = 0x0000,
    .base_23_16 = 0x00,
    .type = 0x0,
    .s = 0x00,
    .dpl = 0x00,
    .p = 0x00,
    .limit_19_16 = 0x00,
    .avl = 0x0,
    .l = 0x0,
    .db = 0x0,
    .g = 0x00,
    .base_31_24 = 0x00,
  },
  [GDT_IDX_CODE0_DESC] = {
    .base_15_0 = 0x0000,
    .base_23_16 = 0x00,
    .base_31_24 = 0x00,
    .limit_15_0 = 0x30ff, // 0x330ff . 
    .limit_19_16 = 0x03,
    .dpl = 0x0,          // Privilege level
    .type = 0xA,          // 1010 = Code = 1 / R = 1
    .s = 0x01,            // 1 = Código o dato | 0 = system
    .p = 0x1,
    .avl = 0x0,           // Siempre 0 
    .l = 0x0,             // 64 bit segment: siempre en 0
    .db = 0x1,            // 1 = 32-bit segment
    .g = 0x1,             // Granularidad
  },
  [GDT_IDX_DATA0_DESC] = {
    .base_15_0 = 0x0000,
    .base_23_16 = 0x00,
    .base_31_24 = 0x00,
    .limit_15_0 = 0x30ff, // 0x330ff . 
    .limit_19_16 = 0x03,
    .dpl = 0x0,          // Privilege level
    .type = 0x2,          // 0010 = W = 1
    .s = 0x01,            // 1 = Código o dato | 0 = system
    .p = 0x1,
    .avl = 0x0,           // Siempre 0 
    .l = 0x0,             // 64 bit segment: siempre en 0
    .db = 0x1,            // 1 = 32-bit segment
    .g = 0x1,             // Granularidad
  },
  [GDT_IDX_CODE3_DESC] = {
    .base_15_0 = 0x0000,
    .base_23_16 = 0x00,
    .base_31_24 = 0x00,
    .limit_15_0 = 0x30ff, // 0x330ff . 
    .limit_19_16 = 0x03,
    .dpl = 0x3,          // Privilege level
    .type = 0xA,          // 1010 = Code = 1 / R = 1
    .s = 0x01,            // 1 = Código o dato | 0 = system
    .p = 0x1,
    .avl = 0x0,           // Siempre 0 
    .l = 0x0,             // 64 bit segment: siempre en 0
    .db = 0x1,            // 1 = 32-bit segment
    .g = 0x1,             // Granularidad
  },
  [GDT_IDX_DATA3_DESC] = {
    .base_15_0 = 0x0000,
    .base_23_16 = 0x00,
    .base_31_24 = 0x00,
    .limit_15_0 = 0x30ff, // 0x330ff . 
    .limit_19_16 = 0x03,
    .dpl = 0x3,          // Privilege level
    .type = 0x2,          // 0010 = W = 1
    .s = 0x01,            // 1 = Código o dato | 0 = system
    .p = 0x1,
    .avl = 0x0,           // Siempre 0 
    .l = 0x0,             // 64 bit segment: siempre en 0
    .db = 0x1,            // 1 = 32-bit segment
    .g = 0x1,             // Granularidad
  },
  [GDT_IDX_VIDEO_DESC] = {
    .base_15_0 = 0x0000,
    .base_23_16 = 0x0A,
    .base_31_24 = 0x00,
    .limit_15_0 = 0xffff, // 1ffff
    .limit_19_16 = 0x01,  // Base en 0xa0000, limite = 1ffff -> hasta c0000
    .dpl = 0x00,          // Privilege level
    .type = 0x2,          // 0010 = W = 1
    .s = 0x01,            // 1 = Código o dato | 0 = system
    .p = 0x1,
    .avl = 0x0,           // Siempre 0 
    .l = 0x0,             // 64 bit segment: siempre en 0
    .db = 0x1,            // 1 = 32-bit segment
    .g = 0x0,             // Granularidad
  },

  // TAREAS
  [GDT_IDX_TSS_INIT_DESC] = {
    .base_15_0 = 0x0000,
    .base_23_16 = 0x00,
    .base_31_24 = 0x00,
    .limit_15_0 = sizeof(tss_initial) - 1,
    .limit_19_16 = 0x00,
    .dpl = 0x0,           // Privilege level
    .type = 0x9,          // 9 = 1001
    .s = 0x0,             // para tss_desc en 0
    .p = 0x1,
    .avl = 0x0,           // Siempre 0 
    .l = 0x0,             // para tss_desc en 0
    .db = 0x0,            // para tss_desc en 0
    .g = 0x0,
  },
  [GDT_IDX_TSS_IDLE_DESC] = {
    .base_15_0 = 0x0000,
    .base_23_16 = 0x00,
    .base_31_24 = 0x00,
    .limit_15_0 = sizeof(tss_idle) - 1,
    .limit_19_16 = 0x00,
    .dpl = 0x00,          // Privilege level
    .type = 0x9,          // 9 = 1001
    .s = 0x0,             // para tss_desc en 0
    .p = 0x1,
    .avl = 0x0,           // Siempre 0 
    .l = 0x0,             // para tss_desc en 0
    .db = 0x0,            // para tss_desc en 0
    .g = 0x0,
  },
  // 10 LEMMINGS
  [GDT_IDX_TSS_FIRST_LEMMING_DESC] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 1] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 2] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 3] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 4] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 5] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 6] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 7] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 8] = tss_desc_lemmings,
  [GDT_IDX_TSS_FIRST_LEMMING_DESC + 9] = tss_desc_lemmings,
};

gdt_descriptor_t GDT_DESC = {sizeof(gdt) - 1, (uint32_t)&gdt};
