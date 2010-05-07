#ifndef DRAW_H_
#define DRAW_H_

#include <avr/pgmspace.h>

/* Draw a compressed image stored in flash */
void
draw_image
(
	prog_uchar *image,	/* Address of the image */
	uint8_t x,			/* x position */
	uint8_t y,			/* y position */
	uint8_t width,		/* Width of the image */
	uint16_t size		/* Size in bytes */
);

#endif DRAW_H_ /* DRAW_H_ */
