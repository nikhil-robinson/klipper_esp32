#include "board/internal.h" // NVIC_SystemReset
#include "autoconf.h"       // CONFIG_FLASH_APPLICATION_ADDRESS
#include "board/irq.h"      // irq_disable
#include "command.h"        // DECL_COMMAND_FLAGS
#include "esp_system.h"

void command_reset(uint32_t *args) { esp_restart(); }
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");