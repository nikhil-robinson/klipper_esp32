// I2C functions for esp32
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.

#include "board/gpio.h"     // i2c_setup, i2c_read, i2c_write
#include "board/internal.h" // pclock, gpio_peripheral
#include "board/misc.h"     // timer_is_before
#include "command.h"        // shutdown
#include "driver/i2c_master.h"
#include "sched.h" // sched_shutdown
#include "soc/clk_tree_defs.h"
#include "soc/soc_caps.h"

DECL_ENUMERATION_RANGE("i2c_bus", "I2C_NUM_0", 0, 2);
DECL_CONSTANT_STR("BUS_PINS_i2c0", "GPIO_NUM_8,GPIO_NUM_9");
DECL_CONSTANT_STR("BUS_PINS_i2c1", "GPIO_NUM_48,GPIO_NUM_47");

struct i2c_info {
  i2c_port_t port;
  uint32_t sda_pin, scl_pin;
};

static const struct i2c_info i2c_bus[] = {
    {I2C_NUM_0, 8, 9},
#if SOC_I2C_NUM >= 2
    {I2C_NUM_1, 48, 47},
#endif
};

struct i2c_config i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr) {
  if (bus > ARRAY_SIZE(i2c_bus))
    shutdown("Invalid i2c bus");

  const struct i2c_info *info = &i2c_bus[bus];

  i2c_master_bus_config_t i2c_mst_config = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = info->port,
      .scl_io_num = info->scl_pin,
      .sda_io_num = info->sda_pin,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = true,
  };

  i2c_master_bus_handle_t bus_handle;
  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

  i2c_device_config_t dev_cfg = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = addr,
      .scl_speed_hz = rate,
  };

  i2c_master_dev_handle_t dev_handle;
  ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

  return (struct i2c_config){.handle = dev_handle};
}

void i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write) {
  i2c_master_transmit(config.handle, write, write_len, -1);
}

void i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg,
              uint8_t read_len, uint8_t *read) {
  i2c_master_transmit_receive(config.handle, reg, reg_len, read, read_len, -1);
}
