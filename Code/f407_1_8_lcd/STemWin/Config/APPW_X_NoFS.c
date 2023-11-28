/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.50 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : APPW_X_emFile.c
Purpose     : emFile file system access
---------------------------END-OF-HEADER------------------------------
*/

#if !defined(WIN32) && !defined(APP_WIZARD)

#include "AppWizard.h"
//
//#include <string.h>
//#include "fatfs.h"
//#include <stdint.h>
//
//#define MAX_APP_FILE_OPEN	10
//static FIL APPW_FS_files[ MAX_APP_FILE_OPEN ];
//static uint8_t APPW_files_open = 0;
//
//#define FILE_SYS		SDFatFS
//#define ROOT_FILE_PATH "\\Resource\\Font\\Arial_24_Normal_EXT_AA4.xbf"
///*********************************************************************
//*
//*       Static code
//*
//**********************************************************************
//*/
//
//static void make_fatFS_path(const char *input_path , char *output_path){
//	char *str_tock1 = input_path , *str_tock2;
//	uint8_t print_size , len_cpy;
//	print_size = sprintf(output_path , "%d:APPWIZARD" , FILE_SYS.drv);
//	while(str_tock1 != NULL){
//		str_tock1 = strstr(str_tock1 , "\\");
//		str_tock1 += 1;
//		str_tock2 = strstr(str_tock1 , "\\");
//		if( str_tock2 != NULL) len_cpy = str_tock2 - str_tock1;
//		else len_cpy = strlen(str_tock1);
//		strcat(output_path , "/");
//		strncat(output_path , str_tock1 , len_cpy);
//		str_tock1 = str_tock2;
//	}
//
//
//}
//
///*********************************************************************
//*
//*       _Open
//*/
//static void * _Open(const char * pFilename) {
////  FS_FILE * pFile;
////  pFile = FS_FOpen(pFilename, "r");
////  return (void *)pFile;
//  char str_path[128];
//  make_fatFS_path(pFilename , str_path);
//  if( APPW_files_open >= MAX_APP_FILE_OPEN) {
//	  return NULL;
//  }
//  FRESULT retFS = f_open(&APPW_FS_files[APPW_files_open] , str_path , FA_READ);
//  if(retFS == FR_OK) {
//	  APPW_files_open++;
//	  return (void *)&APPW_FS_files[APPW_files_open - 1] ;
//  }
//  else return NULL;
//}
//
///*********************************************************************
//*
//*       _Seek
//*/
//static int _Seek(const void * p, uint32_t Off) {
////  FS_FILE * pFile;
////
////  pFile  = (FS_FILE *)p;
////  return FS_SetFilePos(pFile, Off, FS_FILE_BEGIN);
//
//  return f_lseek((FIL*)p , Off);
//}
//
///*********************************************************************
//*
//*       _Read
//*/
//static uint32_t _Read(const void * p, void * pData, uint32_t NumBytes) {
////  FS_FILE * pFile;
////  uint32_t NumBytesRead;
////
////  pFile  = (FS_FILE *)p;
////  NumBytesRead = FS_Read(pFile, pData, NumBytes);
////  return NumBytesRead;
//
//  unsigned int bytes_read;
//  f_read((FIL*)p , pData , NumBytes , &bytes_read);
//  return bytes_read;
//}
//
///*********************************************************************
//*
//*       _Close
//*/
//static void _Close(const void * p) {
////  FS_FILE * pFile;
////
////  pFile = (FS_FILE *)p;
////  FS_FClose(pFile);
//
//  f_close((FIL*)p);
//  APPW_files_open--;
//}
//
///*********************************************************************
//*
//*       _Size
//*/
//static uint32_t _Size(const void * p) {
////  FS_FILE * pFile;
////  uint32_t FileSize;
////
////  pFile = (FS_FILE *)p;
////  FileSize = FS_GetFileSize((const FS_FILE *)pFile);
////  return FileSize;
//
//  uint32_t FileSize = f_size((FIL*)p);
//  return FileSize;
//}

/*********************************************************************
*
*       _FileAccess
*/
static APPW_X_FILEACCESS _FileAccess = {
//  _Open,
//  _Seek,
//  _Read,
//  _Close,
//  _Size,
};

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       APPW_X_FS_Init
*/
void APPW_X_FS_Init(void) {
//  APPW_SetFileAccess(&_FileAccess);
}

#endif

/*************************** End of file ****************************/
