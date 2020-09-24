#include "stm32f4xx.h"                  // Device header

void systickDelayMs(int n);
void LED_play(int value);
void USART2_Init(void);


int main(void)
{
	__disable_irq();											// Disabled to allow us to configure
	
	RCC->AHB1ENR |= 0x1;
	GPIOA->MODER |= 0x400;								// 0x400 = b0100 0000 0000 -? MODER5. Writing 01 gives General
																				// purpose output mode. Sets PA5 to putput
	
	USART2_Init();												// Run USART2 initialise functon
	USART2->CR1 |= 0x20;									// Enable Rx Interrupt
	NVIC_EnableIRQ(USART2_IRQn);					// Enables the USART2 interruot request
	
	__enable_irq();
	
	while(1)
	{
		// Do nothing
	}
}


void USART2_Init(void)
{
	RCC->AHB1ENR |= 0x01;
	RCC->APB1ENR |= 0x20000;							// Enable UART2 clock
	
	GPIOA->AFR[0] |= 0x7000;							// Alt7 for UART2. 0x7000 = b0111 0000 0000 0000. By selecting 
																				// the lower alt function register, writing 0x7000 selects AF7
																				// in AFRL3. And AF7 selects USART1..2
	
	GPIOA->MODER |= 0x80;									// 0x80 = b1000 0000 -> MODER3: writing "10"
																				// to this register sets it to "alternative function mode"
	
	// USART Mode Configuration P548 of Reference manual
	USART2->BRR = 0x008B;									// 15200 baud at 16MHz	
	USART2->CR1 |= 0x004;									// Enable receiver
	USART2->CR1 |= 0x2000; 								// USART enabled
}

void LED_play(int value)
{
	value %= 16;
	for(; value > 0; value--)
	{
		GPIOA->BSRR = 0x20;									// Turns on the LED
		systickDelayMs(100);
		GPIOA->BSRR = 0x00200000;						// Turns off LED
		systickDelayMs(100);
	}
	systickDelayMs(400);
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


void USART2_IRQHandler(void)						// Callback function, does not require prototype at the top
{
	char c;
	if(USART1->SR & 0x0020)								// 0x20 = b0010 0000. Bit 5 is RXNE: Read Data Register Not Empty
																				// This bit is set by hardware when the content of the RDR shift register has been transferred
																				// to the USART_DR register
	{
		// We find ourselves here if the above is TRUE
		c = USART2->DR;
		LED_play(c);
	}
}
