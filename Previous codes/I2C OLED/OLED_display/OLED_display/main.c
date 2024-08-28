/*
 * OLED_display.c
 *
 * Created: 8/19/2024 12:38:35 AM
 * Author : Ama
 */ 

#include <avr/io.h>
#include "i2c.h"
#include "SSD1306.h"

int main(void) {
	
	OLED_Init();  //initialize the OLED
	OLED_Clear(); //clear the display (for good measure)
	
	while (1) {
		// this is a mock up.  Other than the OLED nothing                     //works.
		
		OLED_SetCursor(0, 0);      //set the cursor                                                    //position to (0, 0)
		OLED_Printf("AudioDIWHY Comparator"); //Print out                                                         //some text
		OLED_HorizontalGraph(1,50);
		OLED_SetCursor(3,103);
		OLED_Printf("IN A");
		OLED_HorizontalGraph(3,30);
		OLED_SetCursor(7,103);
		OLED_Printf("IN B");
		
		OLED_SetCursor(5,0);
		OLED_Printf("Mode A < x < B");
		OLED_SetCursor(5,103);
		OLED_Printf("2.2V");
		
	}
	
	return 0; // never reached
}
