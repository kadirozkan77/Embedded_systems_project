
// Libraries 
#include "mcu.h"
#include <stdint.h>
#include <stdio.h> 
#include "lcd.h"
#include "keypad.h"

// Constants
#define DOOR_OPEN_DURATION_MS (3000) // door is open for 3 seconds
#define DOOR_CLOSE_DURATION_MS (2000) // door is closed in 2 seconds
#define FLOOR_MOVING_SPEED_MS (3000) // speed:  1 floor/3 seconds

// if obstacle is detected, the LEDs blinking time
#define FAULT_BLINK_PERIOD_MS (200)
#define FAULT_BLINK_DURATION_MS (3000)

// Floor numbers
#define MIN_FLOOR 0
#define MAX_FLOOR 99

// Global variable (initial)
int16_t CURRENT_FLOOR = 0; // data type is int 16 bit and t is for type


// state 
typedef enum { // typedef is for defining state_t (to make the code readable)
	IDLE = 0, // empty
	GOINGUP = 1, 
	GOINGDOWN = 2,
	DOOR_OPENING = 3,
	DOOR_CLOSING = 4,
	FAULT = 5, // fault
} state_t; // enum includes all the possible states of elevator



// Takes input as destination floor
static int16_t floor_choice(void)
{
	int16_t destination_floor = 0;
	uint8_t key; // pressed key (gets max 255 which is enough)
	uint8_t digits_entered = 0; //entered digit (max 2)

	while (1) { // continuously waits for input from user
		lcd_clrscr(); // clear the screen
		lcd_gotoxy(0, 0); // positions the cursor to left up corner (1st row 1st column)
		lcd_puts("Choose floor"); // prints it to screen
		
		// resets inputs
		destination_floor = 0;
		digits_entered = 0;

		while (1) { // reading key
			key = KEYPAD_GetKey(); // read key from keypad

			if (key >= '0' && key <= '9') { // checks if key is number
				if (digits_entered < 2) { // checks digit number
					destination_floor = (destination_floor * 10) + (key - '0'); // move the entered number to left and place the new number to right
					// '0' converts character to number
					digits_entered++; // count digits

					lcd_gotoxy(0, 1); // go to next row
					if (digits_entered == 1) {
						lcd_putc(key); // prints the first entered number
						} 
						else {
						lcd_clrscr(); 
						lcd_gotoxy(0, 0); // top left corner
						lcd_puts("Choose floor");
						lcd_gotoxy(0, 1); // next row
						if (destination_floor >= 10) {
							lcd_putc((destination_floor / 10) + '0'); // print first number (*10)
							lcd_putc((destination_floor % 10) + '0'); // print second number (to left)
							} else {
							lcd_putc(destination_floor + '0');
						}
					}
				}
			}
			else if (key == '#') { // confirms the entered number
				if (digits_entered > 0 &&
				destination_floor >= MIN_FLOOR &&
				destination_floor <= MAX_FLOOR) { // checks if one number is entered and it is between 0-99
					return destination_floor; 
				}
				
				// else
				lcd_clrscr();
				lcd_gotoxy(0, 0);
				lcd_puts("Invalid input");
				break;
			}
			else if (key == '*') { // reset key
				destination_floor = 0;
				digits_entered = 0;
				lcd_clrscr();
				lcd_gotoxy(0, 0);
				lcd_puts("Choose floor");
			}
		}
	}
}

static state_t choose_direction(int16_t destination_floor) // type is state_t because it returns the states that are defined in enum
{
	if (destination_floor > CURRENT_FLOOR) { // decides the direction
		//return GOINGUP;
	}
	else if (destination_floor < CURRENT_FLOOR) {
		//return GOINGDOWN;
	}
	else { // if destination floor=current floor warns the user
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts("Same floor"); // prints same floor
		return FAULT; // returns 5 (in enum as fault)
	}
}
