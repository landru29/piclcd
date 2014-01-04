#ifndef __LCD_H__
#define __LCD_H__


#define CRISTAL_FREQ    48

#define LCD_DATA_CNF    TRISD
#define LCD_DATA        LATD

#define LCD_RS_CNF      TRISCbits.TRISC2
#define LCD_RS          LATCbits.LATC2

#define LCD_RW_CNF      TRISCbits.TRISC1
#define LCD_RW          LATCbits.LATC1

#define LCD_E_CNF       TRISCbits.TRISC0
#define LCD_E           LATCbits.LATC0


//Entry mode variables
#define INC_CURSOR 0x02
#define DEC_CURSOR 0x00
#define SHIFT_ON 0x01

//Display control variables
#define DISPLAY_ON 0x04
#define DISPLAY_OFF 0x00
#define CURSOR_ON 0x02
#define BLINK_ON 0x01

//Cursor move variables
#define SHIFT_DISP 0x08
#define SHIFT_RIGHT 0x04
#define SHIFT_LEFT 0x00

//Function set variables
#define IFACE_4BIT 0x00
#define IFACE_8BIT 0x10
#define DUAL_LINE 0x08
#define DOTS_5X11 0x04

//DDRAM locations
#define FIRST_LINE 0x00
#define SECOND_LINE 0x40

void lcd_init();
void lcd_print(char* st);
void lcd_pushLetter(unsigned char l);
void lcd_setLine(unsigned char nb);
unsigned char lcd_busy();
void lcd_clear();
void lcd_home();
void lcd_send_cmd(unsigned char cmd);
void lcd_send_data(unsigned char dataval);
void lcd_ddram(unsigned char address);
void lcd_dmode(unsigned char options);
void lcd_cmode(unsigned char options);
void lcd_emode(unsigned char options);
void lcd_fmode(unsigned char options);

#endif
