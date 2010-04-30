/*
 * steering.c
 *
 *  Created on: 2010-01-20
 *      Author: john
 */

#include <avr/io.h>

#include "lcd_iface.h"
#include "lcd.h"
#include "steering.h"
#include "steering_can_impl.h"
#include "input.h"
#include "param_screen.h"

void
steering_init_lcd (void)
{
	steering_lcd_boost_disable ();
	lcd_iface_init ();

	lcd_reset ();
	lcd_system_set (0x28,0x2c);
	lcd_init ();
	lcd_clear_screen ();

	lcd_draw_signal_strength (4,37,1);
	lcd_draw_top_underline ();

	font_init ();
	lcd_draw_clock (10,56);

	steering_lcd_boost_enable ();
}

void
steering_init (void)
{
	steering_can_impl_init ();
	steering_init_lcd ();
	param_screen_init ();

	input_init ();
	input_enable ();
}
