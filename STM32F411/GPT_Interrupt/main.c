#include "stm32f4xx.h"                  // Device header


int main(void)
{
	__disable_irq();
	RCC->AHB1ENR |= 0x01;
	GPIOA->MODER |= 0x400;								// Sets PA5 as output
	
	//TIM2
	RCC->APB1ENR |= 0x01;									// Bit 1 is for timer 2
	TIM2->PSC = 16000 - 1;
	TIM2->ARR = 1000 - 1;
	TIM2->CR1 = 1;
	TIM2->DIER |= 0x01;										// DMA/Interrupt enable register. 
																				// Bit 0 (UIE): update imterrupt enable. Set High
	
	NVIC_EnableIRQ(TIM2_IRQn);
	
	__enable_irq();
	
	while(1)
	{
	}
	
}

void TIM2_IRQHandler(void)
{
	TIM2->SR |= 0x00;											// Clears register
}
