/*
 * param_screen.c
 *
 *  Created on: 2010-02-20
 *      Author: john
 */

#include <avr/io.h>
#include <libpbr/param.h>
#include <libpbr/timeout.h>
#include <stdio.h>
#include <string.h>

#include "lcd.h"
#include "lcd_iface.h"
#include "font.h"
#include "input.h"

#include "param_screen.h"
#include "param_screen_engine.h"
#include "param_screen_telemetry.h"

static uint8_t num_parameters_engine = 0;
static uint8_t num_parameters_telemetry = 0;
static uint8_t num_parameters_braking = 0;
static uint8_t num_parameters = 0;

static uint8_t current_parameter_offset = 0;	/// Currently selected line on screen
static uint8_t scroll_offset = 0;

static uint8_t parameter_locked = 0;
static uint8_t value_locked = 0;

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
param_screen_value_change (int8_t change)
{
	if (value_locked)
	{
		return;
	}

	uint8_t param = current_parameter_offset + scroll_offset;

	if (param < num_parameters_engine + 1)
	{
		param_screen_engine_change_value (param, change);
	}
	else if (param < num_parameters_telemetry + num_parameters_engine)
	{
		param_screen_telemetry_change_value (param - num_parameters_engine, change);
	}
}

void
param_screen_param_change (int8_t change)
{
	if (parameter_locked)
	{
		return;
	}

	if (change > 0 && current_parameter_offset + scroll_offset < num_parameters - 1)
	{
		if (current_parameter_offset == (PIXELS_H/16 -2))	/// At bottom
		{
			scroll_offset++;
			param_screen_update_scroll ();
		}
		else
		{
			param_draw_block((uint8_t) (current_parameter_offset-scroll_offset+1), 40, 0x00);
			param_draw_block((uint8_t) (++current_parameter_offset-scroll_offset+1), 40, 0xff);
		}
	}
	else if (change < 0 && current_parameter_offset + scroll_offset > 0)
	{
		if (current_parameter_offset == 0)					/// At top
		{
			scroll_offset--;
			param_screen_update_scroll ();
		}
		else
		{
			param_draw_block ((uint8_t) (current_parameter_offset-scroll_offset+1), 40, 0x00);
			param_draw_block ((uint8_t) (--current_parameter_offset-scroll_offset+1), 40, 0xff);
		}
	}
}

void
param_screen_update_scroll (void)
{
	lcd_set_scroll (TXT1_ADDRESS, 0x1e, GFX1_ADDRESS, 0xef, TXT2_ADDRESS + GFX_WIDTH * scroll_offset);
}

/**
 * Draw the parameter line text into the text buffer
 */
void
param_screen_draw_param_line
(
	const char	*param_string,
	char		*value,
	uint8_t		line_offset
)
{
	uint8_t buf[FONT_CHAR_WIDTH];
	int8_t string_len = strlen(param_string);

	/* Draw the parameter string, padding spaces, and parameter value seperately since
	 * the implementation of printf here doesn't support passing width as a parameter.
	 */
	sprintf ((char*)buf, "%-s:", param_string);
	memset ((char*)buf+string_len+1, ' ', FONT_CHAR_WIDTH-string_len-4);
	snprintf ((char*)buf+FONT_CHAR_WIDTH-4, 5, "%4s", value);

	//sprintf ((char*)buf, "%-*s: %*s", param_len, module_param_strings[param], FONT_CHAR_WIDTH - param_len - 1, value);
	//sprintf ((char*)buf, "%-*s:", 13, module_param_strings[param]);

	font_draw_string (buf, TXT2_ADDRESS + GFX_WIDTH*(line_offset-1), FONT_CHAR_WIDTH);
}

void
param_screen_lock_parameter (uint8_t lock)
{

}

void
param_screen_init (void)
{
	num_parameters_engine = param_screen_engine_init (1, 5);
	num_parameters_telemetry = param_screen_telemetry_init (num_parameters_engine+1, 6);
	num_parameters = num_parameters_engine + num_parameters_telemetry + num_parameters_braking;

	input_set_parameter_changed_callback (param_screen_param_change);
	input_set_value_changed_callback (param_screen_value_change);
}
