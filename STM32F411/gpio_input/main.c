#include "stm32f4xx.h"                  // Device header

int main()
{
	RCC->AHB1ENR |= 4;										// Enables Port C. Corresponds to bit 3. 4 dec = 100 binary 
	RCC->AHB1ENR |= 1; 										// Enables Port A
	
	GPIOA->MODER |= 0x400;								// MODER is direction register
																				// We don't need to worry about GPIOC, because to set a pin as in input,
																				// you need to set it to 0, and as default, all pins are set to 0 
	
	while(1)
	{
		if(GPIOC->IDR & 0x2000)
		{
			GPIOA->BSRR = 0x002;
		}
		else
		{
			GPIOA->BSRR = 0x20;
		}
		
	}
}


