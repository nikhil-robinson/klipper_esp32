// Watchdog code on rp2040
//
// Copyright (C) 2021-2022  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "esp_system.h"
#include "esp_task_wdt.h"
#include "hal/wdt_hal.h"
#include "sched.h"  // DECL_TASK
#include <stdint.h> // uint32_t

static wdt_hal_context_t rtc_wdt_ctx;

void watchdog_reset(void) { wdt_hal_feed(&rtc_wdt_ctx); }
DECL_TASK(watchdog_reset);

void watchdog_init(void) { wdt_hal_init(&rtc_wdt_ctx, WDT_RWDT, 0, false); }
DECL_INIT(watchdog_init);
