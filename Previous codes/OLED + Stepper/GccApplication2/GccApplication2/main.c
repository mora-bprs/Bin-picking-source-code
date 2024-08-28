/*
 * GccApplication2.c
 *
 * Created: 7/6/2024 11:39:50 AM
 * Author : Ama
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void UART_init(unsigned int ubrr);
unsigned char UART_receive(void);
void RX_coordinates(char *buffer, float *x_coordinate, float *y_coordinate);

void UART_init(unsigned int ubrr) {
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver */
    UCSR0B = (1<<RXEN0);
    /* Set frame format: 8 data bits, 1 stop bit */
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

unsigned char UART_receive(void) {
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0)));
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

int main(void) {
    char buffer[20];
    float x_coordinate = 0.0, y_coordinate = 0.0;
    
    UART_init(MYUBRR);
    
    while (1) {
		// Now x_coordinate and y_coordinate have the parsed values
        RX_coordinates(buffer, &x_coordinate, &y_coordinate);
    
    }
    
    return 0;
}
