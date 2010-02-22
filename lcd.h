/*
 * lcd.h
 * Library for communicating with the Epson SED1335 LCD Controller
 *
 * Created on: 2009-11-08
 * John Hughes <jondo2010@gmail.com>
 */

#ifndef LCD_H_
#define LCD_H_

#define LCD_COMM	*((uint8_t *)(0x1101))
#define LCD_DATA	*((uint8_t *)(0x1100))

#define	PIXELS_W		(320u)
#define PIXELS_H		(240u)

#define TXT1_ADDRESS 	(0x0000)
#define	TXT2_ADDRESS	(TXT1_ADDRESS + 0x50)
#define GFX_WIDTH		(PIXELS_W/8u)							/* 40 */
#define GFX1_ADDRESS	(GFX_WIDTH * (PIXELS_H/8u))				/* 0x04b0 */
#define GFX2_ADDRESS	(GFX1_ADDRESS + GFX_WIDTH * PIXELS_H)	/* 0x2a30 */

/**
 * LCD Command Macros
 */
#define lcd_write_data()				(LCD_COMM = MWRITE)

/**
 * LCD System command codes
 */
#define SYSTEM_SET		0x40
#define SCROLL			0x44
#define HDOT_SCR		0x5A
#define OVLAY			0x5B
#define DISP_OFF		0x58
#define DISP_ON			0x59
#define CSRFORM			0x5D
#define CSRW			0x46
#define MWRITE			0x42
#define CSRDIR			0x4C
#define CGRAM_ADR		0x5C

/// Defines for System Set command
#define EXTERNAL_CG_ROM 0x01 /* Otherwise internal */
#define D6_CORRECTION 0x02
#define PIXEL_CHARS8 0x10 /* Character height */
#define PIXEL_CHARS16 0x14
#define DUAL_PANEL 0x08 /* Otherwise Single */
#define INVERSE_COMPENSATION 0x20

#define WIDE_CHARS8 0x07 /* 8 pixel wide chars */
#define TWO_FRAME_AC_DRIVE 0x80

#define HIGH_CHARS8 0x07 /* 8 pixel high chars */

/// Defines for overlay settings
#define COMPOSE_OR			0x00
#define COMPOSE_XOR			0x01
#define COMPOSE_AND			0x02
#define COMPOSE_PRI_OR		0x03

#define LAYER1_GRAPHICS		0x04
#define LAYER3_GRAPHICS		0x08

#define THREE_LAYER_COM		0x10

/// Defines for lcd state
#define CURSOR_OFF			0x00
#define CURSOR_STEADY		0x01
#define CURSOR_BLINK_2HZ	0x02
#define CURSOR_BLINK_1HZ	0x03

#define SAD_OFF				0x00
#define SAD_STEADY			0x01
#define SAD_BLINK_2HZ		0x02
#define SAD_BLINK_16HZ		0x03

/// Defines for cursor form
#define UNDERSCORE_CURSOR	0x00
#define BLOCK_CURSOR		0x80

/// Defines for cursor direction
#define CURSOR_RIGHT		0x00
#define CURSOR_LEFT			0x01
#define CURSOR_UP			0x02
#define CURSOR_DOWN			0x03

/**
 * Set the scroll position for the first 2 layers
 */
void 
lcd_set_scroll
(
  uint16_t  sad1_address,		/* Start address of SAD1 */
  uint8_t   sad1_height,		/* Height in pixels of SAD1 */
  uint16_t  sad2_address,		/* Start address of SAD2 */
  uint8_t   sad2_height,		/* Height in pixels of SAD2 */
  uint16_t	sad3_address		/* Start address of SAD3 */
);

/**
 * Set horizontal scroll position in pixels
 */
void
lcd_set_hscroll
(
  uint8_t position
);

/**
 * Set overlay parameters
 */
void
lcd_set_overlay
(
  uint8_t overlay
);

/**
 * Set the cursor address
 */
void
lcd_set_cursor_pointer
(
  uint16_t addr
);

/**
 * Draw a string starting at this address
 */
void
lcd_draw_string
(
  uint8_t *string,
  uint16_t addr,
  uint8_t length
);

/**
 * Clear the LCD by filling the graphics layer with 0s and the text layer with space chars.
 */
void
lcd_clear_screen ();

/**
 * Hard reset of LCD
 */
void
lcd_reset ();

/**
 * Call system set command
 */
void
lcd_system_set
(
  uint8_t cr,
  uint8_t tcr
);

/**
 * Set lcd on/off status, cursor and screen blocks states
 */
void
lcd_set_display_state
(
  uint8_t on_off,
  uint8_t cursor_state,
  uint8_t sad1_state,
  uint8_t sad2_state,
  uint8_t sad3_state
);

/**
 * Set the LCD cursor form.
 * Width in pixels from 1 to 16
 * Height in pixels from 2 to 16.
 * cursor_type is either UNDERSCORE_CURSOR or BLOCK_CURSOR
 */
void
lcd_set_cursor_form
(
	uint8_t cursor_width,
	uint8_t cursor_height,
	uint8_t cursor_type
);

/**
 * Set the LCD cursor increment direction
 */
void
lcd_set_cursor_direction
(
	uint8_t cursor_direction
);

/**
 * Initialize the LCD
 */
void
lcd_init ();

#endif /* LCD_H_ */
