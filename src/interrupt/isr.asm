; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "../common/print.mac"

%define mem_keyboard_read 0x60

BITS 32

sched_task_offset:     dd 0x0
sched_task_selector:   dw 0x0

;; PIC
extern pic_finish1

;; Handlers
extern handle_keyboard_int
extern handle_pagefault
extern handle_exception

;; Sched
extern switch_to_idle

;; Game
extern game_tick

; syscalls
extern move
extern explode
extern bridge

;; Global 
global _isr14
global _isr32
global _isr33
global _isr88
global _isr98
global _isr108

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:

    ; xchg bx, bx
    pushad

    ; eax <- type
    push %1
    ; la tarea en ejecucion va a ser desalojada
    call handle_exception
    ; ciclamos infinitamente por las dudas
    mov ax, %1
    jmp $

%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
; ISR 14 page fault
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;; Pagefault
_isr14:
    pushad
    
    call handle_pagefault

    popad
    ; salteamos el error code, dejamos el esp apuntando al eip
    add esp, 4
    iret

;; Rutina de atención del RELOJ
_isr32:
    pushad   
    ; avisar al pic que se recibió la interrupción
    call pic_finish1

    ; imprimir el reloj del sistema
    call next_clock

 
    ; avanza reloj del juego
    call game_tick
    
    ; por cada ciclo de reloj intercambiamos tareas 

    ;si el proximo es cero indica que no debe cambiar tarea
    cmp ax, 0
    je .fin
    
    str cx      ; comparamos con la tarea actual
    cmp ax, cx
    je .fin

        mov word [sched_task_selector], ax
        jmp far [sched_task_offset]

    .fin:
    popad
    iret

;; -------------------------------------------------------------------------- ;;
;; Rutina de atención del TECLADO
_isr33:
    pushad
    
    in al, mem_keyboard_read
    ; cargamos en al el scan code
    push eax
    call handle_keyboard_int
    add esp, 4
    
    ; avisar al pic que se recibió la interrupción
    call pic_finish1
    
    popad
    iret
;; -------------------------------------------------------------------------- ;;
;; Rutinas de atención de las SYSCALL
;; move
_isr88:
    ;pusheamos a mano en vez de usar pushad y popad
    ;para preservar el eax
    push edi
    push esi
    push ecx
    push edx

    ; en eax la direccion en la que nos queremos mover
    push eax
    call move
    ; luego de un move, el scheduler pasa a idle 
    call switch_to_idle

    add esp, 4

    pop edx
    pop ecx
    pop esi
    pop edi
    iret
    
;; explode
_isr98:
    ; no hace falta preservar registros porque la tarea va ser desalojada
    call explode
    ; aca nunca pasa, explode mata a la tarea en ejecucion
    mov eax, 23
    jmp $

    
;; bridge   
_isr108:
    ; no hace falta preservar registros porque la tarea va ser desalojada
    push eax
    call bridge
    ; aca nunca pasa, bridge mata a la tarea en ejecucion
    mov eax, 24
    jmp $

;; -------------------------------------------------------------------------- ;;
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret
