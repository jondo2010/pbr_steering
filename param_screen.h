/*
 * param_screen.h
 *
 *  Created on: 2010-02-20
 *      Author: john
 */

#ifndef PARAM_SCREEN_H_
#define PARAM_SCREEN_H_

typedef module_param_t;

void
param_screen_param_change (int8_t p);

void
param_screen_update_scroll (void);

/**
 * Draw the parameter line text into the text buffer
 */
void
param_screen_draw_param_line
(
	const char	*param_string,
	char		*value,
	uint8_t		line_offset
);

void
param_screen_init (void);

#endif /* PARAM_SCREEN_H_ */
