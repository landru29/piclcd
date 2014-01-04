#include <pic18fregs.h>
#include <delay.h>
#include <stdio.h>
#include "lcd.h"

unsigned int    _cristalMhz;
unsigned char   _autoRedirect=0;

/**
 * Initialize the LCD
 *
 * @param cristalMhz Cristal frequency in Mhz
 *
 */
void lcd_init(unsigned int cristalMhz, unsigned char autoRedirect)
{
    _cristalMhz = cristalMhz;
    _autoRedirect = autoRedirect;

    if (autoRedirect) {
        stdout = STREAM_USER;
    }
    /* Preparing outputs */
    LCD_DATA_CNF = 0;
    LCD_RS_CNF = 0;
    LCD_RW_CNF = 0;
    LCD_E_CNF = 0;
    LCD_E=0;

    /* Device initialization */
    delay1ktcy(cristalMhz*4); /* Wait for 16ms after poweron (here 16ms) */
    //LCD_DATA = 0x3C;
    LCD_DATA = 0x30;
    LCD_RS = 0;
    LCD_RW = 0;

    /* Step 1*/
    LCD_E=1;
    delay1ktcy(cristalMhz); /* Wait for 4.1ms */
    delay10tcy(3*cristalMhz);


    LCD_E=0;
    delay100tcy(cristalMhz*8);
    LCD_E=1;
    delay10tcy(3+cristalMhz); /* Wait for 100µs */
    LCD_E=0;
    delay100tcy(cristalMhz*8);
    LCD_E=1;
    delay10tcy(2*cristalMhz); /* Wait for 42µs */
    LCD_E=0;
    delay100tcy(cristalMhz*8);

    /* Display off */
    //lcd_send_cmd(0x08);
    //lcd_send_cmd(DISPLAY_OFF);
    lcd_send_cmd(0x08);

    /* Display clear */
    lcd_clear();

    /* Entry mode set */
    lcd_emode(INC_CURSOR);

    // Select Function Set
	lcd_fmode(IFACE_8BIT | DUAL_LINE);

    /* Display on, cursor on */
    lcd_dmode(DISPLAY_ON | CURSOR_ON | BLINK_ON);

}

/**
 * Display a string
 *
 * @param st String to display
 */
void lcd_print(char* st)
{
    unsigned char i;
    for(i=0;(st[i]!=0) && (i<16);i++) {
        lcd_send_data((unsigned char)st[i]);
    }
}

/**
 * Putchar redirection to make printf works
 *
 * @param c character to display
 */
void putchar(char c) __wparam
{
    if (_autoRedirect) lcd_send_data(c);
}

/**
 * Display one letter
 *
 * @param l character to display
 */
void lcd_pushLetter(unsigned char l)
{
    lcd_send_data(l);
}

/**
 * Specify the line of the cursor
 *
 * @param nb line number (0 is the first line)
 */
void lcd_setLine(unsigned char nb)
{
    unsigned char i;
    lcd_ddram(SECOND_LINE*nb);
    for(i=0;i<16;i++) {
        lcd_send_data(' ');
    }
    lcd_ddram(SECOND_LINE*nb);
}

/**
 * Clear the LCD screen
 */
void lcd_clear()
{
	lcd_send_cmd(0x01);

}

/**
 * Put the cursor at home
 */
void lcd_home()
{
	lcd_send_cmd(0x02);
}

/**
 * Select DDRAM address
 *
 * @param address Adress to select
 */
void lcd_ddram(unsigned char address)
{
	lcd_send_cmd((address & 0x7F) | 0x80);
}


/**
 * set_emode(options) - Set entry mode
 *
 * @param options:
 *     INC_CURSOR - Incremnt cursor after character written
 *     DEC_CURSOR - Decrement cursor after character written
 *     SHIFT_ON   - Switch Cursor shifting on
 */
void lcd_emode(unsigned char options)
{
	lcd_send_cmd((options & 0x03) | 0x04);
}


/**
 * set_dmode(options) - Configure display mode
 *
 * @param options:
 *     DISPLAY_ON  - Turn Display on
 *     DISPLAY_OFF - Turn Display off
 *     CURSOR_ON   - Turn Cursor on
 *     BLINK_ON    - Blink Cursor
 */
void lcd_dmode(unsigned char options)
{
	lcd_send_cmd((options & 0x07) | 0x08);
}


/**
 * set_cmode(options) - Configure cursor mode
 *
 * @param options:
 *     SHIFT_DISP  - Shift Display
 *     SHIFT_RIGHT - Move cursor right
 *     SHIFT_LEFT  - Move cursor left
 */
void lcd_cmode(unsigned char options)
{
	lcd_send_cmd((options & 0x03) | 0x04);
}


/**
 * set_fmode(options) - Configure function set
 *
 * Options:
 *
 *  4BIT_IFACE - 4-bit interface
 *  8BIT_IFACE - 8-bit interface
 *  1_16_DUTY - 1/16 duty
 *  5X10_DOTS - 5x10 dot characters
 */
void lcd_fmode(unsigned char options)
{
	lcd_send_cmd((options & 0x1F) | 0x20);
}

/**
 * Send a command to the LCD screen
 *
 * @param cmd Command to send
 */
void lcd_send_cmd(unsigned char cmd)
{
    while (lcd_busy());

    LCD_RS = 0;
    LCD_RW = 0;
    LCD_DATA_CNF = 0;

    LCD_DATA=cmd;

    LCD_E=1;
    delay100tcy(_cristalMhz*8);
    LCD_E=0;
    LCD_DATA=0;
}

/**
 * Send data to the LCD screen
 *
 * @param dataval Data to send
 */
void lcd_send_data(unsigned char dataval)
{

	while (lcd_busy());

	LCD_RW = 0;
	LCD_RS = 1;
	LCD_DATA = dataval;
	LCD_E = 1;
    delay100tcy(_cristalMhz*8);
	LCD_E = 0;
	LCD_DATA=0;
}

/**
 * Check if the LCD screen is busy
 *
 * @return true if busy
 */
unsigned char lcd_busy()
{
	unsigned char loop=0;
	unsigned char dataval;

	LCD_DATA_CNF = 0xFF;
	LCD_RW = 1;
	LCD_RS = 0;
	LCD_E = 1;

	delay100tcy(_cristalMhz*8);

	dataval = LCD_DATA;
	LCD_E = 0;

    LCD_DATA_CNF = 0x00;

	if (dataval & 0x80)	return 1;
    return 0;
}
