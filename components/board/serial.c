// rp2040 serial
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h>           // uint32_t
#include "autoconf.h"         // CONFIG_SERIAL
#include "board/irq.h"        // irq_save
#include "board/serial_irq.h" // serial_rx_data
#include "board/internal.h"   // UART0_IRQn
#include "sched.h"            // DECL_INIT

#include <stdio.h>
#include "hal/uart_hal.h"
#include "soc/uart_periph.h"

static const char *TAG = "uart_events";

#define RD_BUF_SIZE (192)

#define UART_NUM 0

// Write tx bytes to the serial port

#define REPL_HAL_DEFN() { .dev = UART_LL_GET_HW(UART_NUM) }

#define RXFIFO_FULL_THR (SOC_UART_FIFO_LEN - 8)

// RXFIFO RX timeout threshold. This is in bit periods, so 10==one byte. Same as ESP-IDF UART driver.
#define RXFIFO_RX_TIMEOUT (10)

static void
kick_tx(void)
{
    size_t remaining = 0;
    uint8_t tx_buffer[96];
    uart_hal_context_t repl_hal = REPL_HAL_DEFN();
    uint32_t written = 0;

    for (;;)
    {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
        {
            break;
        }
        tx_buffer[remaining++] = data;
    }

    uart_hal_write_txfifo(&repl_hal, (const void *)tx_buffer, remaining, &written);
}

// all code executed in ISR must be in IRAM, and any const data must be in DRAM
static void IRAM_ATTR uart_irq_handler(void *arg)
{
    uint8_t rbuf[SOC_UART_FIFO_LEN];
    int len;
    uart_hal_context_t repl_hal = REPL_HAL_DEFN();
    uart_hal_clr_intsts_mask(&repl_hal, UART_INTR_RXFIFO_FULL | UART_INTR_RXFIFO_TOUT | UART_INTR_FRAM_ERR);
    len = uart_hal_get_rxfifo_len(&repl_hal);
    uart_hal_read_rxfifo(&repl_hal, rbuf, &len);
    for (int i = 0; i < len; i++)
    {
        serial_rx_byte(rbuf[i]);
    }
    kick_tx();
}

void serial_enable_tx_irq(void)
{
    irqstatus_t flag = irq_save();
    kick_tx();
    irq_restore(flag);
}

void serial_init(void)
{
    uart_hal_context_t repl_hal = REPL_HAL_DEFN();

    uart_hal_init(&repl_hal, UART_NUM); // Sets defaults: 8n1, no flow control
    uart_hal_set_baudrate(&repl_hal, CONFIG_SERIAL_BAUD, UART_SCLK_DEFAULT);
    uart_hal_rxfifo_rst(&repl_hal);
    uart_hal_txfifo_rst(&repl_hal);

    ESP_ERROR_CHECK(
        esp_intr_alloc(uart_periph_signal[UART_NUM].irq,
                       ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM,
                       uart_irq_handler,
                       NULL,
                       NULL));

    // Enable RX interrupts
    uart_hal_set_rxfifo_full_thr(&repl_hal, RXFIFO_FULL_THR);
    uart_hal_set_rx_timeout(&repl_hal, RXFIFO_RX_TIMEOUT);
    uart_hal_ena_intr_mask(&repl_hal, UART_INTR_RXFIFO_FULL | UART_INTR_RXFIFO_TOUT);
}
DECL_INIT(serial_init);
