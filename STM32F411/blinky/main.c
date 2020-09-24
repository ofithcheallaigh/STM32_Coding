// LED is connected to PA5
// Clock access: AHB1
// RCC->AHB1ENR
// GPIOx_MODER
// GPIOx_ODR

#include "stm32f4xx.h"									// Device header

void delayMS(int delay);

int main(void)
{
	RCC->AHB1ENR |= 1;										// Sets bit 1 high - enables GPIOA clock
	GPIOA->MODER |= 0x00000400;						// 0b -> 0000 0000 0000 0000 0000 0100 0000 0000
	
	while(1){
		GPIOA->ODR = 0x20;									// 0b -> 0010 0000. set high
		delayMS(100);
		GPIOA->ODR &= ~(0x20);							// Sets low
		delayMS(1000);
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

