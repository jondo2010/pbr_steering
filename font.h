/*
 * font.h
 *
 *  Created on: 2010-01-29
 *      Author: john
 */

#ifndef FONT_H_
#define FONT_H_

#define CGRAM_START 	0xf000
#define CHAR1			0x00
#define SAG				(CGRAM_START - (CHAR1 << 4))
#define ASCII_OFFSETA	0xe0
#define ASCII_OFFSETB	0x1b

void
font_init(void);

void
font_draw_string
(
	uint8_t *string,
	uint16_t addr,
	uint8_t length
);

#endif /* FONT_H_ */
