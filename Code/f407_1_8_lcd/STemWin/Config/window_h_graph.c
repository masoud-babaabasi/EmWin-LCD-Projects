
#include "stdio.h"
#include "math.h"
#include "string.h"
#include <stdint.h>
#include "DIALOG.h"
#include "OneWire.h"
#include "LCD_Config.h"
extern GUI_CONST_STORAGE GUI_FONT GUI_Font2Nazanin30;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontBNazanin34;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
const uint32_t color_row[5] = {GUI_RED , GUI_BLUE , GUI_GREEN , GUI_CYAN , GUI_MAGENTA};
#define ID_WINDOW_0	  (GUI_ID_USER + 0x00)
#define ID_TEXT1      (GUI_ID_USER + 0x01)
#define ID_TEXT2      (GUI_ID_USER + 0x02)
#define ID_TEXT3      (GUI_ID_USER + 0x03)
#define ID_TEXT4      (GUI_ID_USER + 0x04)
#define ID_TEXT5      (GUI_ID_USER + 0x05)

const uint16_t id_texts[5] = {ID_TEXT1 , ID_TEXT2, ID_TEXT3, ID_TEXT4, ID_TEXT5};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, X_MAX_PIXEL, Y_MAX_PIXEL, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT1", ID_TEXT1, 0, 0, X_MAX_PIXEL, Y_MAX_PIXEL / 5, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT2", ID_TEXT2, 0, Y_MAX_PIXEL / 5, X_MAX_PIXEL, Y_MAX_PIXEL / 5, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT3", ID_TEXT3, 0, 2 * Y_MAX_PIXEL / 5, X_MAX_PIXEL, Y_MAX_PIXEL / 5, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT4", ID_TEXT4, 0, 3 * Y_MAX_PIXEL / 5, X_MAX_PIXEL, Y_MAX_PIXEL / 5, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT5", ID_TEXT5, 0, 4 * Y_MAX_PIXEL / 5, X_MAX_PIXEL, Y_MAX_PIXEL / 5, 0, 0x0, 0 },
  };
char str_temp[32];
int8_t points;
float data_temp;
WM_HWIN hWin ;
static void _cbDialog(WM_MESSAGE * pMsg) {
  MENU_ITEM_DATA   ItemData;
  WM_HWIN          hItem;
  GRAPH_SCALE_Handle vScale;
  int              NCode;
  int              Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
	  for( uint8_t i =0 ; i < 5 ; i++){
		   hItem = WM_GetDialogItem(pMsg->hWin, id_texts[i]);
	  	   TEXT_SetFont(hItem, &GUI_FontBNazanin34);
	  	   TEXT_SetTextColor(hItem, color_row[i]);
	  	   TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
	     }
	  break;
  case WM_PAINT:
	  for(uint8_t i = 0 ; i < 5 ; i++){
		  hItem = WM_GetDialogItem(pMsg->hWin, id_texts[i]);
	  		  data_temp = Temp[i];
	  		  points = 0;
	  		  while( data_temp != floor(data_temp)){
	  			  data_temp *= 10;
	  			  points++;
	  			  if( points > 10 ){
	  				  points = 0 ;
	  				  data_temp = 0;
	  				  break;
	  			  }
	  		  }
	  		  uint8_t print_size = snprintf(str_temp ,32 , "دما%d = %d" ,i + 1, (int32_t)data_temp);
	  		  if( points != 0){
	  			  //points += 2;// "°C" at the end of the string
	  			  uint8_t point_place = print_size - points;
	  			  memmove(str_temp + point_place + 1 , str_temp + point_place , points + 1);
	  			  *(str_temp + point_place) = '.';
	  		  }
	  		  //GUI_SetColor(color_row[i]);
	  		  //GUI_DispStringAt(str_temp, 5, i * 32);
	  		  TEXT_SetText(hItem, str_temp);
	  	  }
	  break;
  default:
      WM_DefaultProc(pMsg);
      break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateWindow(WM_HWIN parent);
WM_HWIN CreateWindow(WM_HWIN parent) {
	return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, parent, 0, 0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
