# Bin Picking Robot

This repository contains the source code for a bin-picking robot project, developed using Microchip Studio. The robot uses an ATmega328P microcontroller, an OLED display, a stepper motor for a gripper mechanism, and a UART interface for receiving coordinates. The project also utilizes the u8g2 library for driving the OLED display.

## Table of Contents

- [Introduction](#introduction)
- [Main Key Components](#mainkeycomponents )
- [Wiring](#wiring)
- [Code Explanation](#code-explanation)
  - [OLED Test Code](#oled-test-code)
  - [Stepper Motor Test Code](#stepper-motor-test-code)
  - [Gripper Test Code](#gripper-test-code)
  - [Receiving Coordinates](#receiving-coordinates)
  - [Final Code](#final-code)
- [References](#references)

## Introduction

The bin-picking robot is designed to pick and place objects based on received coordinates. It consists of a stepper motor to control the movement, a gripper mechanism to pick and place objects, and an OLED display to show the coordinates and status messages.

## Main Key Components 

- ATmega328P Microcontroller
- SSD1306 OLED Display
- Gripper Mechanism
- UART Interface
- Button
- Membrane Sensor
- DRV8825 Motor driver

## Wiring

- **OLED Display**: Connect the OLED display's SDA and SCL pins to the ATmega328P's corresponding I2C pins.
- **Stepper Motor**: Connect the stepper motor's control pins (STEP_PIN and DIR_PIN) to PD2 and PD4 of the ATmega328P.
- **Motor Driver**:
- **Button**: Connect the button to PD3 with a pull-up resistor.
- **Membrane Sensor**: Connect the membrane sensor to the ADC input channel PC0 of the ATmega328P.

## Code Explanation

### OLED Test Code

After setting  up u8g2 library, following code initializes the OLED display and prints a test message:

```c
#include <avr/io.h>
#include <util/delay.h>
#include <u8g2.h>
#include <u8x8_avr.h>

#define SSD1306_ADDR  0x78

u8g2_t u8g2;

int main(void) {
    u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
    u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_t0_14b_tf);
    u8g2_SetFontRefHeightText(&u8g2);
    u8g2_SetFontPosTop(&u8g2);
    u8g2_DrawStr(&u8g2, 0, 0, "OLED Test");
    u8g2_SendBuffer(&u8g2);


    while (1) {
        // Loop forever
    }
}
```

### Stepper Motor Test Code

The following code initializes the stepper motor and makes it perform a test movement:

```c
#include <avr/io.h>
#include <util/delay.h>

#define STEP_PIN PD2
#define DIR_PIN PD4

void setup_stepper_motor() {
    DDRD |= (1 << STEP_PIN) | (1 << DIR_PIN);
}

void stepper_motor_test() {
    PORTD |= (1 << DIR_PIN); // Set direction
    for (int i = 0; i < 200; i++) {
        PORTD |= (1 << STEP_PIN);
        _delay_us(1000);
        PORTD &= ~(1 << STEP_PIN);
        _delay_us(1000);
    }
}

int main(void) {
    setup_stepper_motor();
    while (1) {
        stepper_motor_test();
        _delay_ms(1000);
    }
}
```
### Gripper Test Code

The following code initializes the gripper mechanism and performs a test open and close cycle:

```c
#include <avr/io.h>
#include <util/delay.h>

#define STEP_PIN PD2
#define DIR_PIN PD4

void gripper_setup() {
    DDRD |= (1 << STEP_PIN) | (1 << DIR_PIN);
}

void gripper_open() {
    PORTD |= (1 << DIR_PIN); // Set direction to open
    for (int i = 0; i < 200; i++) {
        PORTD |= (1 << STEP_PIN);
        _delay_us(1000);
        PORTD &= ~(1 << STEP_PIN);
        _delay_us(1000);
    }
}

void gripper_close() {
    PORTD &= ~(1 << DIR_PIN); // Set direction to close
    for (int i = 0; i < 200; i++) {
        PORTD |= (1 << STEP_PIN);
        _delay_us(1000);
        PORTD &= ~(1 << STEP_PIN);
        _delay_us(1000);
    }
}

int main(void) {
    gripper_setup();
    while (1) {
        gripper_open();
        _delay_ms(1000);
        gripper_close();
        _delay_ms(1000);
    }
}
```
### Receiving Coordinates

The following code initializes the UART interface and receives coordinates via UART:

```c
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void UART_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

unsigned char UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void RX_coordinates(char *buffer, float *x_coordinate, float *y_coordinate) {
    int i = 0;
    while (1) {
        buffer[i] = UART_receive();
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }
        i++;
    }
    char *token = strtok(buffer, ", ");
    if (token != NULL) {
        *x_coordinate = atof(token);
        token = strtok(NULL, ", ");
        if (token != NULL) {
            *y_coordinate = atof(token);
        }
    }
}

int main(void) {
    char buffer[20];
    float x_coordinate = 0.0, y_coordinate = 0.0;
    
    UART_init(MYUBRR);
    
    while (1) {
        RX_coordinates(buffer, &x_coordinate, &y_coordinate);
    }
}
```

### Final Code

The final code integrates the OLED display, stepper motor, gripper mechanism, and UART interface to create the bin-picking robot:

```c
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <u8g2.h>
#include <u8x8_avr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SSD1306_ADDR  0x78 // Define display i2c address
#define STEP_PIN PD2 // Define STEP_PIN as PD2
#define DIR_PIN PD4  // Define DIR_PIN as PD4
#define BAUD 9600 // Define BUAD RATE
#define MYUBRR F_CPU/16/BAUD-1 // Define Ubrr

// Define display instance
u8g2_t u8g2;

// Define gripper functions
void gripper_setup();
void gripper_loop_open();
void gripper_loop_close();
void start_stepper_motor();
void stop_stepper_motor();

// Define UART functions
void UART_init(unsigned int ubrr);
unsigned char UART_receive(void);
void RX_coordinates(char *buffer, float *x_coordinate, float *y_coordinate);

// Define button read function
uint8_t read_button();

// Define membrane sensor read function
uint8_t check_membrane_sensor();

volatile uint16_t step_count = 0;
volatile uint16_t steps_to_move = 0;

void gripper_setup() {
	// Set STEP_PIN and DIR_PIN as output
	DDRD |= (1 << STEP_PIN) | (1 << DIR_PIN);

	// Set up Timer1
	TCCR1A = 0; // Set Timer1 to normal mode
	TCCR1B = (1 << WGM12) | (1 << CS11); // CTC mode, prescaler 8
	OCR1A = 2000; // Set the compare value for a 1000us delay
	TIMSK1 = (1 << OCIE1A); // Enable Timer1 compare interrupt
	sei(); // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	if (step_count < steps_to_move) {
		PORTD ^= (1 << STEP_PIN); // Toggle STEP_PIN
		step_count++;
		} else {
		stop_stepper_motor();
	}
}

void start_stepper_motor() {
	step_count = 0;
	steps_to_move = 800; // Set the number of steps for a full cycle
	TCNT1 = 0; // Reset Timer1 counter
	TCCR1B |= (1 << CS11); // Start Timer1 with prescaler 8
}

void stop_stepper_motor() {
	TCCR1B &= ~(1 << CS11); // Stop Timer1
	PORTD &= ~(1 << STEP_PIN); // Ensure STEP_PIN is low
}

void gripper_loop_open() {
	// Set direction to HIGH
	PORTD |= (1 << DIR_PIN);
	start_stepper_motor();
	while (step_count < steps_to_move); // Wait for the motor to finish
}

void gripper_loop_close() {
	// Set direction to LOW
	PORTD &= ~(1 << DIR_PIN);
	start_stepper_motor();
	while (step_count < steps_to_move); // Wait for the motor to finish
}

void UART_init(unsigned int ubrr) {
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver */
	UCSR0B = (1 << RXEN0);
	/* Set frame format: 8 data bits, 1 stop bit */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

unsigned char UART_receive(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

void RX_coordinates(char *buffer, float *x_coordinate, float *y_coordinate) {
	// Receive string
	int i = 0;
	while (1) {
		buffer[i] = UART_receive();
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			break;
		}
		i++;
	}
	// Parse the received string
	char *token = strtok(buffer, ", ");
	if (token != NULL) {
		*x_coordinate = atof(token);
		token = strtok(NULL, ", ");
		if (token != NULL) {
			*y_coordinate = atof(token);
		}
	}
}

uint8_t read_button() {
	// Read the state of the button
	if (PIND & (1 << PIND3)) {
		// Button not pressed
		return 0;
		} else {
		// Button pressed
		return 1;
	}
}

uint8_t check_membrane_sensor() {
	// Start the ADC conversion
	ADCSRA |= (1 << ADSC);
	// Wait for the conversion to complete
	while (ADCSRA & (1 << ADSC));
	// Read the ADC value
	uint16_t adc_value = ADC;
	// Return 1 if the box is grabbed, 0 otherwise
	if (adc_value > 256) {
		return 1;
		} else {
		return 0;
	}
}

void init_adc() {
	// Set the reference voltage to AVcc
	ADMUX = (1 << REFS0);
	// Set the ADC prescaler to 128 (16MHz/128 = 125kHz)
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// Enable the ADC
	ADCSRA |= (1 << ADEN);
	// Set the input channel to PC0
	ADMUX |= (0 << MUX0);
}

int main(void) {
	// Set up the display
	u8g2_Setup_ssd1306_128x64_alt0_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
	u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);

	// Initialize the display buffer
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_14b_tf);
	u8g2_SetFontRefHeightText(&u8g2);
	u8g2_SetFontPosTop(&u8g2);
	u8g2_DrawStr(&u8g2, 0, 0, "Bin Picking Robot");
	u8g2_SendBuffer(&u8g2);
	
	// Set up UART
	char buffer[20];
	float x_coordinate = 0.0, y_coordinate = 0.0;
	
	UART_init(MYUBRR);
	
	// Set up Gripper
	gripper_setup();
	
	// Set PD3 as input
	DDRD &= ~(1 << DDD3);
	// Enable internal pull-up resistor on PD3
	PORTD |= (1 << PORTD3);
	
	// Initialize the ADC
	init_adc();
	
	while (1) {
		// Get coordinates
		RX_coordinates(buffer, &x_coordinate, &y_coordinate);
		
		// Convert coordinates to strings
		char x_str[10];
		char y_str[10];
		snprintf(x_str, sizeof(x_str), "X=%.3f", x_coordinate);
		snprintf(y_str, sizeof(y_str), "Y=%.3f", y_coordinate);
		
		// Display the received coordinates
		u8g2_ClearBuffer(&u8g2); // Clear buffer for new content
		u8g2_DrawStr(&u8g2, 20, 10, x_str);
		u8g2_DrawStr(&u8g2, 20, 30, y_str);
		
		// Wait until button press
		while (read_button() == 0) {
			// Do nothing, just wait
		}

		// Gripper open
		gripper_loop_open();
		// Check membrane sensor
		if (check_membrane_sensor() == 1) {
			// Display "Grabbed the box"
			u8g2_ClearBuffer(&u8g2);
			u8g2_DrawStr(&u8g2, 20, 50, "Grabbed the box");
			u8g2_SendBuffer(&u8g2);
			
			// Wait until button release
			while (read_button() == 1) {
				// Do nothing, just wait
			}
			
			// Gripper close
			gripper_loop_close();
			// Check membrane sensor
			if (check_membrane_sensor() == 0) {
				// Display "Released the box"
				u8g2_ClearBuffer(&u8g2); // Clear buffer for new content
				u8g2_DrawStr(&u8g2, 20, 70, "Released the box");
				u8g2_SendBuffer(&u8g2);
			}
		}
	}
	
	return 0;
}
```

## References
- [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio)
- [u8g2 Library](https://github.com/olikraus/u8g2)
