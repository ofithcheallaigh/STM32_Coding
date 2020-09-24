#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

int USART2_write(int ch);
void USART2_Init(void);

int data;
double voltage;
double celsius;

int main(void)
{
	RCC->AHB1ENR |= 1;									// Peripheral clock enable register. 
																				// Bit0: GPIOAEN; 1 = IO port A clock enabled
	RCC->APB1ENR |= 1;									// Peripheral clock enable register.
																				// Bit0: TIM2EN; 1 TIM2 clock enable
	
	TIM2->PSC = 1600 - 1;									// Prescaler. PSC contains the value to be loaded into the
																				// active prescale register at each update event. 
	TIM2->ARR = 10000 - 1;								// Auto-reload register.
	TIM2->CNT = 0;											// Counter register. 
	TIM2->CCMR1 = 0x6800;
	TIM2->CCER = 0x0010;
	TIM2->CCR2 = 50 - 1;
	TIM2->CR1 = 1;
	
	// Set up ADC
	RCC->APB2ENR = 0x0100;								// Enables ADC clock
	ADC->CCR |= 0x00800000;
	ADC->CCR &=~ 0x00400000;							// VBATE must be disabled for the temp sensor

	ADC1->SMPR1 |= 0x04000000;						// Sampling time minimum 10us
	ADC1->SQR3 = 18;
	ADC1->CR2 |= 0x13000000;
	ADC1->CR2 |= 1;
	
	USART2_Init();
	printf("STM32F4 Temperature \r\n");
	
	while(1){
		while(!(ADC1->SR)){}									// Wait for the conversion to be done
			data = ADC1->DR;
			voltage = (double)data/4095*3.3;
			celsius = (voltage - 0.76)/0.0025 + 15;
			
			printf("%d, %.2f\370C\r\n",data,celsius);
	}
}



void USART2_Init(void)
{
	RCC->APB1ENR |= 0x20000;							// Enable UART2 clock
	RCC->AHB1ENR |= 1;
	
	GPIOA->AFR[0] |= 0x0700;							
	GPIOA->MODER |= 0x0020;								
	USART2->BRR = 0x0683;									// 9600 baud at 16MHz	
	USART2->CR1 |= 0x0008;								// Enable Tx
	USART2->CR1 |= 0x2000; 								// USART enabled
}

struct __FILE{int handle;};
FILE __stdout = {1};
int fputc(int c, FILE *f)
{
	return USART2_write(c);
}

int USART2_write(int ch)
{
	while(!(USART2->SR & 0x0080))
	{
		// Do nothing
	}
	USART2->DR = ch;
	return ch;
}

