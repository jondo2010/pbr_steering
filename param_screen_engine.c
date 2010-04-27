/*
 * param_screen_engine.c
 *
 *  Created on: 2010-04-25
 *      Author: john
 */

#include <avr/io.h>
#include <libcan/can.h>
#include <pbr_engine/engine_param_iface.h>
#include <libpbr/param.h>
#include <stdio.h>

#include "param_screen_engine.h"

/**
 * Initialize CAN
 */
void
param_screen_engine_can_init (uint8_t mob_index)
{
}

/**
 * Callback for engine module parameter message rx
 */
void
param_screen_engine_params_rx
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
)
{
	//can_read_data (mob_index, (uint8_t *) &dta_data.data1, 8);
}

void
param_screen_engine_params_tx (uint8_t mob_index)
{

}

/**
 * Poll the parameters from the engine module
 */
void
param_screen_engine_poll_parameters (void)
{
	mob_config_t mob;

	mob.id = 0x510;
	mob.mask = 0xffffffff;
	mob.id_type = standard;
	mob.rx_callback_ptr = param_screen_engine_params_rx;
	//mob.tx_callback_ptr
}

/*
 * Format the parameter value strings for the engine module parameters
 */
void
param_screen_engine_format_value
(
	uint8_t *buf,
	module_param_t param
)
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
