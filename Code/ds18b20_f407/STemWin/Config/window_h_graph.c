
#include "stdio.h"
#include "math.h"
#include "string.h"
#include <stdint.h>
#include "DIALOG.h"
#include "OneWire.h"

#define X_MAX_PIXEL		128
#define Y_MAX_PIXEL		64
extern GUI_CONST_STORAGE GUI_FONT GUI_FontBNazanin16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontTahoma14;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0	  (GUI_ID_USER + 0x00)
#define ID_TEXT1      (GUI_ID_USER + 0x01)
#define ID_TEXT2      (GUI_ID_USER + 0x02)
#define ID_TEXT3      (GUI_ID_USER + 0x03)
#define ID_TEXT4      (GUI_ID_USER + 0x04)
#define ID_TEXT5      (GUI_ID_USER + 0x05)

const uint16_t id_texts[5] = {ID_TEXT1 , ID_TEXT2, ID_TEXT3, ID_TEXT4, ID_TEXT5};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, X_MAX_PIXEL, Y_MAX_PIXEL, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT1", ID_TEXT1, 1, 1, X_MAX_PIXEL -2, 13, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT2", ID_TEXT2, 1, 13, X_MAX_PIXEL -2, 13, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT3", ID_TEXT3, 1, 26, X_MAX_PIXEL -2 ,13, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT4", ID_TEXT4, 1, 39, X_MAX_PIXEL -2,13, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TEXT5", ID_TEXT5, 1, 51, X_MAX_PIXEL -2, 12, 0, 0x0, 0 },
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
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_WINDOW_0);
	  WINDOW_SetDefaultBkColor(GUI_BLACK);
	  for( uint8_t i =0 ; i < 5 ; i++){
		   hItem = WM_GetDialogItem(pMsg->hWin, id_texts[i]);
	  	   TEXT_SetFont(hItem, &GUI_FontTahoma14);
	  	   TEXT_SetTextColor(hItem, GUI_WHITE);
	  	   TEXT_SetBkColor(hItem, GUI_BLACK);
	  	   TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
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
	  		  uint8_t print_size = snprintf(str_temp ,32 , "TEMP%d = %d" ,i + 1, (int32_t)data_temp);
	  		  if( points != 0){
	  			  //points += 2;// "°C" at the end of the string
	  			  uint8_t point_place = print_size - points;
	  			  memmove(str_temp + point_place + 1 , str_temp + point_place , points + 1);
	  			  *(str_temp + point_place) = '.';
	  			  print_size ++;
	  			  for( uint8_t p = points ; p < 4 ; p++){
	  				print_size += sprintf(str_temp + print_size , "0");
	  			  }
	  		  }else{
	  			sprintf(str_temp + print_size , ".0000");
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
