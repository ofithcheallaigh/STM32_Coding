#include "stm32f4xx.h"                  // Device header

// LED			:	PA5
// Button		: PC13

void systickDelayMs(int n);

int main(void)
{
	__disable_irq();											// Global interrupts disabled to allow configuration
	
	// Configure interrupt
	RCC->AHB1ENR |= 0x01;									// Enable clock access to Port A
	RCC->AHB1ENR |= 0x04;									// Emable clock access to Port C
	RCC->APB2ENR |= 0x4000;								// Enable SYSCFG Clock
	
	GPIOA->MODER |= 0x400;								
	
	SYSCFG->EXTICR[3] = 0x0020;						// Select Port C for EXTI13. Number 3 references EXTI 4
																				// Reference manual, P.141 explains that 0x20 selects PC13 pin,
																				// which is out button
	
	// EXTI - External interrupt/event controller
	// Consists of up to 23 edge detectors for generating event/interrupt requests
	EXTI->IMR |= 0x2000;									// Unmask EXTI13. IMR = Interrupt Mask Register. 2000 puts a 1 in MR13, 
																				// and a 1 = interrupt request from line x is not masked
	
	EXTI->FTSR |= 0x2000;									// Select a falling edge trigger. FTSR = Falling Trigger Selection Register.
																				// 2000 puts a 1 in TR13. A 1 indicates falling trigger enabled (for event or
																				// interrput) for input line.
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);				// Selected for Interrupt 13
	
	__enable_irq();												// Global interrupts enabled after configuration
	
	while(1)
	{
		//Do nothing
	}
}

// Below is what will happen when the falling edge of the interrupt is detected
void EXTI15_10_IRQHandler(void)
{
	// BSRR is the GPIOx bit set/reset register
	GPIOA->BSRR = 0x20;										// Turns on LED. 0x20 = 0010 0000. A 1 in BS5, sets the corresponding ODRx bit
	systickDelayMs(400);									// Delay
	GPIOA->BSRR = 0x00200000;							// Turns off LED. 0x200000 = 0010 0000 0000 0000 0000 0000. Puts 1 in BR5, to reset
	systickDelayMs(400);									// Delay
	
	GPIOA->BSRR = 0x20;										// Turns on LED
	systickDelayMs(400);									// Delay
	GPIOA->BSRR = 0x00200000;							// Turns off LED
	systickDelayMs(400);									// Delay
	
	EXTI->PR = 0x2000;										// Clear the interrupt flag
}


void systickDelayMs(int n)
{
	SysTick->LOAD = 16000;								// Number of clock cycles in a ms
	SysTick->VAL = 0;											// Clear current value register
	SysTick->CTRL = 0x5;									// Enables SysTick
	
	for(int i = 0; i < n; i++)
	{
		// Wait here while above not zero - wait until the COUNT flag is set
		while((SysTick->CTRL & 0x10000) == 0){}
	}
	SysTick->CTRL = 0;
}
