#include "utility.h"

/**
 * Valor absoluto
 */
uint32_t abs(int x) {
  return x < 0 ? -x : x;
}


lemming_team_e lemming_team(task_id_t lemming_id) {
  return lemming_id < MAX_LEMMINGS_TEAM ? LEMMING_A : LEMMING_B;
}

position_t next(position_t from, direction_e dir) {
  position_t next_pos = from;
  if (dir == DIR_UP) {
    next_pos.row--;
  } else if (dir == DIR_DOWN) {
    next_pos.row++;
  } else if (dir == DIR_RIGHT) {
    next_pos.col++;
  } else if (dir == DIR_LEFT) {
    next_pos.col--;
  }

  return next_pos;
}


bool close(position_t pos1, position_t pos2) {
  int col_diff = abs(pos1.col - pos2.col);
  int row_diff = abs(pos1.row - pos2.row);

  bool same_row_diff_col = row_diff == 0 && col_diff == 1;
  bool same_col_diff_row = col_diff == 0 && row_diff == 1;

  // Consideramos que estan cerca si estan en la misma fila/columna
  // y difieren en 1 en columna/fila
  return same_col_diff_row || same_row_diff_col;
}

uint8_t pos_max(uint8_t v[], uint8_t size) {
  uint8_t p = 0;
  for (size_t i = 0; i < size; i++) {
    if (v[i] > v[p]) {
      p = i;
    }
  }

  return p;
}

bool valid_direction(uint32_t dir) {
  return dir < 4;
}

/**
 * Devuelve true si la excepcion del tipo indicado deja error code en el stack
 */
bool with_err_code(uint8_t exception_type) {
  return (exception_type == 8) || (exception_type >= 10 && exception_type <= 14) || (exception_type == 17);
}

/**
 * Devuelve un string con una breve descripcion del tipo de excepcion indicado
 */
char* get_exteption_label(uint8_t exception_type) {
  switch (exception_type) {
    case 0: return "Divide Error #DE";
    case 1: return "Debug Exception #DB";
    case 2: return "NMI Interrupt -";
    case 3: return "Breakpoint #BP";
    case 4: return "Overflow #OF";
    case 5: return "BOUND Range Exception #BR";
    case 6: return "Invalid Opcode #UD";
    case 7: return "Device Not Available #NM";
    case 8: return "Double Fault #DF";
    case 9: return "Segment Overrun";
    case 10: return "Invalid TSS #TSS";
    case 11: return "Segment Not Present #NP";
    case 12: return "Stack-Segment Fault #SS";
    case 13: return "General Protection #GP";
    case 14: return "Page Fault #PF";
    case 15: return "Intel Reserved -";
    case 16: return "FPU Error #MF";
    case 17: return "Alignment Check #AC";
    case 18: return "Machine Check #MC";
    case 19: return "SIMD Exception #XM";
  }

  return "";
}

exception_debug_info_t from_exception_stack(uint8_t type, exception_stack_t exception_stack, task_id_t task_id) {
  exception_debug_info_t ret;

  ret.type = type;
  ret.task_id = task_id;
  ret.label = get_exteption_label(type);
  ret.with_err_code = with_err_code(type);

  ret.eax = exception_stack.eax;
  ret.ebx = exception_stack.ebx;
  ret.ecx = exception_stack.ecx;
  ret.edx = exception_stack.edx;
  ret.esi = exception_stack.esi;
  ret.edi = exception_stack.edi;
  ret.ebp = exception_stack.ebp;

  ret.err_code = ret.with_err_code ? exception_stack.stack1 : 0;
  ret.eip = ret.with_err_code ? exception_stack.stack2 : exception_stack.stack1;
  ret.flags = ret.with_err_code ? exception_stack.stack4 : exception_stack.stack3;
  ret.esp = ret.with_err_code ? exception_stack.stack5 : exception_stack.stack4;

  ret.cs = ret.with_err_code ? exception_stack.stack3 : exception_stack.stack2;
  ret.ds = rds();
  ret.es = res();
  ret.fs = rfs();
  ret.gs = rgs();
  ret.ss = rss();

  ret.cr0 = rcr0();
  ret.cr2 = rcr2();
  ret.cr3 = rcr3();
  ret.cr4 = rcr4();

  ret.stack = (uint32_t*)ret.esp;

  // cantidad de elementos en el stack a imprimir
  ret.stack_size = 0;
  if (ret.ebp - ret.esp >= 12) ret.stack_size = 3;
  else if (ret.ebp - ret.esp >= 8) ret.stack_size = 2;
  else if (ret.ebp - ret.esp >= 4) ret.stack_size = 1;

  return ret;
}
