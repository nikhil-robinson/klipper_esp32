// I2C functions for rp2040
//
// Copyright (C) 2022  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // timer_is_before
#include "board/gpio.h" // i2c_setup, i2c_read, i2c_write
#include "command.h" // shutdown
#include "sched.h" // sched_shutdown
#include "board/internal.h" // pclock, gpio_peripheral

struct i2c_info {
    uint8_t *i2c;
    uint8_t sda_pin, scl_pin, pclk;
};



struct i2c_config
i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    // if (bus > ARRAY_SIZE(i2c_bus))
    //     shutdown("Invalid i2c bus");

    // const struct i2c_info *info = &i2c_bus[bus];

    // gpio_peripheral(info->sda_pin, 3, 1);
    // gpio_peripheral(info->scl_pin, 3, 1);

    // if (!is_enabled_pclock(info->pclk)) {
    //     enable_pclock(info->pclk);

    //     void *i2c = info->i2c;

    //     i2c->enable = 0;

    //     // We set up the bus in 400 kHz mode, but then set timings afterwards
    //     // to match either 100k or 400k mode. This simplifies the setup.

    //     i2c->con = I2C_IC_CON_SPEED_VALUE_FAST << I2C_IC_CON_SPEED_LSB
    //              | I2C_IC_CON_MASTER_MODE_BITS
    //              | I2C_IC_CON_IC_SLAVE_DISABLE_BITS
    //              | I2C_IC_CON_IC_RESTART_EN_BITS;

    //     i2c->tx_tl = 0;
    //     i2c->rx_tl = 0;

    //     uint32_t pclk = get_pclock_frequency(info->pclk);

    //     // See `i2c_set_baudrate` in the Pico SDK `hardware_i2c/i2c.c` file
    //     // for details on the calculations here.
    //     if (rate > 1000000)
    //         rate = 1000000; // Clamp the rate to 1Mbps
    //     uint32_t period = (pclk + rate / 2) / rate;
    //     uint32_t lcnt = period * 3 / 5;
    //     uint32_t hcnt = period - lcnt;
    //     uint32_t sda_tx_hold_count = ((pclk * 3) / 10000000) + 1;

    //     i2c->fs_scl_hcnt = hcnt;
    //     i2c->fs_scl_lcnt = lcnt;
    //     i2c->fs_spklen = lcnt < 16 ? 1 : lcnt / 16;
    //     hw_write_masked(&i2c->sda_hold,
    //                     sda_tx_hold_count << I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_LSB,
    //                     I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_BITS);
    // }

    return (struct i2c_config){ .i2c=NULL, .addr=addr };
}

void
i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    // void *i2c = (void*)config.i2c;
    // uint32_t timeout = timer_read_time() + timer_from_us(5000);

    // i2c_start(i2c, config.addr);
    // i2c_do_write(i2c, config.addr, write_len, write, 1, timeout);
    // i2c_stop(i2c);
}

void
i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg
         , uint8_t read_len, uint8_t *read)
{
    // void *i2c = (void*)config.i2c;
    // uint32_t timeout = timer_read_time() + timer_from_us(5000);

    // i2c_start(i2c, config.addr);
    // if (reg_len != 0)
    //     i2c_do_write(i2c, config.addr, reg_len, reg, 0, timeout);
    // i2c_do_read(i2c, config.addr, read_len, read, timeout);
    // i2c_stop(i2c);
}
