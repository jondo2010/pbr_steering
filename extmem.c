/*
 * extmem.c
 *
 *  Created on: 2010-01-20
 *      Author: john
 */

#include <avr/io.h>

#include "extmem.h"

void
extmem_init (void)
{
	/// Enable external memory interface
	XMCRA = _BV (SRE) | _BV (SRW10); // | _BV (SRW10) | _BV (SRW11);

	/// Enable bus-keeper, release Port C
	XMCRB = _BV (XMBK) | _BV (XMM2) | _BV (XMM1) | _BV (XMM0);
}
