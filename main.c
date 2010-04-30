/*
 * main.c
 *
 *  Created on: 2010-01-19
 *      Author: john
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <libpbr/timeout.h>

#include "lcd.h"
#include "steering.h"
#include "input.h"
//#include "logo.h"
#include "param_screen.h"

#include "steering_can_impl.h"

static uint8_t delay = 50;
static uint8_t hscroll = 0;

void
param_callback (uint8_t b)
{
	delay = b;
	lcd_draw_signal_strength(b,37,1);
}

void
blinker_timeout ()
{
	PORTB ^= _BV (PB7);
	timeout_set (100, blinker_timeout, 0);
}

int
main (void)
{
	/// Status LED
	DDRB = _BV (PB7);

	steering_init ();
	timeout_init ();
	timeout_set (100, blinker_timeout, 0);

	//CLKPR = _BV (CLKPCE);
	//CLKPR = 0; //_BV (CLKPS0); /// Scale master clock by 2

	sei ();

	for (;;)
	{
		//draw_rpm_bargraph (can_impl_get_dta_struct ()->data1.rpm);
	}

	return 0;
}
