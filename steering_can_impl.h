/*
 * steering_can_impl.h
 *
 *  Created on: 2010-01-31
 *      Author: john
 */

#ifndef STEERING_CAN_IMPL_H_
#define STEERING_CAN_IMPL_H_

#include <libcan/can.h>
#include <libpbr/dtafast.h>

dta_struct_t *
can_impl_get_dta_struct (void);

void
can_impl_rx_dta
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
);

void
steering_can_impl_init (void);

/**
 * Callback for when a DTA packet arrives
 */
void
steering_can_impl_dta_rx
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
);

#endif /* STEERING_CAN_IMPL_H_ */
