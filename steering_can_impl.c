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
	can_init (can_baud_1000);

	dta_can_impl_init (0);
}

