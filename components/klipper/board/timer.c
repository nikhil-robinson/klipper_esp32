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
#include "esp_task_wdt.h"
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
static TaskHandle_t klipper_task = NULL;

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

    // Signal that timer dispatch is needed.
    // Do NOT call gptimer_stop() here: stopping the timer freezes the hardware
    // counter, making timer_read_time() return stale values until the next
    // timer_set() call.  With auto_reload_on_alarm=false the alarm fires once
    // and will not re-trigger on its own.
    TimerInfo.must_dispatch = 1;

    // Wake the Klipper task so irq_wait() returns immediately instead of
    // waiting for the full sleep timeout.
    if (klipper_task)
        vTaskNotifyGiveFromISR(klipper_task, &high_task_awoken);

    return (high_task_awoken == pdTRUE);
}

void timer_init(void) {
    ESP_LOGI(TAG, "Initializing timer with frequency %d Hz", CONFIG_CLOCK_FREQ);

    // Capture the handle of the task that calls timer_init() (the Klipper main
    // task) so the ISR can send it a direct-to-task notification.
    klipper_task = xTaskGetCurrentTaskHandle();

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

// Global IRQ spinlock: used by irq_disable/enable/save/restore.
// irq_wait() temporarily releases this lock so the timer alarm callback
// (which runs at ISR level) can execute and set must_dispatch.
static portMUX_TYPE global_irq_spinlock = portMUX_INITIALIZER_UNLOCKED;

void irq_disable(void) {
    portENTER_CRITICAL(&global_irq_spinlock);
}

void irq_enable(void) {
    portEXIT_CRITICAL(&global_irq_spinlock);
}

irqstatus_t irq_save(void) {
    portENTER_CRITICAL(&global_irq_spinlock);
    return 1;
}

void irq_restore(irqstatus_t flag) {
    if (flag)
        portEXIT_CRITICAL(&global_irq_spinlock);
}

void irq_wait(void) {
    // irq_wait() is called from inside run_tasks() while the global_irq_spinlock
    // is held (between irq_disable()/irq_enable()).  We must release the spinlock
    // before blocking so that:
    //  a) the gptimer alarm callback (ISR level) can run and set must_dispatch
    //  b) FreeRTOS blocking calls work correctly
    portEXIT_CRITICAL(&global_irq_spinlock);

    // Dispatch any pending timers so their callbacks can call sched_wake_task()
    // and change tasks_status — otherwise the do-while idle loop in run_tasks()
    // would spin forever waiting for tasks_status to change while the very timer
    // that would change it is never dispatched.
    irq_poll();

    // Feed the hardware watchdog.  sched_main() never returns (run_tasks() is
    // an infinite loop), so this is the only reliable place to reset it.
    esp_task_wdt_reset();

    // Sleep until the timer ISR wakes us via task notification, or until the
    // timeout expires.  Using a notification instead of vTaskDelay() gives
    // sub-millisecond dispatch latency when an alarm fires.
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5));

    // Re-acquire the spinlock before returning; run_tasks() expects it held.
    portENTER_CRITICAL(&global_irq_spinlock);
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