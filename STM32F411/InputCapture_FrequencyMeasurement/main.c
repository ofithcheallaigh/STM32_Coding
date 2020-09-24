#include "stm32f4xx.h"                  // Device header

// Global variables
int period;
float freq;

int main(void)
{
	// Local variables
	int lastValue = 0;
	int currentValue = 0;
	
	RCC->AHB1ENR |= 0x01;									// Sets bus for PA5
	GPIOA->MODER |= 0x800;								// Select alt function mode
	GPIOA->AFR[0] |= 0x00100000;					// Set oin to AF1 for TIM2 CH1
	
	RCC->APB1ENR |= 0x01;
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = 10000 - 1;								// These two lines give us a 1Hz timer 1600 * 10000 = 16000000 -> 16000000/16000000 = 1
	TIM2->CCMR1 = 0x30;
	TIM2->CCR1 = 0x00;
	TIM2->CCER |=0x01;
	TIM2->CNT = 0x00;
	TIM2->CR1 = 0x01;
	
	
	RCC->AHB1ENR |=0x01;
	GPIOA->MODER |= 0x2000;
	GPIOA->AFR[0] |= 0x02000000;					// Set pin to AF2, the alt function for TIM3 CH1
	
	RCC->APB1ENR |= 0x02;
	TIM3->PSC = 16000 - 1;								// Divide by 16000
	TIM3->CCMR1 = 0x41;
	TIM3->CCER = 0x0B;
	TIM3->CR1 = 0x01;
	
	while(1)
	{
		while(!(TIM3->SR & 0x02))						// Wait until edge is captured
		{
			// Wait
		}
		currentValue = TIM3->CCR1;
		period = currentValue - lastValue;
		lastValue = currentValue;
		freq = 1000.0f/period;
		lastValue = currentValue;
	}
	
}

