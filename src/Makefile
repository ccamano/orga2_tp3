# ** por compatibilidad se omiten tildes **
# ==============================================================================
# TRABASO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
# ==============================================================================

BUILDDIR = ../build

MCOPY = mcopy

KERNEL_OBJ = kernel.o
KERNEL_BIN = $(addprefix $(BUILDDIR)/, kernel.bin)

OBJS = $(KERNEL_OBJ) screen.o game.o map.o
OBJ = $(addprefix $(BUILDDIR)/, $(OBJS))
LINK_OBJS = sched.o tss.o gdt.o mmu.o idt.o isr.o pic.o handlers.o utility.o
LINK_OBJ = $(addprefix $(BUILDDIR)/, $(LINK_OBJS))

# Dependencias, generado por el flag -MMD de gcc, util para recompilar cuando actualizamos headers.
DEP = $(OBJ:%.o=%.d)
DISK_IMG = diskette.img
DISK_IMG_DST = $(addprefix $(BUILDDIR)/, $(DISK_IMG))
DISK_IMG_BZ = diskette.img.bz2

CFLAGS=-Wall -Wextra -pedantic -std=c99 -Werror -Og -ggdb \
  -m32 -ffreestanding -fno-pie -fno-zero-initialized-in-bss -fno-stack-protector

TASKS = $(addprefix $(BUILDDIR)/, tareas.tsk)

CODESIZE=$$((0x18000 - 0x1200))
TASKSSIZE=$$((2 * (0x1000) * 2 + 0x1000)) # Dos tareas de 2 paginas + 1 pagina idle.
KERNELSIZE=$$(($(CODESIZE)+$(TASKSSIZE)))

CC=gcc
NASM=nasm
NASMFLAGS=-felf32 -g -F DWARF
OBJCOPY=objcopy
NM=nm
LD=ld
LDFLAGS=-static -m elf_i386 -b elf32-i386 -e start -Ttext 0x1200

QUIET = @

.PHONY: all image tasks memory interrupt common

all: tasks memory interrupt common image 

format:
	clang-format -i *.h *.c

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(BUILDDIR)/%.o: %.asm
	$(NASM) $(NASMFLAGS) $< -MD $(basename $@).d -o $@

seed:
	RNG=$$(head -c 32 /dev/urandom | shasum -a 256 | cut -c1-16);\
	sed -i "s/seed.*/seed=0x$${RNG}ull;/" seed.c;\

-include $(DEP)

$(KERNEL_BIN): | tasks memory interrupt common $(OBJ)
	@echo 'Linkeando kernel...'
	$(LD) $(LDFLAGS) -o $@.elf $(OBJ) $(LINK_OBJ)
	@echo 'Extrayendo tabla de simbolos...'
	$(NM) $@.elf | grep -v " U " | awk '{print "0x" $$1 " " $$3}' > $(BUILDDIR)/kernel.sym
	@echo 'Generando binario sin informacion elf...'
	$(OBJCOPY) -S -O binary $@.elf $@
	@echo ''
	@echo 'Generando imagen del kernel...'
	mv $@ $@.tmp
	dd if=/dev/zero of=$@ bs=1 count=$(KERNELSIZE) conv=notrunc status=noxfer > /dev/null
	dd if=$@.tmp of=$@ bs=1 count=$(CODESIZE) conv=notrunc status=noxfer > /dev/null
	dd if=$(TASKS) of=$@ bs=1 count=$(TASKSSIZE) seek=$(CODESIZE) conv=notrunc status=noxfer > /dev/null
	rm $@.tmp
	@echo ''

image: $(KERNEL_BIN) $(DISK_IMG)
	@echo 'Copiando el $(KERNEL_BIN) a la imagen de diskette'
	$(MCOPY) -o -i $(DISK_IMG_DST) $(KERNEL_BIN) ::/
	@echo ''

$(DISK_IMG): $(DISK_IMG_BZ)
	bzip2 -dk $(DISK_IMG_BZ)
	mv $(DISK_IMG) $(DISK_IMG_DST)


tasks:
	make -C tasks

memory:
	make -C memory

interrupt:
	make -C interrupt

common:
	make -C common
