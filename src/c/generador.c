#include <stdio.h>
#include <stdlib.h>
#include "ledger.h"

static uint32_t estado = 2024;
static uint32_t rnd(void) {
    estado = estado * 1664525u + 1013904223u;
    return estado >> 8;
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 500;
    if (n <= 0 || n > 1000000) { fprintf(stderr, "uso: %s [num_transacciones]\n", argv[0]); return 1; }

    FILE *f = fopen(LF_ARCHIVO_TRX, "w");
    if (!f) { perror(LF_ARCHIVO_TRX); return 1; }

    for (int i = 0; i < n; i++) {
        int cuenta = 100001 + (int)(rnd() % 20);
        char tipo  = (rnd() % 100 < 45) ? 'D' : 'C';
        int monto  = 500 + (int)(rnd() % 250000);
        int mes = 1 + (int)(rnd() % 12), dia = 1 + (int)(rnd() % 28);
        fprintf(f, "%06d%c%09d%04d%02d%02d\n", cuenta, tipo, monto, 2026, mes, dia);
    }
    fclose(f);

    f = fopen(LF_ARCHIVO_TRX, "rb");
    fseek(f, 0, SEEK_END); long len = ftell(f); rewind(f);
    uint8_t *buf = malloc((size_t)len);
    if (fread(buf, 1, (size_t)len, f) != (size_t)len) { perror("fread"); return 1; }
    fclose(f);

    uint32_t sum = adler32_asm(buf, (size_t)len);
    free(buf);

    f = fopen(LF_ARCHIVO_SUM, "w");
    fprintf(f, "%08x\n", sum);
    fclose(f);

    printf("[C]    %d transacciones generadas (%ld bytes), Adler-32 = %08x\n", n, len, sum);
    return 0;
}
