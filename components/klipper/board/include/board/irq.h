#ifndef __GENERIC_IRQ_H
#define __GENERIC_IRQ_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sched.h" // DECL_SHUTDOWN
typedef unsigned long irqstatus_t;

static inline void irq_disable(void) {}
static inline void irq_enable(void) {}
static inline irqstatus_t irq_save(void) { return 0; }

static inline void irq_restore(irqstatus_t flag) {}

static inline void irq_wait(void) {
  extern void console_kick();
  console_kick();
  vTaskDelay(1);
}
static inline void irq_poll(void) {}

static inline void clear_active_irq(void) {}
DECL_SHUTDOWN(clear_active_irq);

#endif // irq.h
