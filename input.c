/*
 * input.c
 *
 * Handles GPIO and interrupts from external input
 * from buttons, paddles, and rotary knobs.
 *
 *  Created on: 2010-01-26
 *      Author: John Hughes <jondo2010@gmail.com>
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "input.h"

/// Callback function pointers
static void (*parameter_changed_callback)	(int8_t)	= 0;
static void (*value_changed_callback)		(int8_t)	= 0;
static void (*vdm_changed_callback)			(int8_t)	= 0;

/**
 * PE4,PE5: Parameter encoder
 * PD1,PD0: Value encoder
 * PD3,PD2: VDM encoder
 *
 * PE6: Upshift
 * PE7: Downshift
 * PF1: Button1
 * PF0: Button2
 * PF3: Button3
 * PF2: Button4
 */

/**
 * Interrupt for value encoder
 */

ISR (INT1_vect)
{
	uint8_t temp = PIND;

	if (value_changed_callback)
	{
		if (temp & _BV (PIND0))
			value_changed_callback (1);
		else
			value_changed_callback (-1);
	}
}

/**
 * Interrupt for VDM encoder
 */

ISR (INT3_vect)
{
	if (vdm_changed_callback)
		if (PIND & _BV (PIND2))
			vdm_changed_callback (1);
		else
			vdm_changed_callback (-1);
}

/**
 * Interrupt for parameter encoder
 */

ISR (INT4_vect)
{
	if (parameter_changed_callback)
		if (PINE & _BV (PINE5))
			parameter_changed_callback (1);
		else
			parameter_changed_callback (-1);
}

ISR (INT6_vect)
{

}

ISR (INT7_vect)
{

}

void
input_init (void)
{
	/// Enable pull-up resistors
	DDRE &= ~_BV (PE4) & ~_BV (PE5) & ~_BV (PE6) & ~_BV (PE7);
	PORTE |= _BV (PE4) | _BV (PE5) | _BV (PE6) | _BV (PE7);

	DDRD &= ~(_BV (PD0) | _BV (PD1) | _BV (PD2) | _BV (PD3));
	PORTD |= _BV (PD0) | _BV (PD1) | _BV (PD2) | _BV (PD3);

	PORTF |= _BV (PF0) | _BV (PF1) | _BV (PF2) | _BV (PF3);

	/// Enable falling edge triggered interrupts on INT3, INT1, INT4, INT6, INT7
	EICRA |= _BV (ISC31) | _BV (ISC11);
	EICRB |= _BV (ISC71) | _BV (ISC61) | _BV (ISC41);
	EIMSK |= _BV (INT1) | _BV (INT3) | _BV (INT4) | _BV (INT6) | _BV (INT7);
}

void
input_enable (void)
{
	EIMSK |= _BV (INT1) | _BV (INT3) | _BV (INT4) | _BV (INT6) | _BV (INT7);
}

void
input_disable (void)
{
	EIMSK &= ~_BV (INT1) & ~_BV (INT3) & ~_BV (INT4) & ~_BV (INT6) & ~_BV (INT7);
}

void
input_set_parameter_changed_callback
(
	void (*callback_func)(int8_t)
)
{
	parameter_changed_callback = callback_func;
}

void
input_set_value_changed_callback
(
	void (*callback_func)(int8_t)
)
{
	value_changed_callback = callback_func;
}

void
input_set_vdm_changed_callback
(
	void (*callback_func)(int8_t)
)
{
	vdm_changed_callback = callback_func;
}
