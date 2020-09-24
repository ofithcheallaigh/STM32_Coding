// Simple ms delay

#include "stm32f4xx.h"                  // Device header

void systickDelayMs(int n);

int main(void)
{
	RCC->AHB1ENR |= 0x1;
	GPIOA->MODER |= 0x400;
	
	while(1)
	{
		systickDelayMs(200);
		GPIOA->ODR ^= 0x20;									// Toggling PA5 on and off
	}
}

void systickDelayMs(int n)
{
	SysTick->LOAD = 16000;								// Number of clock cycles in a ms
	SysTick->VAL = 0;											// Clear current value register
	SysTick->CTRL = 0x5;									// Enables SysTick
	
	for(int i = 0; i < n; i++)
	{
		// Wait here while above not zero - wait until the COUNT flag is set
		while((SysTick->CTRL & 0x10000) == 0){}
	}
	SysTick->CTRL = 0;
}
