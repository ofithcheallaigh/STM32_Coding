/*
* We will convert an analogue signal from the ADC channel 1
* ADC Channel 1 is connected to PA1 of the uC
* To test this, we will use a POT, woth one one of the side legs
* connected to GND, the other side leg connected to VDD, and
* the middle leg connected to PA5	
*/

#include "stm32f4xx.h"                  // Device header

// Global variable
int analogValue;

int main(void)
{
	RCC->AHB1ENR |= 0x01;
	GPIOA->MODER |= 0x0C;									// Set PA1 to analogue. 0x0C = b1100
	
	RCC->APB2ENR |= 0x100;								// 0x100 = b0001 0000 0000. Sets bit 8 = ADC1EN = ADC1 clock enable
																				// Setting high enables the clock asscess, setting low disbales clock access
	ADC1->CR2 |= 0x00;										// ADC_CR2 (ADC Control Register 2) Bit 0: A/D Converter ON/OFF.
																				// Setting High Enables ADC, setting low disables ADC and goes into power doewn mode
	ADC1->SQR3 |= 0x01;										// ADC Regular Sequence Register 3. This says conversion sequence should start at channel 1
	ADC1->CR2 |= 0x01;										// Enable ADC.
	
	while(1)
	{
		ADC1->CR2 |= 0x40000000;						// Corresponds to Bit30 in CR2 which is SWSTART - start conversion of regular channels
		// Wait for the conversion to be complete
		while(!(ADC1->SR & 0x02))						// Checks Bit1 of the SR - EOC: regular channel end of conversion. Bit is set by HW
																				// at the end of the conversion of a regular group of channels. It is cleared by SW or by
																				// reading ADC_DR
		{
			// Wait here while the conversion is not complete
		}
		analogValue = ADC1->DR;							// When conversion is complete, read back data register
	}
}

