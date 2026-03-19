#include "mcu.h"
#include <stdint.h>
#include <stdio.h>

#include "lcd.h"
#include "keypad.h"

#define DOOR_OPEN_DURATION_MS (3000)
#define DOOR_CLOSE_DURATION_MS (2000)
#define FLOOR_MOVING_SPEED_MS (3000)
#define FAULT_BLINK_PERIOD_MS (200)
#define FAULT_BLINK_DURATION_MS (3000)

#define MIN_FLOOR 0
#define MAX_FLOOR 99

int16_t CURRENT_FLOOR = 0;

typedef enum {
	IDLE = 0,
	GOINGUP = 1,
	GOINGDOWN = 2,
	DOOR_OPENING = 3,
	DOOR_CLOSING = 4,
	FAULT = 5,
} state_t;

static int16_t floor_choice(void)
{
	int16_t destination_floor = 0;
	uint8_t key;
	uint8_t digits_entered = 0;

	while (1) {
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts("Choose floor");

		destination_floor = 0;
		digits_entered = 0;

		while (1) {
			key = KEYPAD_GetKey();

			if (key >= '0' && key <= '9') {
				if (digits_entered < 2) {
					destination_floor = (destination_floor * 10) + (key - '0');
					digits_entered++;

					lcd_gotoxy(0, 1);
					if (digits_entered == 1) {
						lcd_putc(key);
						} else {
						lcd_clrscr();
						lcd_gotoxy(0, 0);
						lcd_puts("Choose floor");
						lcd_gotoxy(0, 1);
						if (destination_floor >= 10) {
							lcd_putc((destination_floor / 10) + '0');
							lcd_putc((destination_floor % 10) + '0');
							} else {
							lcd_putc(destination_floor + '0');
						}
					}
				}
			}
			else if (key == '#') {
				if (digits_entered > 0 &&
				destination_floor >= MIN_FLOOR &&
				destination_floor <= MAX_FLOOR) {
					return destination_floor;
				}

				lcd_clrscr();
				lcd_gotoxy(0, 0);
				lcd_puts("Invalid input");
				break;
			}
			else if (key == '*') {
				destination_floor = 0;
				digits_entered = 0;
				lcd_clrscr();
				lcd_gotoxy(0, 0);
				lcd_puts("Choose floor");
			}
		}
	}
}

static state_t choose_direction(int16_t destination_floor)
{
	if (destination_floor > CURRENT_FLOOR) {
		//return GOINGUP;
	}
	else if (destination_floor < CURRENT_FLOOR) {
		//return GOINGDOWN;
	}
	else {
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts("Same floor");
		return FAULT;
	}
}