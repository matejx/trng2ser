#ifndef MAT_TRNG_H
#define MAT_TRNG_H

#include <inttypes.h>

uint8_t rng_init(void);
uint8_t rng_get(uint32_t* r);

#endif
