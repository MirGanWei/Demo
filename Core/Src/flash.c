#include "flash.h"
#include "usart.h"

/*********************************************************************************
�������� �� GetSector_Number
��    �� �� ��ȡ�������
��    �� �� addr��flash��ַ
�� �� ֵ �� ��
˵    �� �� ��
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
�������� �� Erase_Flash
��    �� �� ����flash��ַ
��    �� �� start_addr����ʼ��ַ��end_addr��������ַ
�� �� ֵ �� ��
˵    �� �� ��
**********************************************************************************/
void Erase_Flash(uint32_t start_addr, uint32_t end_addr)
{
	uint32_t start_sector, end_sector, sector_error;
	
	FLASH_EraseInitTypeDef pEraseInit;
	
	//FLASH��־λ�����ֶ����
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
�������� �� Write_System_Config
��    �� �� дϵͳ���ú���
��    �� �� start_addr����ʼ��ַ��data��д���ݣ�lentgh�����ݳ���
�� �� ֵ �� ��
˵    �� �� ��
**********************************************************************************/
void Write_Flash(uint32_t start_addr, uint8_t *data, uint8_t lentgh)
{
	uint8_t i;
	HAL_StatusTypeDef status = HAL_ERROR;
	
	__disable_irq();
	HAL_FLASH_Unlock(); 
	
	//FLASH��־λ�����ֶ����
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
�������� �� Read_Flash
��    �� �� ���ڲ�flash
��    �� �� addr������ַ��buffer�������ݻ���, length�������ݳ���
�� �� ֵ �� flash����������
˵    �� �� ��
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

