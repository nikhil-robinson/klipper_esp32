#ifndef __GENERIC_IRQ_H
#define __GENERIC_IRQ_H

typedef unsigned long irqstatus_t;

static inline void irq_disable(void)
{

}
static inline void irq_enable(void)
{

}
static inline irqstatus_t irq_save(void)
{
    return 0;
}

static inline void irq_restore(irqstatus_t flag)
{
}

static inline void irq_wait(void)
{

}
static inline void irq_poll(void)
{
    
}

#endif // irq.h
