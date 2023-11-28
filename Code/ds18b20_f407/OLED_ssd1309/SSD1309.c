/*
 * SSD1309.c
 *
 *  Created on: Jun 9, 2023
 *      Author: Masoud Babaabasi
 */
#include "SSD1309.h"

static uint8_t SSD1309_Buffer[SSD1309_WIDTH * SSD1309_HEIGHT / 8];
static SSD1309_t SSD1309;
/*
 * custom delay
 */
static void _delay(uint8_t input_d){
	while(input_d--);
}
extern SPI_HandleTypeDef hspi3;
/*
 * software SPI write for multiple bytes
 */
void SOFT_SPI_WriteData(uint8_t *Data , uint32_t number)
{
//	char i=0;
//	const int DELAY = 10;
//	for(int count =0 ; count < number ; count++){
//		for(i = 8 ; i > 0 ; i--)
//		{
//		  //_delay(DELAY);
//		  OLED_SCK_CLR;
//		  if( *Data & 0x80 ) OLED_MOSI_SET;
//		  else OLED_MOSI_CLR;
//		  //_delay(DELAY);
//		  OLED_SCK_SET;
//		  *Data <<= 1;
//		}
//		Data++;
//	}
	HAL_SPI_Transmit(&hspi3, Data, number, 500);
}
/*
 *  oled write command
 */

void OLED_WriteIndex(uint8_t Index){
   OLED_CS_CLR;
   OLED_DC_CLR;
   SOFT_SPI_WriteData(&Index , 1);
   OLED_CS_SET;
}
/*
 * oled write daa
 */

void OLED_WriteData(uint8_t Data){
   OLED_CS_CLR;
   OLED_DC_SET;
   SOFT_SPI_WriteData(&Data , 1);
   OLED_CS_SET;
}
/*
 *
 */
void OLED_WriteData_Multiple(uint8_t *Data , uint32_t num){
   OLED_CS_CLR;
   OLED_DC_SET;
   SOFT_SPI_WriteData(Data , num);
   OLED_CS_SET;
}
/*
 *
 */
void OLED_reset(void){
	OLED_RST_CLR;
	HAL_Delay(50);
	OLED_RST_SET;
	HAL_Delay(100);
}
void OLED_init(void){
	OLED_reset();
	OLED_WriteIndex(0xAE);
	OLED_WriteIndex(0x20);
	OLED_WriteIndex(0x10);
	OLED_WriteIndex(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
#ifdef SSD1306_MIRROR_VERT
	OLED_WriteIndex(0xC0); // Mirror vertically
#else
	OLED_WriteIndex(0xC8); //Set COM Output Scan Direction
#endif

	OLED_WriteIndex(0x00); //---set low column address
	OLED_WriteIndex(0x10); //---set high column address

	OLED_WriteIndex(0x40); //--set start line address - CHECK

	OLED_WriteIndex(0x81); //--set contrast control register - CHECK
	OLED_WriteIndex(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
	OLED_WriteIndex(0xA0); // Mirror horizontally
#else
	OLED_WriteIndex(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef SSD1306_INVERSE_COLOR
	OLED_WriteIndex(0xA7); //--set inverse color
#else
	OLED_WriteIndex(0xA6); //--set normal color
#endif

	OLED_WriteIndex(0xA8); //--set multiplex ratio(1 to 64) - CHECK
	OLED_WriteIndex(0x3F); //

	OLED_WriteIndex(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

	OLED_WriteIndex(0xD3); //-set display offset - CHECK
	OLED_WriteIndex(0x00); //-not offset

	OLED_WriteIndex(0xD5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteIndex(0xF0); //--set divide ratio

	OLED_WriteIndex(0xD9); //--set pre-charge period
	OLED_WriteIndex(0x22); //

	OLED_WriteIndex(0xDA); //--set com pins hardware configuration - CHECK
	OLED_WriteIndex(0x12);

	OLED_WriteIndex(0xDB); //--set vcomh
	OLED_WriteIndex(0x20); //0x20,0.77xVcc

	OLED_WriteIndex(0x8D); //--set DC-DC enable
	OLED_WriteIndex(0x14); //
	OLED_WriteIndex(0xAF); //--turn on SSD1306 panel
	ssd1306_Fill(Black);
	// Flush buffer to screen
	ssd1306_UpdateScreen();

	SSD1309.CurrentX = 0;
	SSD1309.CurrentY = 0;

	SSD1309.Initialized = 1;
}

// Fill the whole screen with the given color
void ssd1306_Fill(SSD1309_COLOR color) {
    /* Set memory */
    uint32_t i;

    for(i = 0; i < sizeof(SSD1309_Buffer); i++) {
        SSD1309_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

// Write the screenbuffer with changed to the screen
void ssd1306_UpdateScreen(void) {
    uint8_t i;
    for(i = 0; i < 8; i++) {
    	OLED_WriteIndex(0xB0 + i);
    	OLED_WriteIndex(0x00);
    	OLED_WriteIndex(0x10);
    	OLED_WriteData_Multiple(&SSD1309_Buffer[SSD1309_WIDTH*i],SSD1309_WIDTH);
    }
}
void ssd1309_DrawPixel(uint8_t x, uint8_t y, SSD1309_COLOR color) {
    if(x >= SSD1309_WIDTH || y >= SSD1309_HEIGHT) {
        // Don't write outside the buffer
        return;
    }

    // Check if pixel should be inverted
    if(SSD1309.Inverted) {
        color = (SSD1309_COLOR)!color;
    }

    // Draw in the right color
    if(color == White) {
        SSD1309_Buffer[x + (y / 8) * SSD1309_WIDTH] |= 1 << (y % 8);
    } else {
        SSD1309_Buffer[x + (y / 8) * SSD1309_WIDTH] &= ~(1 << (y % 8));
    }
}
