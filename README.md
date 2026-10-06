# LedgerForge

Pipeline contable donde cada lenguaje hace lo que mejor sabe:

| Lenguaje | Archivo | Rol |
|---|---|---|
| **C** | `src/c/generador.c` | Genera transacciones de ancho fijo (datos deterministas) |
| **Ensamblador** (NASM x86-64) | `src/asm/checksum.asm` | Adler-32 sin saltos (`cmov`), usado por C y C++ |
| **COBOL** (GnuCOBOL) | `src/cobol/saldos.cob` | Procesa el archivo y calcula saldos por cuenta |
| **C++17** | `src/cpp/analisis.cpp` | Verifica integridad, valida a COBOL con un cálculo independiente y saca estadísticas |

```
generador (C) ──► transacciones.dat ──► saldos (COBOL) ──► saldos.dat ──► analisis (C++)
       └── adler32_asm ──► .adler ───────────────────────────────────────────┘ (verifica)
```

## Requisitos (Ubuntu/Debian)
    sudo apt install build-essential nasm gnucobol

## Uso
    make run          # compila y ejecuta todo
    ./build/generador 100000   # más transacciones
    make clean

## Ideas para seguir
- Versión SIMD (SSE2/AVX2) del Adler-32 en ensamblador
- Reglas de negocio en COBOL (sobregiro, comisiones, cierre mensual)
- Leer transacciones con `mmap` en C++
