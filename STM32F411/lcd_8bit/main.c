// PC0 -> PC7 = D0 -> D7
// PB5 = RS Line
// PB6 = R/W 
// PB7 = EN


#include "stm32f4xx.h"                  // Device header

#define RS	0x20												// 0x20 = 0010 0000
#define RW 	0x40												// 0x40 = 0100 0000
#define	EN	0x80												// 0x80 = 1000 0000

void delayMs(int delay);

int main(void)
{
	
	
}


void FPIO_Init(void)
{
	RCC->AHB1ENR |= 0x06;									// Sets clock for bus. 0x06 = 0110 0000. Corresponds to bits 5 and 6
	GPIOB->MODER |= 0x5400;								// 0x54 = 0101 0100 0000 0000
	GPIOB->BSRR |= 0x00C;									// Set EN and RW low. 0x00C = 0000 0000 1100
	
	// Setting data pins - PC0 to PC7 as output
	GPIOC->MODER |= 0x00005555;						// Sets PC0 - PC7 as output pins

}


void LCD_command(unsigned char command)
{
	GPIOB->BSRR = (RS | RW) << 16;				// Sets RS = 0, R/W = 0
	GPIOC->ODR = command;
	GPIOB->BSRR = EN;
	delayMs(0);
	GPIOB->BSRR = EN << 16;								// Clear EN
}

void LED_data(char data)
{
	
}

void delayMs(int delay)
{
	int i;
	for(; delay > 0; delay--)
	{
		for(i = 0; i < 3195; i++)
		{
			
		}
	}
}


