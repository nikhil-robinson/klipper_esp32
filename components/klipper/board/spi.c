// SPI functions on rp2040
//
// Copyright (C) 2021  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h" // spi_setup, spi_prepare, spi_transfer
#include "command.h" // shutdown"
#include "sched.h" // sched_shutdown"
#include "board/internal.h" // pclock, gpio_peripheral



struct spi_info {
    void *spi;
    uint8_t miso_pin, mosi_pin, sck_pin;
    uint32_t pclk;
};


struct spi_config
spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
struct spi_config res = {NULL, 0, 0};

    return res;
}

void
spi_prepare(struct spi_config config)
{
    // spi_hw_t *spi = config.spi;
    // if (spi->cr0 == config.cr0 && spi->cpsr == config.cpsr)
    //     return;
    // spi->cr1 = 0;
    // spi->cr0 = config.cr0;
    // spi->cpsr = config.cpsr;
    // spi->cr1 = SPI_SSPCR1_SSE_BITS;
}

void
spi_transfer(struct spi_config config, uint8_t receive_data,
             uint8_t len, uint8_t *data)
{
    // void *spi = config.spi;
    // while (len--) {
    //     spi->dr = *data;
    //     while (!(spi->sr & SPI_SSPSR_RNE_BITS))
    //         ;
    //     uint8_t rdata = spi->dr;
    //     if(receive_data)
    //         *data = rdata;
    //     data++;
    // }
}
