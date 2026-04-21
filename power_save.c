#include "power_save.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Using INT0 (PD2 on UNO)
// This pin is connected to the button
#define WAKEUP_DDR   DDRH
#define WAKEUP_PORT  PORTH
#define WAKEUP_PIN   PDH4



void power_save_init(void)
{
    // Set PD2 as INPUT (button pin)
    WAKEUP_DDR &= ~(1 << WAKEUP_PIN);

    // Enable internal pull-up resistor
    // Pin is normally HIGH
    // When button is pressed, goes LOW
    WAKEUP_PORT |= (1 << WAKEUP_PIN);

    // Enable external interrupt INT0
    // MCU will "watch" this pin
    EIMSK |= (1 << INT4);

    // Set interrupt to trigger on FALLING EDGE
    // HIGH, LOW (button press)
    EICRA |= (1 << ISC41);
    EICRA &= ~(1 << ISC40);

    // Enable global interrupts (must have!)
    sei();
}



void enter_light_sleep(void)
{
    // Select light sleep mode (CPU stops, peripherals still work)
    set_sleep_mode(SLEEP_MODE_IDLE);

    // Allow MCU to sleep
    sleep_enable();

    // MCU goes to sleep here 
    // It will stay here until interrupt happens
    sleep_cpu();

    // After wake-up, disable sleep
    sleep_disable();
}



// Interrupt Service Routine (ISR)
// This runs when button is pressed
ISR(INT0_vect)
{
    // Nothing needed here
    // This interrupt automatically wakes the MCU 
}
