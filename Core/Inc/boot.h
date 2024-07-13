#ifndef __BOOT_H
#define __BOOT_H

/* Includes ------------------------------------------------------------------*/	
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

/* Declaration ---------------------------------------------------------------*/
void jmp_addr(uint32_t address);

#endif
