#include <avr/io.h>
#include <avr/pgmspace.h>

#include "draw.h"
#include "lcd_iface.h"
#include "lcd.h"

typedef enum Header
{
	NOT_HEADER = 0,
	PACKED_DATA = 1,
	LITERAL_DATA = 2
}
Header;

void
draw_image(prog_uchar *image, uint8_t x, uint8_t y, uint8_t width, uint16_t size)
{
	uint16_t i = 0;
	uint8_t img_x = 0, img_y = 0;
	uint8_t count = 0;
	uint8_t curr_byte = 0;
	Header header = 0;

	lcd_set_cursor_xy(x, y);
	lcd_write_data();
	
	for (i = 0; i < size; ++i)
	{
		curr_byte = pgm_read_byte(&image[i]);
		if (header == NOT_HEADER)
		{
			if (curr_byte > 127)
			{
				count = curr_byte - 127;
				header = PACKED_DATA;
			}
			else
			{
				count = curr_byte + 1;
				header = LITERAL_DATA;
			}
		}
		else if (header == PACKED_DATA)
		{
			while (count > 0)
			{
				//lcd_set_data(curr_byte);
				LCD_DATA = curr_byte;
				img_x++;
				if (img_x >= width)
				{
					img_x = 0;
					img_y++;
					lcd_set_cursor_xy(x, y + img_y);
					lcd_write_data();
				}
				count--;
			}
			header = NOT_HEADER;
		}
		else if (header == LITERAL_DATA)
		{
			//lcd_set_data(curr_byte);
			LCD_DATA = curr_byte;
			img_x++;
			if (img_x >= width)
			{
				img_x = 0;
				img_y++;
				lcd_set_cursor_xy(x, y + img_y);
				lcd_write_data();
			}
			count--;
			if (count == 0)
				header = NOT_HEADER;
		}
	}
}
