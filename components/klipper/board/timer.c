// esp32 timer support
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.

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

static gptimer_handle_t gptimer = NULL;

volatile bool dispatch_timer = false;

/****************************************************************
 * Low level timer code
 ****************************************************************/

// Return the current time (in absolute clock ticks).
uint32_t timer_read_time(void) {
  uint32_t count;
  ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &count));
  return count;
}

void timer_set(uint32_t next) {
  gptimer_alarm_config_t alarm_config = {
      .alarm_count = next, // period = 1s
  };
  ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
  ESP_ERROR_CHECK(gptimer_start(gptimer));
}

// Activate timer dispatch as soon as possible
void timer_kick(void) { timer_set(timer_read_time() + 50); }

/****************************************************************
 * Setup and irqs
 ****************************************************************/

static bool IRAM_ATTR example_timer_on_alarm_cb_v1(
    gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata,
    void *user_data) {

  BaseType_t high_task_awoken = pdFALSE;
  gptimer_stop(timer);
  dispatch_timer = true;
  return (high_task_awoken == pdTRUE);
}

void timer_init(void) {
  irq_disable();
  gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1000000, // 1MHz, 1 tick=1us
  };
  ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

  gptimer_event_callbacks_t cbs = {
      .on_alarm = example_timer_on_alarm_cb_v1,
  };
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

  ESP_ERROR_CHECK(gptimer_enable(gptimer));
  timer_kick();
  irq_enable();
}
DECL_INIT(timer_init);



void timer_dispatch()
{
  if(dispatch_timer)
  {
    uint32_t next = timer_dispatch_many();
    timer_set(next);
    dispatch_timer= false;
  }
}


void irq_disable(void) {


}
void irq_enable(void) {


}
irqstatus_t irq_save(void) { return 0; }

void irq_restore(irqstatus_t flag) {

  
}

void irq_wait(void) {
  extern void console_kick();
  console_kick();
  vTaskDelay(1);
}
void irq_poll(void) {
  timer_dispatch();
}

void clear_active_irq(void) {}
DECL_SHUTDOWN(clear_active_irq);