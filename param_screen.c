/*
 * param_screen.c
 *
 *  Created on: 2010-02-20
 *      Author: john
 */

#include <avr/io.h>
#include "lcd.h"
#include "param.h"
#include "font.h"
#include "param_screen.h"

static module_param_t module_parameter_top;
static module_param_t module_parameter_bottom;

static module_param_t current_parameter;
static uint8_t parameter_scroll = 0;

/**
 * Draw a block backgrounding text
 */
void
param_draw_block
(
	uint8_t line,
	uint8_t width,
	uint8_t value
)
{
	uint8_t i, j;
	lcd_set_cursor_direction (CURSOR_DOWN);

	for (i=0; i < width; i++)
	{
		lcd_set_cursor_xy (i, (line+1)*16);
		LCD_COMM = MWRITE;
		for (j=0; j < 16; j++)
		{
			LCD_DATA = value;
		}
	}
}

void
param_screen_param_change (int8_t change)
{
	if (change > 0 && current_parameter != module_parameter_bottom)
	{
		if (current_parameter - parameter_scroll == (PIXELS_H/16 - 2))
		{
			current_parameter++;
			parameter_scroll++;
			param_screen_update_scroll ();
		}
		else
		{
			param_draw_block((uint8_t) (current_parameter-parameter_scroll), 40, 0x00);
			param_draw_block((uint8_t) (++current_parameter-parameter_scroll), 40, 0xff);
		}
	}
	else if (change < 0 && current_parameter != module_parameter_top)
	{
		if (current_parameter - parameter_scroll == 1)
		{
			current_parameter--;
			parameter_scroll--;
			param_screen_update_scroll ();
		}
		else
		{
			param_draw_block ((uint8_t) (current_parameter-parameter_scroll), 40, 0x00);
			param_draw_block ((uint8_t) (--current_parameter-parameter_scroll), 40, 0xff);
		}
	}
}

void
param_screen_update_scroll (void)
{
	lcd_set_scroll (TXT1_ADDRESS, 0x20, GFX1_ADDRESS, 0xef, TXT2_ADDRESS + GFX_WIDTH * parameter_scroll);
}

void
param_screen_init (void)
{
	uint8_t i;

	module_parameter_top = param_telemetry_channel;
	module_parameter_bottom = param_test5;

	for (i = module_parameter_top; i <= module_parameter_bottom; i++)
	{
		font_draw_string (module_param_strings[i], TXT2_ADDRESS + GFX_WIDTH*(i-1), strlen(module_param_strings[i]));
	}

	current_parameter = param_telemetry_channel;
	param_screen_param_change (-1);
}
