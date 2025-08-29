// Hardware PWM control
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.

#include "gpio.h"     // struct gpio_pwm
#include "internal.h" // NSECS_PER_TICK
#include "command.h"        // shutdown
#include "sched.h"          // sched_shutdown
#include <stdio.h>
#include <string.h>

#include "driver/ledc.h"
#include "esp_err.h"
#include "hal/ledc_types.h"

#define PWM_MAX (1 << 13)
DECL_CONSTANT("PWM_MAX", PWM_MAX);

uint8_t timer = 0;
uint8_t channel = 0;

ledc_timer_config_t ledc_timer = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                  .timer_num = 0,
                                  .duty_resolution = LEDC_TIMER_13_BIT,
                                  .freq_hz = 0, // Set output frequency at 4 kHz
                                  .clk_cfg = LEDC_AUTO_CLK};

struct gpio_pwm gpio_pwm_setup(uint32_t pin, uint32_t cycle_time,
                               uint16_t val) {
  // Prepare and then apply the LEDC PWM timer configuration

  struct gpio_pwm g = {};
  if (cycle_time != ledc_timer.freq_hz) {
    timer++;
    if (timer >= LEDC_TIMER_MAX) {
      goto fail;
    }
    ledc_timer.timer_num = timer;
    ledc_timer.freq_hz = cycle_time;

    ledc_timer_config(&ledc_timer);
  }

  channel++;
  if (channel >= LEDC_CHANNEL_MAX) {
    goto fail;
  }
  ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                        .channel = channel,
                                        .timer_sel = timer,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .gpio_num = pin,
                                        .duty = val,
                                        .hpoint = 0};
  int ret = ledc_channel_config(&ledc_channel);
  if (ret != ESP_OK) {
    goto fail;
  }

  g.channel = channel;

  return g;
fail:
  shutdown("Unable to config pwm device");
}

void gpio_pwm_write(struct gpio_pwm g, uint16_t val) {
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, g.channel, val));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, g.channel));
}
