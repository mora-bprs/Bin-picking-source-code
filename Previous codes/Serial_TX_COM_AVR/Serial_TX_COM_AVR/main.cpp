/*
 * Serial_TX_COM_AVR.cpp
 *
 * Created: 8/14/2024 11:55:04 AM
 * Author : Hirusha
 */ 

#define F_CPU 1000000  // Adjust to your clock frequency
#define BAUD_RATE 9600
#define RXDELAY (((F_CPU / BAUD_RATE) - 9) / 3)

#include <avr/io.h>

// Declare the external assembly function
extern "C" char RxTimedByte(char delay);

int main(void)
{
	// Set up UART pin (assume PB0 for this example)
	DDRB &= ~(1 << PB0);  // Set PB0 as input for RX
	PORTB |= (1 << PB0);  // Enable pull-up on PB0 (optional, depending on your circuit)

	while(1)
	{
		// Call the assembly function to receive a byte
		char receivedData = RxTimedByte(RXDELAY);

		// Process received data
		// Example: Transmit the received data back to the computer
		TxTimedByte(receivedData, RXDELAY);
	}
}

