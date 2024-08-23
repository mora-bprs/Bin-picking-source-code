/*
 * first_project.c
 *
 * Created: 7/6/2024 6:05:21 AM
 * Author : Ama
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#define STEP_PIN PD2 // Define STEP_PIN as PD2
#define DIR_PIN PD4  // Define DIR_PIN as PD5

void setup() {
	// Set STEP_PIN and DIR_PIN as output
	DDRD |= (1 << STEP_PIN) | (1 << DIR_PIN);
}

void loop() {
	// Set direction to HIGH
	PORTD |= (1 << DIR_PIN);
	
	// Make 800 pulses (one full cycle rotation)
	for (int x = 0; x < 200; x++) {
		PORTD |= (1 << STEP_PIN);  // Set STEP_PIN HIGH
		_delay_us(220);           // Wait 220 microseconds
		PORTD &= ~(1 << STEP_PIN); // Set STEP_PIN LOW
		_delay_us(220);           // Wait 220 microseconds
	}
	_delay_ms(1000); // One second delay

	// Set direction to LOW
	PORTD &= ~(1 << DIR_PIN);
	
	// Make 800 pulses (another full cycle rotation)
	for (int x = 0; x < 200; x++) {
		PORTD |= (1 << STEP_PIN);  // Set STEP_PIN HIGH
		_delay_us(220);           // Wait 220 microseconds
		PORTD &= ~(1 << STEP_PIN); // Set STEP_PIN LOW
		_delay_us(220);           // Wait 220 microseconds
	}
	_delay_ms(1000); // One second delay
}

int main(void) {
	setup();  // Call the setup function
	while (1) {
		loop(); // Continuously call the loop function
	}
}
