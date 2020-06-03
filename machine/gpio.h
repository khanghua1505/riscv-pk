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

#ifndef _RISCV_GPIO_H_
#define _RISCV_GPIO_H_

#include <stdint.h>

enum {
    GPIO0
};

#define GPIO_REG_INPUTVAL       (0x0)       /* Pin value */
#define GPIO_REG_INPUTEN        (0x1)       /* Pin input enable */
#define GPIO_REG_OUTPUTEN       (0x2)       /* Pin output enable */
#define GPIO_REG_OUTPUTVAL      (0x3)       /* Pin output val */
#define GPIO_REG_PUE            (0x4)       /* Interrupt pull-up enable */
#define GPIO_REG_DS             (0x5)       /* Pin drive strength */
#define GPIO_REG_RISE_IE        (0x6)       /* Rise interrupt enable */
#define GPIO_REG_RISE_IP        (0x7)       /* Rise interrupt pending */
#define GPIO_REG_FALL_IE        (0x8)       /* Fall interrupt enable */
#define GPIO_REG_FALL_IP        (0x9)       /* Fall interrupt enable */
#define GPIO_REG_HIGH_IE        (0xA)       /* High interrupt enable */
#define GPIO_REG_HIGH_IP        (0xB)       /* High interrupt pending */
#define GPIO_REG_LOW_IE         (0xC)       /* Low interrupt enable */
#define GPIO_REG_LOW_IP         (0xD)       /* Low interrupt pending */
#define GPIO_REG_OUT_XOR        (0xE)       /* Output XOR (invert) */

#define GPIO_PIN_0    ((uint32_t) (1 << 0))
#define GPIO_PIN_1    ((uint32_t) (1 << 1))
#define GPIO_PIN_2    ((uint32_t) (1 << 2))
#define GPIO_PIN_3    ((uint32_t) (1 << 3))
#define GPIO_PIN_4    ((uint32_t) (1 << 4))
#define GPIO_PIN_5    ((uint32_t) (1 << 5))
#define GPIO_PIN_6    ((uint32_t) (1 << 6))
#define GPIO_PIN_7    ((uint32_t) (1 << 7))
#define GPIO_PIN_8    ((uint32_t) (1 << 8))
#define GPIO_PIN_9    ((uint32_t) (1 << 9))
#define GPIO_PIN_10   ((uint32_t) (1 << 10))
#define GPIO_PIN_11   ((uint32_t) (1 << 11))
#define GPIO_PIN_12   ((uint32_t) (1 << 12))
#define GPIO_PIN_13   ((uint32_t) (1 << 13))
#define GPIO_PIN_14   ((uint32_t) (1 << 14))
#define GPIO_PIN_15   ((uint32_t) (1 << 15))

typedef enum {
  GPIO_MODE_INPUT,
  GPIO_MODE_OUTPUT,
}
gpio_mode_t;

enum {
  GPIO_PIN_RESET = 0x00,
  GPIO_PIN_SET = 0x01,
};

typedef uint32_t gpio_pin_t;

void query_gpio(uintptr_t dtb);
uintptr_t gpio_get_address(int id);
int gpio_get_number(void);
void gpio_pin_mode(int gpio_id, gpio_mode_t mode);
void gpio_pin_setval(int gpio_id, gpio_pin_t pin, int value);
int gpio_pin_getval(int id, gpio_pin_t pin);

#endif  // _RISCV_GPIO_H_
