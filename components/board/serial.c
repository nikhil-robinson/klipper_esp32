// rp2040 serial
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t

#include "autoconf.h"
#include "generic/irq.h"        // irq_save
#include "generic/serial_irq.h" // serial_rx_data
#include "sched.h"              // DECL_INIT

#include <stdio.h>
#include "driver/uart.h" // For uart_get_sclk_freq()
#include "hal/uart_hal.h"
#include "soc/uart_periph.h"
#include "esp_rom_gpio.h"

#define UART_CONSOLE 0
#define UART_HAL() { .dev = UART_LL_GET_HW(UART_CONSOLE) }
#define UARTx_IRQn UART0_IRQ_IRQn
// RXFIFO Full interrupt threshold. Set the same as the ESP-IDF UART driver
#define RXFIFO_FULL_THR (SOC_UART_FIFO_LEN - 8)

// RXFIFO RX timeout threshold. This is in bit periods, so 10==one byte. Same as ESP-IDF UART driver.
#define RXFIFO_RX_TIMEOUT (10)
#define GPIO_Rx 1
#define GPIO_Tx 0

// Write tx bytes to the serial port
static void kick_tx(void) {
  uart_hal_context_t repl_hal = UART_HAL();
  uart_dev_t * hw =UART_LL_GET_HW(UART_CONSOLE);
  for (;;) {
    uint8_t *data;
    int ret = serial_get_tx_byte(&data);
    if (ret) {
      // No more data to send - disable tx irq
      break;
    }
    hw->fifo.val = (int)data;
  }

}

static void  UARTx_IRQHandler(void *arg) {

    uint8_t rbuf[SOC_UART_FIFO_LEN];
    int len;
    uart_hal_context_t repl_hal = UART_HAL();

    uart_hal_clr_intsts_mask(&repl_hal, UART_INTR_RXFIFO_FULL | UART_INTR_RXFIFO_TOUT | UART_INTR_FRAM_ERR);

    len = uart_hal_get_rxfifo_len(&repl_hal);

    uart_hal_read_rxfifo(&repl_hal, rbuf, &len);

    for (int i = 0; i < len; i++) {
        serial_rx_byte(rbuf[i]);
    }

    kick_tx();
}

void serial_enable_tx_irq(void) {

    // irqstatus_t flag = irq_save();
    kick_tx();
    // irq_restore(flag);
}

void serial_init(void) {

  uart_hal_context_t uart_hal = UART_HAL();
  soc_module_clk_t sclk;

  uint32_t sclk_freq;
  uart_hal_get_sclk(&uart_hal,
                    &sclk); // To restore SCLK after uart_hal_init() resets it
  ESP_ERROR_CHECK(uart_get_sclk_freq(sclk, &sclk_freq));
  uart_hal_init(&uart_hal, UART_CONSOLE); // Sets defaults: 8n1, no flow control

  esp_rom_gpio_connect_out_signal(GPIO_Tx, UART_PERIPH_SIGNAL(UART_CONSOLE, SOC_UART_TX_PIN_IDX), 0, 0);
  esp_rom_gpio_connect_out_signal(GPIO_Rx, UART_PERIPH_SIGNAL(UART_CONSOLE, SOC_UART_RX_PIN_IDX), 0, 0);
  uart_hal_set_sclk(&uart_hal, sclk);
  uart_hal_set_baudrate(&uart_hal, CONFIG_SERIAL_BAUD, sclk_freq);
  uart_hal_rxfifo_rst(&uart_hal);
  uart_hal_txfifo_rst(&uart_hal);

  ESP_ERROR_CHECK(esp_intr_alloc(uart_periph_signal[UART_CONSOLE].irq,
                                 ESP_INTR_FLAG_LOWMED,
                                 UARTx_IRQHandler, NULL, NULL));
  uart_hal_set_rxfifo_full_thr(&uart_hal, RXFIFO_FULL_THR);
  uart_hal_set_rx_timeout(&uart_hal, RXFIFO_RX_TIMEOUT);
  uart_hal_ena_intr_mask(&uart_hal,
                         UART_INTR_RXFIFO_FULL | UART_INTR_RXFIFO_TOUT);
}
DECL_INIT(serial_init);
