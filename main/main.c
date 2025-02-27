
#include <string.h>
#include "board/misc.h" // console_sendf
#include "command.h" // DECL_CONSTANT
#include "board/internal.h" // console_setup
#include "sched.h" // sched_main
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

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
    console_setup();
    for (;;)
    {
        sched_main();
    }
    vTaskDelete(NULL);
    
}

void app_main(void)
{
    xTaskCreatePinnedToCore(main_task, "main_task", 4096, NULL, 20, NULL,
                              0);
}
