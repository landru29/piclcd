#include <pic18f4550.h>
#include "config.h"
#include "lcd.h"
#include <stdio.h>

/*

Designed for a 20MHz cristal

*/

void main() {
    lcd_init(48, 1);
    //lcd_print("Hello world");
    printf("Coucou!");
    while (1);
}
