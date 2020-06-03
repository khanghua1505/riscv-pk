/* Copyright (c) 2019 - 2020, Khang Hua, email: khanghua1505@gmail.com
 * All right reserved.
 *
 * This file is written and modified by Khang Hua.
 *
 * This model is free software; you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License; either version 2.1 of the License, or (at your option)
 * any later version. See the GNU Lesser General Public License for more details,
 *
 * This model is distributed in the hope that it will be useful.
 */

#ifndef _KEYCORE_CONFIG_H_
#define _KEYCORE_CONFIG_H_

#include <gpio.h>
#include <spi.h>

#ifndef KEYCORE_ETHERNET_SPI_INSTANCE      
#define KEYCORE_ETHERNET_SPI_INSTANCE (SPI1)        
#endif  

#ifndef KEYCORE_ETHERNET_CS_GPIO_INSTANCE
#define KEYCORE_ETHERNET_CS_GPIO_INSTANCE (GPIO0)        
#endif 

#ifndef KEYCORE_ETHERNET_CS_GPIO_PIN
#define KEYCORE_ETHERNET_CS_GPIO_PIN (GPIO_PIN_1)
#endif 

extern const int ETHERNET_SPI_INS;
extern const int ETHERNET_CS_GPIO_INS;
extern const int ETHERNET_CS_GPIO_PIN;
extern const int ETHERNET_SOCK;

#endif  // _KEYCORE_CONFIG_H_

