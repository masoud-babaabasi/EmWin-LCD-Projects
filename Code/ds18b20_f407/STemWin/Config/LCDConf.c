/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @file    LCDConf_stm3210e_eval.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-September-2016
  * @brief   Driver for STM3210E-EVAL board LCD
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_SPage.h"
#include "main.h"
#include "SSD1309.h"
#include "string.h"
/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
#define XSIZE_PHYS  SSD1309_WIDTH
#define YSIZE_PHYS  SSD1309_HEIGHT

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif

/*********************************************************************
*
*       Defines, sfrs
*
**********************************************************************
*/

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
	GUI_PORT_API PortAPI = {0};
	CONFIG_SPAGE Config = {0};
	GUI_DEVICE * pDevice;
	//
	// Set display driver and color conversion for 1st layer
	//
	pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_SPAGE_1C1, GUICC_1, 0, 0);
	//
	// Display size configuration
	//
	if (LCD_GetSwapXY()) {
	LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
	LCD_SetVSizeEx(0, VYSIZE_PHYS, VXSIZE_PHYS);
	} else {
	LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
	LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
	}
	//
	// Driver configuration
	//
	Config.FirstSEG = 0;//256 - 224;
	GUIDRV_SPage_Config(pDevice, &Config);
	//
	// Configure hardware routines
	//
	PortAPI.pfWrite8_A0 = OLED_WriteIndex;
	PortAPI.pfWrite8_A1 = OLED_WriteData;
	PortAPI.pfWriteM8_A1 = OLED_WriteData_Multiple;
	//PortAPI.pfReadM8_A1 = LCD_X_8080_8_ReadM01;
	GUIDRV_SPage_SetBus8(pDevice, &PortAPI);
	//
	// Controller configuration
	//
	GUIDRV_SPage_Set1510(pDevice);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;

  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
	  OLED_init();
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}


