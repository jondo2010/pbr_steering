/*
 * lcd.c
 * Library for the Epson SED1335 LCD Controller
 *
 * Created on: 2009-11-08
 * John Hughes <jondo2010@gmail.com>
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

#include "lcd.h"
#include "picture.h"

/*
 * Issue a scroll command to the LCD. In 2-layer mode, SAD1 and
 * SAD3 are both text, and SAD3 starts right after SAD2.
 */

void
lcd_set_scroll
(
  uint16_t  sad1_address,		/* Start address of SAD1 */
  uint8_t   sad1_height,		/* Height in pixels of SAD1 */
  uint16_t  sad2_address,		/* Start address of SAD2 */
  uint8_t   sad2_height,		/* Height in pixels of SAD2 */
  uint16_t	sad3_address		/* Start address of SAD3 */
)
{
	LCD_COMM = SCROLL;
	LCD_DATA = (uint8_t) sad1_address; // P1: (SAD 1L)
	LCD_DATA = (sad1_address >> 8); // P2: (SAD 1H)
	LCD_DATA = (sad1_height); // P3: (SL 1)

	LCD_DATA = ((uint8_t) sad2_address); // P4: (SAD 2L)
	LCD_DATA = (sad2_address >> 8); // P5: (SAD 2H)
	LCD_DATA = (sad2_height); // P6: (SL 2)

	LCD_DATA = ((uint8_t) sad3_address); // P7: (SAD 3L)
	LCD_DATA = (sad3_address >> 8); // P8: (SAD 3H)

	//LCD_DATA = (0x00); // P9: (SAD 4L)
	//LCD_DATA = (0x00); // P10: (SAD 4H)
}

void
lcd_set_hscroll
(
  uint8_t position
)
{
	LCD_COMM = (HDOT_SCR);
	LCD_DATA = (position);
}

void
lcd_set_overlay
(
  uint8_t overlay
)
{
	LCD_COMM = (OVLAY); // Set display overlay format
	LCD_DATA = (overlay);
}

void 
lcd_set_cursor_pointer 
(
  uint16_t addr
)
{
	LCD_COMM = (CSRW);
	LCD_DATA = ((uint8_t)addr);
	LCD_DATA = (addr >> 8);
}

void
lcd_draw_string
(
  uint8_t *string,
  uint16_t addr,
  uint8_t length
)
{
	uint8_t n;

	lcd_set_cursor_pointer (addr);
	LCD_COMM = (MWRITE);
	for (n=0; n<length; n++)
	{
		LCD_DATA = (*string++);
	}
}

void
lcd_draw_bitmap
(
	prog_uchar *bitmap
)
{
	uint16_t n;
	lcd_set_cursor_pointer (GFX1_ADDRESS);
	LCD_COMM = MWRITE;

	for(n=0;n<9600;n++)
	{
		LCD_DATA = pgm_read_byte(&(bitmap[n]));
	}
}

/**
 * Draws a 24x16 signal strength graphic
 * 0 to 4 bars
 */

void
lcd_draw_signal_strength
(
	uint8_t bars,
	uint8_t x,
	uint8_t y
)
{
	uint8_t line, tmp;
	uint16_t gfx_start = GFX1_ADDRESS + x * y;

	//lcd_set_cursor_pointer (GFX_ADDRESS + x * y);

	for (line=0; line<16; line++)
	{
		lcd_set_cursor_pointer (gfx_start + GFX_WIDTH * line);
		LCD_COMM = MWRITE;

		/// First 2 bytes
		if (line < 8) /// Top half
		{
			LCD_DATA = pgm_read_byte(&(signal_top[line][0]));
			LCD_DATA = pgm_read_byte(&(signal_top[line][1]));
		}
		else /// Bottom half
		{
			LCD_DATA = 0x01;

			tmp = 0x80;

			if (bars>1)
				tmp |= 0x03;

			if (bars>0 && line>11)
				tmp |= 0x30;

			LCD_DATA = tmp;
		}

		tmp = 0;

		/// Third byte
		if (bars>3)
			tmp |= 0x03;

		if (bars>2 && line>4)
			tmp |= 0x30;

		LCD_DATA = tmp;
	}
}

void
lcd_set_cursor_xy
(
	uint8_t x,
	uint8_t y
)
{
	lcd_set_cursor_pointer (GFX1_ADDRESS + y * 40 + x);
}

void
lcd_draw_top_underline ()
{
	uint8_t n;

	lcd_set_cursor_xy (0,18);
	lcd_set_cursor_direction (CURSOR_RIGHT);
	lcd_write_data ();

	for (n=0; n<40; n++)
		LCD_DATA = 0xff;
}

