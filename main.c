/*
 * main.c
 *
 *  Created on: 2010-01-19
 *      Author: john
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"
#include "extmem.h"
#include "input.h"
//#include "logo.h"
#include "param_screen.h"

#include "can_impl.h"

static uint8_t delay = 50;
static uint8_t hscroll = 0;

void
param_callback (uint8_t b)
{
	delay = b;
	lcd_draw_signal_strength(b,37,1);
}

void
value_callback (uint8_t b)
{
	//hscroll = b;
	//draw_rpm_bargraph (b);
}

int
main (void)
{
	/// Status LED
	DDRB = _BV (PB7);

	/// Boost enable pin
	DDRC |= _BV (PC7);
	PORTC |= _BV (PC7);

	/// LCD Reset line
	DDRD |= _BV (PD7);
	PORTD &= ~_BV (PD7);

	extmem_init ();
	//DDRG |= _BV (PG0) | _BV (PG1) | _BV (PG2);
	//DDRA = 0xFF;
	//PORTG |= _BV (PG0) | _BV (PG1);

	lcd_reset ();
	lcd_system_set (0x28,0x2c);
	lcd_init ();
	lcd_clear_screen ();

	//uint8_t *text = "LCD LARGE FONT TEST 0123456789";
	//lcd_draw_string( text, 0x0000, 12 );
	//lcd_draw_bitmap (logo);

	lcd_draw_signal_strength (4,37,1);
	lcd_draw_top_underline ();
	lcd_draw_clock (10,56);

	font_init ();
	//font_draw_string (text, 0x0028, strlen(text));
	param_screen_init ();

	input_init ();
	input_set_parameter_changed_callback (param_callback);
	input_set_value_changed_callback (param_screen_param_change);
	input_enable ();

	can_impl_int ();

	sei ();

	for (;;)
	{
		PORTB ^= _BV (PB7);
		_delay_ms(delay);
		//draw_rpm_bargraph (can_impl_get_dta_struct ()->data1.rpm);
	}

	return 0;
}
