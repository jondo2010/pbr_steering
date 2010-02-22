/*
 * can_impl.c
 *
 *  Created on: 2010-01-31
 *      Author: john
 */

#include <avr/io.h>

#include <can.h>

#include "dtafast.h"
#include "can_impl.h"

static dta_struct_t dta_data;

dta_struct_t *
can_impl_get_dta_struct (void)
{
	return &dta_data;
}

void
can_impl_rx_dta
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
)
{
	switch (id)
	{
	case 0x2000:
		can_read_data (mob_index, (uint8_t *) &dta_data.data1, 8);
		break;
	case 0x2001:
		can_read_data (mob_index, (uint8_t *) &dta_data.data2, 8);
		break;
	case 0x2002:
		can_read_data (mob_index, (uint8_t *) &dta_data.data3, 8);
		break;
	case 0x2003:
		can_read_data (mob_index, (uint8_t *) &dta_data.data4, 8);
		break;
	}
}

void
can_impl_int (void)
{
	can_init (baud_1000);

	mob_config_t dta_mob;
	dta_mob.id = 0x2000;
	dta_mob.mask = 0x01FFFFFC;
	dta_mob.mask = 0x00;
	dta_mob.id_type = extended;
	dta_mob.mode = receive;
	dta_mob.rx_callback_ptr = &can_impl_rx_dta;
	can_config_mob (0, &dta_mob);
}
