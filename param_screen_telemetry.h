/*
 * param_screen_telemetry.h
 *
 *  Created on: 2010-04-29
 *      Author: john
 */

#ifndef PARAM_SCREEN_TELEMETRY_H_
#define PARAM_SCREEN_TELEMETRY_H_

uint8_t
param_screen_telemetry_init
(
	uint8_t line_offset,
	uint8_t mob_index
);

void
param_screen_telemetry_change_value
(
	uint8_t param,
	int8_t change
);

#endif /* PARAM_SCREEN_TELEMETRY_H_ */
