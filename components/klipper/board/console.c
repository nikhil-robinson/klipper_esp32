
// UART based Console
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.

#include "board/internal.h" // console_setup
#include "board/irq.h"      // irq_wait
#include "board/misc.h"     // console_sendf
#include "command.h"        // command_find_block
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "sched.h" // sched_wake_task
#include "sdkconfig.h"
#include <errno.h>  // errno
#include <stdio.h>  // fprintf
#include <string.h> // memmove

// Report 'errno' in a message written to stderr
void report_errno(char *where, int rc) {
  int e = errno;
  printf("Got error %d in %s: (%d)%s\n", rc, where, e, strerror(e));
}

static struct task_wake console_wake;
static uint8_t receive_buf[4096];
static int receive_pos;

/****************************************************************
 * Setup
 ****************************************************************/

int console_setup(char *name) {
  uart_config_t uart_config = {
      .baud_rate = CONFIG_SERIAL_BAUD,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };
  int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
  intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

  ESP_ERROR_CHECK(uart_driver_install(0, sizeof(receive_buf) * 2, 0, 0, NULL,
                                      intr_alloc_flags));
  ESP_ERROR_CHECK(uart_param_config(0, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                               UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

  return 0;
}

/****************************************************************
 * Console handling
 ****************************************************************/

void *console_receive_buffer(void) { return receive_buf; }

// Process any incoming commands
void console_task(void) {
  if (!sched_check_wake(&console_wake))
    return;
  int ret = uart_read_bytes(0, &receive_buf[receive_pos],
                            sizeof(receive_buf) - receive_pos, 1);
  if (ret < 0) {
    if (errno == EWOULDBLOCK) {
      ret = 0;
    } else {
      report_errno("read", ret);
      return;
    }
  }
  if (ret == 15 && receive_buf[receive_pos + 14] == '\n' &&
      memcmp(&receive_buf[receive_pos], "FORCE_SHUTDOWN\n", 15) == 0)
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
void console_sendf(const struct command_encoder *ce, va_list args) {
  // Generate message
  uint8_t buf[MESSAGE_MAX];
  uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

  // Transmit message
  int ret = uart_write_bytes(0, buf, msglen);
  if (ret < 0)
    report_errno("write", ret);
}

void console_kick() { sched_wake_task(&console_wake); }
