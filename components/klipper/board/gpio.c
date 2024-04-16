// GPIO functions on rp2040
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // gpio_out_setup
#include "board/irq.h"  // irq_save
#include "command.h"    // shutdown
#include "sched.h"      // sched_shutdown
#include <string.h>     // ffs

/****************************************************************
 * Pin mappings
 ****************************************************************/

// DECL_ENUMERATION_RANGE("pin", "GPIO_NUM_", 0, 30);
DECL_ENUMERATION_RANGE("pin", "P0", 0, 30);
DECL_ENUMERATION_RANGE("pin", "P1", 1, 30);
DECL_ENUMERATION_RANGE("pin", "P2", 2, 30);
DECL_ENUMERATION_RANGE("pin", "P3", 3, 30);
DECL_ENUMERATION_RANGE("pin", "P4", 4, 30);
DECL_ENUMERATION_RANGE("pin", "P5", 5, 30);
DECL_ENUMERATION_RANGE("pin", "P6", 6, 30);
DECL_ENUMERATION_RANGE("pin", "P7", 7, 30);
DECL_ENUMERATION_RANGE("pin", "P8", 8, 30);
DECL_ENUMERATION_RANGE("pin", "P9", 9, 30);
DECL_ENUMERATION_RANGE("pin", "P10", 10, 30);
DECL_ENUMERATION_RANGE("pin", "P11",11, 30);
DECL_ENUMERATION_RANGE("pin", "P12", 12, 30);
DECL_ENUMERATION_RANGE("pin", "P13", 13, 30);
DECL_ENUMERATION_RANGE("pin", "P14", 14, 30);
DECL_ENUMERATION_RANGE("pin", "P15", 15, 30);
DECL_ENUMERATION_RANGE("pin", "P16", 16, 30);
DECL_ENUMERATION_RANGE("pin", "P17", 17, 30);
DECL_ENUMERATION_RANGE("pin", "P18", 18, 30);
DECL_ENUMERATION_RANGE("pin", "P19", 19, 30);
DECL_ENUMERATION_RANGE("pin", "P20", 20, 30);
DECL_ENUMERATION_RANGE("pin", "P21", 21, 30);
DECL_ENUMERATION_RANGE("pin", "P22", 22, 30);
DECL_ENUMERATION_RANGE("pin", "P23", 23, 30);
DECL_ENUMERATION_RANGE("pin", "P24", 24, 30);


// // Set the mode and extended function of a pin
// void
// gpio_peripheral(uint32_t gpio, int func, int pull_up)
// {
//     // padsbank0_hw->io[gpio] = (
//     //     PADS_BANK0_GPIO0_IE_BITS
//     //     | (PADS_BANK0_GPIO0_DRIVE_VALUE_4MA << PADS_BANK0_GPIO0_DRIVE_MSB)
//     //     | (pull_up > 0 ? PADS_BANK0_GPIO0_PUE_BITS : 0)
//     //     | (pull_up < 0 ? PADS_BANK0_GPIO0_PDE_BITS : 0));
//     // iobank0_hw->io[gpio].ctrl = func << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
// }

// // Convert a register and bit location back to an integer pin identifier
// static int
// mask_to_pin(uint32_t mask)
// {
//     // return ffs(mask)-1;
//     return 0;
// }

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out gpio_out_setup(uint8_t pin, uint8_t val) {
  struct gpio_out g = {.pin = pin, .val = val};
  esp_rom_gpio_pad_select_gpio(pin);
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);

  gpio_out_reset(g, val);

  return g;
}

void gpio_out_reset(struct gpio_out g, uint8_t val) {
  g.val = val;
  gpio_out_write(g, val);
  gpio_reset_pin(g.pin);
  esp_rom_gpio_pad_select_gpio(g.pin);
  gpio_set_direction(g.pin, GPIO_MODE_OUTPUT);
}

void gpio_out_toggle_noirq(struct gpio_out g) {
  g.val = !g.val;
  gpio_set_level(g.pin, g.val);
  // sio_hw->gpio_togl = g.bit;
}

void gpio_out_toggle(struct gpio_out g) { gpio_out_toggle_noirq(g); }

void gpio_out_write(struct gpio_out g, uint8_t val) {
  g.val = val;
  gpio_set_level(g.pin, g.val);
}

struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up) {
  struct gpio_in g = {.pin = pin};
  gpio_in_reset(g, pull_up);
  return g;
}

void gpio_in_reset(struct gpio_in g, int8_t pull_up) {
  gpio_reset_pin(g.pin);
  esp_rom_gpio_pad_select_gpio(g.pin);
  gpio_set_direction(g.pin, GPIO_MODE_INPUT);
  if (pull_up > 0) {
    gpio_pullup_en(g.pin);
  } else if (pull_up < 0) {
    gpio_pulldown_en(g.pin);
  }
}

uint8_t gpio_in_read(struct gpio_in g) { return gpio_get_level(g.pin); }
