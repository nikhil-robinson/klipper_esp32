#ifndef __AUTOCONF_H
#define __AUTOCONF_H

// Include ESP-IDF configuration
#include "sdkconfig.h"

// Ensure CONFIG_CLOCK_FREQ is defined with fallback
#ifndef CONFIG_CLOCK_FREQ
#ifdef CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ
#define CONFIG_CLOCK_FREQ (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ * 1000000)
#elif defined(CONFIG_ESP32S2_DEFAULT_CPU_FREQ_MHZ)
#define CONFIG_CLOCK_FREQ (CONFIG_ESP32S2_DEFAULT_CPU_FREQ_MHZ * 1000000)
#elif defined(CONFIG_ESP32S3_DEFAULT_CPU_FREQ_MHZ)
#define CONFIG_CLOCK_FREQ (CONFIG_ESP32S3_DEFAULT_CPU_FREQ_MHZ * 1000000)
#elif defined(CONFIG_ESP32C3_DEFAULT_CPU_FREQ_MHZ)
#define CONFIG_CLOCK_FREQ (CONFIG_ESP32C3_DEFAULT_CPU_FREQ_MHZ * 1000000)
#elif defined(CONFIG_ESP32C2_DEFAULT_CPU_FREQ_MHZ)
#define CONFIG_CLOCK_FREQ (CONFIG_ESP32C2_DEFAULT_CPU_FREQ_MHZ * 1000000)
#elif defined(CONFIG_ESP32C6_DEFAULT_CPU_FREQ_MHZ)
#define CONFIG_CLOCK_FREQ (CONFIG_ESP32C6_DEFAULT_CPU_FREQ_MHZ * 1000000)
#elif defined(CONFIG_ESP32H2_DEFAULT_CPU_FREQ_MHZ)
#define CONFIG_CLOCK_FREQ (CONFIG_ESP32H2_DEFAULT_CPU_FREQ_MHZ * 1000000)
#else
// Fallback to 1MHz timer frequency (common for Klipper ESP32)
#define CONFIG_CLOCK_FREQ 1000000
#endif
#endif

// Compatibility macros for boolean configs that might not be defined
#ifndef CONFIG_MACH_ESP32
#define CONFIG_MACH_ESP32 1
#endif

#ifndef CONFIG_MACH_AVR
#define CONFIG_MACH_AVR 0
#endif

#ifndef CONFIG_MACH_ATSAM
#define CONFIG_MACH_ATSAM 0
#endif

#ifndef CONFIG_MACH_ATSAMD
#define CONFIG_MACH_ATSAMD 0
#endif

#ifndef CONFIG_MACH_LPC176X
#define CONFIG_MACH_LPC176X 0
#endif

#ifndef CONFIG_MACH_STM32
#define CONFIG_MACH_STM32 0
#endif

#ifndef CONFIG_MACH_HC32F460
#define CONFIG_MACH_HC32F460 0
#endif

#ifndef CONFIG_MACH_RP2040
#define CONFIG_MACH_RP2040 0
#endif

#ifndef CONFIG_MACH_PRU
#define CONFIG_MACH_PRU 0
#endif

#ifndef CONFIG_MACH_AR100
#define CONFIG_MACH_AR100 0
#endif

#ifndef CONFIG_MACH_LINUX
#define CONFIG_MACH_LINUX 0
#endif

#ifndef CONFIG_MACH_SIMU
#define CONFIG_MACH_SIMU 0
#endif

// String config defaults
#ifndef CONFIG_INITIAL_PINS
#define CONFIG_INITIAL_PINS ""
#endif

#ifndef CONFIG_USB_SERIAL_NUMBER
#define CONFIG_USB_SERIAL_NUMBER ""
#endif

#ifndef CONFIG_INLINE_STEPPER_HACK
#define CONFIG_INLINE_STEPPER_HACK 0
#endif

#ifndef CONFIG_HAVE_STRICT_TIMING
#define CONFIG_HAVE_STRICT_TIMING 0
#endif

#ifndef CONFIG_HAVE_BOOTLOADER_REQUEST
#define CONFIG_HAVE_BOOTLOADER_REQUEST 0
#endif

// ========================================================================
// ESP32 Console Configuration (from Kconfig.projbuild)
// ========================================================================

// Console type selection based on ESP-IDF configuration
#ifdef CONFIG_KLIPPER_CONSOLE_UART
#define CONFIG_CONSOLE_UART 1
#else
#define CONFIG_CONSOLE_UART 0
#endif

#ifdef CONFIG_KLIPPER_CONSOLE_USB_CDC
#define CONFIG_CONSOLE_USB_CDC 1
#else
#define CONFIG_CONSOLE_USB_CDC 0
#endif

// UART Console Configuration (from ESP-IDF Kconfig)
#ifdef CONFIG_KLIPPER_CONSOLE_UART
#define CONFIG_UART_NUM                 CONFIG_KLIPPER_UART_NUM
#define CONFIG_UART_BAUD_RATE          CONFIG_KLIPPER_UART_BAUD_RATE
#define CONFIG_UART_TX_PIN             CONFIG_KLIPPER_UART_TX_PIN
#define CONFIG_UART_RX_PIN             CONFIG_KLIPPER_UART_RX_PIN

// Handle optional pins (-1 means not used)
#if CONFIG_KLIPPER_UART_RTS_PIN >= 0
#define CONFIG_UART_RTS_PIN            CONFIG_KLIPPER_UART_RTS_PIN
#else
#define CONFIG_UART_RTS_PIN            UART_PIN_NO_CHANGE
#endif

#if CONFIG_KLIPPER_UART_CTS_PIN >= 0
#define CONFIG_UART_CTS_PIN            CONFIG_KLIPPER_UART_CTS_PIN
#else
#define CONFIG_UART_CTS_PIN            UART_PIN_NO_CHANGE
#endif

#define CONFIG_UART_TX_BUFFER_SIZE     CONFIG_KLIPPER_UART_TX_BUFFER_SIZE
#define CONFIG_UART_RX_BUFFER_SIZE     CONFIG_KLIPPER_UART_RX_BUFFER_SIZE
#endif

// Console buffer configuration
#define CONFIG_CONSOLE_RX_BUFFER_SIZE  CONFIG_KLIPPER_CONSOLE_RX_BUFFER_SIZE

// Auto-detect USB CDC availability (read-only)
#ifdef CONFIG_SOC_USB_SERIAL_JTAG_SUPPORTED
#define CONFIG_USB_CDC_AVAILABLE 1
#else
#define CONFIG_USB_CDC_AVAILABLE 0
#endif

// Validation: Ensure at least one console type is configured
#if !CONFIG_CONSOLE_UART && !CONFIG_CONSOLE_USB_CDC
#error "No console type configured. Enable either UART or USB CDC console in menuconfig."
#endif

// Validation: USB CDC console requires USB support
#if CONFIG_CONSOLE_USB_CDC && !CONFIG_USB_CDC_AVAILABLE
#error "USB CDC console selected but SOC doesn't support USB. Use UART console instead."
#endif


#endif // __AUTOCONF_H