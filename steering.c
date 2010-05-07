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
#include "draw.h"
//#include "picture.h"

extern const prog_uchar pbr_logo[];
extern const prog_uchar nhd_logo[];

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

void logo()
{
	static uint8_t w = 0;
	lcd_clear_screen ();
	if (w)
	{
		draw_image (pbr_logo, 0, 0, 320/8, 6837);
		w = 0;
	}
	else
	{
		draw_image (nhd_logo, 0, 240/8/2, 320/8, 933);
		w = 1;
	}
}

void
steering_init (void)
{
	steering_can_impl_init ();
	steering_init_lcd ();
	dashboard_screen_init ();
	//param_screen_init ();

	//timeout_set (2000, logo, 1);

	input_init ();
	input_enable ();
}
