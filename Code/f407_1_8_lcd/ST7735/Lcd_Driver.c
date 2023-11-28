#include "Lcd_Driver.h"
extern SPI_HandleTypeDef 	hspi3;
#define LCD_SPI 			hspi3
extern DMA_HandleTypeDef hdma_spi3_tx;
_lcd_dev lcddev;
static uint8_t position_cmd = 0;
static uint8_t spi_tx_compelete = 0;
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	if( hspi == &hspi3){
		spi_tx_compelete = 1;
	}
}
static void  SPI_WriteData(u8 *Data , int number)
{
	/*char i=0;
	for(int count =0 ; count < number ; count++){
		for(i=8;i>0;i--)
		{
		  LCD_SCL_CLR;
		  if(*Data & 0x80)
		  LCD_SDA_SET;
		  else LCD_SDA_CLR;

		  LCD_SCL_SET;
		  *Data<<=1;
		}
		Data++;
	}*/
	//HAL_SPI_Transmit(&hspi3 , Data , number , 1000);
	spi_tx_compelete = 0;
	HAL_StatusTypeDef ret_val = HAL_SPI_Transmit_DMA(&LCD_SPI, Data, number);
//	ret_val = HAL_DMA_PollForTransfer(&hdma_spi3_tx , HAL_DMA_FULL_TRANSFER , 100);
	uint32_t start = HAL_GetTick();
	while(spi_tx_compelete == 0){
		if( HAL_GetTick() - start >= 100){
			break;
		}
	}
}
static void SPI_WriteData_one(u8 data){
	HAL_SPI_Transmit(&LCD_SPI , &data , 1 , 50);
}

void Lcd_WriteIndex(u8 Index)
{
	if( Index == 0x2a) position_cmd = 2;
	else if( Index == 0x2b ) position_cmd = 1;
   LCD_CS_CLR;
   LCD_RS_CLR;
	 SPI_WriteData_one(Index);
   LCD_CS_SET;
}


void Lcd_WriteData(u8 Data)
{
	static uint8_t i =0 ;
	if( position_cmd){
		Data += position_cmd;
		i++;
		if( i == 4) {
			position_cmd = 0;
			i = 0;
		}
	}
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteData_one(Data);
   LCD_CS_SET; 
}

void Lcd_WriteData_Multiple(u8 *Data , int num)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteData(Data , num);
   LCD_CS_SET;
}

void Lcd_WriteData_16(u16 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
   SPI_WriteData_one(Data >> 8);
   SPI_WriteData_one(Data);
   LCD_CS_SET;
}
static void _delay(uint8_t input_d){
	while(input_d--);
}
void LCD_ReadData_Multiple(u8 CMD, u8 *Data , int num){
	LCD_CS_CLR;
	LCD_RS_CLR;
	SPI_WriteData_one(CMD);
	LCD_SDA_INPUT_MODE;
	for(int j = 0 ; j < num ; j++){
		//read 8bit data
		*(Data + j) = 0;
		for(int8_t i = 7 ; i >= 0 ; i--){
			LCD_SCL_CLR;
			//_delay(5);
			LCD_SCL_SET;
			//_delay(5);
			if( LCD_SDA_INPUT_VAL ) *(Data + j) |= (1UL << i);
		}
	}
	LCD_CS_SET;
	LCD_SDA_OUTPUT_MODE;
}
uint32_t LCD_Read_ID(void){
	uint8_t data[4];
	uint32_t ID = 0;
	data[0] = 0x04;
	LCD_CS_CLR;
	LCD_RS_CLR;
	SPI_WriteData( data, 1);
	LCD_SDA_INPUT_MODE;
	//dummy cycles
	LCD_SCL_CLR;
	_delay(5);
	LCD_SCL_SET;
	//read 24bit data
	for(int8_t i = 23 ; i >= 0 ; i--){
		LCD_SCL_CLR;
		_delay(5);
		LCD_SCL_SET;
		_delay(5);
		if( LCD_SDA_INPUT_VAL ) ID |= (1UL << i);
	}
	LCD_CS_SET;
	LCD_SDA_OUTPUT_MODE;
	return ID;
}

