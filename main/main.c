
#include <string.h>
#include "board/misc.h" // console_sendf
#include "command.h" // DECL_CONSTANT
#include "board/internal.h" // console_setup
#include "sched.h" // sched_main
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "board/gpio.h"
DECL_CONSTANT_STR("MCU", "linux");


void command_config_reset(uint32_t *args)
{
    if (! sched_is_shutdown())
        shutdown("config_reset only available when shutdown");
    esp_restart();
}
DECL_COMMAND_FLAGS(command_config_reset, HF_IN_SHUTDOWN, "config_reset");

void app_main(void)
{
    struct gpio_out g = gpio_out_setup(1, 1);

    while (true)
    {
        gpio_out_toggle(g);
        printf("%ld\n",g.state);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    // gpio_out_write(g,0);
    // uint32_t val = get_out_val(g);
    // printf("Val %ld", val);


    // sched_main();
}
