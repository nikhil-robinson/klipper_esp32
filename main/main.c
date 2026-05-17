
#include <string.h>
#include "misc.h" // console_sendf
#include "command.h" // DECL_CONSTANT
#include "internal.h" // console_setup
#include "sched.h" // sched_main
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

DECL_CONSTANT_STR("MCU", "esp32");


void command_config_reset(uint32_t *args)
{
    if (! sched_is_shutdown())
        shutdown("config_reset only available when shutdown");
    esp_restart();
}
DECL_COMMAND_FLAGS(command_config_reset, HF_IN_SHUTDOWN, "config_reset");

void main_task(void *pvparameters)
{
    // Subscribe this task to the watchdog timer (5 second timeout)
    esp_task_wdt_add(NULL);

    console_setup(NULL);
    for (;;)
    {
        sched_main();
        // Reset watchdog each scheduler loop
        esp_task_wdt_reset();
    }
    vTaskDelete(NULL);
    
}

void app_main(void)
{
    // 16KB stack: Klipper's scheduler + command dispatch need headroom
    xTaskCreatePinnedToCore(main_task, "main_task", 16384, NULL, 20, NULL, 0);
}
