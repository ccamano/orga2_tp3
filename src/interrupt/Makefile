# ** por compatibilidad se omiten tildes **
# ==============================================================================
# TRABASO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
# ==============================================================================
BUILDDIR = ../../build

OBJS = idt.o isr.o pic.o handlers.o
OBJ = $(addprefix $(BUILDDIR)/, $(OBJS))
# Dependencias, generado por el flag -MMD de gcc, util para recompilar cuando actualizamos headers.
DEP = $(OBJ:%.o=%.d)

CFLAGS=-Wall -Wextra -pedantic -std=c99 -Werror -Og -ggdb \
  -m32 -ffreestanding -fno-pie -fno-zero-initialized-in-bss -fno-stack-protector


CC = gcc
NASM = nasm
NASMFLAGS = -felf32 -g -F DWARF

.PHONY: all

all: $(OBJ)

format:
	clang-format -i *.h *.c

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(BUILDDIR)/%.o: %.asm
	$(NASM) $(NASMFLAGS) $< -MD $(basename $@).d -o $@

-include $(DEP)
