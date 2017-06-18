
#include "stm32f4xx.h"

uint8_t rng_init(void)
{
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);
	RNG_ClearITPendingBit(RNG_IT_CEI);
	RNG_ClearITPendingBit(RNG_IT_SEI);
	return 0;
}

uint8_t rng_get(uint32_t* r)
{
	while( RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET );

	if( RNG_GetITStatus(RNG_IT_CEI) == SET ) return 1; // clock error
	if( RNG_GetITStatus(RNG_IT_SEI) == SET ) return 2; // seed error

	*r = RNG_GetRandomNumber();

	return 0;
}
