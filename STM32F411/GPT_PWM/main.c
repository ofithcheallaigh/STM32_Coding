#include "stm32f4xx.h"                  // Device header

/*
 * The O/P of channel 1 is turned on when the counter starts counting from 0
 * When the counter matches the contents of the CCR1, Channel 1 output is turned off
 * When the counter matches ARR the counter is cleared to zeroand the output is turned on
 * The counter starts counting up again. 
 *
 * The LED is going to be on for 8889/26667, which is about 30% of the time
 */

int main(void)
{
	RCC->AHB1ENR |= 0x01;
	GPIOA->AFR[0] |= 0x00100000;					// Set PA5 as alt function got TIM2
	GPIOA->MODER |= 0x00000800;
	
	// Timer set up
	RCC->APB1ENR |= 0x01;
	TIM2->PSC = 10 - 1;										// Divide by 10
	TIM2->ARR = 26667 - 1;								// Further divide
	TIM2->CNT  |= 0x00;
	TIM2->CCMR1 |= 0x0060;								// Enable PWM mode
	TIM2->CCER |= 0x01;										// Enable PWM on Ch1
	TIM2->CCR1 = 8889 - 1;								// Sets pulse width to 1/3 of the period
	TIM2->CR1 |= 0x01;
	
	while(1)
	{
		// Do nothing
	}
	
}

