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
#include <string.h>
#include "fdt.h"
#include "spi.h"

#define SPI_MAX         4

typedef struct
{
    int compat;
    uintptr_t reg;
} SpiScan_t;

static int spi_number = 0;
static spi_ctrl* spi[SPI_MAX];

static void fdt_spi_open(const struct fdt_scan_node *node,
                         void *extra)
{
  SpiScan_t *scan = (SpiScan_t *) extra;
  memset(scan, 0, sizeof(SpiScan_t));
}

static void fdt_spi_prop(const struct fdt_scan_prop *prop, 
                         void *extra)
{
  SpiScan_t *scan = (SpiScan_t *) extra;
  if (!strcmp(prop->name, "compatible") && 
    !strcmp((const char*)prop->value, "sifive,spi0")) {
    scan->compat = 1;
  } else if (!strcmp(prop->name, "reg")) {
     fdt_get_address(prop->node->parent, prop->value, &scan->reg);
  }
}

static void fdt_spi_done(const struct fdt_scan_node *node, 
                         void *extra)
{
  SpiScan_t *scan = (SpiScan_t *) extra;
  if (scan->compat && scan->reg && spi_number <= SPI_MAX) {
    spi[spi_number++] = (spi_ctrl *) scan->reg;
  }
}

void query_spi(uintptr_t dtb)
{
  if (spi_number > 0) {
    return ;
  }
  
  SpiScan_t scan;
  struct fdt_cb cb = {
    .open = fdt_spi_open,
    .prop = fdt_spi_prop,
    .done = fdt_spi_done,
    .extra = &scan
  };
  
  fdt_scan(dtb, &cb);
}

uintptr_t spi_get_address(int id)
{
  return (uintptr_t) (id < spi_number ? spi[id] : 0);
}

int spi_get_number(void)
{
  return spi_number;
}

void spi_tx(int id, uint8_t ch)
{
#if __riscv_atomic
  int32_t r = 0;
  do {
    asm volatile (
      "amoor.w %0, %2, %1\n"
      : "=r" (r), "+A" (spi[id]->txdata.raw_bits)
      : "r" (ch));
  } while (r < 0);
#else
  while ((int32_t)spi[id]->txdata.raw_bits < 0);
  spi[id]->txdata.data = ch;
#endif
}

int spi_rx(int id)
{
  int32_t out;
  while ((out=(int32_t) spi[id]->rxdata.raw_bits) < 0);
  return (uint8_t) out;
}

int spi_txrx(int id, uint8_t ch)
{
  spi_tx(id, ch);
  return spi_rx(id);
}
  
