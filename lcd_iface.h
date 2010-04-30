/*
 * lcd_iface.h
 *
 *  Created on: 2010-04-29
 *      Author: john
 */

#ifndef LCD_IFACE_H_
#define LCD_IFACE_H_

#define steering_lcd_boost_enable() \
	PORTC |= _BV (PC7)

#define steering_lcd_boost_disable() \
	PORTC &= ~_BV (PC7);

#define LCD_COMM	*((uint8_t *)(0x1101))
#define LCD_DATA	*((uint8_t *)(0x1100))

void
lcd_iface_init (void);

#endif /* LCD_IFACE_H_ */
