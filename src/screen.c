/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones de impresion por pantalla.
*/

#include "screen.h"

#define LEMMING_A_FG_COLOR C_FG_RED
#define LEMMING_B_FG_COLOR C_FG_BLUE
#define LEMMING_A_BG_COLOR C_BG_RED
#define LEMMING_B_BG_COLOR C_BG_BLUE
#define CLOCK_CHARS 4

char lemming_clock[CLOCK_CHARS] = {'/', '|', '-', '\\'};
uint8_t lemming_clock_counter[MAX_LEMMINGS] = {0};

uint32_t print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  for (i = 0; text[i] != 0; i++) {
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS) {
      x = 0;
      y++;
    }
  }

  return i;
}

void print_char(char c, uint32_t col, uint32_t row, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  p[row][col].c = (uint8_t)c;
  p[row][col].a = (uint8_t)attr;
}

void print_exception(exception_debug_info_t debug_info) {
  screen_draw_box(0, 21, 40, 40, ' ', C_BG_BLACK | C_FG_BLACK);

  uint16_t attr_labels = C_BG_BLACK | C_FG_WHITE;
  uint16_t attr_values = C_BG_BLACK | C_FG_LIGHT_GREEN;

  // char* text = "Exception de tipo ";
  uint32_t label_len = print(debug_info.label, 23, 3, attr_labels);
  print_dec(debug_info.type, 3, 23 + label_len + 1, 3, attr_values);

  print("task id: ", 47, 4, attr_labels);
  print_dec(debug_info.task_id, 1, 57, 4, attr_values);

  print("eax ", 23, 6, attr_labels);
  print_hex(debug_info.eax, 8, 27, 6, attr_values);
  print("cr0 ", 47, 7, attr_labels);
  print_hex(debug_info.cr0, 8, 51, 7, attr_values);
  print("ebx ", 23, 8, attr_labels);
  print_hex(debug_info.ebx, 8, 27, 8, attr_values);
  print("cr2 ", 47, 9, attr_labels);
  print_hex(debug_info.cr2, 8, 51, 9, attr_values);
  print("ecx ", 23, 10, attr_labels);
  print_hex(debug_info.ecx, 8, 27, 10, attr_values);
  print("cr3 ", 47, 11, attr_labels);
  print_hex(debug_info.cr3, 8, 51, 11, attr_values);
  print("edx ", 23, 12, attr_labels);
  print_hex(debug_info.edx, 8, 27, 12, attr_values);
  print("cr4 ", 47, 13, attr_labels);
  print_hex(debug_info.cr4, 8, 51, 13, attr_values);
  print("esi ", 23, 14, attr_labels);
  print_hex(debug_info.esi, 8, 27, 14, attr_values);
  if (debug_info.with_err_code) {
    print("err ", 47, 15, attr_labels);
    print_hex(debug_info.err_code, 8, 51, 15, attr_values);
  }
  print("edi ", 23, 16, attr_labels);
  print_hex(debug_info.edi, 8, 27, 16, attr_values);
  print("ebp ", 23, 18, attr_labels);
  print_hex(debug_info.ebp, 8, 27, 18, attr_values);
  print("esp ", 23, 20, attr_labels);
  print_hex(debug_info.esp, 8, 27, 20, attr_values);
  print("eip ", 23, 22, attr_labels);
  print_hex(debug_info.eip, 8, 27, 22, attr_values);

  // stack
  print("stack", 45, 19, attr_labels);
  uint8_t stack_init_row = 21;
  for (int i = 0; i < debug_info.stack_size; i++) {
    print_hex(debug_info.stack[i], 8, 45, stack_init_row + (i * 2), attr_values);
  }

  print("cs ", 24, 24, attr_labels);
  print_hex(debug_info.cs, 8, 27, 24, attr_values);
  print("ds ", 24, 26, attr_labels);
  print_hex(debug_info.ds, 8, 27, 26, attr_values);
  print("es ", 24, 28, attr_labels);
  print_hex(debug_info.es, 8, 27, 28, attr_values);
  print("fs ", 24, 30, attr_labels);
  print_hex(debug_info.fs, 8, 27, 30, attr_values);
  print("gs ", 24, 32, attr_labels);
  print_hex(debug_info.gs, 8, 27, 32, attr_values);
  print("ss ", 24, 34, attr_labels);
  print_hex(debug_info.ss, 8, 27, 34, attr_values);

  print("eflags ", 23, 37, attr_labels);
  print_hex(debug_info.flags, 8, 30, 37, attr_values);
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
  uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  uint8_t letras[16] = "0123456789";

  for (i = 0; i < size; i++) {
    uint32_t resto = numero % 10;
    numero = numero / 10;
    p[y][x + size - i - 1].c = letras[resto];
    p[y][x + size - i - 1].a = attr;
  }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
  uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  uint8_t hexa[8];
  uint8_t letras[16] = "0123456789ABCDEF";
  hexa[0] = letras[(numero & 0x0000000F) >> 0];
  hexa[1] = letras[(numero & 0x000000F0) >> 4];
  hexa[2] = letras[(numero & 0x00000F00) >> 8];
  hexa[3] = letras[(numero & 0x0000F000) >> 12];
  hexa[4] = letras[(numero & 0x000F0000) >> 16];
  hexa[5] = letras[(numero & 0x00F00000) >> 20];
  hexa[6] = letras[(numero & 0x0F000000) >> 24];
  hexa[7] = letras[(numero & 0xF0000000) >> 28];
  for (i = 0; i < size; i++) {
    p[y][x + size - i - 1].c = hexa[i];
    p[y][x + size - i - 1].a = attr;
  }
}

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
  uint32_t cSize, uint8_t character, uint8_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = fInit; f < fInit + fSize; f++) {
    for (c = cInit; c < cInit + cSize; c++) {
      p[f][c].c = character;
      p[f][c].a = attr;
    }
  }
}

