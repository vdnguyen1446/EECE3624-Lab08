/*****************************************************************************
 *	    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified:
 *   ModDate:
 * 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // Double quotes for local library.

#define BAUDRATE 9600; // standard values from 1200 to 115200
#define DATABITS    8; // 5-8
#define STOPBITS    1; // 1,2
#define PARITY      0; // 0=none, 1=odd, 2=even

/* Your code here.  */
void TransmitArray(const char *array);
void StartUp(void);

int main(void)
{
	// Initialization
	DDRC = (1 << DDC0);
	long unsigned int BaudRate = BAUDRATE;
	unsigned char DataBits = DATABITS;
	unsigned char StopBits = STOPBITS;
	unsigned char Parity = PARITY;
	
	/* Your code here.  */
	uart_init(BaudRate, DataBits, StopBits, Parity);
	
    /* See the UARTLibrary.h for available calls.
	 *     	  // initialize the UART
	 *     	  uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	 *
	 *        // transmit a value (0x41) to the comm port
	 *        c = 0x41;
	 *        uart_tx(c);
	 *
	 *        // receive a byte from the comm port
	 *        c = uart_rx();
	 */
	
	PORTC |= (1 << DDC0);
	StartUp();
	
	int count = 0;
	while(1)
	{
		/* your loop code here */
		unsigned char c = 0;
		
		c = uart_rx();
		if(c != 0){
			if(c == '\r'){    // If '\r' is detected, also start a new line
				uart_tx(c);
				uart_tx('\n');
			}
			else if(c == '\x1b'){   // If ESc is detected, redo the start up screen
				StartUp();
			}
			else if(count == 0 && c == 'e'){
				count = 1;
				uart_tx(c);
			}
			else if(count == 1 && c == 'n'){
				count = 2;
				uart_tx(c);
			}
			else if(count == 2 && c == 'd'){
				
				// End Detected, GoodBye
				uart_tx(c);
				const char blank[] = "\r\n\0";
				TransmitArray(blank);
				TransmitArray(blank);
				const char bye[] = "Bye";
				TransmitArray(bye);
				while(1);
				
			}
			else{   // Transmit the received character
				uart_tx(c);
				count = 0;
			}
			// LED0 Flash
			PORTC &= (0 << PORTC0);
			_delay_ms(75);
			PORTC |= (1 << PORTC0);
		}
		
		
	}
	return 0;
}

void TransmitArray(const char *array){
	int i = 0;
	while(array[i] != '\0'){
		uart_tx(array[i]);
		i++;
	}
}

void StartUp(void){
	const char blank[] = "\r\n\0";
	TransmitArray(blank);
	TransmitArray(blank);
	const char server[] = "RS232 Communications Server\r\n\0";
	TransmitArray(server);
	const char status[] = "Status: Active\r\n\0";
	TransmitArray(status);
	const char name[] = "Vincent Nguyen\r\n\0";
	TransmitArray(name);
	const char classMsg[] = "EECE3624\r\n\0";
	TransmitArray(classMsg);
	const char semester[] = "Fall 2026\r\n\0";
	TransmitArray(semester);
	TransmitArray(blank);
	TransmitArray(blank);
}