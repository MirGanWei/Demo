#ifndef __CRC16_CHECK_H
#define __CRC16_CHECK_H

/* Includes ---------------------------------------------------------------------*/
#include "main.h"

/* Declaration ------------------------------------------------------------------*/
uint16_t CRC16_Check(uint8_t* pchMsg, uint16_t wDataLen);
uint8_t Check_Sum(uint8_t *Check_Data,uint16_t Lenght);

#endif
