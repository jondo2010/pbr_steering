/*
 * steering.c
 *
 *  Created on: 2010-01-20
 *      Author: john
 */

#include <avr/io.h>

#include "steering.h"

#define steering_lcd_boost_enable() \
	PORTC |= _BV (PC7)

#define steering_lcd_boost_disable() \
	PORTC &= ~_BV (PC7);

void
steering_init_lcd (void)
{
	/// Enable external memory interface
	XMCRA = _BV (SRE) | _BV (SRW10); // | _BV (SRW10) | _BV (SRW11);

	/// Enable bus-keeper, release Port C
	XMCRB = _BV (XMBK) | _BV (XMM2) | _BV (XMM1) | _BV (XMM0);

	/// Boost enable pin, turn off for now
	DDRC |= _BV (PC7);
	steering_lcd_boost_disable ();

	/// LCD Reset line
	DDRD |= _BV (PD7);
	PORTD &= ~_BV (PD7);

	lcd_reset ();
	lcd_system_set (0x28,0x2c);
	lcd_init ();
	lcd_clear_screen ();

	lcd_draw_signal_strength (4,37,1);
	lcd_draw_top_underline ();
	lcd_draw_clock (10,56);

	font_init ();
	//font_draw_string (text, 0x0028, strlen(text));
	param_screen_init ();

	steering_lcd_boost_enable ();
}

void
steering_init (void)
{
	/// Status LED
	DDRB = _BV (PB7);

	steering_init_lcd ();

	input_init ();
	//input_set_parameter_changed_callback (param_callback);
	input_enable ();

	steering_can_impl_init ();
}
