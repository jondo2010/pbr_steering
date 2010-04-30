/*
 * param_screen_engine.c
 *
 *  Created on: 2010-04-25
 *      Author: john
 */

#include <stdio.h>
#include <avr/io.h>
#include <libcan/can.h>
#include <libpbr/timeout.h>
#include <libpbr/param.h>
#include <pbr_engine/engine_param_iface.h>

#include "param_screen.h"
#include "param_screen_engine.h"

static uint8_t					m_line_offset = 0;
static engine_param_struct_t	engine_params;
static uint8_t					m_mob_index = 0;
static uint8_t					dirty_parameters = 0;
static timeout_t				*redraw_timeout = 0;
const static uint8_t			redraw_delay = 100;
static mob_config_t 			mob;

static void prv_format_value (uint8_t *buf, uint8_t param);
static void prv_can_init (void);
static void prv_can_rx (uint8_t mob_index, uint32_t id, packet_type_t type);
//static void prv_can_tx (uint8_t mob_index);
static void prv_redraw_parameters (void);
static void prv_redraw_parameters_callback (void);

uint8_t
param_screen_engine_init (uint8_t line_offset, uint8_t mob_index)
{
	m_line_offset = line_offset;
	m_mob_index = mob_index;

	dirty_parameters = 1;
	prv_redraw_parameters ();
	dirty_parameters = 0;
	prv_can_init ();

	return ENGINE_NUM_PARAMS;
}

void
param_screen_engine_change_value (uint8_t param, int8_t change)
{
	uint8_t update = 0;
	uint8_t buf[8];

	switch (param)
	{
		case param_engine_otstart_enabled:
			if (engine_params.otstart_enabled && change < 0)
			{
				*(uint8_t *)buf = 0;
				update = 1;
			}
			else if(engine_params.otstart_enabled == 0 && change > 0)
			{
				*(uint8_t *)buf = 1;
				update = 1;
			}
			break;
		case param_engine_otstart_timeout:
			if (change > 0 && engine_params.otstart_timeout < OTSTART_MAX_TIME)
			{
				*(uint16_t *)buf = (engine_params.otstart_timeout + OTSTART_GRAN);
				update = 2;
			}
			else if (change < 0 && engine_params.otstart_timeout > OTSTART_MIN_TIME)
			{
				*(uint16_t *)buf = (engine_params.otstart_timeout - OTSTART_GRAN);
				update = 2;
			}
			break;
		case param_engine_idle_rpm:
			if (change > 0 && engine_params.idle_rpm < IDLE_MAX)
			{
				*(uint16_t *)buf = (engine_params.idle_rpm + IDLE_GRAN);
				update = 2;
			}
			else if (change < 0 && engine_params.idle_rpm > IDLE_MIN)
			{
				*(uint16_t *)buf = (engine_params.idle_rpm - IDLE_GRAN);
				update = 2;
			}
			break;
		default:
			break;
	}

	if (update)
	{
		mob.id = 0x8000 | (ENGINE_MODULE_ID << 8) | param;
		can_config_mob (m_mob_index, &mob);
		can_load_data (m_mob_index, buf, update);
		can_ready_to_send (m_mob_index);
	}
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

	for (uint8_t i = 0; i < ENGINE_NUM_PARAMS; i++)
	{
		if (dirty_parameters == 0)
		{
			prv_format_value (value, i);
		}
		param_screen_draw_param_line (engine_param_defs[i].string, value, m_line_offset + i);
	}
}

/**
 * Initialize CAN
 */
static void
prv_can_init (void)
{
	mob.id = 0x4000 | (ENGINE_MODULE_ID << 8);
	mob.mask = 0x01ffff00;
	mob.id_type = extended;
	mob.rx_callback_ptr = prv_can_rx;
	//mob.tx_callback_ptr = prv_can_tx;
	can_config_mob (m_mob_index, &mob);
	can_ready_to_receive (m_mob_index);
}

/**
 * Callback for engine module parameter message rx
 */
static void
prv_can_rx (uint8_t mob_index, uint32_t id, packet_type_t type)
{
	uint8_t param = id & 0x0f;
	switch (param)
	{
		case param_engine_otstart_enabled:
			can_read_data(mob_index, &(engine_params.otstart_enabled), engine_param_defs[param].size);
			break;
		case param_engine_otstart_timeout:
			can_read_data(mob_index, (uint8_t *)&(engine_params.otstart_timeout), engine_param_defs[param].size);
			break;
		case param_engine_idle_rpm:
			can_read_data(mob_index, (uint8_t *)&(engine_params.idle_rpm), engine_param_defs[param].size);
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
		case param_engine_otstart_enabled:
			sprintf ((char*)buf, "%4s", engine_params.otstart_enabled ? "YES" : "NO");
			break;
		case param_engine_otstart_timeout:
			sprintf ((char*)buf, "%4u", engine_params.otstart_timeout);
			break;
		case param_engine_idle_rpm:
			sprintf ((char*)buf, "%4u", engine_params.idle_rpm);
			break;
		default:
			sprintf ((char*)buf, "%4c", 'X');
			break;
	}
}
