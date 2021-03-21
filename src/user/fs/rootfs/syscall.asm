[BITS 64]

; syscall numbers are defined in userland.md, syscall.asm (userland) and syscalls.h (kernel)
; process
SYSCALL_PROCESS_SLEEP           EQU	1100
SYSCALL_PROCESS_EXIT            EQU	1101
; bga
SYSCALL_BGA_GETRESOLUTION       EQU	1200
SYSCALL_BGA_SETRESOLUTION       EQU	1201
SYSCALL_BGA_GETBUFFERSIZE       EQU	1202
SYSCALL_BGA_BLT                 EQU	1203
; console
SYSCALL_CONSOLE_WRITE           EQU	1401
; keyboard
SYSCALL_KEYBOARD_READ           EQU 1600
; tick
SYSCALL_TICK_READ               EQU 2200
; serial
SYSCALL_SERIAL_READCHAR         EQU	2300
SYSCALL_SERIAL_WRITECHAR        EQU	2301
; memory
SYSCALL_MEMORY_MALLOC           EQU	2400
SYSCALL_MEMORY_FREE             EQU	2401
SYSCALL_MEMORY_REALLOC          EQU	2402
; hostid
SYSCALL_HOSTID_GETID            EQU	2700

global sycall;
global tge;

sycall:
    mov rax, SYSCALL_PROCESS_EXIT
    mov rbx, rsp
    syscall
    ret

tge:
    push rax

    mov rax, SYSCALL_CONSOLE_WRITE
    mov rbx, rsp
    syscall

    mov rax, SYSCALL_PROCESS_EXIT
    mov rbx, rsp
    syscall

    loop:
    jmp loop
