// ESP32 Console Implementation with VFS support - Robust version
// Supports both UART and USB CDC based on configuration
//
// Based on Linux console.c from Klipper
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
// ESP32 adaptation: Copyright (C) 2024 Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "autoconf.h"  // CONFIG_* definitions
#include "config.h"

#ifdef CONFIG_CONSOLE_UART
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#endif

#ifdef CONFIG_CONSOLE_USB_CDC
#include "esp_vfs_usb_serial_jtag.h"
#include "driver/usb_serial_jtag.h"
#endif

#include "esp_vfs_console.h"
#include "esp_log.h"

#include "board/irq.h"
#include "board/misc.h"
#include "command.h"
#include "board/internal.h"
#include "sched.h"

static const char* TAG = "console";

// Buffer size constants
#ifndef CONFIG_CONSOLE_RX_BUFFER_SIZE
#define CONFIG_CONSOLE_RX_BUFFER_SIZE 256
#endif

#ifndef CONFIG_UART_TX_BUFFER_SIZE 
#define CONFIG_UART_TX_BUFFER_SIZE 256
#endif

// Console state
static struct task_wake console_wake;
static uint8_t receive_buf[CONFIG_CONSOLE_RX_BUFFER_SIZE];
static int receive_pos;
static volatile uint8_t console_active = 0;

/****************************************************************
 * Utility Functions
 ****************************************************************/

// Report 'errno' in a message written to stderr
void report_errno(char *where, int rc)
{
    int e = errno;
    fprintf(stderr, "Got error %d in %s: (%d) %s\n", rc, where, e, strerror(e));
}

// Set file descriptor to non-blocking mode
int set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        report_errno("fcntl getfl", flags);
        return -1;
    }
    int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (ret < 0) {
        report_errno("fcntl setfl", ret);
        return -1;
    }
    return 0;
}

// Set close-on-exec flag (no-op on ESP32, kept for API compatibility)
int set_close_on_exec(int fd) 
{ 
    (void)fd; 
    return 0; 
}

/****************************************************************
 * Console Setup
 ****************************************************************/

#ifdef CONFIG_CONSOLE_UART
static int setup_uart_console(void)
{
    ESP_LOGI(TAG, "Setting up UART console on UART%d (TX:%d, RX:%d, baud:%d)", 
             CONFIG_UART_NUM, CONFIG_UART_TX_PIN, CONFIG_UART_RX_PIN, CONFIG_UART_BAUD_RATE);

    // Configure UART
    uart_config_t uart_config = {
        .baud_rate = CONFIG_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    esp_err_t err;
    
    // Install UART driver
    err = uart_driver_install(CONFIG_UART_NUM, CONFIG_UART_RX_BUFFER_SIZE, 
                             CONFIG_UART_TX_BUFFER_SIZE, 0, NULL, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install UART driver: %s", esp_err_to_name(err));
        return -1;
    }

    // Configure UART parameters
    err = uart_param_config(CONFIG_UART_NUM, &uart_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure UART: %s", esp_err_to_name(err));
        return -1;
    }

    // Set UART pins
    err = uart_set_pin(CONFIG_UART_NUM, CONFIG_UART_TX_PIN, CONFIG_UART_RX_PIN, 
                       CONFIG_UART_RTS_PIN, CONFIG_UART_CTS_PIN);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set UART pins: %s", esp_err_to_name(err));
        return -1;
    }

    // Install VFS UART driver
    esp_vfs_dev_uart_use_driver(CONFIG_UART_NUM);
    
    return 0;
}
#endif

#ifdef CONFIG_CONSOLE_USB_CDC
static int setup_usb_cdc_console(void)
{
    ESP_LOGI(TAG, "Setting up USB CDC console");

    // Configure USB Serial JTAG
    usb_serial_jtag_driver_config_t usb_serial_jtag_config = {
        .rx_buffer_size = CONFIG_CONSOLE_RX_BUFFER_SIZE,
        .tx_buffer_size = CONFIG_UART_TX_BUFFER_SIZE,
    };

    esp_err_t err;
    
    // Install USB Serial JTAG driver
    err = usb_serial_jtag_driver_install(&usb_serial_jtag_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install USB Serial JTAG driver: %s", esp_err_to_name(err));
        return -1;
    }

    // Tell VFS to use USB Serial JTAG driver
    esp_vfs_usb_serial_jtag_use_driver();
    
    return 0;
}
#endif

