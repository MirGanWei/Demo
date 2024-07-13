
/* Includes ------------------------------------------------------------------*/	
#include "boot.h"
#include "usart.h" 

__asm void boot( uint32_t address )
{

   LDR SP, [R0]

   LDR PC, [R0, #4]

}

void jmp_addr(uint32_t address)
{
	printf("JMP: 0x%08x\r\n", address);
	
	HAL_UART_DeInit(&huart1);
	
	boot(address);
}
