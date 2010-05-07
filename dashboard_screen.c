/*
 * main_screen.c
 *
 *  Created on: 2010-01-30
 *      Author: john
 */

#include <avr/io.h>
#include "libcan/can.h"
#include <libpbr/dta_can_impl.h>
#include "lcd_iface.h"
#include "lcd.h"
#include "font.h"
#include "draw.h"

extern const prog_uchar *gear_graphics[];
extern const uint16_t gear_graphics_lengths[];

void prv_redraw_screen (void);
void prv_draw_rpm_bargraph (uint16_t rpm);
void prv_draw_gear (void);
void prv_value_change (int8_t change);

void
dashboard_screen_init (void)
{
	input_set_parameter_changed_callback (0);
	input_set_value_changed_callback (prv_value_change);

	//timeout_set (25, to, 1);

	prv_draw_gear ();
}

void
prv_redraw_screen (void)
{
	prv_draw_gear ();
	prv_draw_rpm_graph ();
}

void
prv_value_change (int8_t change)
{
	uint8_t g = 0;
	g+=change;
	prv_draw_gear ();
}

void
prv_draw_gear (void)
{
	uint8_t gear = dta_can_impl_get_dta_struct ()->gear;

	if (gear > 6)
	{
		gear = 6;
	}

	draw_image (gear_graphics[gear], 16, 120, 8, gear_graphics_lengths[gear]);
}

#define MAX_RPM				10000
#define GRAPH_TOP_Y			30
#define GRAPH_BOTTOM_Y		222
#define GRAPH_HEIGHT		(GRAPH_BOTTOM_Y - GRAPH_TOP_Y)
#define GRAPH_WIDTH			(320 - 16 * 5)

void
prv_draw_rpm_graph ()
{
	uint16_t rpm = dta_can_impl_get_dta_struct ()->rpm;

	char buf[6];
	snprintf(buf, 6, "%5u", rpm);

	font_draw_string (buf, TXT2_ADDRESS + GFX_WIDTH*(15), 5);

	for (uint8_t row=0; row < 16; row++)
	{
		lcd_set_cursor_xy (16 * 5, 224+row);

		if ((rpm*100)/MAX_RPM >= (GRAPH_WIDTH - n)*100/(GRAPH_WIDTH))

		for (uint8_t col=0; col < 320 - 16 * 5; col++)
		{
			LCD_DATA = 0xff;
		}
	}
}

void
prv_draw_rpm_bargraph (uint16_t rpm)
{
	static uint16_t last_rpm;
	static uint8_t first_run = 1;
	uint8_t col, n;
	uint8_t base, pattern = 0xaa;
	uint8_t top, height;

	lcd_set_cursor_direction (CURSOR_DOWN);
	lcd_write_data ();

	if (first_run)
	{
		top = GRAPH_TOP_Y;
		height = GRAPH_HEIGHT;
	}
	else
	{
		//top = (rpm > last_rpm) ?
	}

	/// Draw the bargraph in 3 colummns
	for (col=0; col<3; col++)
	{
		if (col==0)
		{
			base = 0x80;
		}
		else if (col==2)
		{
			base = 0x01;
		}
		else
		{
			base = 0;
		}

		lcd_set_cursor_xy (col,GRAPH_TOP_Y);
		lcd_write_data ();
		LCD_DATA = 0xff; /// Outline at top
		for (n=0; n<GRAPH_HEIGHT-2; n++)
		{
			if ((rpm*100)/MAX_RPM >= (GRAPH_HEIGHT - n)*100/(GRAPH_HEIGHT))
				LCD_DATA = base | pattern;
			else
				LCD_DATA = base;

			pattern = ~pattern;
		}
		LCD_DATA = 0xff;
	}
}