void print_libretas() {
  char* lib1 = "310/17";
  char* lib2 = "236/18";
  char* lib3 = "752/19";
  print(lib1, 38, 19, C_BG_BLACK | C_FG_RED);
  print(lib2, 38, 20, C_BG_BLACK | C_FG_RED);
  print(lib3, 38, 21, C_BG_BLACK | C_FG_RED);
}

void print_debug_mode(bool debug_mode) {
  print(debug_mode ? "debug" : "     ", 1, VIDEO_FILS - 1, C_BG_BLACK | C_FG_WHITE);
}

void screen_update_spawned(lemming_team_e team, size_t spawned) {
  uint8_t col = team == LEMMING_A ? 12 : 64;
  print_dec(spawned, 6, col, 45, (team == LEMMING_A ? LEMMING_A_BG_COLOR : LEMMING_B_BG_COLOR) | C_FG_WHITE);
}

//imprime reloj si lemmin esta vivo, sino XX
void screen_print_lemming_status(task_id_t lemming_id, bool alive) {
  lemming_team_e team = lemming_team(lemming_id);
  uint8_t col_start = team == LEMMING_A ? 21 : 41;
  uint8_t offset = (lemming_id % MAX_LEMMINGS_TEAM) * 4;
  uint16_t attrs = alive ? C_BG_BLACK | C_FG_WHITE : (team == LEMMING_A ? C_BG_BLACK | LEMMING_A_FG_COLOR : C_BG_BLACK | LEMMING_B_FG_COLOR);
  if (alive) {
    // alive ? "OO"
    lemming_clock_counter[lemming_id]++;
    if (lemming_clock_counter[lemming_id] >= CLOCK_CHARS) lemming_clock_counter[lemming_id] = 0;
    print_char(lemming_clock[lemming_clock_counter[lemming_id]], col_start + offset, 43, attrs);
    print_char(' ', col_start + offset + 1, 43, C_BG_BLACK | C_FG_BLACK);
  } else {
    print("XX", col_start + offset, 43, attrs);
  }

}

void screen_init_layout() {
  screen_draw_box(MAP_ROWS, 0, 9, VIDEO_COLS, ' ', C_BG_BLACK | C_FG_BLACK);
  print("Amalin", 10, 41, C_BG_BLACK | LEMMING_A_FG_COLOR);
  print("00  01  02  03  04", 21, 41, C_BG_BLACK | LEMMING_A_FG_COLOR);
  print("00  01  02  03  04", 41, 41, C_BG_BLACK | LEMMING_B_FG_COLOR);
  print("Betarote", 62, 41, C_BG_BLACK | LEMMING_B_FG_COLOR);

  for (task_id_t i = 0; i < MAX_LEMMINGS; i++) {
    screen_print_lemming_status(i, false);
  }

  screen_draw_box(44, 10, 3, 10, ' ', LEMMING_A_BG_COLOR | LEMMING_A_FG_COLOR);
  screen_update_spawned(LEMMING_A, 0);

  screen_draw_box(44, 62, 3, 10, ' ', LEMMING_B_BG_COLOR | LEMMING_B_FG_COLOR);
  screen_update_spawned(LEMMING_B, 0);
}

void print_game_end(lemming_team_e winner, size_t spawned_a, size_t spawned_b) {
  screen_draw_box(0, 0, VIDEO_FILS - 2, VIDEO_COLS, ' ', C_BG_BLACK | C_FG_BLACK);
  uint16_t attrs = (winner == LEMMING_A ? LEMMING_A_BG_COLOR : LEMMING_B_BG_COLOR) | C_FG_WHITE;
  uint16_t attrs_box = winner == LEMMING_A ? (LEMMING_A_BG_COLOR | LEMMING_A_FG_COLOR) : (LEMMING_B_BG_COLOR | LEMMING_B_FG_COLOR);
  screen_draw_box(0, 21, 40, 40, ' ', attrs_box);

  uint32_t len = print("Juego terminado! Ganador: ", 23, 20, attrs);
  print(winner == LEMMING_A ? "Amalin" : "Betarote", 23 + len + 1, 20, attrs);

  screen_draw_box(44, 10, 3, 10, ' ', LEMMING_A_BG_COLOR | LEMMING_A_FG_COLOR);
  screen_update_spawned(LEMMING_A, spawned_a);
  screen_draw_box(44, 62, 3, 10, ' ', LEMMING_B_BG_COLOR | LEMMING_B_FG_COLOR);
  screen_update_spawned(LEMMING_B, spawned_b);
}
