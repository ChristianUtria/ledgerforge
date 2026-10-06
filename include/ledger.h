#ifndef LEDGER_H
#define LEDGER_H
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t adler32_asm(const uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

#define LF_REG_LEN 24
#define LF_ARCHIVO_TRX "data/transacciones.dat"
#define LF_ARCHIVO_SUM "data/transacciones.adler"
#define LF_ARCHIVO_SAL "data/saldos.dat"

#endif
