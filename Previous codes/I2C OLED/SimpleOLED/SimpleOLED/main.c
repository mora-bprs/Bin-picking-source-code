//****main.c****//
#include "lcd.h"

int main(void){
	
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_set_contrast(0x00);
	lcd_gotoxy(4,1);
	lcd_puts("Normal Size");
	lcd_charMode(DOUBLESIZE);
	lcd_gotoxy(0,4);
	lcd_puts("  Double  \r\n   Size");
	lcd_charMode(NORMALSIZE);
	
	#ifdef GRAPHICMODE
	lcd_display();
	#endif
	for(;;){
		//main loop
	}
	return 0;
}