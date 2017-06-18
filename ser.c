
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "stm32f4xx.h"

struct USART_DevDef
{
	uint8_t usart_apb;
	USART_TypeDef* usart;
	uint32_t usart_clk;
	uint8_t usart_irqn;
	GPIO_TypeDef* gpio;
	uint32_t gpio_clk;
	uint16_t pin_tx;
};

struct USART_DevDef USART1_PinDef = {2, USART1, RCC_APB2Periph_USART1, USART1_IRQn, GPIOA, RCC_AHB1Periph_GPIOA, GPIO_Pin_9};

struct USART_DevDef* usart_get_pdef(uint8_t devnum)
{
	if( devnum == 1 ) return &USART1_PinDef;
	//if( devnum == 2 ) return &USART2_PinDef;
	//if( devnum == 3 ) return &USART3_PinDef;

	return 0;
}

void ser_init(const uint8_t devnum, const uint32_t br)
{
	struct USART_DevDef* pdef = usart_get_pdef(devnum);

	// clock config
	if( pdef->usart_apb == 1 ) {
		RCC_APB1PeriphClockCmd(pdef->usart_clk, ENABLE);
	} else {
		RCC_APB2PeriphClockCmd(pdef->usart_clk, ENABLE);
	}
	RCC_AHB1PeriphClockCmd(pdef->gpio_clk, ENABLE);

	// TODO: Fix for all usarts
	GPIO_PinAFConfig(pdef->gpio, GPIO_PinSource9, GPIO_AF_USART1);

	// GPIO config
	GPIO_InitTypeDef iotd;
	iotd.GPIO_Pin = pdef->pin_tx;
	iotd.GPIO_Speed = GPIO_Speed_50MHz;
	iotd.GPIO_Mode = GPIO_Mode_AF;
	iotd.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(pdef->gpio, &iotd);

	// USART config
	USART_InitTypeDef uatd;
	uatd.USART_BaudRate = br;
	uatd.USART_WordLength = USART_WordLength_8b;
	uatd.USART_StopBits = USART_StopBits_1;
	uatd.USART_Parity = USART_Parity_No;
	uatd.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uatd.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(pdef->usart, &uatd);
	//USART_ITConfig(pdef->usart, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(pdef->usart, USART_IT_TXE, ENABLE);
	USART_Cmd(pdef->usart, ENABLE);
}

void ser_putc(const uint8_t devnum, const char a)
{
	struct USART_DevDef* pdef = usart_get_pdef(devnum);

	while( RESET == USART_GetFlagStatus(pdef->usart, USART_FLAG_TXE) ) {}

	USART_SendData(pdef->usart, a);
}

void ser_puts(const uint8_t devnum, const char* s)
{
	while( *s )	{
		ser_putc(devnum, *s);
		s++;
	}
}
