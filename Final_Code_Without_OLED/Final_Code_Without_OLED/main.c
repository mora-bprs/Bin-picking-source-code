/*
 * Final_Code_Without_OLED.c
 *
 * Created: 8/23/2024 10:14:17 PM
 * Author : Hirusha
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STEP_PIN PD2 // Define STEP_PIN as PD2
#define DIR_PIN PD4  // Define DIR_PIN as PD4
#define BUTTON_PIN PD3 // Define BUTTON_PIN as PD3
#define DEBOUNCE_DELAY 100 // Debounce delay in milliseconds
#define BAUD 115200 // Define BUAD RATE
#define MYUBRR F_CPU/16/BAUD-1 // Define Ubrr

volatile char RxData[10];  // Buffer to store received data
volatile uint8_t indx = 0;  // Index to track position in buffer

int steps_to_move = 500;

void gripper_setup();
void gripper_opening();
void gripper_closing();
void uartInit(void);
void btn_state();

volatile uint8_t buttonPressed = 0; // Variable to count button presses

void gripper_setup() {
	// Set STEP_PIN and DIR_PIN as output
	DDRD |= (1 << STEP_PIN) | (1 << DIR_PIN);
}

void gripper_opening() {
	// Set direction to HIGH
	PORTD |= (1 << DIR_PIN);
	
	// Make 500 pulses (half cycle rotation)
	for (int x = 0; x < steps_to_move; x++) {
		PORTD |= (1 << STEP_PIN);  // Set STEP_PIN HIGH
		_delay_us(220);           // Wait 220 microseconds
		PORTD &= ~(1 << STEP_PIN); // Set STEP_PIN LOW
		_delay_us(220);           // Wait 220 microseconds
	}
}

void gripper_closing() {
	// Set direction to LOW
	PORTD &= ~(1 << DIR_PIN);
	
	// Make 500 pulses (half cycle rotation)
	for (int x = 0; x < steps_to_move; x++) {
		PORTD |= (1 << STEP_PIN);  // Set STEP_PIN HIGH
		_delay_us(220);           // Wait 220 microseconds
		PORTD &= ~(1 << STEP_PIN); // Set STEP_PIN LOW
		_delay_us(220);           // Wait 220 microseconds
	}
}

void uartInit(void) {
	uint16_t ubrr_value = MYUBRR;
	UBRR0H = (ubrr_value >> 8);
	UBRR0L = ubrr_value;
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);  // Enable RX, TX, and RX interrupt
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);  // 8 data bits, 1 stop bit, no parity
	
	DDRD |= (1<<PD1);  // PD1 (TXD) as output
	DDRD &= ~(1<<PD0);  // PD0 (RXD) as input
}

void btn_state() {
	while(1){
		if ((PIND & (1 << BUTTON_PIN)) != 0) { // Button pressed (PD3 reads as 0)
			_delay_ms(DEBOUNCE_DELAY);
			buttonPressed++;
			break;
		}
	}
}


ISR(USART_RX_vect) {
	char receivedChar = UDR0;  // Read the received character
	RxData[indx++] = receivedChar;  // Store the character in the buffer

	if (indx >= sizeof(RxData)) {  // Reset index if buffer is full
		indx = 0;
	}
	RxData[indx] = '\0';  // Null-terminate the string
}

int main(void) {
	gripper_setup();  // Set up Gripper
	uartInit();
	sei();  // Enable global interrupts

	while (1) {
		while(1){
			char temp[10];  // Temporary non-volatile char array
			strcpy(temp, (char*)RxData);  // Copy data from volatile char array to non-volatile array
			// Check if the received string is not empty (receivedString != NULL equivalent)
			if (strlen(temp) > 0) {
				break;
			}
		}
		btn_state();
		if (buttonPressed == 1)
		{
			gripper_opening();
			_delay_ms(1);
			btn_state();
			if (buttonPressed == 2)
			{
				gripper_closing();
			}
		}

		if (buttonPressed == 2) {
			buttonPressed = 0; // Reset buttonPressed after showing "ON"
		}
	}
}