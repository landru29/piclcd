#include <pic18f4550.h>
#include "config.h"
#include "lcd.h"
#include <stdio.h>

/*

Designed for a 20MHz cristal

*/

void main() {
    lcd_init();
    lcd_print("Hello world");
    while (1);
}
