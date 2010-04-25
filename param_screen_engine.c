/*
 * param_screen_engine.c
 *
 *  Created on: 2010-04-25
 *      Author: john
 */

#include <avr/io.h>
#include <pbr_engine/engine_param_iface.h>
#include <libpbr/param.h>
#include <stdio.h>

#include "param_screen_engine.h"

/*
 * Format the parameter value strings for the engine module parameters
 */
void
param_screen_engine_format_value (uint8_t *buf, module_param_t param)
{
	switch (param) {
		case param_engine_otstart_enabled:
			sprintf ((char*)buf, "%4s", "YES");
			break;
		case param_engine_otstart_timeout:
			sprintf ((char*)buf, "%4u", 0);
			break;
		case param_engine_idle_rpm:
			sprintf ((char*)buf, "%4u", 0);
			break;
		default:
			sprintf ((char*)buf, "%4c", 'X');
			break;
	}
}
