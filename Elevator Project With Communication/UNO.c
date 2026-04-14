/*
 * UNO.c
 *
 * Created: 13/04/2026 18.39.15
 * Author : kadir
 */ 
/*SPI_UNO_SLAVE

 * ATmega328p is slave */

#define F_CPU 16000000UL //clock speed
#define FOSC 16000000UL // system clock frequency


#include "uart.h"
#include "mcu.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>



#include "avr_gpio.h"
#include "board_config.h"
#include "delay.h"

#define DOOR_OPEN_DURATION_MS (3000)
#define DOOR_CLOSE_DURATION_MS (2000)
#define FLOOR_MOVING_SPEED_MS (3000)


// defining all the states
typedef enum { // typedef is for defining state_t (to make the code readable)
	IDLE = 0, // empty
	GOINGUP = 1,
	GOINGDOWN = 2,
	DOOR_OPENING = 3,
	DOOR_CLOSING = 4,
	FAULT = 5, // fault
} state_t; // enum includes all the possible states of elevator




// LED configurations
avr_gpio_t movement_led = { // avr_gpio_t stores all register adresses to control GPIO pin
	.port = &IO_2_PORT, // decides output value if it is 1 LED is on if it is 0 LED is off
	.direction = &IO_2_DIRECTION, // decides is pin input or output (LED is always output)
	.pin_offset = IO_2_PIN, // The correspondence pin of bit in port register (position)
	.input = &IO_2_INPUT // read
};

// Door opening
avr_gpio_t door_led = {
	.port = &IO_3_PORT,
	.direction = &IO_3_DIRECTION,
	.pin_offset = IO_3_PIN,
	.input = &IO_3_INPUT
};


// Door closing
avr_gpio_t door_close_led = {
	.port = &IO_5_PORT,
	.direction = &IO_5_DIRECTION,
	.pin_offset = IO_5_PIN,
	.input = &IO_5_INPUT
};









// Door opening

static state_t door_opening(void)
{
	set_as_output(&door_led); // makes pin output
	set_gpio(&door_led); // LED ON

	
	DELAY_ms(DOOR_OPEN_DURATION_MS); // time to open the door

	clear_gpio(&door_led); // LED OFF

	return DOOR_CLOSING; // next step after the door opening
}


// door closing function
static state_t door_closing(void) // please add LED here as well as instructions says that: "Door closing LED is on for 2 seconds"
{

	// LED on
	set_as_output(&door_close_led); // makes pin output
	set_gpio(&door_close_led); // LED ON


	DELAY_ms(DOOR_CLOSE_DURATION_MS); // time to close the door
	clear_gpio(&door_close_led); // LED OFF
	

	return IDLE; // waits for the next order (next floor)
}

// door closing function
static state_t movement_functions(void)
{
	

	set_as_output(&movement_led); // makes pin output
	set_gpio(&movement_led); // LED ON

	DELAY_ms(DOOR_CLOSE_DURATION_MS); // time to close the door
	clear_gpio(&movement_led); // LED OFF
	

	return DOOR_OPENING; // waits for the next order (next floor)
}




int
main(void)
{
	setup_uart_io();
	
	
	
	DDRB  |= (1 << PB4); //  set MISO as output, pin 12 (of UNO) (PB4)*89
	
	SPCR  |= (1 << SPE); // SPI is enabled
	
	/* Create variable data array that will be sent and received */
	char spi_send_data[20] = "Slave to master\n\r"; //to master
	char spi_receive_data[20]; //from master
	
	/* send message to master and receive message from master */
	while (1)
	{
		
		for(int8_t spi_data_index = 0; spi_data_index < sizeof(spi_send_data); spi_data_index++) //to read data from master
		{
			
			SPDR = spi_send_data[spi_data_index]; // Use SPI data register (SPDR) to send byte of data
			
			/* wait until the transmission is complete */
			
			//SPDR = 0x00;
			// SPIF is flag to check the transmission continuation
			// If SPIF is 1 it means transmission is completed
			while(!(SPSR & (1 << SPIF))) // waits to complete the data transmission
			{
				;
			}
			//char received = SPDR;
			//printf("%c",received);
			
			spi_receive_data[spi_data_index] = SPDR; // receive byte from the SPI data register
			printf("%c",spi_receive_data[spi_data_index]);
			
			// Movement conditions
			if (spi_receive_data[spi_data_index] == 'U' || spi_receive_data[spi_data_index] =='D' ){
				movement_functions();
			}
			
			if(spi_receive_data[spi_data_index] == 'O'){
				door_opening();
			}
			
			
		}
		
		
		
		/* Print the received data*/
		//printf("%s",spi_receive_data);
	}
	
	return 0;
}

