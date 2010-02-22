/*
 * picture.h
 *
 *  Created on: 2009-11-06
 *      Author: john
 */

#ifndef PICTURE_H_
#define PICTURE_H_

#include <avr/pgmspace.h>

const prog_uchar signal_top[8][2] PROGMEM =
{
	{ 0x7f, 0xfe },
	{ 0xff, 0xff },
	{ 0x71, 0x8e },
	{ 0x39, 0x9c },
	{ 0x1d, 0xb8 },
	{ 0x0f, 0xf0 },
	{ 0x07, 0xe0 },
	{ 0x03, 0xc0 }
};

#endif /* PICTURE_H_ */
