/*
 * param_screen_telemetry.c
 *
 *  Created on: 2010-04-29
 *      Author: john
 */

#include <stdio.h>
#include <avr/io.h>
#include <libcan/can.h>
#include <libpbr/timeout.h>
#include <libpbr/param.h>
#include <pbr_telemetry/telemetry_param_iface.h>

#include "param_screen.h"
#include "param_screen_telemetry.h"

static uint8_t					m_line_offset = 0;
static telemetry_param_struct_t	telemetry_params;
static uint8_t					m_mob_index = 0;
static uint8_t					dirty_parameters = 0;
static timeout_t				*redraw_timeout = 0;
const static uint8_t			redraw_delay = 100;
static mob_config_t 			mob;

static void prv_format_value (uint8_t *buf, uint8_t param);
static void prv_can_init (void);
static void prv_can_rx (uint8_t mob_index, uint32_t id, packet_type_t type);
static void prv_redraw_parameters (void);
static void prv_redraw_parameters_callback (void);

uint8_t
param_screen_telemetry_init (uint8_t line_offset, uint8_t mob_index)
{
	m_line_offset = line_offset;
	m_mob_index = mob_index;

	dirty_parameters = 1;
	prv_redraw_parameters ();
	prv_can_init ();

	return TELEMETRY_NUM_PARAMS;
}

void
param_screen_telemetry_change_value (uint8_t param, int8_t change)
{
	uint8_t update = 0;
	uint8_t buf[8];

	switch (param)
	{
	case param_telemetry_channel:
		if (change > 0 && telemetry_params.channel < CHANNEL_MAX)
		{
			*(uint8_t *)buf = (telemetry_params.channel + 1);
		}
		else if (change < 0 && telemetry_params.channel > CHANNEL_MIN)
		{
			*(uint8_t *)buf = (telemetry_params.channel - 1);
		}
		else
		{
			return;
		}
		break;
	case param_telemetry_pan_id:
		if (change > 0 && telemetry_params.pan_id < PAN_MAX)
		{
			*(uint16_t *)buf = (telemetry_params.pan_id + 1);
		}
		else if (change < 0 && telemetry_params.pan_id > PAN_MIN)
		{
			*(uint16_t *)buf = (telemetry_params.pan_id - 1);
		}
		else
		{
			return;
		}
		break;
	case param_telemetry_car_id:
		if (change > 0 && telemetry_params.car_id < XBEE_ID_MAX)
		{
			*(uint16_t *)buf = (telemetry_params.car_id + 1);
		}
		else if (change < 0 && telemetry_params.car_id > XBEE_ID_MIN)
		{
			*(uint16_t *)buf = (telemetry_params.car_id - 1);
		}
		else
		{
			return;
		}
		break;
	case param_telemetry_dta_id:
		if (change > 0 && telemetry_params.dta_node_address < XBEE_ID_MAX)
		{
			*(uint16_t *)buf = (telemetry_params.dta_node_address + 1);
		}
		else if (change < 0 && telemetry_params.dta_node_address > XBEE_ID_MIN)
		{
			*(uint16_t *)buf = (telemetry_params.dta_node_address - 1);
		}
		else
		{
			return;
		}
		break;
	case param_telemetry_dac_id:
		if (change > 0 && telemetry_params.dac_node_address < XBEE_ID_MAX)
		{
			*(uint16_t *)buf = (telemetry_params.dac_node_address + 1);
		}
		else if (change < 0 && telemetry_params.dac_node_address > XBEE_ID_MIN)
		{
			*(uint16_t *)buf = (telemetry_params.dac_node_address - 1);
		}
		else
		{
			return;
		}
		break;
	case param_telemetry_dta_enabled:
		if (telemetry_params.status_flags & dta_interface_enabled && change < 0)
		{
			*(uint8_t *)buf = 0;
		}
		else if (!(telemetry_params.status_flags & dta_interface_enabled) && change > 0)
		{
			*(uint8_t *)buf = 1;
		}
		break;
	case param_telemetry_dac_enabled:
		if (telemetry_params.status_flags & dac_interface_enabled && change < 0)
		{
			*(uint8_t *)buf = 0;
		}
		else if (!(telemetry_params.status_flags & dta_interface_enabled) && change > 0)
		{
			*(uint8_t *)buf = 1;
		}
		break;
	default:
		break;
	}

	mob.id = 0x8000 | (TELEMETRY_MODULE_ID << 8) | param;
	can_config_mob (m_mob_index, &mob);
	can_load_data (m_mob_index, buf, telemetry_param_defs[param].size);
	can_ready_to_send (m_mob_index);
}

