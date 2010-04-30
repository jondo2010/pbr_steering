/*
 * param_screen_engine.h
 *
 *  Created on: 2010-04-25
 *      Author: john
 */

#ifndef PARAM_SCREEN_ENGINE_H_
#define PARAM_SCREEN_ENGINE_H_

uint8_t
param_screen_engine_init
(
	uint8_t line_offset,
	uint8_t mob_index
);

void
param_screen_engine_change_value
(
	uint8_t param,
	int8_t change
);

#endif /* PARAM_SCREEN_ENGINE_H_ */