void
lcd_draw_clock
(
	uint8_t hh,
	uint8_t mm
)
{
	uint8_t s[5];

	lcd_set_cursor_xy (15,0);
	lcd_write_data ();
	sprintf(s, "%u:%u", hh, mm);
	font_draw_string (s,TXT1_ADDRESS+15,5);
}

void
lcd_clear_screen ()
{
	uint16_t n;

	lcd_set_cursor_pointer (TXT1_ADDRESS);
	LCD_COMM = (MWRITE);
	for (n=0;n<1200;n++)
		LCD_DATA = (0x00);

	lcd_set_cursor_pointer (GFX1_ADDRESS);
	LCD_COMM = (MWRITE);
	for (n=0;n<9600;n++)
		LCD_DATA = (0x00);
}

void
lcd_reset ()
{
	PORTD &= ~_BV (PD7);
	_delay_ms (10);
	PORTD |= _BV (PD7);
}

void
lcd_system_set
(
  uint8_t cr,
  uint8_t tcr
)
{
	LCD_COMM = (SYSTEM_SET);
	_delay_ms (5);
	//LCD_DATA =(PIXEL_CHARS16 | INVERSE_COMPENSATION); // P1: (0 0 IV 1 W/S M2 M1 M0)
	LCD_DATA = (PIXEL_CHARS16 | EXTERNAL_CG_ROM);
	LCD_DATA = (TWO_FRAME_AC_DRIVE | WIDE_CHARS8); // P2: (WF 0 0 0 0 FX1 FX2 FX3)
	//LCD_DATA = (HIGH_CHARS8); // P3: (0 0 0 0 FY1 FY2 FY3 FY4)
	LCD_DATA = (0x0f); /// 16-pixel high chars
	LCD_DATA = (cr); // P4: (C/R 0..7) 39d characters per display line
	LCD_DATA = (tcr); // P5: (TC/R 0..7) 57d total characters
	LCD_DATA = (0xEF); // P6: (L/F 0..7) 239 Lines per frame
	LCD_DATA = (0x28); // P7: (APL 0..7) 40 addresses per line virtual
	LCD_DATA = (0x00); // P8: (APH 0..7)
}

void
lcd_set_display_state
(
  uint8_t on_off,
  uint8_t cursor_state,
  uint8_t sad1_state,
  uint8_t sad2_state,
  uint8_t sad3_state
)
{
	if (on_off)
		LCD_COMM = (DISP_ON);
	else
		LCD_COMM = (DISP_OFF);

	/* [FP5 FP4] [FP3 FP2] [FP1 FP0] FC1 FC0
	 * FC0,1: the cursor state
	 * FP0,1: sad1 state
	 * FP2,3: sad2 state
	 * FP4,5: sad3 state
	 */
	_delay_ms(5);

	LCD_DATA = (
    cursor_state |
    (sad1_state << 2) |
    (sad2_state << 4) |
    (sad3_state << 6)
  );
}

void
lcd_set_cursor_form
(
  uint8_t cursor_width,
  uint8_t cursor_height,
  uint8_t cursor_type
)
{
	LCD_COMM = (CSRFORM);
	LCD_DATA = ((cursor_width - 1) & 0x0f);
	LCD_DATA = (cursor_type | ((cursor_height -1) & 0x0f));
}

void
lcd_set_cursor_direction
(
  uint8_t cursor_direction
)
{
	LCD_COMM = (CSRDIR | (cursor_direction & 0x03));
}

void
lcd_init ()
{
	/// SAD1: 2 rows 16x16 chars
	/// SAD2: full-screen gfx
	/// SAD3: 13 rows 16x16 chars

	lcd_set_scroll (TXT1_ADDRESS, 0x20, GFX1_ADDRESS, 0xef, TXT2_ADDRESS);
	lcd_set_hscroll (0x00); // Set horizontal scroll position
	lcd_set_overlay (COMPOSE_XOR);

	// Turn off display, set blinking cursor, all display blocks steady on
	lcd_set_display_state (0, CURSOR_BLINK_2HZ, SAD_STEADY, SAD_STEADY, SAD_STEADY);
	lcd_set_cursor_form (5, 7, BLOCK_CURSOR); // Set a 4x6 block cursor
	lcd_set_cursor_direction (CURSOR_RIGHT);

	// Turn display back on, cursor blinking, display blocks 1 and 2 steady, 3 off.
	lcd_set_display_state (1, CURSOR_BLINK_2HZ, SAD_STEADY, SAD_STEADY, SAD_STEADY);
}
