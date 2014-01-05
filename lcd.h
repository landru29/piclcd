#ifndef __LCD_H__
#define __LCD_H__

#ifndef LCD_DATA
    #define LCD_DATA_CNF    TRISD
    #define LCD_DATA        LATD
#endif // LCD_DATA

#ifndef LCD_RS
    #define LCD_RS_CNF      TRISCbits.TRISC2
    #define LCD_RS          LATCbits.LATC2
#endif // LCD_RS

#ifndef LCD_RW
    #define LCD_RW_CNF      TRISCbits.TRISC1
    #define LCD_RW          LATCbits.LATC1
#endif // LCD_RW

#ifndef LCD_E
    #define LCD_E_CNF       TRISCbits.TRISC0
    #define LCD_E           LATCbits.LATC0
#endif // LCD_E

#define LCD_DATA_4BITS          0x00
#define LCD_DATA_8BITS          0x01
#define USE_PRINTF              0x01
#define MANUAL_USE              0x00

//Entry mode variables
#define ENTRY_VARIABLE_SET      0x04
#define INC_CURSOR              0x02
#define DEC_CURSOR              0x00
#define SHIFT_ON                0x01
#define SHIFT_OFF               0x00

//Display control variables
#define DISPLAY_VARIABLE_SET    0x08
#define DISPLAY_ON              0x04
#define DISPLAY_OFF             0x00
#define CURSOR_ON               0x02
#define CURSOR_OFF              0x00
#define BLINK_ON                0x01
#define BLINK_OFF               0x00

//Cursor move variables
#define CURSOR_VARIABLE_SET     0x10
#define SHIFT_DISP              0x08
#define SHIFT_RIGHT             0x04
#define SHIFT_LEFT              0x00

//Function set variables
#define FUNCTION_SET            0x20
#define IFACE_4BIT              0x00
#define IFACE_8BIT              0x10
#define DUAL_LINE               0x08
#define DOTS_5X7                0x00
#define DOTS_5X10               0x04

//DDRAM locations
#define DDRAM_SET               0x80
#define FIRST_LINE              0x00
#define SECOND_LINE             0x40

//Standard command
#define LCD_HOME                0x02
#define LCD_CLEAR               0x01



void lcd_init(unsigned int cristalMhz, unsigned char autoRedirect, unsigned char is_8bit_data);
void lcd_print(char* st);
void lcd_pushLetter(unsigned char l);
void lcd_setLine(unsigned char nb);
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
