#include <pic18f4550.h>
#include "config.h"
#include "lcd.h"
#include <stdio.h>

/*

Designed for a 20MHz cristal

*/

void main() {
    lcd_init(48, USE_PRINTF, LCD_DATA_4BITS);
    printf("Coucou!");
    while (1);
}
