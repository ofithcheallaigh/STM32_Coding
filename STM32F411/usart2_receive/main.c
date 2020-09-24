#include "stm32f4xx.h"                  // Device header

void delayMS(int delay);
void USART2_Init(void);
char USART2_read(void);
void LED_play(int value);

int main(void)
{
	RCC->AHB1ENR |= 0x01;									// Enables clock for Port A
	GPIOA->MODER |= 0x400;								// Sets the pin as output -- PA5
	
	USART2_Init();													// Run my init function
	char ch;															// Variable used to store read in charater
	while(1)
	{
		ch = USART2_read();
		LED_play(ch);
	}
}

void USART2_Init(void)
{
	RCC->AHB1ENR |= 0x01; 								// Enable GPIOA Clock
	RCC->APB1ENR |= 0x20000;							// Enable USART2 Clock
	
	// Configure PA3 as USART2 Rx
	GPIOA->AFR[0] |= 0x7000;								// USART connected by ALT Function 7
	GPIOA->MODER |= 0x0080;								// Enables alternative function
	
	USART2->BRR |= 0x008B;								// 115200 baud rate @ 16MHz
	USART2->CR1 |= 0x0004;								// Enables Rx
	USART2->CR1 |= 0x2000;								// Enables USART 2
}

char USART2_read(void)
{
	while(!(USART2->SR & 0x0020))					// Wait until character arrives
	{
		
	}
	return USART2->DR;
}

void LED_play(int value)
{
	value %= 16;
	for(; value > 0; value--)
	{
		GPIOA->BSRR = 0x20;									// Turns on the LED
		delayMS(100);
		GPIOA->BSRR = 0x00200000;						// Turns off LED
		delayMS(100);
	}
	delayMS(400);
}


void delayMS(int delay)
{
	int i;
	for(; delay > 0; delay--)
	{
		for(i = 0; i < 3195; i++); 
	}
}

