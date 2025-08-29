#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_console.h"
#include "esp_log.h"

#include "irq.h"
#include "misc.h"
#include "command.h"
#include "internal.h"
#include "sched.h"

#define CONSOLE_UART_NUM   UART_NUM_0
#define CONSOLE_BAUD_RATE  115200
#define CONSOLE_UART_TX_PIN  UART_PIN_NO_CHANGE
#define CONSOLE_UART_RX_PIN  UART_PIN_NO_CHANGE


#define TAG "console_uart_vfs"

// Pollfd like original code
static struct pollfd main_pfd[1];
#define MP_TTY_IDX 0

static struct task_wake console_wake;
static uint8_t receive_buf[4096];
static int receive_pos;

// ===== Original helper =====
void report_errno(char *where, int rc)
{
    int e = errno;
    fprintf(stderr, "Got error %d in %s: (%d) %s\n", rc, where, e, strerror(e));
}

// ===== No-op on ESP, kept for API compatibility =====
int set_non_blocking(int fd) { (void)fd; return 0; }
int set_close_on_exec(int fd) { (void)fd; return 0; }

// ===== Return original buffer pointer =====
void *console_receive_buffer(void)
{
    return receive_buf;
}

// ===== Setup UART as VFS console =====
int console_setup(char *name)
{
    (void)name; // not used on ESP

    // Install UART driver
    uart_config_t uart_config = {
        .baud_rate = CONSOLE_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    esp_err_t err;
    err = uart_driver_install(CONSOLE_UART_NUM, 256, 0, 0, NULL, 0);
    if (err != ESP_OK) {
        report_errno("uart_driver_install", err);
        return -1;
    }
    uart_param_config(CONSOLE_UART_NUM, &uart_config);
    uart_set_pin(CONSOLE_UART_NUM, CONSOLE_UART_TX_PIN, CONSOLE_UART_RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Use VFS layer so read/write/printf work normally
    esp_vfs_dev_uart_use_driver(CONSOLE_UART_NUM);
    esp_vfs_console_register(CONSOLE_UART_NUM);

    // Set stdin/stdout to non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Prepare pollfd for console_sleep()
    main_pfd[MP_TTY_IDX].fd = STDIN_FILENO;
    main_pfd[MP_TTY_IDX].events = POLLIN;

    receive_pos = 0;
    return 0;
}

// ===== Task to process UART input =====
void console_task(void)
{
    if (!sched_check_wake(&console_wake))
        return;

    // Read available bytes from stdin
    int ret = read(STDIN_FILENO, &receive_buf[receive_pos],
                   sizeof(receive_buf) - receive_pos);
    if (ret < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return;
        report_errno("read", ret);
        return;
    }
    if (ret == 0)
        return;

    // FORCE_SHUTDOWN detection
    if (ret == 15 && receive_buf[receive_pos+14] == '\n' &&
        memcmp(&receive_buf[receive_pos], "FORCE_SHUTDOWN\n", 15) == 0) {
        shutdown("Force shutdown command");
    }

    // Process commands
    int len = receive_pos + ret;
    uint_fast8_t pop_count;
    uint_fast8_t msglen = len > MESSAGE_MAX ? MESSAGE_MAX : len;
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

// ===== Send encoded message =====
void console_sendf(const struct command_encoder *ce, va_list args)
{
    uint8_t buf[MESSAGE_MAX];
    uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

    int ret = write(STDOUT_FILENO, buf, msglen);
    if (ret < 0)
        report_errno("write", ret);
}

// ===== Wait for UART input =====
void console_sleep(sigset_t *sigset)
{
    (void)sigset; // signals unused in ESP

    int ret = poll(main_pfd, 1, -1); // block until data
    if (ret > 0 && (main_pfd[MP_TTY_IDX].revents & POLLIN)) {
        sched_wake_task(&console_wake);
    } else if (ret < 0 && errno != EINTR) {
        report_errno("poll", ret);
    }
}