static void
prv_redraw_parameters (void)
{
	if (redraw_timeout)
	{
		timeout_reset (redraw_timeout, redraw_delay);
	}
	else
	{
		timeout_set (redraw_delay, prv_redraw_parameters_callback, 0);
	}
}

static void
prv_redraw_parameters_callback (void)
{
	uint8_t value[5] = "XXXX";

	for (uint8_t i = 0; i < TELEMETRY_NUM_PARAMS; i++)
	{
		if (dirty_parameters == 0)
		{
			prv_format_value (value, i);
		}
		param_screen_draw_param_line (telemetry_param_defs[i].string, value, m_line_offset + i);
	}
	dirty_parameters = 0;
}

/**
 * Initialize CAN
 */
void
prv_can_init (void)
{
	mob.id = 0x4000 | (TELEMETRY_MODULE_ID << 8);
	mob.mask = 0x01ffff00;
	mob.id_type = extended;
	mob.rx_callback_ptr = prv_can_rx;
	//mob.tx_callback_ptr = prv_can_tx;
	can_config_mob (m_mob_index, &mob);
	can_ready_to_receive (m_mob_index);
}

/**
 * Callback for telemetry module parameter message rx
 */
static void
prv_can_rx (uint8_t mob_index, uint32_t id, packet_type_t type)
{
	uint8_t param = id & 0x0f;
	switch (param)
	{
	case param_telemetry_channel:
		can_read_data (mob_index, (uint8_t *)&(telemetry_params.channel), telemetry_param_defs[param].size);
		break;
	case param_telemetry_pan_id:
		can_read_data (mob_index, (uint8_t *)&(telemetry_params.pan_id), telemetry_param_defs[param].size);
		break;
	case param_telemetry_car_id:
		can_read_data (mob_index, (uint8_t *)&(telemetry_params.car_id), telemetry_param_defs[param].size);
		break;
	case param_telemetry_dta_id:
		can_read_data (mob_index, (uint8_t *)&(telemetry_params.dta_node_address), telemetry_param_defs[param].size);
		break;
	case param_telemetry_dac_id:
		can_read_data (mob_index, (uint8_t *)&(telemetry_params.dac_node_address), telemetry_param_defs[param].size);
		break;
	case param_telemetry_dta_enabled:
		//uint8_t f;
		//can_read_data (mob_index, &f, 1);
		// TODO
		//telemetry_params.status_flags |= (~telemetry_params.status_flags &)
		break;
	case param_telemetry_dac_enabled:
		// TODO
		break;
	default:
			break;
	}

	prv_redraw_parameters ();
	can_ready_to_receive (m_mob_index);
}

/*
 * Format the parameter value strings for the engine module parameters
 */
static void
prv_format_value
(
	uint8_t *buf,
	uint8_t param
)
{
	switch (param)
	{
		case param_telemetry_channel:
			sprintf ((char*)buf, "%4X", telemetry_params.channel);
			break;
		case param_telemetry_pan_id:
			sprintf ((char*)buf, "%4X", telemetry_params.pan_id);
			break;
		case param_telemetry_car_id:
			sprintf ((char*)buf, "%4X", telemetry_params.car_id);
			break;
		case param_telemetry_dta_id:
			sprintf ((char*)buf, "%4X", telemetry_params.dta_node_address);
			break;
		case param_telemetry_dac_id:
			sprintf ((char*)buf, "%4X", telemetry_params.dac_node_address);
			break;
		case param_telemetry_dta_enabled:
			sprintf ((char*)buf, "%4s", (telemetry_params.status_flags & dta_interface_enabled) ? "YES" : "NO");
			break;
		case param_telemetry_dac_enabled:
			sprintf ((char*)buf, "%4s", (telemetry_params.status_flags & dac_interface_enabled) ? "YES" : "NO");
			break;
		default:
			sprintf ((char*)buf, "%4c", 'X');
			break;
	}
}
