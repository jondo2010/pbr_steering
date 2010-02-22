/*
 * can_impl.h
 *
 *  Created on: 2010-01-31
 *      Author: john
 */

#ifndef CAN_IMPL_H_
#define CAN_IMPL_H_

#include <can.h>

#include "dtafast.h"

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
can_impl_int (void);

#endif /* CAN_IMPL_H_ */
