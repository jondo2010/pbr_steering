/*
 * main_screen.c
 *
 *  Created on: 2010-01-30
 *      Author: john
 */

#include <avr/io.h>
#include "lcd_iface.h"
#include "lcd.h"

#define MAX_RPM				255
#define GRAPH_TOP_Y			30
#define GRAPH_BOTTOM_Y		222
#define GRAPH_HEIGHT		GRAPH_BOTTOM_Y - GRAPH_TOP_Y


void
draw_rpm_bargraph
(
	uint16_t rpm
)
{
	static uint16_t last_rpm;
	static uint8_t first_run = 1;
	uint8_t col, n;
	uint8_t base, pattern = 0xaa;
	uint8_t top, height;

	lcd_set_cursor_direction (CURSOR_DOWN);

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
