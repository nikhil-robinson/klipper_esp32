
#include <string.h>
#include "sched.h" // sched_main
#include "command.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
void app_main(void)
{
    xTaskCreate(sched_main, "sched main", 1024 * 4, NULL, 12, NULL);
    sendf("This is a test");
}