int console_setup(char *name)
{
    (void)name; // Not used on ESP32

    int ret = -1;

#ifdef CONFIG_CONSOLE_UART
    ret = setup_uart_console();
#elif defined(CONFIG_CONSOLE_USB_CDC)
    ret = setup_usb_cdc_console();
#else
    ESP_LOGE(TAG, "No console interface configured");
    return -1;
#endif

    if (ret != 0) {
        return ret;
    }

    // Set stdin to non-blocking
    ret = set_non_blocking(STDIN_FILENO);
    if (ret != 0) {
        ESP_LOGE(TAG, "Failed to set stdin non-blocking");
        return ret;
    }

    // Initialize console state
    receive_pos = 0;
    console_active = 1;

    ESP_LOGI(TAG, "Console setup complete");
    return 0;
}

/****************************************************************
 * Console Operation
 ****************************************************************/

// Return pointer to receive buffer
void *console_receive_buffer(void)
{
    return receive_buf;
}

// Process incoming console data - robust implementation
void console_task(void)
{
    if (!sched_check_wake(&console_wake) || !console_active)
        return;

    // Read available data from console
    int ret = read(STDIN_FILENO, &receive_buf[receive_pos],
                   sizeof(receive_buf) - receive_pos);
    
    if (ret < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return; // No data available
        }
        report_errno("console read", ret);
        return;
    }
    
    if (ret == 0) {
        return; // No data read
    }

    // Check for force shutdown command
    if (ret == 15 && receive_pos + ret >= 15 && receive_buf[receive_pos + 14] == '\n' &&
        memcmp(&receive_buf[receive_pos], "FORCE_SHUTDOWN\n", 15) == 0) {
        shutdown("Force shutdown command");
        return;
    }

    // Process received data for commands
    int len = receive_pos + ret;
    uint_fast8_t pop_count;
    uint_fast8_t msglen = len > MESSAGE_MAX ? MESSAGE_MAX : len;
    
    ret = command_find_and_dispatch(receive_buf, msglen, &pop_count);
    if (ret) {
        len -= pop_count;
        if (len > 0) {
            memmove(receive_buf, &receive_buf[pop_count], len);
            sched_wake_task(&console_wake);
        }
    }
    
    // Bounds check for receive_pos
    if (len >= sizeof(receive_buf)) {
        // Buffer overflow protection - keep only the most recent data
        len = sizeof(receive_buf) - 1;
        ESP_LOGW(TAG, "Console buffer overflow, truncating");
    }
    
    receive_pos = len;
}
DECL_TASK(console_task);

// Encode and transmit a response message - with error handling
void console_sendf(const struct command_encoder *ce, va_list args)
{
    uint8_t buf[MESSAGE_MAX];
    uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

    if (msglen == 0 || msglen > MESSAGE_MAX) {
        ESP_LOGE(TAG, "Invalid message length: %d", msglen);
        return;
    }

    int ret = write(STDOUT_FILENO, buf, msglen);
    if (ret < 0) {
        report_errno("console write", ret);
    } else if (ret != msglen) {
        ESP_LOGW(TAG, "Partial write: expected %d, wrote %d", msglen, ret);
    }
}

// Console kick function - wake up console processing
void console_kick(void)
{
    if (console_active) {
        sched_wake_task(&console_wake);
    }
}

// Sleep until console input is available - improved version
void console_sleep(void)
{
    if (!console_active) {
        vTaskDelay(pdMS_TO_TICKS(100));
        return;
    }
    
    // On ESP32 with VFS, we don't need complex polling
    // Just yield to other tasks and let the console_task handle input
    vTaskDelay(pdMS_TO_TICKS(10)); // 10ms delay
    
    // Check if there might be data available by attempting a non-blocking read
    // This will trigger console_task to wake up if data is available
    sched_wake_task(&console_wake);
}

// Shutdown console cleanly
void console_shutdown(void)
{
    console_active = 0;
    ESP_LOGI(TAG, "Console shutdown");
}
DECL_SHUTDOWN(console_shutdown);
