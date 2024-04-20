// HW PWM upport via Linux PWM sysfs interface
//
// Copyright (C) 2019  Janne Grunau <janne-3d@jannau.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdio.h>
#include <string.h>
#include "board/gpio.h" // struct gpio_pwm
#include "internal.h" // NSECS_PER_TICK
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown

#include "driver/ledc.h"
#include "esp_err.h"
#include "soc/gpio_sig_map.h"


DECL_CONSTANT("PWM_MAX", LEDC_TIMER_13_BIT);




struct gpio_pwm gpio_pwm_setup(uint32_t pin, uint32_t cycle_time, uint16_t val)
{
    shutdown("Unable to config pwm device");
}


void gpio_pwm_write(struct gpio_pwm g, uint16_t val)
{
    
}
