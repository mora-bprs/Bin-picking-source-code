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

## Wiring

- **OLED Display**: Connect the OLED display's SDA and SCL pins to the ATmega328P's corresponding I2C pins.
- **Stepper Motor**: Connect the stepper motor's control pins (STEP_PIN and DIR_PIN) to PD2 and PD4 of the ATmega328P.
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
