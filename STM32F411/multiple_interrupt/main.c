// Dealing with multiple interrupts in the same application

#include "stm32f4xx.h"                  // Device header

// Function prototypes
void systickDelayMs(int n);

volatile int pb10Counter;
volatile int pc13Counter;

int main(void)
{
	__disable_irq();
	
	RCC->AHB1ENR |= 0x01;									// Clock access for Port A
	RCC->AHB1ENR |= 0x02;									// Clock access for Port B
	RCC->AHB1ENR |= 0x04;									// Clock access for Port C. This has the push button 
	// This could all be done in one instruction:
	//			RCC->AHB1ENR |= 0x07;
	RCC->APB2ENR |= 0x4000;								// Enables SYSCFG clock
	
	GPIOA->MODER |= 0x400;								// P.156 of Ref manual. MODER is the port mode register
																				// Two bits are required to set up each port mode register, which configures the IO  direction mode
																				// 0x400 = 0100 0000 0000. This means we are looking at MODER5, where 01 in MODER5 sets
																				// it as "General purpose output mode
	
	SYSCFG->EXTICR[3] |= 0x20;						// Select Port C for EXTI13
	EXTI->IMR |= 0x2000;									// Unmask EXTI13
	EXTI->FTSR |= 0x2000;									// Select falling edge trigger
	
	GPIOB->PUPDR |= 0x100000;							// Enables pull up reistor for PB10
	
	SYSCFG->EXTICR[2] |= 0x100;						// Selects Port B for EXT10
	EXTI->IMR |= 0x400;										// Unmask EXT10
	EXTI->FTSR |= 0x400;									// Selects falling edge trigger
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	__enable_irq();
	
	while(1)
	{
		// Do nothing
	}
	
}


void EXTI15_10_IRQHandler(void)
{
	// PC13
	if(EXTI->PR == 0x2000)
	{
		for(int i = 0; i <= 2; i++)
		{
			GPIOA->BSRR = 0x20;										
			systickDelayMs(200);									
			GPIOA->BSRR = 0x00200000;							
			systickDelayMs(200);
		}
		pc13Counter++;
		EXTI->PR |= 0x2000;							// Clears interrput flag
	}
	// PB10
	else if(EXTI->PR == 0x400)
	{
				for(int i = 0; i < 6; i++)
		{
			GPIOA->BSRR = 0x20;										
			systickDelayMs(200);									
			GPIOA->BSRR = 0x00200000;							
			systickDelayMs(200);
		}
		pb10Counter++;
		EXTI->PR |= 0x400;							// Clears interrupt pending flag
	}
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

