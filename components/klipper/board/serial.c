// TTY based IO
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#define _GNU_SOURCE
#include <errno.h> // errno
#include <stdio.h> // fprintf
#include <string.h> // memmove
#include "board/irq.h" // irq_wait
#include "board/misc.h" // console_sendf
#include "command.h" // command_find_block
#include "board/internal.h" // console_setup
#include "sched.h" // sched_wake_task
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define ECHO_TEST_TXD 24//(47)
#define ECHO_TEST_RXD 23//(48)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      (1)
#define ECHO_UART_BAUD_RATE     (CONFIG_SERIAL_BAUD)
#define ECHO_TASK_STACK_SIZE    (CONFIG_EXAMPLE_TASK_STACK_SIZE)

// Report 'errno' in a message written to stderr
void
report_errno(char *where, int rc)
{
    int e = errno;
    printf("Got error %d in %s: (%d)%s\n", rc, where, e, strerror(e));
}

static struct task_wake console_wake;
static uint8_t receive_buf[4096];
static int receive_pos;

/****************************************************************
 * Setup
 ****************************************************************/

int
console_setup(char *name)
{
    uart_config_t uart_config = {
        .baud_rate = CONFIG_SERIAL_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, sizeof(receive_buf) * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));


    return 0;
}


/****************************************************************
 * Console handling
 ****************************************************************/



void *
console_receive_buffer(void)
{
    return receive_buf;
}

// Process any incoming commands
void
console_task(void)
{
    if (!sched_check_wake(&console_wake))
        return;

    // Read data
    // int ret = read(main_pfd[MP_TTY_IDX].fd, &receive_buf[receive_pos]
    //                , sizeof(receive_buf) - receive_pos);

    int ret = uart_read_bytes(ECHO_UART_PORT_NUM, &receive_buf[receive_pos], sizeof(receive_buf) - receive_pos, 1);
    // if (ret)
    // {
    //     printf("%s\n",receive_buf);
    //     ESP_LOG_BUFFER_HEX("USB",receive_buf,ret);
    // }
    
    if (ret < 0) {
        if (errno == EWOULDBLOCK) {
            ret = 0;
        } else {
            report_errno("read", ret);
            return;
        }
    }
    if (ret == 15 && receive_buf[receive_pos+14] == '\n'
        && memcmp(&receive_buf[receive_pos], "FORCE_SHUTDOWN\n", 15) == 0)
        shutdown("Force shutdown command");

    // Find and dispatch message blocks in the input
    int len = receive_pos + ret;
    uint_fast8_t pop_count, msglen = len > MESSAGE_MAX ? MESSAGE_MAX : len;
    ret = command_find_and_dispatch(receive_buf, msglen, &pop_count);
    if (ret) {
        len -= pop_count;
        if (len) {
            memmove(receive_buf, &receive_buf[pop_count], len);
            sched_wake_task(&console_wake);
        }
    }
    receive_pos = len;
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Generate message
    uint8_t buf[MESSAGE_MAX];
    uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

    // Transmit message
    // int ret = write(main_pfd[MP_TTY_IDX].fd, buf, msglen);
    uart_write_bytes(ECHO_UART_PORT_NUM, buf, msglen);
    // if (ret < 0)
    //     report_errno("write", ret);
}


void console_kick()
{
    sched_wake_task(&console_wake);
}
