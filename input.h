/*
 * input.h
 *
 *  Created on: 2010-01-26
 *      Author: John Hughes <jondo2010@gmail.com>
 */

#ifndef INPUT_H_
#define INPUT_H_

void
input_init (void);

void
input_enable (void);

void
input_disable (void);

void
input_set_parameter_changed_callback
(
	void (*callback_func)(int8_t)
);

void
input_set_value_changed_callback
(
	void (*callback_func)(int8_t)
);

void
input_set_vdm_changed_callback
(
	void (*callback_func)(int8_t)
);

#endif /* INPUT_H_ */
