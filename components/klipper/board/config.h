#ifndef __ESP32_CONFIG_H
#define __ESP32_CONFIG_H

/*
 * ESP32 Klipper Configuration
 * 
 * This file contains user-configurable options for the ESP32 Klipper port.
 * 
 * CONSOLE CONFIGURATION:
 * ======================
 * 
 * You can choose between UART and USB CDC console interfaces:
 * 
 * 1. UART Console (recommended for most users):
 *    - Uncomment CONFIG_CONSOLE_UART
 *    - Configure UART pins and settings below
 *    - Works on all ESP32 variants
 * 
 * 2. USB CDC Console:
 *    - Uncomment CONFIG_CONSOLE_USB_CDC  
 *    - Only works on ESP32-S2, ESP32-S3, ESP32-C3, and newer chips with USB support
 *    - Appears as a USB serial device when connected to PC
 * 
 * 3. Auto-detection (default):
 *    - If neither is defined, automatically chooses USB CDC if available, otherwise UART
 * 
 * UART CONFIGURATION:
 * ===================
 * 
 * When using UART console, configure these settings:
 * - CONFIG_UART_NUM: Which UART peripheral to use (UART_NUM_0, UART_NUM_1, UART_NUM_2)
 * - CONFIG_UART_TX_PIN: GPIO pin for TX (1 is default for UART0)
 * - CONFIG_UART_RX_PIN: GPIO pin for RX (3 is default for UART0)  
 * - CONFIG_UART_BAUD_RATE: Communication speed (115200 is recommended)
 * 
 * Common ESP32 UART pin configurations:
 * - UART0: TX=1, RX=3 (default, shared with USB programming)
 * - UART1: TX=10, RX=9 (often used for debugging)
 * - UART2: TX=17, RX=16 (recommended for external communication)
 */

// Console Configuration
// Define which console interface to use
// Comment out both to enable auto-detection, or define one of:
#define CONFIG_CONSOLE_UART     1
// #define CONFIG_CONSOLE_USB_CDC  1

// UART Console Configuration (only used if CONFIG_CONSOLE_UART is defined)
#define CONFIG_UART_NUM         UART_NUM_0
#define CONFIG_UART_BAUD_RATE   115200
#define CONFIG_UART_TX_PIN      1     // GPIO 1 (default TX)
#define CONFIG_UART_RX_PIN      3     // GPIO 3 (default RX)
#define CONFIG_UART_RTS_PIN     UART_PIN_NO_CHANGE
#define CONFIG_UART_CTS_PIN     UART_PIN_NO_CHANGE

// UART Buffer sizes
#define CONFIG_UART_TX_BUFFER_SIZE  256
#define CONFIG_UART_RX_BUFFER_SIZE  1024

// Console receive buffer size
#define CONFIG_CONSOLE_RX_BUFFER_SIZE  4096

// USB CDC Configuration (only used if CONFIG_CONSOLE_USB_CDC is defined)
#if defined(CONFIG_CONSOLE_USB_CDC) && !defined(CONFIG_SOC_USB_SERIAL_JTAG_SUPPORTED)
#error "USB CDC console requires SOC with USB support. Use UART console instead."
#endif

// Auto-detect USB CDC availability
#ifdef CONFIG_SOC_USB_SERIAL_JTAG_SUPPORTED
#define CONFIG_USB_CDC_AVAILABLE 1
#else
#define CONFIG_USB_CDC_AVAILABLE 0
#endif

// Validation: Only one console type should be defined
#if defined(CONFIG_CONSOLE_UART) && defined(CONFIG_CONSOLE_USB_CDC)
#error "Only one console type can be defined: CONFIG_CONSOLE_UART or CONFIG_CONSOLE_USB_CDC"
#endif

// Default to UART if no console is specified and USB CDC is not available
#if !defined(CONFIG_CONSOLE_UART) && !defined(CONFIG_CONSOLE_USB_CDC)
#if CONFIG_USB_CDC_AVAILABLE
#define CONFIG_CONSOLE_USB_CDC  1
#else
#define CONFIG_CONSOLE_UART     1
#endif
#endif

#endif // __ESP32_CONFIG_H
