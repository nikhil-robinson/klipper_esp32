// ESP32 Console - direct driver implementation (no VFS/stdin/stdout)
// Uses usb_serial_jtag_read/write_bytes() or uart_read/write_bytes() directly
// so that Klipper's binary framing is never mangled by line-ending translation.
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
// Based on Klipper Linux console.c by Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "autoconf.h"

#ifdef CONFIG_KLIPPER_CONSOLE_UART
#include "driver/uart.h"
#endif

#ifdef CONFIG_KLIPPER_CONSOLE_USB_CDC
#include "driver/usb_serial_jtag.h"
#include "hal/usb_serial_jtag_ll.h"
#endif

#include "esp_log.h"

#include "board/irq.h"
#include "board/misc.h"
#include "command.h"
#include "board/internal.h"
#include "board/serial_irq.h"
#include "sched.h"

#ifndef CONFIG_KLIPPER_CONSOLE_RX_BUFFER_SIZE
#define CONFIG_KLIPPER_CONSOLE_RX_BUFFER_SIZE 4096
#endif
#ifndef CONFIG_KLIPPER_UART_TX_BUFFER_SIZE
#define CONFIG_KLIPPER_UART_TX_BUFFER_SIZE 256
#endif

static const char *TAG = "console";

static volatile uint8_t console_active = 0;
static volatile uint8_t tx_pending = 0;

// Stubs required by internal.h / Linux-compat layer
void report_errno(char *where, int rc) { (void)where; (void)rc; }
int  set_non_blocking(int fd)          { (void)fd; return 0; }
int  set_close_on_exec(int fd)         { (void)fd; return 0; }

/****************************************************************
 * Low-level read / write wrappers
 ****************************************************************/

static int console_read(uint8_t *buf, int maxlen)
{
#ifdef CONFIG_KLIPPER_CONSOLE_USB_CDC
    // First try ring buffer driver
    int n = (int)usb_serial_jtag_read_bytes(buf, (size_t)maxlen, 0);
    if (n > 0) return n;
    // Fallback: poll hardware FIFO directly (in case ISR-driven driver missed bytes)
    if (usb_serial_jtag_ll_rxfifo_data_available()) {
        return (int)usb_serial_jtag_ll_read_rxfifo(buf, (uint32_t)maxlen);
    }
    return 0;
#elif defined(CONFIG_KLIPPER_CONSOLE_UART)
    return (int)uart_read_bytes(CONFIG_KLIPPER_UART_NUM, buf, (uint32_t)maxlen, 0);
#else
    (void)buf; (void)maxlen;
    return 0;
#endif
}

static int console_write(const uint8_t *buf, int len)
{
#ifdef CONFIG_KLIPPER_CONSOLE_USB_CDC
    // Try ring buffer driver first; if it returns 0 bytes, flush directly
    int n = (int)usb_serial_jtag_write_bytes(buf, (size_t)len, pdMS_TO_TICKS(10));
    if (n < len) {
        // Fallback: write directly to TX FIFO
        usb_serial_jtag_ll_write_txfifo(buf + n, (uint32_t)(len - n));
        usb_serial_jtag_ll_txfifo_flush();
    }
    return len;
#elif defined(CONFIG_KLIPPER_CONSOLE_UART)
    return (int)uart_write_bytes(CONFIG_KLIPPER_UART_NUM, buf, (uint32_t)len);
#else
    (void)buf; (void)len;
    return 0;
#endif
}

/****************************************************************
 * Setup
 ****************************************************************/

int console_setup(char *name)
{
    (void)name;

#ifdef CONFIG_KLIPPER_CONSOLE_USB_CDC
    ESP_LOGI(TAG, "Setting up USB CDC console (direct driver)");
    usb_serial_jtag_driver_config_t cfg = {
        .rx_buffer_size = CONFIG_KLIPPER_CONSOLE_RX_BUFFER_SIZE,
        .tx_buffer_size = CONFIG_KLIPPER_UART_TX_BUFFER_SIZE,
    };
    esp_err_t err = usb_serial_jtag_driver_install(&cfg);
    if (err == ESP_ERR_INVALID_STATE) {
        // Driver already installed by ESP-IDF console (CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG_ENABLED)
        ESP_LOGW(TAG, "USB JTAG driver already installed, reusing it");
    } else if (err != ESP_OK) {
        ESP_LOGE(TAG, "usb_serial_jtag_driver_install: %s",
                 esp_err_to_name(err));
        return -1;
    }

#elif defined(CONFIG_KLIPPER_CONSOLE_UART)
    ESP_LOGI(TAG, "Setting up UART console on UART%d baud %d",
             CONFIG_KLIPPER_UART_NUM, CONFIG_KLIPPER_UART_BAUD_RATE);
    uart_config_t uart_cfg = {
        .baud_rate  = CONFIG_KLIPPER_UART_BAUD_RATE,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    esp_err_t err = uart_driver_install(CONFIG_KLIPPER_UART_NUM,
                                        CONFIG_KLIPPER_CONSOLE_RX_BUFFER_SIZE,
                                        CONFIG_KLIPPER_UART_TX_BUFFER_SIZE,
                                        0, NULL, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "uart_driver_install: %s", esp_err_to_name(err));
        return -1;
    }
    uart_param_config(CONFIG_KLIPPER_UART_NUM, &uart_cfg);
    uart_set_pin(CONFIG_KLIPPER_UART_NUM,
                 CONFIG_KLIPPER_UART_TX_PIN, CONFIG_KLIPPER_UART_RX_PIN,
                 CONFIG_KLIPPER_UART_RTS_PIN, CONFIG_KLIPPER_UART_CTS_PIN);

#else
    ESP_LOGE(TAG, "No console interface configured");
    return -1;
#endif

    console_active = 1;
    tx_pending = 0;
    ESP_LOGI(TAG, "Console ready");
    return 0;
}

void
console_init(void)
{
    console_setup("serial");
}
DECL_INIT(console_init);

/****************************************************************
 * Console I/O task (board layer)
 ****************************************************************/

void serial_enable_tx_irq(void)
{
    tx_pending = 1;
    sched_wake_tasks();  // ensure console_io_task runs promptly to drain TX
}

void console_io_task(void)
{
    if (!console_active)
        return;

    // RX: poll and feed each byte into serial_irq framing
    uint8_t rxbuf[CONFIG_KLIPPER_CONSOLE_RX_BUFFER_SIZE];
    int n = console_read(rxbuf, sizeof(rxbuf));
    for (int i = 0; i < n; i++)
        serial_rx_byte(rxbuf[i]);

    // TX: drain bytes queued by serial_irq.c
    if (tx_pending) {
        tx_pending = 0;
        uint8_t txbuf[MESSAGE_MAX];
        int len = 0;
        uint8_t b;
        while (len < (int)sizeof(txbuf) && serial_get_tx_byte(&b) >= 0)
            txbuf[len++] = b;
        if (len > 0)
            console_write(txbuf, len);
        if (serial_get_tx_byte(&b) >= 0)
            tx_pending = 1;
    }
}
DECL_TASK(console_io_task);

void console_shutdown(void)
{
    // Keep console_active = 1 so that I/O continues during shutdown.
    // Klipper's host needs serial to remain functional to send recovery
    // commands (e.g. config_reset).  Only TX draining and RX polling are
    // performed here; the scheduler already blocks non-shutdown commands.
}
DECL_SHUTDOWN(console_shutdown);

void console_sleep(void)
{
    if (!console_active)
        vTaskDelay(pdMS_TO_TICKS(100));
}
