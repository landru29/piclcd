#include <pic18fregs.h>
#include <delay.h>
#include <stdio.h>
#include "lcd.h"

unsigned int    _cristalMhz;
unsigned char   _autoRedirect=0;
unsigned char   _data_format=1;

#define NOP __asm__ ("nop")
#define LCD_STROBE LCD_E=0;delay100tcy(4*_cristalMhz);LCD_E=1;NOP;LCD_E=0

void lcd_write(unsigned char dataval, unsigned char isData);

/**
 * Initialize the LCD
 *
 * @param cristalMhz Cristal frequency in Mhz
 *
 */
void lcd_init(unsigned int cristalMhz, unsigned char autoRedirect, unsigned char data_format)
{
    unsigned char initByte;
    _cristalMhz = cristalMhz;
    _autoRedirect = autoRedirect;
    _data_format = data_format;

    /* Preparing outputs */
    if (_data_format == LCD_DATA_8BITS) {
        LCD_DATA_CNF = 0x00;
    } else {
        LCD_DATA_CNF &= 0xF0;
    }
    LCD_RS_CNF = 0;
    LCD_RW_CNF = 0;
    LCD_E_CNF = 0;

    /* setting controls to low level */
    LCD_E=0;
    LCD_RS = 0;
    LCD_RW = 0;

    if (_data_format == LCD_DATA_8BITS) {
        initByte = 0x30;
    } else {
        initByte = 0x03;
    }
    TRISB = 0;
    LATB = initByte;

    /* Device initialization */
    delay1ktcy(cristalMhz*4); /* Wait for 16ms after power on */

    /* Step 1*/
    //LCD_STROBE;
    LCD_DATA = initByte;
    delay1ktcy(cristalMhz); // Wait for 4.1ms
    LCD_STROBE;

    delay1ktcy(cristalMhz); // Wait for 4.1ms
    LCD_STROBE;

    delay10tcy(4*cristalMhz); // Wait for 150µs
    LCD_STROBE;

    if (_data_format == LCD_DATA_4BITS) {
        LCD_DATA = FUNCTION_SET >> 4;
        LCD_STROBE;
    }

    /* Display mode */
    if (_data_format == LCD_DATA_8BITS) {
        lcd_fmode(IFACE_8BIT | DUAL_LINE | DOTS_5X7);
    } else {
        lcd_fmode(IFACE_4BIT | DUAL_LINE | DOTS_5X7);
    }

    /* Display off */
    lcd_dmode(DISPLAY_OFF);

    /* Display clear */
    lcd_clear();

    /* LCD home */
    lcd_home();

    /* Entry mode set */
    lcd_emode(INC_CURSOR);

    /* Display on, cursor on */
    lcd_dmode(DISPLAY_ON | CURSOR_ON | BLINK_ON);

    if (_autoRedirect == USE_PRINTF) {
        stdout = STREAM_USER;
    }
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
        lcd_pushLetter((unsigned char)st[i]);
    }
}

/**
 * Putchar redirection to make printf works
 *
 * @param c character to display
 */
void putchar(char c) __wparam
{
    if (_autoRedirect == USE_PRINTF) lcd_send_data(c);
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
	lcd_send_cmd(LCD_CLEAR);

}

/**
 * Put the cursor at home
 */
void lcd_home()
{
	lcd_send_cmd(LCD_HOME);
}

/**
 * Select DDRAM address
 *
 * @param address Adress to select
 */
void lcd_ddram(unsigned char address)
{
	lcd_send_cmd((address & 0x7F) | DDRAM_SET);
}


/**
 * set_emode(options) - Set entry mode
 *
 * @param options:
 *     INC_CURSOR - Incremnt cursor after character written
 *     DEC_CURSOR - Decrement cursor after character written
 *     SHIFT_ON   - Switch Cursor shifting on
 *     SHIFT_OFF  - Switch Cursor shifting off
 */
void lcd_emode(unsigned char options)
{
	lcd_send_cmd((options & 0x03) | ENTRY_VARIABLE_SET);
}


/**
 * set_dmode(options) - Configure display mode
 *
 * @param options:
 *     DISPLAY_ON  - Turn Display on
 *     DISPLAY_OFF - Turn Display off
 *     CURSOR_ON   - Turn Cursor on
 *     CURSOR_OFF  - Turn Cursor off
 *     BLINK_ON    - Blink Cursor
 *     BLINK_OFF   - Static Cursor
 */
void lcd_dmode(unsigned char options)
{
	lcd_send_cmd((options & 0x07) | DISPLAY_VARIABLE_SET);
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
	lcd_send_cmd((options & 0x0C) | CURSOR_VARIABLE_SET);
}


/**
 * set_fmode(options) - Configure function set
 *
 * Options:
 *
 *  4BIT_IFACE - 4-bit interface
 *  8BIT_IFACE - 8-bit interface
 *  1_16_DUTY  - 1/16 duty
 *  DOTS_5X10  - 5x10 dot characters
 *  DOTS_5X7   - 5x7 dot characters
 */
void lcd_fmode(unsigned char options)
{
	lcd_send_cmd((options & 0x1F) | FUNCTION_SET);
}

/**
 * Send a command to the LCD screen
 *
 * @param cmd Command to send
 */
void lcd_send_cmd(unsigned char cmd)
{
    lcd_write(cmd, 0);
}

/**
 * Send data to the LCD screen
 *
 * @param dataval Data to send
 */
void lcd_send_data(unsigned char dataval)
{
    lcd_write(dataval, 1);
}

/**
 * Expose data on the bus
 *
 * @param dataval data to expose on the bus
 * @param isData  boolean; true if data, false if command
 */
void lcd_write(unsigned char dataval, unsigned char isData)
{

	LCD_RW = 0;
	LCD_RS = isData;
    if (_data_format == LCD_DATA_8BITS) {
        LCD_DATA = dataval;
        LCD_STROBE;
    } else {
        LCD_DATA = 0x0F & (dataval >> 4);
        LCD_STROBE;
        LCD_DATA = 0x0F & dataval;
        LCD_STROBE;
    }
}

/**
 * Wait the LCD to be available
 *
 */
/*void lcd_busy()
{
	unsigned char dataval;

    if (_data_format == LCD_DATA_8BITS) {
        LCD_DATA_CNF = 0xFF;
    } else {
        LCD_DATA_CNF |= 0x0F;
    }

	LCD_RW = 1;
	LCD_RS = 0;

	do {
        LCD_E = 1;
        delay100tcy(_cristalMhz*8);
        dataval = LCD_DATA;
        LCD_E = 0;
        if (_data_format == LCD_DATA_4BITS) {
            LCD_STROBE;
            dataval = (dataval & 0x0F) >> 3;
        } else {
            dataval >>= 7;
        }
    } while (dataval);

    if (_data_format == LCD_DATA_8BITS) {
        LCD_DATA_CNF = 0x00;
    } else {
        LCD_DATA_CNF &= 0xF0;
    }
    LCD_RW = 0;
}*/
