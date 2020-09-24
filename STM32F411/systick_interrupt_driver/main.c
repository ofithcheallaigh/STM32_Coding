// Will have a 1Hz interrupt to toggle LED
// System clock is 16MHz 

#include "stm32f4xx.h"                  // Device header

int main(void)
{
	__disable_irq();
	
	RCC->AHB1ENR |= 0x01;									// Enabkes bus
	GPIOA->MODER |= 0x400;								// Sets PA5 as output
	
	SysTick->LOAD = 16000000 - 1;					// Minus 1 because we count from zero
	SysTick->VAL = 0;
	SysTick->CTRL = 0x07;
	
	__enable_irq();
	
	while(1)
	{
		// Do nothing
	}
}

void SysTick_Handler(void)
{
	GPIOA->ODR ^= 0x20;
}

