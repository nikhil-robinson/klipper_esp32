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
static portMUX_TYPE timer_spinlock = portMUX_INITIALIZER_UNLOCKED;

// Timer state tracking
static struct {
    volatile uint8_t must_dispatch;
    uint32_t next_wake_time;
} TimerInfo;

// Return the current time (in absolute clock ticks).
// The hardware counter is 64-bit; we return the lower 32 bits which
// wrap every ~71 minutes.  timer_is_before() handles the wrap correctly.
uint32_t timer_read_time(void) {
    uint64_t count;
    gptimer_get_raw_count(gptimer, &count);
    return (uint32_t)count;
}

// Map a 32-bit Klipper alarm time to the 64-bit hardware counter value.
// Picks the earliest future 64-bit time whose low 32 bits equal 'next'.
static uint64_t alarm_count_for(uint32_t next) {
    uint64_t hw;
    gptimer_get_raw_count(gptimer, &hw);
    uint64_t base = hw & 0xFFFFFFFF00000000ULL;
    uint64_t candidate = base | (uint64_t)next;
    // If the candidate is already in the past, advance by one 32-bit period
    if (candidate <= hw)
        candidate += 0x100000000ULL;
    return candidate;
}

// Set timer for next interrupt
void timer_set(uint32_t next) {
    portENTER_CRITICAL(&timer_spinlock);

    gptimer_alarm_config_t alarm_config = {
        .alarm_count = alarm_count_for(next),
        .reload_count = 0,
        .flags.auto_reload_on_alarm = false
    };

    gptimer_set_alarm_action(gptimer, &alarm_config);

    esp_err_t err = gptimer_start(gptimer);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE)
        ESP_ERROR_CHECK(err);

    TimerInfo.next_wake_time = next;

    portEXIT_CRITICAL(&timer_spinlock);
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
    
    portENTER_CRITICAL(&timer_spinlock);
    
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
    
    portEXIT_CRITICAL(&timer_spinlock);
    
    timer_kick();
    
    ESP_LOGI(TAG, "Timer initialized successfully");
}
DECL_INIT(timer_init);

// Robust timer dispatch function based on Linux implementation
void timer_dispatch() {
    portENTER_CRITICAL(&timer_spinlock);
    
    if (!TimerInfo.must_dispatch) {
        portEXIT_CRITICAL(&timer_spinlock);
        return;
    }
        
    TimerInfo.must_dispatch = 0;
    
    portEXIT_CRITICAL(&timer_spinlock);
    
    // Use the generic timer dispatch function
    uint32_t next = timer_dispatch_many();
    
    // Schedule next timer
    timer_set(next);
}

/****************************************************************
 * Interrupt wrappers - ESP32 specific implementations
 ****************************************************************/

// Global IRQ disable using FreeRTOS critical sections
static portMUX_TYPE global_irq_spinlock = portMUX_INITIALIZER_UNLOCKED;

void irq_disable(void) {
    portENTER_CRITICAL(&global_irq_spinlock);
}

// Global IRQ enable using FreeRTOS critical sections  
void irq_enable(void) {
    portEXIT_CRITICAL(&global_irq_spinlock);
}

// Save current interrupt state
irqstatus_t irq_save(void) { 
    portENTER_CRITICAL(&global_irq_spinlock);
    return 1; // Indicate IRQs were disabled
}

// Restore interrupt state
void irq_restore(irqstatus_t flag) {
    if (flag) {
        portEXIT_CRITICAL(&global_irq_spinlock);
    }
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
DECL_TASK(irq_poll);

void clear_active_irq(void) {
    portENTER_CRITICAL(&timer_spinlock);
    TimerInfo.must_dispatch = 0;
    portEXIT_CRITICAL(&timer_spinlock);
}
DECL_SHUTDOWN(clear_active_irq);