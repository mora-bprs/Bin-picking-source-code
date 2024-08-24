/*
 * TX_AVR.c
 *
 * Created: 8/24/2024 1:12:24 AM
 * Author : Hirusha
 */ 
#define F_CPU 8000000UL  // Define clock speed (in Hz)

#include <avr/io.h>
#include <util/delay.h>  // Include delay functions


#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
void USART_Transmit_String(const char *str);

int main(void)
{
	// Initialize USART with the calculated UBRR value
	USART_Init(MYUBRR);

	// Keep looping indefinitely
	while (1)
	{
		// Transmit the string
		USART_Transmit_String("Hello, USART!");

		// Delay for 1000 ms
		_delay_ms(1000);
	}
}

void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	/* Set frame format: 8 data bits, 2 stop bits */
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
	;

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_Transmit_String(const char *str)
{
	while (*str)
	{
		USART_Transmit(*str++);
	}
}

