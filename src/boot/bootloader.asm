
bits 64

section .multiboot
        dd 0x1BADB002
        dd 0x0                   ;TODO add any needed flags
        dd - (0x1BADB002 + 0x0)  ;checksum

section .text
global start
extern main         

start:
        cli
        mov esp, stack_space
        call main
        hlt

        ; TODO create multilevel page table for 64 bit
        ; TODO add bigger stack space



section .bss
resb 8192
stack_space:
