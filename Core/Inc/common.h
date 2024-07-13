#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Declaration ---------------------------------------------------------------*/
uint16_t crc_16(uint8_t *q, uint16_t len);
uint16_t crc_16_1021(uint16_t crc, uint8_t *data, uint32_t len);

#endif
