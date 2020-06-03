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
#include "gpio.h"

#define GPIO_MAX    4

typedef volatile uint32_t* GpioBaseAddress_t;

typedef struct {
  int compat;
  uintptr_t reg;
} GpioScan_t;

static int gpio_number = 0;
static GpioBaseAddress_t gpio[GPIO_MAX];

static void fdt_gpio_open(const struct fdt_scan_node *node,
                          void *extra)
{
  GpioScan_t *scan = (GpioScan_t *) extra;
  memset(scan, 0, sizeof(GpioScan_t));
}

static void fdt_gpio_prop(const struct fdt_scan_prop *prop,
                          void *extra)
{
  GpioScan_t *scan = (GpioScan_t *) extra;
  if (!strcmp(prop->name, "compatible") && 
      !strcmp((const char*)prop->value, "sifive,gpio0")) {
    scan->compat = 1;
  } else if (!strcmp(prop->name, "reg"))  {
    fdt_get_address(prop->node->parent, prop->value, &scan->reg);
  }
}

static void fdt_gpio_done(const struct fdt_scan_node *node,
                         void *extra)
{
  GpioScan_t *scan = (GpioScan_t *) extra;
  if (scan->compat && scan->reg && gpio_number <= GPIO_MAX) {
    gpio[gpio_number++] = (void *) scan->reg;
  }
}

void query_gpio(uintptr_t dtb)
{
  if (gpio_number > 0) 
    return;
  
  GpioScan_t scan;
  struct fdt_cb cb = {
    .open = fdt_gpio_open,
    .prop = fdt_gpio_prop,
    .done = fdt_gpio_done,
    .extra = &scan
  };
  
  fdt_scan(dtb, &cb);
}

uintptr_t gpio_get_address(int id)
{
  return (uintptr_t) (id < gpio_number ? gpio[id] : 0); 
}

void gpio_pin_setval(int id, gpio_pin_t pin, int value)
{
  if (value == GPIO_PIN_RESET) {
    gpio[id][GPIO_REG_OUTPUTVAL] &= (~pin);
  } else {
    gpio[id][GPIO_REG_OUTPUTVAL] |= pin;
  }
}

int gpio_pin_getval(int id, gpio_pin_t pin)
{
  if (gpio[id][GPIO_REG_INPUTVAL] | pin) {
    return GPIO_PIN_SET;
  } else {
    return GPIO_PIN_RESET;
  }
}

