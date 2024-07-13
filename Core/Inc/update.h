#ifndef __UPDATE_H
#define __UPDATE_H

/* Includes ------------------------------------------------------------------*/	
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/* Declaration ---------------------------------------------------------------*/
#define UPDATE_FRAM_SIZE						0x0200			//�����������ݵĳߴ�

//BOOTLOADER��FLASH�е�λ�ô�С
#define STORE_BOOT_ADDR							0x08000000	//������ʼ��ַ
#define STORE_BOOT_SIZE							0x10000			//BOOT�������64K

//Ӧ�ó�����FLASH�е�λ�ô�С
#define STORE_APP_CODE_ADDR					0x08010000
#define STORE_APP_CODE_SIZE					0x20000			//Ӧ�ó������128K

#define STORE_UPDATE_CONFIG_ADDR		0x08040000	//�����������õ�ַ
#define STORE_UPDATE_CODE_ADDR			0x08060000	//��������洢��ַ

/* Typedef -------------------------------------------------------------------*/
//������Ϣ�洢�ṹ��
typedef struct
{
	uint32_t file_size;		//�����ļ��ܴ�С
	uint16_t need_update;	//�Ƿ���Ҫִ������
	uint16_t crc16;				//������Ϣ�洢У��
}Update_Store_t;

/* Declaration ---------------------------------------------------------------*/
void Read_Update_Config(void);
void Update_System_Code(void);

#endif
