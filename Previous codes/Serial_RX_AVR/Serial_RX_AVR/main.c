/*
 * Serial_RX_AVR.c
 *
 * Created: 8/14/2024 12:18:48 PM
 * Author : Hirusha
 */ 
#include <avr/io.h>
#include <string.h>

#define F_CPU 1000000  // Adjust this to your clock frequency
#define BAUD_RATE 9600
#define RXDELAY (((F_CPU / BAUD_RATE) - 9) / 3)
#define BAD_CHAR 0xFF  // Define BAD_CHAR to indicate an error in reception

#define BUFFER_SIZE 20  // Adjust this size based on your expected data length

// Declare the external assembly functions
extern char RxTimedByte(char delay);

void receiveData(char* buffer, uint8_t maxLength);

int main(void)
{
	// Example to receive x, y coordinates
	char data[BUFFER_SIZE];
	
	while(1)
	{
		// Clear the buffer
		memset(data, 0, BUFFER_SIZE);

		// Receive data into the buffer
		receiveData(data, BUFFER_SIZE);
		
		// Now 'data' contains the received string (e.g., "1.02,220.2")
		// You can then parse this data as needed.
	}
}

void receiveData(char* buffer, uint8_t maxLength)
{
	uint8_t index = 0;
	
	// Loop to receive data until buffer is full or a newline character is received
	while(index < maxLength - 1) // -1 to leave space for the null terminator
	{
		char receivedByte = RxTimedByte(RXDELAY);

		// Check if a delimiter or end of data (you can customize this)
		if(receivedByte == '\n' || receivedByte == '\r' || receivedByte == BAD_CHAR) // Newline or carriage return or error
		{
			break; // End of the data
		}
		
		// Store the received byte in the buffer
		buffer[index++] = receivedByte;
	}
	
	// Null-terminate the string
	buffer[index] = '\0';
}

