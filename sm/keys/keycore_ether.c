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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <spi.h>
#include <gpio.h>
#include "socket.h"
#include "keycore.h"
#include "config.h"
#include "mtrap.h"

static uint8_t  sock = 0;
static uint16_t port = 8080;
static uint8_t  serverip[] = {10,10,10,225};

static const uint8_t clientip[4] = {10,10,10,2};
static const uint8_t mac_addr[6] = {0xEA,0x11,0x22,0x33,0x44,0xEA};
static const uint8_t default_gateway[4] = {10,10,10,225};
static const uint8_t subnet_mask[4] = {255,255,255,0};

static wiz_NetInfo net_info;
static uint8_t chip_txrx_buffsize[] = {2, 2, 2, 2, 2, 2, 2, 2};

static void spi_select(void);
static void spi_deselect(void);
static void spi_read(uint8_t *buffer, uint16_t len);
static void spi_write(uint8_t *buffer, uint16_t len);
static uint8_t spi_read_byte(void);
static void spi_write_byte(uint8_t byte);

static void wait_for(unsigned long cycle)
{
  for (unsigned long i = 0; i < cycle; i++) 
    asm ("nop\n");
}

static void spi_select(void)
{
  gpio_pin_setval(ETHERNET_CS_GPIO_INS, ETHERNET_CS_GPIO_PIN, 
                  GPIO_PIN_RESET);
}

static void spi_deselect(void)
{
  gpio_pin_setval(ETHERNET_CS_GPIO_INS, ETHERNET_CS_GPIO_PIN, 
                  GPIO_PIN_SET);
}

static void spi_read(uint8_t *buffer, uint16_t len)
{
  uint16_t index;
  for (index = 0; index < len; index++) {
    buffer[index] = spi_read_byte();
  }
}

static void spi_write(uint8_t *buffer, uint16_t len)
{
  uint16_t index;
  for (index = 0; index < len; index++) {
    spi_write_byte(buffer[index]);
  }
}

static uint8_t spi_read_byte(void)
{
  uint8_t temp = 0x55;
  uint8_t r = spi_txrx(ETHERNET_SPI_INS, temp);
  
  wait_for(1000);
  
  return  r;
}

static void spi_write_byte(uint8_t byte)
{
  (void)spi_txrx(ETHERNET_SPI_INS, byte);
  wait_for(1000);
}

static void gpio_config(void)
{
  gpio_pin_setval(ETHERNET_CS_GPIO_INS, ETHERNET_CS_GPIO_PIN, 
                  GPIO_PIN_SET);
}

static int chip_init(void)
{
  reg_wizchip_cs_cbfunc(spi_select, spi_deselect);
  reg_wizchip_spi_cbfunc(spi_read_byte, spi_write_byte);
  reg_wizchip_spiburst_cbfunc(spi_read, spi_write);
  
  return 0;
}

int ethernet_init(void)
{
  gpio_config();
  chip_init();
  
  return 0;
}

ssize_t ethernet_write(uint8_t *buffer, uint16_t len)
{
  ssize_t byte_transferred;
  
  byte_transferred = send(ETHERNET_SOCK, buffer, len);
  if (byte_transferred <= 0) {
    printm("Socket send failed\r\n");
    close(ETHERNET_SOCK);
    return -1;
  }
  
  return byte_transferred;
}

ssize_t ethernet_read(uint8_t *buffer, uint16_t len)
{
  return recv(ETHERNET_SOCK, &buffer[0], len);
}

keycore_handle_t keycore = {
  .initfn = ethernet_init,
  .readfn = ethernet_read,
  .writefn = ethernet_write,
};





