
#include "autoconf.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "generic/irq.h"        // irq_save
#include "generic/serial_irq.h" // serial_rx_data
#include "sched.h"              // DECL_INIT
#include <stdint.h>             // uint32_t
#include <stdio.h>
#include <string.h>

static const char *TAG = "uart_events";

#define EX_UART_NUM UART_NUM_0
#define PATTERN_CHR_NUM                                                        \
  (3) /*!< Set the number of consecutive and identical characters received by  \
         receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
static QueueHandle_t uart0_queue;

void kick_tx() {
  size_t buffered_size = 0;
  uint8_t data;
  uint8_t tx_buffer[96];
  while (!serial_get_tx_byte(&data)) {
    tx_buffer[buffered_size] = data;
    buffered_size++;
  }
  if (buffered_size) {
    uart_write_bytes(EX_UART_NUM, (const char *)tx_buffer, buffered_size);
  }
}

static void uart_event_task(void *pvParameters) {
  uart_event_t event;

  uint8_t *dtmp = (uint8_t *)malloc(RD_BUF_SIZE);

  for (;;) {
    if (xQueueReceive(uart0_queue, (void *)&event, (TickType_t)portMAX_DELAY)) {
      bzero(dtmp, RD_BUF_SIZE);
      ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
      switch (event.type) {
      case UART_DATA:

        ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
        uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
        for (size_t i = 0; i < event.size; i++) {
          serial_rx_byte(dtmp[i]);
        }
        kick_tx();
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

void serial_init(void) {
  esp_log_level_set(TAG, ESP_LOG_INFO);

  /* Configure parameters of an UART driver,
   * communication pins and install the driver */
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
  uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Set uart pattern detect function.
  uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
  // Reset the pattern queue length to record at most 20 pattern positions.
  uart_pattern_queue_reset(EX_UART_NUM, 20);

  // Create a task to handler UART event from ISR
  xTaskCreate(uart_event_task, "uart_event_task", 1024 * 4, NULL, 12, NULL);
}

DECL_INIT(serial_init);

void serial_enable_tx_irq(void) { kick_tx(); }
