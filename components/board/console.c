
#include "autoconf.h"
#include "board/internal.h" // console_setup
#include "board/irq.h"      // irq_wait
#include "board/irq.h"      // irq_save
#include "board/misc.h"     // console_sendf
#include "command.h"        // command_find_block
#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sched.h"  // DECL_INIT
#include "sched.h"  // sched_wake_task
#include <stdint.h> // uint32_t
#include <stdio.h>
#include <stdio.h> // fprintf
#include <string.h>
#include <string.h>   // memmove
#include <sys/stat.h> // chmod
#include <time.h>     // struct timespec
#include <unistd.h>   // ttyname

static const char *TAG = "uart_events";

#define EX_UART_NUM UART_NUM_1

#define UART_RX_NUM 23
#define UART_TX_NUM 24

#define BUF_SIZE (4096)
static QueueHandle_t uart0_queue;

static struct task_wake console_wake;
static uint8_t receive_buf[BUF_SIZE];
static int receive_pos;

static volatile size_t recv_size = 0;

static void uart_event_task(void *pvParameters) {
  uart_event_t event;

  uint8_t *dtmp = (uint8_t *)malloc(BUF_SIZE);

  for (;;) {
    if (xQueueReceive(uart0_queue, (void *)&event, (TickType_t)portMAX_DELAY)) {
      bzero(dtmp, BUF_SIZE);
      ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
      switch (event.type) {
      case UART_DATA:
        sched_wake_task(&console_wake);
        break;
      default:
        ESP_LOGI(TAG, "uart event type: %d", event.type);
        break;
      }
    }
  }
  free(dtmp);
  dtmp = NULL;
  vTaskDelete(NULL);
}

void report_errno(char *where, int rc) {
  fprintf(stderr, "Got error %d in %s\n", rc, where);
}

int console_setup() {
  esp_log_level_set(TAG, ESP_LOG_INFO);

  uart_config_t uart_config = {
      .baud_rate = CONFIG_SERIAL_BAUD,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };
  // Install UART driver, and get the queue.
  uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue,
                      0);
  uart_param_config(EX_UART_NUM, &uart_config);

  // Set UART log level
  esp_log_level_set(TAG, ESP_LOG_INFO);
  // Set UART pins (using UART0 default pins ie no changes.)
  uart_set_pin(EX_UART_NUM, UART_TX_NUM, UART_RX_NUM, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
  uart_pattern_queue_reset(EX_UART_NUM, 20);

  // Create a task to handler UART event from ISR
  xTaskCreate(uart_event_task, "uart_event_task", 1024 * 4, NULL, 12, NULL);
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

  int ret = uart_read_bytes(EX_UART_NUM, &receive_buf[receive_pos],
                            sizeof(receive_buf) - receive_pos, portMAX_DELAY);
  if (ret == 15 && receive_buf[receive_pos + 14] == '\n' &&
      memcmp(&receive_buf[receive_pos], "FORCE_SHUTDOWN\n", 15) == 0)

  {
    shutdown("Force shutdown command");
  }

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

  int ret = uart_write_bytes(EX_UART_NUM, (const char *)buf, msglen);
  if (ret < 0)
    report_errno("write", ret);
}

// Sleep until a signal received (waking early for console input if needed)
void console_sleep(sigset_t *sigset) {}
