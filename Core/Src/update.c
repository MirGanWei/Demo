
/* Includes ------------------------------------------------------------------*/	
#include "update.h"
#include "common.h"
#include "flash.h"
#include "usart.h" 

/* Global variable ------------------------------------------------------------*/
Update_Store_t Update_Info = 
{
	.file_size = 0,
	.need_update = 0,
	.crc16 = 0,
};

/*********************************************************************************
函数名称 ： Write_Update_Config
功    能 ： 写升级配置
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
**********************************************************************************/
void Write_Update_Config(void)
{
	uint16_t i;
	uint16_t crc16 = 0;
	uint8_t *p = 0;
	uint16_t data = 0,length = 0;
	uint32_t addr = 0;
	HAL_StatusTypeDef status = HAL_ERROR;
	
	crc16 = crc_16((uint8_t *)&Update_Info, sizeof(Update_Store_t) - 2);
	
	Update_Info.crc16 = crc16;
	p = (uint8_t *)&Update_Info;
	
	addr = STORE_UPDATE_CONFIG_ADDR;
	length = sizeof(Update_Store_t);
	
	__disable_irq();
	
	HAL_FLASH_Unlock(); 
	Erase_Flash(STORE_UPDATE_CONFIG_ADDR,STORE_UPDATE_CODE_ADDR);
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	
	for(i=0; i<length; i+=2)
	{
		data = (p[i+1] << 8) | p[i];
		
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, data);
		if(status != HAL_OK)
		{
			HAL_FLASH_Lock();
			printf("flash program error\r\n");
			return;
		}
		
		addr += 2;
	}
	printf("Write Config finish\r\n");
	
	HAL_FLASH_Lock();
}

/*********************************************************************************
函数名称 ： Update_System_Code
功    能 ： 更新系统程序
参    数 ： 无
返 回 值 ： 无
说    明 ： 无
**********************************************************************************/
void Update_System_Code(void)
{
	uint16_t fram_index = 0, fram_count = 0;
	uint32_t length = 0, read_length = 0;
	uint8_t buff[UPDATE_FRAM_SIZE];
	uint16_t crc16 = 0;
	HAL_StatusTypeDef status = HAL_ERROR;
	uint16_t data = 0, i = 0;
	uint32_t update_code_addr = 0,app_code_addr = 0;

	memcpy((uint8_t *)&Update_Info, (void *)(STORE_UPDATE_CONFIG_ADDR), sizeof(Update_Store_t));
	
	crc16 = crc_16((uint8_t *)&Update_Info, sizeof(Update_Store_t) - 2);
	if(crc16 != Update_Info.crc16) 
	{
		printf("config file crc16 error\r\n");
		return ;
	}
	printf("config file crc16 success\r\n");
	
	if(Update_Info.need_update != 0x01) 
	{
		printf("system no need update\r\n");
		return;
	}
	printf("system need update\r\n");
	
	if(Update_Info.file_size == 0) return;
	if(Update_Info.file_size > STORE_APP_CODE_SIZE) return;

	//计算升级位置
	fram_count = Update_Info.file_size / UPDATE_FRAM_SIZE;
	if(Update_Info.file_size % UPDATE_FRAM_SIZE != 0) fram_count++;//得到总包数
	
	app_code_addr = STORE_APP_CODE_ADDR;			//应用层代码地址
	update_code_addr = STORE_UPDATE_CODE_ADDR;//新代码地址
	
	//读取程序文件0x08010000中的标志，新代码中存储的标志地址：0x08014000
	for(i=0;i<23;i++)
	{
		buff[i] = *(uint32_t *)(update_code_addr+0x4000+i);
	}
	
	if(memcmp(buff, "Coordinator_4_Channel\r\n", 23) != 0)
	{
		printf("check update file flag fail\r\n");
		return;
	}
	
	__disable_irq();
	
	//开始升级
	HAL_FLASH_Unlock(); 
	printf("flash unlock\r\n");
	
	//STM32F407擦除flash只能按扇区擦除，写flash可以单地址和多地址同时写
	printf("ERASE flash\r\n");
	
	Erase_Flash(STORE_APP_CODE_ADDR,STORE_UPDATE_CONFIG_ADDR);//擦除当前应用程序
	
	printf("ERASE flash finish\r\n");
	
	//FLASH标志位必须手动清除
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	
	printf("PROGRAM flash\r\n");
	length = 0;
	for(fram_index=0; fram_index < fram_count; fram_index++)
	{
		read_length = ((Update_Info.file_size - length) > UPDATE_FRAM_SIZE) ? UPDATE_FRAM_SIZE : (Update_Info.file_size - length);

		memset(buff, 0, UPDATE_FRAM_SIZE);
		
		for(i=0;i<read_length;i++)
		{
			buff[i] = *(uint32_t *)(update_code_addr+i);
		}
		
		for(i=0; i<read_length; i+=2)
		{
			data = (buff[i+1] << 8) | buff[i];//STM32内部flash，uint16_t类型写数据时，数据是低位在前，高位在后
			
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, app_code_addr, data);
			if(status != HAL_OK)
			{
				HAL_FLASH_Lock();
				printf("flash program error\r\n");
				return;
			}
			app_code_addr += 2;
		}
		
		length += read_length;
		update_code_addr += read_length;
	}
	printf("PROGRAM flash finish\r\n");
	
	HAL_FLASH_Lock();
	printf("flash lock\r\n");
	
	//清除Flash标志
	Update_Info.need_update = 0x00;
	Write_Update_Config();
	
	printf("sysytem update success\r\n");
}
