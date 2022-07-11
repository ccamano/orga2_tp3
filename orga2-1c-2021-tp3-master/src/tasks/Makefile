# ** por compatibilidad se omiten tildes **
# ==============================================================================
# TRABASO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
# ==============================================================================
BUILDDIR = ../../build
OBJS = tss.o sched.o
OBJ = $(addprefix $(BUILDDIR)/, $(OBJS))
# Dependencias, generado por el flag -MMD de gcc, util para recompilar cuando actualizamos headers.
DEP = $(OBJ:%.o=%.d)

CFLAGS=-Wall -Wextra -pedantic -std=c99 -Werror -Og -ggdb \
  -m32 -ffreestanding -fno-pie -fno-zero-initialized-in-bss -fno-stack-protector

TASKSCFLAGS=$(CFLAGS) -fno-toplevel-reorder

TASKIDLE = $(addprefix $(BUILDDIR)/, idle.tsk)
TASKLEMMINGA = $(addprefix $(BUILDDIR)/, taskLemmingA.tsk)
TASKLEMMINGB = $(addprefix $(BUILDDIR)/, taskLemmingB.tsk)

TASKS = $(addprefix $(BUILDDIR)/, tareas.tsk)

TASKSBINS = $(TASKIDLE) $(TASKLEMMINGA) $(TASKLEMMINGB)

TASKSSIZE = $$((2 * (0x1000) * 2 + 0x1000)) # Dos tareas de 2 paginas + 1 pagina idle.

# Direccion virtual donde empieza el codigo de las tareas.
TASKSTARTADDR = 0x08000000

CC = gcc
NASM = nasm
NASMFLAGS = -felf32 -g -F DWARF
LD = ld
LDTASKFLAGS = -s -static -m elf_i386 --oformat binary -b elf32-i386 -e task -Ttext $(TASKSTARTADDR)

.PHONY: all tasks

all: $(OBJ) tasks

format:
	clang-format -i *.h *.c

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(BUILDDIR)/%.o: %.asm
	$(NASM) $(NASMFLAGS) $< -MD $(basename $@).d -o $@

-include $(DEP)

$(TASKIDLE): idle.asm
	@echo 'Compilando tarea idle...'
	$(NASM) -fbin -o $@ $^
	@echo ''
	@echo 'Generando imagen de la tarea idle...'
	mv $@ $@.tmp
	dd if=/dev/zero bs=1 count=4K of=$@ status=noxfer > /dev/null 2>&1
	dd if=$@.tmp bs=1 count=4K seek=0 conv=notrunc of=$@ status=noxfer > /dev/null 2>&1
	rm $@.tmp
	@echo ''

$(BUILDDIR)/task%.tsko: task%.c
	$(CC) $(TASKSCFLAGS) -MMD -c -o $@ $^

$(BUILDDIR)/%.tsk: $(BUILDDIR)/%.tsko
	@echo 'Linkeando tarea...'
	$(LD) $(LDTASKFLAGS) -o $@.tmp $^
	@echo ''
	mv $@.tmp $@

tasks: $(TASKSBINS)
	@echo 'Generando imagen de las tareas...'
	$(QUIET) dd if=/dev/zero   bs=1 count=$(TASKSSIZE) of=$(TASKS) status=noxfer > /dev/null 2>&1
	$(QUIET) dd if=$(TASKLEMMINGA)   bs=1 count=8K of=$(TASKS) seek=0K   status=noxfer > /dev/null 2>&1
	$(QUIET) dd if=$(TASKLEMMINGB)   bs=1 count=8K of=$(TASKS) seek=8K  status=noxfer > /dev/null 2>&1
	$(QUIET) dd if=$(TASKIDLE) bs=1 count=4K of=$(TASKS) seek=16K  status=noxfer > /dev/null 2>&1
	@echo ''
