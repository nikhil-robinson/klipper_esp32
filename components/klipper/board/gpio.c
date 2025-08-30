// GPIO functions on esp32
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.

#include "gpio.h" // gpio_out_setup
#include "irq.h"  // irq_save
#include "command.h"    // shutdown
#include "esp_log.h"
#include "hal/gpio_ll.h"
#include "sched.h" // sched_shutdown
#include "soc/gpio_num.h"
#include "soc/gpio_struct.h"
#include <string.h> // ffs, memset
#define TAG "KLIPPER_GPIO"

/****************************************************************
 * Pin mappings
 ****************************************************************/

DECL_ENUMERATION_RANGE("pin", "GPIO_NUM_0", 0, GPIO_NUM_MAX-1);

gpio_dev_t *hw = &GPIO;

/****************************************************************
 * General Purpose Input Output (GPIO) pins
 ****************************************************************/

struct gpio_out gpio_out_setup(uint32_t gpio_num, uint32_t val) {
  
  if (gpio_num >= GPIO_NUM_MAX) {
    shutdown("Invalid GPIO pin");
  }
  
  static struct gpio_line gpio_lines[GPIO_NUM_MAX];
  static int initialized = 0;
  
  if (!initialized) {
    memset(gpio_lines, 0, sizeof(gpio_lines));
    initialized = 1;
  }
  
  struct gpio_line *line = &gpio_lines[gpio_num];
  line->pin = gpio_num;
  line->state = !!val;
  struct gpio_out g = {.line = line};
  gpio_ll_output_enable(hw, gpio_num);
  gpio_out_write(g, val);
  return g;
}

void gpio_out_reset(struct gpio_out g, uint32_t val) {
  gpio_out_setup(g.line->pin, val);
}

void gpio_out_toggle_noirq(struct gpio_out g) {

  gpio_out_write(g, !g.line->state);
}

void gpio_out_toggle(struct gpio_out g) { gpio_out_toggle_noirq(g); }

void gpio_out_write(struct gpio_out g, uint32_t val) {
  gpio_ll_set_level(hw, g.line->pin, val);
  g.line->state = !!val;
}

struct gpio_in gpio_in_setup(uint8_t pin, int8_t pull_up) {
  if (pin >= GPIO_NUM_MAX) {
    shutdown("Invalid GPIO pin");
  }
  
  struct gpio_in g = {.pin = pin};
  gpio_ll_input_enable(hw, pin);
  if (pull_up > 0) {
    gpio_ll_pullup_en(hw, pin);
    gpio_ll_pulldown_dis(hw, pin);
  } else if (pull_up < 0) {
    gpio_ll_pullup_dis(hw, pin);
    gpio_ll_pulldown_en(hw, pin);
  } else {
    gpio_ll_pullup_dis(hw, pin);
    gpio_ll_pulldown_dis(hw, pin);
  }

  return g;
}

void gpio_in_reset(struct gpio_in g, int8_t pull_up) {
  gpio_in_setup(g.pin, pull_up);
}

uint8_t gpio_in_read(struct gpio_in g) { return gpio_ll_get_level(hw, g.pin); }
