#include "flash.h"
#include "usart.h"

/*********************************************************************************
函数名称 ： GetSector_Number
功    能 ： 获取扇区编号
参    数 ： addr：flash地址
返 回 值 ： 无
说    明 ： 无
**********************************************************************************/
uint32_t GetSector_Number(uint32_t addr)
{
  uint32_t sector = 0;
  
  if((addr < ADDR_FLASH_SECTOR_1) && (addr >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((addr < ADDR_FLASH_SECTOR_2) && (addr >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((addr < ADDR_FLASH_SECTOR_3) && (addr >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((addr < ADDR_FLASH_SECTOR_4) && (addr >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((addr < ADDR_FLASH_SECTOR_5) && (addr >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((addr < ADDR_FLASH_SECTOR_6) && (addr >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((addr < ADDR_FLASH_SECTOR_7) && (addr >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else 
  {
    sector = FLASH_SECTOR_7;  
  }
 
  return sector;
}

/*********************************************************************************
函数名称 ： Erase_Flash
功    能 ： 擦除flash地址
参    数 ： start_addr：起始地址，end_addr：结束地址
返 回 值 ： 无
说    明 ： 无
**********************************************************************************/
void Erase_Flash(uint32_t start_addr, uint32_t end_addr)
{
	uint32_t start_sector, end_sector, sector_error;
	
	FLASH_EraseInitTypeDef pEraseInit;
	
	//FLASH标志位必须手动清除
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	
	start_sector = GetSector_Number(start_addr);
	end_sector = GetSector_Number(end_addr);

	pEraseInit.TypeErase = TYPEERASE_SECTORS;
	pEraseInit.Sector = start_sector;
	pEraseInit.NbSectors = end_sector - start_sector;
	pEraseInit.VoltageRange = VOLTAGE_RANGE_3;
	
	if(HAL_FLASHEx_Erase(&pEraseInit, &sector_error) != HAL_OK)
	{
		printf("flash rease error\r\n");
		return;
	}
}

/*********************************************************************************
函数名称 ： Write_System_Config
功    能 ： 写系统配置函数
参    数 ： start_addr：起始地址，data：写数据，lentgh：数据长度
返 回 值 ： 无
说    明 ： 无
**********************************************************************************/
void Write_Flash(uint32_t start_addr, uint8_t *data, uint8_t lentgh)
{
	uint8_t i;
	HAL_StatusTypeDef status = HAL_ERROR;
	
	__disable_irq();
	HAL_FLASH_Unlock(); 
	
	//FLASH标志位必须手动清除
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	
	for(i=0;i<lentgh;i++)
	{
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, start_addr+i, data[i]);
		if(status != HAL_OK)
		{
			printf("flash program error\r\n");
			return;
		}
	}

	HAL_FLASH_Lock();
	__enable_irq();
}

/*********************************************************************************
函数名称 ： Read_Flash
功    能 ： 读内部flash
参    数 ： addr：读地址，buffer：读数据缓存, length：读数据长度
返 回 值 ： flash读出的数据
说    明 ： 无
**********************************************************************************/
void Read_Flash(uint32_t addr, uint8_t *buffer, uint8_t length)
{
	uint8_t i;
	uint32_t psrc = addr;
		
	for(i=0;i<length;i++)
	{
		buffer[i] = *(uint8_t *)(psrc++);
	}
}

