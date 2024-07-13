#ifndef __UPDATE_H
#define __UPDATE_H

/* Includes ------------------------------------------------------------------*/	
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/* Declaration ---------------------------------------------------------------*/
#define UPDATE_FRAM_SIZE						0x0200			//单包接收数据的尺寸

//BOOTLOADER在FLASH中的位置大小
#define STORE_BOOT_ADDR							0x08000000	//程序起始地址
#define STORE_BOOT_SIZE							0x10000			//BOOT程序最大64K

//应用程序在FLASH中的位置大小
#define STORE_APP_CODE_ADDR					0x08010000
#define STORE_APP_CODE_SIZE					0x20000			//应用程序最大128K

#define STORE_UPDATE_CONFIG_ADDR		0x08040000	//在线升级配置地址
#define STORE_UPDATE_CODE_ADDR			0x08060000	//升级代码存储地址

/* Typedef -------------------------------------------------------------------*/
//升级信息存储结构体
typedef struct
{
	uint32_t file_size;		//升级文件总大小
	uint16_t need_update;	//是否需要执行升级
	uint16_t crc16;				//升级信息存储校验
}Update_Store_t;

/* Declaration ---------------------------------------------------------------*/
void Read_Update_Config(void);
void Update_System_Code(void);

#endif
