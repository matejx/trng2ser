/**
trng2ser - send random data generated by TRNG over USART1

@file		main.c
@author		Matej Kogovsek (matej@hamradio.si)
@copyright	GPL v2
*/

#include "stm32f4xx.h"
#include "ser.h"
#include "rng.h"

//-----------------------------------------------------------------------------
//  Global variables
//-----------------------------------------------------------------------------

volatile uint32_t msTicks;	// counts SysTicks

//-----------------------------------------------------------------------------
//  newlib required functions
//-----------------------------------------------------------------------------

void _exit(int status)
{
	while(1) {}
}

//-----------------------------------------------------------------------------
//  SysTick handler
//-----------------------------------------------------------------------------

void SysTick_Handler(void)
{
	msTicks++;			// increment counter necessary in _delay_ms()
}

//-----------------------------------------------------------------------------
//  delay functions
//-----------------------------------------------------------------------------

void _delay_ms(uint32_t ms)
{
	uint32_t curTicks = msTicks;
	while ((msTicks - curTicks) < ms);
}

void DDR(GPIO_TypeDef* port, uint16_t pin, GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef iotd;
	iotd.GPIO_Pin = pin;
	iotd.GPIO_Speed = GPIO_Speed_2MHz;
	iotd.GPIO_Mode = mode;
	iotd.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(port, &iotd);
}

//-----------------------------------------------------------------------------
//  MAIN function
//-----------------------------------------------------------------------------
// LED1 PA6
// LED2 PA7
// KEY0 PE3
// KEY1 PE4

int main(void)
{
	if( SysTick_Config(SystemCoreClock / 1000) ) { // setup SysTick Timer for 1 msec interrupts
		while( 1 );                                  // capture error
	}

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); // disable preemption

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// configure LED2 pin as output
	DDR(GPIOA, GPIO_Pin_6, GPIO_Mode_OUT);
	// turn LED2 off
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);

	ser_init(1, 115200);

	rng_init();

	while( 1 ) {
		uint32_t rnd;
		if( 0 == rng_get(&rnd) ) {
			ser_putc(1, rnd);
			ser_putc(1, rnd >> 8);
			ser_putc(1, rnd >> 16);
			ser_putc(1, rnd >> 24);
		} else {
			// something went wrong with TRNG, turn LED2 on
			GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
		}
	}
}
