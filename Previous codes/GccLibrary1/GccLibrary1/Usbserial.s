/*
 * Usbserial.s
 *
 * Created: 8/14/2024 11:45:53 AM
 * Author: Hirusha Maduwantha
 * 
 * This code is based on the half-duplex 81N serial UART assembler implementation by Ralph Doncaster.
 * Original version optimized for minimal jitter and 1%/2% Tx/Rx timing error for 115.2kbps@8Mhz,
 * and 2%/1% Tx/Rx timing error for 230.4kbps@8Mhz.
 * 
 * I have modified the code to suit my specific requirements. 
 * Any significant changes or improvements to the original code have been made for this purpose.
 * 
 * Original Author: Ralph Doncaster
 * Original Version: $Id$
 */


#define startWait  r19
#define delayCount r18
#define BAD_CHAR 0xFF  // You can choose a value that's unlikely to be received as valid data


#include <avr/io.h>

#define UART_PORT PORTB
; correct for avr/io.h 0x20 port offset for io instructions
#define UART_PORTX (UART_PORT - 0x20)
#define UART_PIN 0 // Change the UART PIN

; Supports using only one pin on the AVR for both Tx and Rx
;              D1
;  AVR ----+--|>|-----+----- Tx
;          |      10K $ R1
;          +--------(/^\)--- Rx
;               NPN E   C

.global TxTimedByte
; transmit byte in r24 with bit delay in r22 - 15 instructions
; calling code must set Tx line to idle state (high) or 1st byte may be lost
; i.e. PORTB |= (1<<UART_PIN)
TxTimedByte:
        cli
        sbi UART_PORTX-1, UART_PIN        ; set Tx line to output
        cbi UART_PORTX, UART_PIN          ; start bit
        in r0, UART_PORTX
        ldi r25, 3                      ; stop bit + idle state
TxLoop:
        ; 8 cycle loop + delay - total = 7 + 3*r22
        mov delayCount, r22
TxDelay:
; delay (3 cycle * delayCount) -1
        dec delayCount
        brne TxDelay
        bst r24, 0			; store lsb in T
        bld r0, UART_PIN
        lsr r25
        ror r24
        out UART_PORTX, r0
        brne TxLoop
        reti				; return and enable interrupts


.global RxTimedByte
; receive byte into r24 with bit delay in r22 & R24 - 16 instructions
RxTimedByte:
        cbi UART_PORTX-1, UART_PIN  	; set Rx line to input
#ifdef RX_PULLUP
        sbi UART_PORTX, UART_PIN  	; enable pullup
#endif
        mov delayCount, r24		; 1.5 bit delay
        ldi startWait, 0xff             ; Around 5 bits @ 57.6KBaud
        ldi r24, 0x80 			; bit shift counter
WaitStart:
        dec  startWait
        breq NoData
        sbic UART_PORTX-2, UART_PIN 	; wait for start edge
        rjmp WaitStart
        cli
RxBit:
        ; 6 cycle loop + delay - total = 5 + 3*r22
; delay (3 cycle * delayCount) -1 and clear carry with subi
        subi delayCount, 1
        brne RxBit
        mov delayCount, r22
        sbic UART_PORTX-2, UART_PIN	; check UART PIN
        sec
        ror r24
        brcc RxBit
StopBit:
        dec delayCount
        brne StopBit
        reti				; return and enable interrupts
NoData:
        ldi r24, BAD_CHAR
        reti