/*
 * input.h
 *
 *  Created on: 2010-01-26
 *      Author: John Hughes <jondo2010@gmail.com>
 */

#ifndef INPUT_H_
#define INPUT_H_

/**
 * Initialize the steering module input:
 *   - External interrupts for encoder knobs and push buttons
 *   - External memory interface to the LCD
 */
void
input_init (void);

/**
 * Enable input by turning on the external interrupt flags
 */
void
input_enable (void);

/**
 * Disable input by turning off the external interrupt flags
 */
void
input_disable (void);

/**
 * Set the callback function for when the parameter knob is changed.
 */
void
input_set_parameter_changed_callback
(
	void (*callback_func)(int8_t)
);

/**
 * Set the callback function for when the value knob is changed.
 */
void
input_set_value_changed_callback
(
	void (*callback_func)(int8_t)
);

/**
 * Set the callback function for when the VDM knob is changed.
 */
void
input_set_vdm_changed_callback
(
	void (*callback_func)(int8_t)
);

#endif /* INPUT_H_ */
