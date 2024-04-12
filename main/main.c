
#include <string.h>
#include "sched.h" // sched_main
#include "command.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"


void command_config_reset(uint32_t *args)
{
    if (! sched_is_shutdown())
        shutdown("config_reset only available when shutdown");
    esp_restart();
}
DECL_COMMAND_FLAGS(command_config_reset, HF_IN_SHUTDOWN, "config_reset");

void app_main(void)
{
    xTaskCreate(sched_main, "sched main", 1024 * 4, NULL, 12, NULL);
    sendf("This is a test");
}
