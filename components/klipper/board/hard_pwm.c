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
#include "board/irq.h"      // irq_save/restore
#include <stdio.h>
#include <string.h>

#include "driver/ledc.h"
#include "esp_err.h"
#include "hal/ledc_types.h"

#define PWM_MAX (1 << 13)
DECL_CONSTANT("PWM_MAX", PWM_MAX);

// Resource tracking with thread safety
static struct {
    uint8_t next_timer;
    uint8_t next_channel; 
    uint32_t timer_frequencies[LEDC_TIMER_MAX];
    uint8_t timer_used[LEDC_TIMER_MAX];
    uint8_t channel_used[LEDC_CHANNEL_MAX];
    uint8_t initialized;
} pwm_state = {0};

// Initialize PWM subsystem (called once)
static void pwm_init(void) {
    if (pwm_state.initialized)
        return;
        
    // Clear all state
    memset(&pwm_state, 0, sizeof(pwm_state));
    pwm_state.initialized = 1;
}

// Find or allocate a timer for the given frequency
static int find_or_allocate_timer(uint32_t freq_hz) {
    // First check if we already have a timer with this frequency
    for (int i = 0; i < LEDC_TIMER_MAX; i++) {
        if (pwm_state.timer_used[i] && pwm_state.timer_frequencies[i] == freq_hz) {
            return i;
        }
    }
    
    // Find first unused timer
    for (int i = 0; i < LEDC_TIMER_MAX; i++) {
        if (!pwm_state.timer_used[i]) {
            pwm_state.timer_used[i] = 1;
            pwm_state.timer_frequencies[i] = freq_hz;
            return i;
        }
    }
    
    return -1; // No available timers
}

// Allocate a channel
static int allocate_channel(void) {
    for (int i = 0; i < LEDC_CHANNEL_MAX; i++) {
        if (!pwm_state.channel_used[i]) {
            pwm_state.channel_used[i] = 1;
            return i;
        }
    }
    return -1; // No available channels
}

struct gpio_pwm gpio_pwm_setup(uint32_t pin, uint32_t cycle_time, uint16_t val) {
    struct gpio_pwm g = {.channel = LEDC_CHANNEL_MAX}; // Invalid channel as default
    
    // Initialize PWM subsystem if needed
    pwm_init();
    
    // Validate parameters
    if (pin >= GPIO_NUM_MAX) {
        shutdown("Invalid PWM pin");
    }
    
    if (val > PWM_MAX) {
        shutdown("Invalid PWM value");
    }
    
    // Convert cycle_time to frequency (assuming cycle_time is in Hz)
    uint32_t freq_hz = cycle_time;
    if (freq_hz == 0 || freq_hz > 40000000) { // ESP32 LEDC max ~40MHz
        shutdown("Invalid PWM frequency");
    }

    irqstatus_t flag = irq_save();
    
    // Find or allocate timer
    int timer_num = find_or_allocate_timer(freq_hz);
    if (timer_num < 0) {
        irq_restore(flag);
        shutdown("No available PWM timers");
    }
    
    // Allocate channel
    int channel_num = allocate_channel();
    if (channel_num < 0) {
        irq_restore(flag);
        shutdown("No available PWM channels");
    }
    
    irq_restore(flag);
    
    // Configure timer if it's new
    if (pwm_state.timer_frequencies[timer_num] == freq_hz) {
        ledc_timer_config_t timer_config = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_num = timer_num,
            .duty_resolution = LEDC_TIMER_13_BIT,
            .freq_hz = freq_hz,
            .clk_cfg = LEDC_AUTO_CLK
        };
        
        esp_err_t ret = ledc_timer_config(&timer_config);
        if (ret != ESP_OK) {
            shutdown("Failed to configure PWM timer");
        }
    }
    
    // Configure channel
    ledc_channel_config_t channel_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = channel_num,
        .timer_sel = timer_num,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = pin,
        .duty = val,
        .hpoint = 0
    };
    
    esp_err_t ret = ledc_channel_config(&channel_config);
    if (ret != ESP_OK) {
        // Free allocated channel on failure
        irqstatus_t flag = irq_save();
        pwm_state.channel_used[channel_num] = 0;
        irq_restore(flag);
        shutdown("Failed to configure PWM channel");
    }

    g.channel = channel_num;
    return g;
}

void gpio_pwm_write(struct gpio_pwm g, uint16_t val) {
    // Validate channel
    if (g.channel >= LEDC_CHANNEL_MAX || !pwm_state.channel_used[g.channel]) {
        shutdown("Invalid PWM channel");
    }
    
    // Validate duty value
    if (val > PWM_MAX) {
        shutdown("PWM value out of range");
    }
    
    esp_err_t ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, g.channel, val);
    if (ret != ESP_OK) {
        shutdown("Failed to set PWM duty");
    }
    
    ret = ledc_update_duty(LEDC_LOW_SPEED_MODE, g.channel);
    if (ret != ESP_OK) {
        shutdown("Failed to update PWM duty");
    }
}
