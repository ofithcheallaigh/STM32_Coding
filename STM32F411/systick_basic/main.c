#include "stm32f4xx.h"                  // Device header


int main(void)
{
	RCC->AHB1ENR |= 0x01;									// Enables bus for LED
	GPIOA->MODER |= 0x400;								// PA5 as output
	
	// Configure systick 
	SysTick->LOAD = 1600000 - 1;					// 100ms delay
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
	
	while(1)
	{
		if(SysTick->CTRL & 0x10000)					// Check if the flag is set
		{
			GPIOA->ODR ^= 0x20;
		}
	}
}
