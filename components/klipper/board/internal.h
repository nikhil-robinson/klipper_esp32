#include "autoconf.h" // CONFIG_FLASH_APPLICATION_ADDRESS
#include "stdlib.h"

void command_reset(uint32_t *args);
void report_errno(char *where, int rc);
int set_non_blocking(int fd);
int set_close_on_exec(int fd);
int console_setup(char *name);
void console_sleep();

// timer.c
int timer_check_periodic(uint32_t *ts);
void timer_disable_signals(void);
void timer_enable_signals(void);

// watchdog.c
int watchdog_setup(void);