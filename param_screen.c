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
#include "font.h"
#include "param_screen.h"
#include "param_screen_engine.h"
#include "input.h"

static module_param_t module_parameter_top;
static module_param_t module_parameter_bottom;

static module_param_t current_parameter;
static uint8_t parameter_scroll = 0;

const void (*param_value_function_map[])(uint8_t *, module_param_t) =
{
	0,	// param_general_vdm_mode		= 0x00,		/* VDM Mode */

	/* Telemetry */
	0, //param_telemetry_channel		= 0x01,		/* Zigbee channel */
	0, //param_telemetry_pan			= 0x02,		/* 16-bit PAN ID */
	0, //param_telemetry_car_address	= 0x03,		/* 16-bit address of the car's modem */
	0, //param_telemetry_dta_address	= 0x04,
	0, //param_telemetry_dac_address	= 0x05,
	0, //param_telemetry_dta_enabled	= 0x06,
	0, //param_telemetry_dac_enabled	= 0x07,

	/* Brake */
	0, //param_brake_bias				= 0x08,		/* Signed 8-bit brake bias front-to-back */

	/* Engine */
	param_screen_engine_format_value, //param_engine_otstart_enabled	= 0x09,		/* One-touch start */
	param_screen_engine_format_value, //param_engine_otstart_timeout	= 0x0a,		/* Timeout in ms */
	param_screen_engine_format_value, //param_engine_idle_rpm			= 0x0b,		/* Idle RPM used to determine if running */

	0, //param_test1,
	0, //param_test2,
	0, //param_test3,
	0, //param_test4,
	0 //param_test5
};

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
test ()
{
	static uint8_t x = 0;

	if ((x++) % 2 > 0)
	{
		lcd_draw_clock (10,56);
	}
	else
	{
		lcd_draw_clock (00,00);
	}

	timeout_set (50, test, 0);
}

void
param_screen_value_change (int8_t change)
{
	timeout_set (50, test, 0);
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
	lcd_set_scroll (TXT1_ADDRESS, 0x1e, GFX1_ADDRESS, 0xef, TXT2_ADDRESS + GFX_WIDTH * parameter_scroll);
}

/**
 * Draw the parameter line text into the text buffer
 */
void
param_screen_draw_param_line (module_param_t param)
{
	uint8_t buf[FONT_CHAR_WIDTH], value[5];
	int8_t param_len = strlen((char*)module_param_strings[param]);

	if (param_value_function_map[param])
		param_value_function_map[param](value, param);
	else
		strcpy((char*)value, "X");

	/* Draw the parameter string, padding spaces, and parameter value seperately since
	 * the implementation of printf here doesn't support passing width as a parameter.
	 */
	sprintf ((char*)buf, "%-s:", module_param_strings[param]);
	memset ((char*)buf+param_len+1, ' ', FONT_CHAR_WIDTH-param_len-4);
	snprintf ((char*)buf+FONT_CHAR_WIDTH-4, 5, "%4s", value);

	//sprintf ((char*)buf, "%-*s: %*s", param_len, module_param_strings[param], FONT_CHAR_WIDTH - param_len - 1, value);
	//sprintf ((char*)buf, "%-*s:", 13, module_param_strings[param]);

	font_draw_string (buf, TXT2_ADDRESS + GFX_WIDTH*(param-1), FONT_CHAR_WIDTH);
}

void
param_screen_init (void)
{
	uint8_t i;

	module_parameter_top = param_telemetry_channel;
	module_parameter_bottom = param_test5;

	for (i = module_parameter_top; i <= module_parameter_bottom; i++)
	{
		param_screen_draw_param_line (i);
	}

	current_parameter = param_telemetry_channel;
	param_screen_param_change (-1);

	input_set_parameter_changed_callback (param_screen_param_change);
	input_set_value_changed_callback (param_screen_value_change);


}
