default rel
global adler32_asm

section .text
adler32_asm:
    mov     r8d, 1
    xor     r9d, r9d
    mov     r10d, 65521
    test    rsi, rsi
    jz      .fin
.bucle:
    movzx   eax, byte [rdi]
    add     r8d, eax
    mov     ecx, r8d
    sub     ecx, r10d
    cmp     r8d, r10d
    cmovae  r8d, ecx
    add     r9d, r8d
    mov     ecx, r9d
    sub     ecx, r10d
    cmp     r9d, r10d
    cmovae  r9d, ecx
    inc     rdi
    dec     rsi
    jnz     .bucle
.fin:
    mov     eax, r9d
    shl     eax, 16
    or      eax, r8d
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
