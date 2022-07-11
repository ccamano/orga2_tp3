; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "common/print.mac"

%define CODE0_SELECTOR (0x8 << 3)
%define DATA0_SELECTOR (0x9 << 3)
%define VIDEO_SELECTOR (0xC << 3)
%define VIDEO_OFFSET 0x18000 
%define VIDEO 0xB8000

%define STACK_BASE 0x25000

%define TSS_INIT_DESC (13 << 3)
%define TSS_IDLE_DESC (14 << 3)

global start
extern GDT_DESC
extern IDT_DESC
extern idt_init

extern pic_reset
extern pic_enable

extern mmu_init
extern mmu_init_kernel_dir

extern tss_init
extern tss_init_idle

extern sched_init

extern game_init

extern print_libretas

BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0


    ; Habilitar A20
    call A20_enable
    call A20_check
    
    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1b
    mov cr0, eax
    
    ; Saltar a modo protegido
    jmp CODE0_SELECTOR:modo_protegido

BITS 32    
modo_protegido:

    ; Establecer selectores de segmentos
    mov ax, DATA0_SELECTOR  ; selector de segmento de datos nivel 0
    mov ds, ax              ; ds = selector de segmento de datos
    mov es, ax              ; es = selector de segmento de datos
    mov fs, ax              ; fs = selector de segmento de datos
    mov gs, ax              ; gs = selector de segmento de datos
    mov ss, ax              ; ss = selector de segmento de pila

    ; Establecer la base de la pila
    mov ebp, STACK_BASE
    mov esp, STACK_BASE
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 3, 0

    ; Inicializar pantalla
    mov ax, VIDEO_SELECTOR
    mov fs, ax
    call screen_init_fs

    ; Inicializar el manejador de memoria
    call mmu_init

    ; Inicializar el directorio de paginas
    call mmu_init_kernel_dir

    ; Cargar directorio de paginas
    mov cr3, eax ; mmu_init_kernel_dir devuelve la información que necesitamos cargar en cr3

    ; Habilitar paginacion
    mov eax, cr0        
    or eax, 0x80000000  ; activa el último bit de cr0: PG (paginación)
    mov cr0, eax        ; mueve a cr0 para activar paginación

    ; ==============================================================================
    ; 4c
    call print_libretas
    ; ==============================================================================

    ; ==============================================================================
    ; 5d - no debe estar implementado en la solucion final
    ; mov eax, cr3 
    ; push eax

    ; push 0x18000 ; inicio codigo Amalin
    ; call mmu_init_task_dir
    ; add esp, 4

    ; mov cr3, eax ; cargamos en cr3 para que apunte al DT de la tarea

    ; add byte [VIDEO + 1], 2

    ; ; cargamos el cr3 del kernel
    ; pop eax
    ; mov cr3, eax
    ; ==============================================================================

    ; Inicializar tss
    call tss_init

    ; Inicializar tss de la tarea Idle
    call tss_init_idle
    
    ; Inicializar el scheduler
    call sched_init

    ; Inicilizar el juego
    call game_init

    ; Inicializar la IDT
    call idt_init
    
    ; Cargar IDT
    lidt [IDT_DESC]
    
    ; Configurar controlador de interrupciones
    call pic_reset      ; carga la configuración del pic
    call pic_enable

    ; Cargar tarea inicial
    mov ax, TSS_INIT_DESC
    ltr ax

    ; ; Habilitar interrupciones -> las habilita el cambio de tarea
    ; sti

    ; ==============================================================================
    ; prueba excepciones
    ; int3 ; interrupcion tipo 3, breakpoint
    ; mov al, 01111111b
    ; add al, al
    ; into  ; exception tipo 4, overflw
    ; xor eax, eax    ; dividir por 0, excepcion tipo 0
    ; idiv eax

    ; int 88
    ; int 98
    ; int 108
    ; ==============================================================================

    ; Saltar a la primera tarea: idle
    jmp TSS_IDLE_DESC:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
;; Fin del código del kernel

global switch_to_idle

switch_to_idle:
    mov dx, TSS_IDLE_DESC
    str cx
    cmp dx, cx
    ; comparamos que no estamos en la tarea idle
    ; (no deberia pasar que salte a .fin)
    je .fin

        jmp TSS_IDLE_DESC:0

