// General Purpose Timers
// Create a 1Hz timer

#include "stm32f4xx.h"                  // Device header

int main(void)
{
	// Configure Port A
	RCC->AHB1ENR |= 0x01;
	GPIOA->MODER |= 0x400;
	
	// Cionfigure timer
	RCC->APB1ENR |= 0x01;									// Bit 1 set High, corresponds to Timer 2
	TIM2->PSC = 1600 - 1;									// 16MHz Divided by 1600 = 10000
	TIM2->ARR = 10000 - 1;								// Auto reload register. Divide by 10000 therefore 10000 / 10000 = 1 
	TIM2->CNT = 0x00;											// Clear timer counter
	TIM2->CR1 = 0x01;											// Enable TIM2
	
	while(1)
	{
		// Check to see if timer flag is set
		while(!(TIM2->SR & 1))
		{
			// If above is not true, wait here.
			// Basically, waiting to see if the timer has got to zero
			// Status register 0 is Update Interrupt Flag (UIF) and is set by HW, cleared by SW
			// If 0: no update occured
			// if 1: Update interrupt pending
		}
		TIM2->SR &=~ 0x01;
		GPIOA->ODR ^=0x20;									// Toggles LED
		
	}
}
