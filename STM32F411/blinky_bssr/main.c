// BSRR, not BSSR for 

// LED is connected to PA5
// Clock access: AHB1
// RCC->AHB1ENR
// GPIOx_MODER
// GPIOx_ODR

#include "stm32f4xx.h"                  // Device header

void delayMS(int delay);

int main(void)
{
	RCC->AHB1ENR |= 1;
	GPIOA->MODER |= 0x400;
	
	
	while(1){
		GPIOA->BSRR = 0x20;									// Sets PA5 HIGH. Correspons to BS5 for Bit Set pin 5
		delayMS(100);
		GPIOA->BSRR = 0x200000;							// Reset bit - 0b 0010 0000 0000 0000 0000 0000
	}
}


// Taking into account 16MHz system clock
void delayMS(int delay)
{
	int i;
	for(; delay > 0; delay--){
		for(i = 0; i < 3195; i++);
	}
}

