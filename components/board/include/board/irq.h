#ifndef __GENERIC_IRQ_H
#define __GENERIC_IRQ_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sched.h" // DECL_SHUTDOWN
typedef unsigned long irqstatus_t;

static inline void irq_disable(void) { vTaskDelay(1); }
static inline void irq_enable(void) { vTaskDelay(1); }
static inline irqstatus_t irq_save(void) { return 0; }

static inline void irq_restore(irqstatus_t flag) { vTaskDelay(1); }

static inline void irq_wait(void) { vTaskDelay(1); }
static inline void irq_poll(void) { vTaskDelay(1); }

static inline void clear_active_irq(void) { vTaskDelay(1); }
DECL_SHUTDOWN(clear_active_irq);

#endif // irq.h
