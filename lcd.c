#include <pic18fregs.h>
#include <delay.h>
#include "lcd.h"

#define LCDPAUSE delay100tcy(CRISTAL_FREQ*8);

void lcd_init() {
    /* Preparing outputs */
    LCD_DATA_CNF = 0;
    LCD_RS_CNF = 0;
    LCD_RW_CNF = 0;
    LCD_E_CNF = 0;
    LCD_E=0;

    /* Device initialization */
    delay1ktcy(CRISTAL_FREQ*4); /* Wait for 16ms after poweron (here 16ms) */
    LCD_DATA = 0x3C;
    LCD_RS = 0;
    LCD_RW = 0;

    /* Step 1*/
    LCD_E=1;
    delay1ktcy(CRISTAL_FREQ); /* Wait for 4.1ms */
    delay10tcy(3*CRISTAL_FREQ);
    LCD_E=0;
    LCDPAUSE;
    LCD_E=1;
    delay10tcy(3+CRISTAL_FREQ); /* Wait for 100µs */
    LCD_E=0;
    LCDPAUSE;
    LCD_E=1;
    delay10tcy(2*CRISTAL_FREQ); /* Wait for 42µs */
    LCD_E=0;
    LCDPAUSE;

    /* Display off */
    //lcd_send_cmd(0x08);

    /* Display clear */
    lcd_clear();

    /* Entry mode set */
    lcd_emode(INC_CURSOR);

    // Select Function Set
	lcd_fmode(IFACE_8BIT | DUAL_LINE);

    /* Display on, cursor on */
    lcd_dmode(DISPLAY_ON | CURSOR_ON | BLINK_ON);

}

void lcd_print(char* st) {
    unsigned char i;
    for(i=0;(st[i]!=0) && (i<16);i++)
        lcd_send_data((unsigned char)st[i]);
}

void lcd_pushLetter(unsigned char l) {
    lcd_send_data(l);
}

void lcd_setLine(unsigned char nb) {
    unsigned char i;
    lcd_ddram(SECOND_LINE*nb);
    for(i=0;i<16;i++) lcd_send_data(' ');
    lcd_ddram(SECOND_LINE*nb);
}

void lcd_clear()
{
	lcd_send_cmd(0x01);

}

void lcd_home()
{
	lcd_send_cmd(0x02);
}

void lcd_ddram(unsigned char address)
{
	lcd_send_cmd((address & 0x7F) | 0x80);
}


/**
 * set_emode(options) - Set entry mode
 *
 * Options:
 *
 *  INC_CURSOR - Incremnt cursor after character written
 *  DEC_CURSOR - Decrement cursor after character written
 *  SHIFT_ON - Switch Cursor shifting on
 */
void lcd_emode(unsigned char options)
{
	lcd_send_cmd((options & 0x03) | 0x04);
}


/**
 * set_dmode(options) - Configure display mode
 *
 * Options:
 *
 *  DISPLAY_ON - Turn Display on
 *  DISPLAY_OFF - Turn Display off
 *  CURSOR_ON  - Turn Cursor on
 *  BLINK_ON - Blink Cursor
 */
void lcd_dmode(unsigned char options)
{
	lcd_send_cmd((options & 0x07) | 0x08);
}


/**
 * set_cmode(options) - Configure cursor mode
 *
 * Options:
 *
 *  SHIFT_DISP - Shift Display
 *  SHIFT_RIGHT - Move cursor right
 *  SHIFT_LEFT - Move cursor left
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

void lcd_send_cmd(unsigned char cmd) {

    while (lcd_busy());

    LCD_RS = 0;
    LCD_RW = 0;
    LCD_DATA_CNF = 0;

    LCD_DATA=cmd;

    LCD_E=1;
    LCDPAUSE;
    LCD_E=0;
    LCD_DATA=0;
}

void lcd_send_data(unsigned char dataval)
{

	while (lcd_busy());

	LCD_RW = 0;
	LCD_RS = 1;
	LCD_DATA = dataval;
	LCD_E = 1;
    LCDPAUSE;
	LCD_E = 0;
	LCD_DATA=0;
}


unsigned char lcd_busy() {
	unsigned char loop=0;
	unsigned char dataval;

	LCD_DATA_CNF = 0xFF;
	LCD_RW = 1;
	LCD_RS = 0;
	LCD_E = 1;

	LCDPAUSE;

	dataval = LCD_DATA;
	LCD_E = 0;

    LCD_DATA_CNF = 0x00;

	if (dataval & 0x80)	return 1;
    return 0;
}
