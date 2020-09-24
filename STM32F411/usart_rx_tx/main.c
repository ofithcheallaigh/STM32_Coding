

#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

/***** Function prototypes *****/
int fputc(int c, FILE *f);
int fgetc(FILE *f);
int USART2_read(void);
int USART2_write(int ch);
void USART2_Init(void);

/***** Main *****/
int main(void)
{
	int n;
	char str[100];
	USART2_Init();
	printf("Hello from the other side\r\n");
	fprintf(stdout, " test for stdout\r\n");
	fprintf(stderr, " test for stderr\r\n");
	
	while(1)
	{
		printf("How old are you?");
		scanf("%d", &n);
		printf("Your age is: %d\r\n",n);
		printf("Enter your first name:");
		gets(str);
		printf("I like your style, ");
		puts(str);
		printf("\r\n");
	}
}

/***** Functions *****/
void USART2_Init(void)
{
	// Set up bus clocks
	RCC->AHB1ENR |= 0x01;
	RCC->APB1ENR |= 0x20000;
	
	GPIOA->AFR[0] |= 0x7700;								// Set up alternative function for pin
	GPIOA->MODER |= 0x00A0;								// Enables alt function for PA2 and PA3
	
	USART2->BRR |= 0x0683;								// 9600 @ 16Mhz
	USART2->CR1 |= 0x000C;								// Enables both Tx and Rx bits
	USART2->CR1 |= 0x2000;								// Enabvles register
}

int USART2_write(int ch)
{
	// Wait for Tx buffer empty
	while(!(USART2->SR & 0x0080))					// Checks the Rx buffer status
	{
		// Do nothing
	}
	
	USART2->DR = (ch& 0xFF);
	return ch;
}

int USART2_read(void)
{
	while(!(USART2->SR & 0x0020))	   			// Checks the Rx buffer status
	{
		// Do nothing while waiting
	}
	return USART2->DR;
}

// Interface to the c-standard I/O library
struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};

int fgetc(FILE *f)
{
	int c;
	c = USART2_read();
	if(c == '\r')
	{
		USART2_write(c);
		c = '\n';
	}
	USART2_write(c);
	
	return c;
}

int fputc(int c, FILE *f)
{
	return USART2_write(c);
}

