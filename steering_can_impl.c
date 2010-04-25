/*
 * steering_can_impl.c
 *
 *  Created on: 2010-01-31
 *      Author: john
 */

#include <avr/io.h>
#include <libcan/can.h>
#include <libpbr/dtafast.h>
#include <libpbr/dta_can_impl.h>

#include "steering_can_impl.h"

void
steering_can_impl_init (void)
{
	mob_config_t mob;

	can_init (can_baud_1000);

	mob.id = 0x330;
	mob.mask = 0xffffffff;
	mob.id_type = standard;
	mob.rx_callback_ptr = 0; //steering_can_impl_error_rx;
	mob.tx_callback_ptr = 0;
	can_config_mob (0, &mob);

	dta_can_impl_init (1);
}

/**
 * Callback for when an error message is received
 */
void
steering_can_impl_error_rx
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
)
{
	can_ready_to_receive (mob_index);
}


