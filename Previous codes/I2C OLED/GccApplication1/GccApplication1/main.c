#include <avr/io.h>
#include <util/delay.h>
#include <u8g2.h>
#include <u8x8_avr.h>

//#define SSD1306_ADDR  0x78
#define SSD1306_ADDR  0x3C


u8g2_t u8g2;

int main (void)
{
	//u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
	//u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
	u8g2_Setup_sh1106_64x32_1(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
	u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);

	
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_t0_14b_tf);
	u8g2_SetFontRefHeightText(&u8g2);
	u8g2_SetFontPosTop(&u8g2);
	u8g2_DrawStr(&u8g2, 0, 0, "Bin Picking Robot");
	u8g2_SendBuffer(&u8g2);
	while(1){
	}
}