//void LCD_WriteData_16Bit(u16 Data)
//{
//   LCD_CS_CLR;
//   LCD_RS_SET;
//	 //SPI_WriteData(Data>>8);
//	 SPI_WriteData(Data , 16);
//   LCD_CS_SET;
//}
//void LCD_WriteData_16Bit_Multiple(u16 *Data , int num){
//   LCD_CS_CLR;
//   LCD_RS_SET;
//   while(num--){
//	 //SPI_WriteData(*Data>>8);
//	 SPI_WriteData(*Data , 16);
//	 Data++;
//   }
//   LCD_CS_SET;
//}
void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}

void Lcd_Reset(void)
{
	LCD_RST_CLR;
	HAL_Delay(100);
	LCD_RST_SET;
	HAL_Delay(100);
}

//LCD Init For 1.44Inch LCD Panel with ST7735R.
void Lcd_Init(void)
{	
	Lcd_Reset(); //Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit
	HAL_Delay (120);

	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB2);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB3);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB4); //Column inversion
	Lcd_WriteData(0x07);

	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0);
	Lcd_WriteData(0xA2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x84);
	Lcd_WriteIndex(0xC1);
	Lcd_WriteData(0xC5);

	Lcd_WriteIndex(0xC2);
	Lcd_WriteData(0x0A);
	Lcd_WriteData(0x00);

	Lcd_WriteIndex(0xC3);
	Lcd_WriteData(0x8A);
	Lcd_WriteData(0x2A);
	Lcd_WriteIndex(0xC4);
	Lcd_WriteData(0x8A);
	Lcd_WriteData(0xEE);

	Lcd_WriteIndex(0xC5); //VCOM
	Lcd_WriteData(0x0E);

	Lcd_WriteIndex(0x36); //MX, MY, RGB mode
	Lcd_WriteData(0xC0);

	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x1a);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x18);
	Lcd_WriteData(0x2f);
	Lcd_WriteData(0x28);
	Lcd_WriteData(0x20);
	Lcd_WriteData(0x22);
	Lcd_WriteData(0x1f);
	Lcd_WriteData(0x1b);
	Lcd_WriteData(0x23);
	Lcd_WriteData(0x37);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x10);

	Lcd_WriteIndex(0xe1);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x1b);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x17);
	Lcd_WriteData(0x33);
	Lcd_WriteData(0x2c);
	Lcd_WriteData(0x29);
	Lcd_WriteData(0x2e);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x39);
	Lcd_WriteData(0x3f);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x03);
	Lcd_WriteData(0x10);

	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x7f);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x9f);

	Lcd_WriteIndex(0xF0); //Enable test command
	Lcd_WriteData(0x01);
	Lcd_WriteIndex(0xF6); //Disable ram power save mode
	Lcd_WriteData(0x00);

	Lcd_WriteIndex(0x3A); //65k mode
	Lcd_WriteData(0x05);

	Lcd_Clear(WHITE);
	Lcd_WriteIndex(0x29);//Display on
}


/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{		
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData((x_start) >> 8);
	Lcd_WriteData(x_start);
	Lcd_WriteData((x_end) >> 8);
	Lcd_WriteData(x_end);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData((y_start) >> 8);
	Lcd_WriteData(y_start);
	Lcd_WriteData((y_end) >> 8);
	Lcd_WriteData(y_end);
	
	Lcd_WriteIndex(0x2c);

}

/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(u16 x,u16 y)
{
  	Lcd_SetRegion(x,y,x,y);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetRegion(x,y,x+1,y+1);
	LCD_WriteData_16Bit(Data);

}    

/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
unsigned int Lcd_ReadPoint(u16 x,u16 y)
{
  unsigned int Data;
  Lcd_SetXY(x,y);

  //Lcd_ReadData();//���������ֽ�
  //Data=Lcd_ReadData();
  Lcd_WriteData(Data);
  return Data;
}
/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   //Lcd_WriteIndex(0x2C);
   //uint8_t Data[2] = { Color >> 8 , Color};
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {

	  	Lcd_WriteData_16(Color);
    }
}

