#include "power_save.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define WAKEUP_DDR   DDRH
#define WAKEUP_PORT  PORTH
#define WAKEUP_PIN   PH4

// Flag to detect activity
volatile uint8_t wake_flag = 0;



void power_save_init(void)
{
    WAKEUP_DDR &= ~(1 << WAKEUP_PIN);     // input
    WAKEUP_PORT |= (1 << WAKEUP_PIN);     // pull-up

    EIMSK |= (1 << INT4);                 // enable INT4

    // falling edge trigger
    EICRB |= (1 << ISC41);
    EICRB &= ~(1 << ISC40);

    sei();
}

// 10 SECOND WAIT

uint8_t wait_before_sleep(void)
{
    wake_flag = 0;

    for (uint8_t i = 0; i < 10; i++)   // 10 seconds
    {
        _delay_ms(1000);               // wait 1 second

        if (wake_flag)                // button pressed?
        {
            return 0;                // cancel sleep
        }
    }

    return 1; // safe to sleep
}


void enter_light_sleep(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();

    sleep_cpu();   // sleep here

    sleep_disable();
}



ISR(INT4_vect)
{
    wake_flag = 1;   // mark that button was pressed
}