.fin:
    ret





;;
;; Función para inicializar pantalla
;; Ejercicio 1d
;; -------------------------------------------------------------------------- ;;
%define color_verde 0x02
%define color_rojo 0x04
%define color_azul 0x01
%define color_blanco 0x0f
%define color_negro 0x00
%define caracter_background 219

amalin_equipo: times 10 db ' ',
db    'Amalin',
times 6 db ' ',
db '00  01  02  03  04  '
amalin_equipo_len: equ $ - amalin_equipo
betarote_equipo: db  '00  01  02  03  04   Betarote',
times 11 db ' '
betarote_equipo_len: equ $ - betarote_equipo
estado_inicial_lemmings: times 20 db ' ',
db '-   -   -   -   -   -   -   -   -   -'
estado_inicial_lemmings_len: equ $ - estado_inicial_lemmings
final_pantalla: times 10 db caracter_background, color_negro
times 10 db caracter_background, color_rojo
times 43 db caracter_background, color_negro
times 10 db caracter_background, color_azul
times 7 db caracter_background, color_negro
; primera fila
times 10 db caracter_background, color_negro
times 2 db caracter_background, color_rojo
times 6 db '0', color_blanco
times 2 db caracter_background, color_rojo
times 43 db caracter_background, color_negro
times 2 db caracter_background, color_azul
times 6 db '0', color_blanco
times 2 db caracter_background, color_azul
times 7 db caracter_background, color_negro
; segunda fila
times 10 db caracter_background, color_negro
times 10 db caracter_background, color_rojo
times 43 db caracter_background, color_negro
times 10 db caracter_background, color_azul
times 7 db caracter_background, color_negro

final_pantalla_len: equ ($ - final_pantalla) >> 1


; en fs el selector de segmento que apunta a la memoria de video
screen_init_fs:
    mov ebx, VIDEO_OFFSET    ; ebx = puntero a la base del buffer de video
    mov ecx, 0
    mov ah, color_verde     ; ah = color
    ; shl al, 8
    mov al, 219        ; al = caracter
.cicloMapa:
    cmp ecx, (80 * 40)
    je .finCicloMapa

    mov [fs:ebx + ecx * 2],  ax

    inc ecx
    jmp .cicloMapa
.finCicloMapa:
    lea ebx, [ebx + ecx * 2]
    add ebx, (80 * 2) ; salteamos una fila
    mov ah, color_rojo
    ; pintamos Amalin
    mov ecx, amalin_equipo_len
    mov edi, amalin_equipo
.cicloAmalin:
    cmp ecx, 0
    je .finCicloAmalin

    mov al, [edi]
    mov [fs:ebx],  ax

    inc edi
    add ebx, 2
    dec ecx
    jmp .cicloAmalin

 .finCicloAmalin:
    ; pintamos Betarote
    mov ah, color_azul
    mov ecx, betarote_equipo_len
    mov edi, betarote_equipo
.cicloBetarote:
    cmp ecx, 0
    je .finCicloBetarote

    mov al, [edi]
    mov [fs:ebx],  ax

    inc edi
    add ebx, 2
    dec ecx
    jmp .cicloBetarote

 .finCicloBetarote:
 ;pintamos estado inicial lemmings
    mov ah, color_blanco
    mov ecx, estado_inicial_lemmings_len
    mov edi, estado_inicial_lemmings
.cicloEstadoLemmings:
    cmp ecx, 0
    je .finCicloEstadoLemmings

    mov al, [edi]
    mov [fs:ebx],  ax

    inc edi
    add ebx, 2
    dec ecx
    jmp .cicloEstadoLemmings

 .finCicloEstadoLemmings:
 ;pintamos los rectangulos
    mov ebx, (VIDEO_OFFSET + (80 * 43 * 2))
    mov ecx, final_pantalla_len
    mov edi, final_pantalla
.cicloFinalPantalla:
    cmp ecx, 0
    je .finCicloFinalPantalla

    mov ax, [edi]
    mov [fs:ebx],  ax

    add edi, 2
    add ebx, 2
    dec ecx
    jmp .cicloFinalPantalla

 .finCicloFinalPantalla:
    ret
%include "memory/a20.asm"
