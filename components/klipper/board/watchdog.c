// Watchdog code on rp2040
//
// Copyright (C) 2021-2022  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include "sched.h" // DECL_TASK

void
watchdog_reset(void)
{
    // watchdog_hw->load = 0x800000; // ~350ms
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{

}
DECL_INIT(watchdog_init);
