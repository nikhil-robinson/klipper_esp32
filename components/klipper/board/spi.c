// Spi control for esp32
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "gpio.h"     // spi_setup, spi_prepare, spi_transfer
#include "internal.h" // pclock, gpio_peripheral
#include "command.h"  // shutdown, try_shutdown
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "sched.h"    // sched_shutdown
#include "esp_log.h"
#include <string.h>

static const char* TAG = "spi";

DECL_ENUMERATION("spi_bus", "SPI1_HOST", 0);
DECL_ENUMERATION("spi_bus", "SPI2_HOST", 1);
DECL_ENUMERATION("spi_bus", "SPI3_HOST", 2);

struct spi_info {
    spi_host_device_t bus;
    uint8_t miso_pin, mosi_pin, sck_pin;
    bool initialized;
};

struct spi_device_info {
    spi_device_handle_t handle;
    uint32_t bus_id;
    uint32_t rate;
    uint8_t mode;
    bool in_use;
};

#define MAX_SPI_DEVICES 8
static struct spi_device_info spi_devices[MAX_SPI_DEVICES];
static int spi_device_count = 0;

static struct spi_info spi_bus[] = {
    {SPI1_HOST, 7, 8, 6, false},
    {SPI2_HOST, 13, 12, 11, false},
#if SOC_SPI_PERIPH_NUM > 2
    {SPI3_HOST, 16, 19, 18, false},
#endif
};

// Maximum SPI transfer size
#define SPI_MAX_TRANSFER_SIZE 4092

// Find or create SPI device handle
static spi_device_handle_t spi_get_device(uint32_t bus_id, uint8_t mode, uint32_t rate) {
    // Check for existing device with same configuration
    for (int i = 0; i < spi_device_count; i++) {
        if (spi_devices[i].in_use && 
            spi_devices[i].bus_id == bus_id && 
            spi_devices[i].mode == mode && 
            spi_devices[i].rate == rate) {
            return spi_devices[i].handle;
        }
    }
    
    // Find available slot
    int slot = -1;
    for (int i = 0; i < MAX_SPI_DEVICES; i++) {
        if (!spi_devices[i].in_use) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        shutdown("Too many SPI devices");
    }
    
    if (bus_id >= ARRAY_SIZE(spi_bus)) {
        shutdown("Invalid spi bus");
    }

    struct spi_info *info = &spi_bus[bus_id];

    // Initialize bus only once
    if (!info->initialized) {
        spi_bus_config_t buscfg = {
            .miso_io_num = info->miso_pin,
            .mosi_io_num = info->mosi_pin,
            .sclk_io_num = info->sck_pin,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,
        };

        esp_err_t ret = spi_bus_initialize(info->bus, &buscfg, SPI_DMA_CH_AUTO);
        if (ret != ESP_OK) {
            shutdown("SPI bus initialization failed");
        }
        
        info->initialized = true;
        ESP_LOGI(TAG, "SPI bus %ld initialized", bus_id);
    }

    // Configure device
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = rate,
        .mode = mode,
        .spics_io_num = -1,  // CS will be managed manually
        .queue_size = 1,     // Single transaction queue for simplicity
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
    };

    esp_err_t ret = spi_bus_add_device(info->bus, &devcfg, &spi_devices[slot].handle);
    if (ret != ESP_OK) {
        shutdown("SPI device add failed");
    }

    spi_devices[slot].bus_id = bus_id;
    spi_devices[slot].rate = rate;
    spi_devices[slot].mode = mode;
    spi_devices[slot].in_use = true;
    
    if (slot >= spi_device_count) {
        spi_device_count = slot + 1;
    }

    ESP_LOGI(TAG, "SPI device configured: bus=%ld, mode=%d, rate=%ld Hz", bus_id, mode, rate);
    
    return spi_devices[slot].handle;
}

struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate) {
    spi_device_handle_t handle = spi_get_device(bus, mode, rate);
    return (struct spi_config){.bus = handle, .rate = rate};
}

void spi_prepare(struct spi_config config) {
    // Prepare for upcoming SPI transaction
    // In ESP32, this can be used to acquire the bus if needed
    // For now, we'll keep it simple as bus acquisition is done per-transaction
}

void spi_transfer(struct spi_config config, uint8_t receive_data, uint8_t len, uint8_t *data) {
    if (len == 0) {
        return;
    }
    
    if (len > SPI_MAX_TRANSFER_SIZE) {
        shutdown("SPI transfer too large");
    }

    spi_transaction_t t = {0};
    
    if (len <= 4) {
        // Use internal buffer for small transfers (faster)
        if (receive_data) {
            t.flags = SPI_TRANS_USE_RXDATA;
            if (data) {
                t.flags |= SPI_TRANS_USE_TXDATA;
                memcpy(t.tx_data, data, len);
            }
        } else {
            t.flags = SPI_TRANS_USE_TXDATA;
            memcpy(t.tx_data, data, len);
        }
        t.length = len * 8;
        t.rxlength = len * 8;
    } else {
        // Use external buffer for larger transfers
        if (receive_data) {
            t.tx_buffer = data;
            t.rx_buffer = data;
        } else {
            t.tx_buffer = data;
            t.rx_buffer = NULL;
        }
        t.length = len * 8;
        t.rxlength = receive_data ? len * 8 : 0;
    }

    esp_err_t ret = spi_device_polling_transmit(config.bus, &t);
    if (ret != ESP_OK) {
        try_shutdown("SPI transfer failed");
        return;
    }

    // Copy received data back for small transfers using internal buffer
    if (receive_data && len <= 4) {
        memcpy(data, t.rx_data, len);
    }
}

// Cleanup function for shutdown
void spi_shutdown(void) {
    // Free all SPI devices
    for (int i = 0; i < spi_device_count; i++) {
        if (spi_devices[i].in_use && spi_devices[i].handle) {
            spi_bus_remove_device(spi_devices[i].handle);
            spi_devices[i].in_use = false;
            spi_devices[i].handle = NULL;
        }
    }
    spi_device_count = 0;
    
    // Free all SPI buses
    for (uint32_t i = 0; i < ARRAY_SIZE(spi_bus); i++) {
        if (spi_bus[i].initialized) {
            spi_bus_free(spi_bus[i].bus);
            spi_bus[i].initialized = false;
            ESP_LOGI(TAG, "SPI bus %ld freed", i);
        }
    }
}
DECL_SHUTDOWN(spi_shutdown);
