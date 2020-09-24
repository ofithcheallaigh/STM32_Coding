// Toggles LED at 1Hz, using timer2 compare mode
// LED can be replaced by any periphel as needed

#include "stm32f4xx.h"                  // Device header

int main(void)
{
	RCC->AHB1ENR |= 0x01;
	GPIOA->MODER |= 0x800;								// Sets PA5 to alternate function 
	GPIOA->AFR[0] |= 0x00100000;					// Set pin AF1 for TIM2 CH1
	
	RCC->APB1ENR |= 0x01;
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = 10000-1;
	TIM2->CCMR1 = 0x30; 									// Set output to toggle on match
	TIM2->CCR1 = 0x00;										// Set match mode
	TIM2->CCER |= 0x01;										// Enable chanel 1 compare mode
	TIM2->CNT = 0x00;											// Clear counter
	TIM2->CR1 = 0x01;											// Enable TIM2
	
	while(1)
	{
		// Do nothing
	}
}
