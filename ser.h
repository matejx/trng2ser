#ifndef MAT_SERIALQ_H
#define MAT_SERIALQ_H

#include <inttypes.h>

void ser_init(const uint8_t devnum, const uint32_t br);

void ser_putc(const uint8_t devnum, const char a);
void ser_puts(const uint8_t devnum, const char* s);

#endif
