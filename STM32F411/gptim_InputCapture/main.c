#include "stm32f4xx.h"                  // Device header

int timeStamp;

int main(void)
{
	RCC->AHB1ENR |= 0x01;
	GPIOA->MODER |= 0x800;								// Sets PA5 as alternatibe function
	GPIOA->AFR[0] |= 0x00100000;					// Set pin to AF for TIM2 Ch1
	
	RCC->APB1ENR |= 0x01;
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = 10000 - 1;
	TIM2->CCMR1 = 0x30;										// Sets output to toggle on match
	TIM2->CCR1 = 0;												// Set match value
	TIM2->CCER = 0x01;										// Enable Ch1 compare mode
	TIM2->CNT = 0;												// Clear counter
	TIM2->CR1 = 0x01;											// Enable timer 2
	
	// Configure PA6 as input of TIM3 CH1
	RCC->AHB1ENR |= 0x01;									// Used for modular design purposes
	GPIOA->MODER |= 0x2000;								// Sets PA6 to alt function
	GPIOA->AFR[0] |= 0x02000000;					// Set Pin2 AF2 for TIM3 CH1
	
	// Configure TIM3
	RCC->APB1ENR |= 0x02;									// Enables TIM3
	TIM3->PSC = 16000;										// Divide by 16000
	TIM3->CCMR1 = 0x41;										// Set CH1 to capture at every edge
	TIM3->CCER = 0x01;										// Enables it to capture at the rising edge
	TIM3->CR1 = 0x01;
	
	while(1)
	{
		while(!(TIM3->SR & 0x02))
		{
			// Do nothing
		}
		timeStamp = TIM3->CCR1;
		
	}
	
	
}
