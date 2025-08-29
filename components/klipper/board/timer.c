// ESP32 timer support - robust implementation based on Linux reference
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"        // CONFIG_CLOCK_FREQ  
#include "board/irq.h"       // irq_disable
#include "board/misc.h"      // timer_read_time
#include "board/timer_irq.h" // timer_dispatch_many
#include "command.h"         // DECL_SHUTDOWN
#include "driver/gptimer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sched.h" // DECL_INIT
#include <stdio.h>

// Timer resolution: 1MHz = 1µs per tick
#define TIMER_FREQ 1000000

// Default clock frequency if not defined in config
#ifndef CONFIG_CLOCK_FREQ
#define CONFIG_CLOCK_FREQ TIMER_FREQ
#endif

static const char* TAG = "timer";

static gptimer_handle_t gptimer = NULL;
static volatile uint32_t timer_high = 0;
static volatile uint32_t last_timer_read = 0;

// Timer state tracking
static struct {
    volatile uint8_t must_dispatch;
    uint32_t next_wake_time;
} TimerInfo;

/****************************************************************
 * Low level timer code
 ****************************************************************/

DECL_CONSTANT("CLOCK_FREQ", CONFIG_CLOCK_FREQ);

// Return the current time (in absolute clock ticks).
uint32_t timer_read_time(void) {
    uint32_t count_low, count_high_before, count_high_after;
    
    // Handle 32-bit overflow by tracking high bits
    do {
        count_high_before = timer_high;
        ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &count_low));
        count_high_after = timer_high;
    } while (count_high_before != count_high_after);
    
    uint32_t current_time = (count_high_before << 16) | (count_low & 0xFFFF);
    
    // Check for timer wrap-around and update high bits
    if (count_low < last_timer_read && count_low < 0x8000 && last_timer_read > 0x8000) {
        timer_high++;
        current_time = (timer_high << 16) | (count_low & 0xFFFF);
    }
    
    last_timer_read = count_low;
    return current_time;
}

// Set timer for next interrupt
void timer_set(uint32_t next) {
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = next,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = false
    };
    
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    
    // Only start if not already running
    esp_err_t err = gptimer_start(gptimer);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_ERROR_CHECK(err);
    }
    
    TimerInfo.next_wake_time = next;
}

// Activate timer dispatch as soon as possible
void timer_kick(void) { 
    uint32_t now = timer_read_time();
    timer_set(now + timer_from_us(50)); // 50µs from now
}

/****************************************************************
 * Setup and irqs
 ****************************************************************/

static bool IRAM_ATTR timer_irq_handler(
    gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata,
    void *user_data) {

    BaseType_t high_task_awoken = pdFALSE;
    
    // Stop the current timer
    gptimer_stop(timer);
    
    // Signal that timer dispatch is needed
    TimerInfo.must_dispatch = 1;
    
    return (high_task_awoken == pdTRUE);
}

void timer_init(void) {
    ESP_LOGI(TAG, "Initializing timer with frequency %d Hz", CONFIG_CLOCK_FREQ);
    
    irq_disable();
    
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = TIMER_FREQ, // 1MHz, 1 tick=1µs
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_irq_handler,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    
    // Initialize timer state
    TimerInfo.must_dispatch = 0;
    TimerInfo.next_wake_time = 0;
    timer_high = 0;
    last_timer_read = 0;
    
    timer_kick();
    irq_enable();
    
    ESP_LOGI(TAG, "Timer initialized successfully");
}
DECL_INIT(timer_init);

// Robust timer dispatch function based on Linux implementation
void timer_dispatch() {
    if (!TimerInfo.must_dispatch)
        return;
        
    TimerInfo.must_dispatch = 0;
    
    // Use the generic timer dispatch function
    uint32_t next = timer_dispatch_many();
    
    // Schedule next timer
    timer_set(next);
}

/****************************************************************
 * Interrupt wrappers - ESP32 specific implementations
 ****************************************************************/

void irq_disable(void) {
    // On FreeRTOS, we use critical sections instead of global IRQ disable
    // This is handled by the calling code using taskENTER_CRITICAL/taskEXIT_CRITICAL
}

void irq_enable(void) {
    // On FreeRTOS, we use critical sections instead of global IRQ enable
    // This is handled by the calling code using taskENTER_CRITICAL/taskEXIT_CRITICAL
}

irqstatus_t irq_save(void) { 
    // Return current interrupt state - not used in FreeRTOS context
    return 0; 
}

void irq_restore(irqstatus_t flag) {
    // Restore interrupt state - not used in FreeRTOS context
}

void irq_wait(void) {
    // Yield to allow other tasks to run and check for console activity
    extern void console_kick(void);
    console_kick();
    
    // Small delay to prevent busy waiting
    vTaskDelay(pdMS_TO_TICKS(1));
}

void irq_poll(void) { 
    timer_dispatch(); 
}

void clear_active_irq(void) {
    TimerInfo.must_dispatch = 0;
}
DECL_SHUTDOWN(clear_active_irq);