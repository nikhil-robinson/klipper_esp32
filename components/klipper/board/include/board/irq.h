#ifndef __GENERIC_IRQ_H
#define __GENERIC_IRQ_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sched.h" // DECL_SHUTDOWN
typedef unsigned long irqstatus_t;

void irq_disable(void);
void irq_enable(void);
irqstatus_t irq_save(void);
void irq_restore(irqstatus_t flag);
void irq_wait(void);
void irq_poll(void);
void clear_active_irq(void);

#endif // irq.h
