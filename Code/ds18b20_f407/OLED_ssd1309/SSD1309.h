#ifndef __SSD1309__
#define __SSD1309__
#include "main.h"

#define SSD1309_WIDTH		128
#define SSD1309_HEIGHT		64

typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1309_COLOR;

typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1309_t;

#define OLED_SCK        		GPIO_PIN_10
#define OLED_SCK_PORT			GPIOC

#define OLED_MOSI        		GPIO_PIN_11
#define OLED_MOSI_PORT			GPIOC

#define OLED_CS        			GPIO_PIN_2
#define OLED_CS_PORT			GPIOD

#define OLED_DC         		GPIO_PIN_1
#define OLED_DC_PORT			GPIOD

#define OLED_RST     			GPIO_PIN_0
#define OLED_RST_PORT			GPIOD

#define OLED_SCK_shift		(OLED_SCK    << 16 )
#define OLED_MOSI_shift		(OLED_MOSI   << 16 )
#define OLED_CS_shift		(OLED_CS     << 16 )
#define OLED_DC_shift		(OLED_DC    << 16 )
#define OLED_RST_shift		(OLED_RST   << 16 )

#define	OLED_SCK_SET  	OLED_SCK_PORT->BSRR = OLED_SCK
#define	OLED_MOSI_SET  	OLED_MOSI_PORT->BSRR = OLED_MOSI
#define	OLED_CS_SET  	OLED_CS_PORT->BSRR = OLED_CS
#define	OLED_DC_SET  	OLED_DC_PORT->BSRR = OLED_DC
#define	OLED_RST_SET  	OLED_RST_PORT->BSRR = OLED_RST

#define	OLED_SCK_CLR  	OLED_SCK_PORT->BSRR = OLED_SCK_shift
#define	OLED_MOSI_CLR  	OLED_MOSI_PORT->BSRR = OLED_MOSI_shift
#define	OLED_CS_CLR  	OLED_CS_PORT->BSRR = OLED_CS_shift
#define	OLED_RST_CLR  	HAL_GPIO_WritePin(OLED_RST_PORT , OLED_RST , GPIO_PIN_RESET)
#define	OLED_DC_CLR  	OLED_DC_PORT->BSRR = OLED_DC_shift


void OLED_WriteIndex(uint8_t Index);
void OLED_WriteData(uint8_t Data);
void OLED_WriteData_Multiple(uint8_t *Data , uint32_t num);
void OLED_init(void);
void OLED_reset(void);
void ssd1306_Fill(SSD1309_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1309_DrawPixel(uint8_t x, uint8_t y, SSD1309_COLOR color) ;

#endif
