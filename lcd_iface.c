/*
 * lcd_iface.c
 *
 *  Created on: 2010-04-29
 *      Author: john
 */

#include <avr/io.h>
#include <util/delay.h>
#include "lcd_iface.h"

void
lcd_iface_init (void)
{
	/// Enable external memory interface
	//XMCRA = _BV (SRE); // | _BV (SRW10) | _BV (SRW11) | _BV (SRW11) | _BV (SRW00);
	XMCRA = _BV (SRE);

	/// Enable bus-keeper, release Port C
	XMCRB = /*_BV (XMBK) |*/ _BV (XMM2) | _BV (XMM1) | _BV (XMM0);

	/// Boost enable pin, turn off for now
	DDRC |= _BV (PC7);

	/// LCD Reset line
	DDRD |= _BV (PD7);
	PORTD &= ~_BV (PD7);
}

