// APB1 bit 17 needed for USART
// USART connected to PA2

#include "stm32f4xx.h"                  // Device header

void USART_write(char ch);
void USART2_Init(void);
void delayMS(int delay);

int main(void)
{
	USART2_Init();
	
	while(1){															// Sending characters
		USART_write('H');
		USART_write('i');
		USART_write(' ');
		
		delayMS(10);
	}
}

void USART2_Init(void)
{
	// Enable clock source for USART
	RCC->APB1ENR |= 0x20000;							// 0x20000 = 0b100000000000000000 (bit 17) 
	
	// Enable clock source of normal GPIO, because we have to go to this function and set it for an alternative fucntion
	RCC->AHB1ENR |= 0x1;
	
	// Set PA2 as alterbative function
	GPIOA->MODER |= 0x0020;								// Set PA2 as alternatibe function
	GPIOA->AFR[0] |= 0x0700;								// This corresponds to enbaling USART2
	
	USART2->BRR |= 0x0683;									// 9600 at 16MHz
	USART2->CR1 |= 0x0008;									// Enables Tx by setting the 4th bit high
	USART2->CR1 |= 0x2000;								// Enables USART. Advised to configure USART, then enable USART
	
}

void USART_write(char ch)
{
	while(!(USART2->SR & 0x0080)){}				// Wait while the Tx buffer is empty
		USART2->DR |= (ch & 0xFF);
	
}

void delayMS(int delay){
	int i;
	for(; delay > 0; delay--){
		for(i = 0; i < 3195; i++);
	}
}
