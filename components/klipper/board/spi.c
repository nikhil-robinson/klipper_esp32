#include "board/gpio.h"     // spi_setup, spi_prepare, spi_transfer
#include "board/internal.h" // pclock, gpio_peripheral
#include "command.h"        // shutdown"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "sched.h" // sched_shutdown"
#include <string.h>

DECL_ENUMERATION("spi_bus", "SPI1_HOST", 0);
DECL_ENUMERATION("spi_bus", "SPI2_HOST", 1);
DECL_ENUMERATION("spi_bus", "SPI3_HOST", 2);

struct spi_info {
  spi_host_device_t bus;
  uint8_t miso_pin, mosi_pin, sck_pin;
};

static const struct spi_info spi_bus[] = {
    {SPI1_HOST, 7, 8, 6},
    {SPI2_HOST, 13, 12, 11},
#if SOC_SPI_PERIPH_NUM > 2
    {SPI3_HOST, 16, 19, 18},
#endif
};

struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate) {
  if (bus >= ARRAY_SIZE(spi_bus))
    shutdown("Invalid spi bus");

  const struct spi_info *info = &spi_bus[bus];

  spi_bus_config_t buscfg = {
      .miso_io_num = info->miso_pin,
      .mosi_io_num = info->mosi_pin,
      .sclk_io_num = info->sck_pin,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 32,
  };
  // Initialize the SPI bus

  spi_device_interface_config_t devcfg = {
      .clock_speed_hz = rate, // Clock out at 10 MHz
      .mode = mode,           // SPI mode 0
      .spics_io_num = -1,     // CS pin
      .queue_size = 7, // We want to be able to queue 7 transactions at a time
  };

  spi_device_handle_t spi;
  spi_bus_initialize(info->bus, &buscfg, SPI_DMA_CH_AUTO);
  spi_bus_add_device(info->bus, &devcfg, &spi);

  return (struct spi_config){.bus = spi};
}

void spi_prepare(struct spi_config config) {}

void spi_write(struct spi_config config, uint8_t len, uint8_t *data) {
  spi_transaction_t t = {
      .length = len * 8,
      .flags = SPI_TRANS_USE_TXDATA,
      .tx_data = {data},
      .user = 0,
  };
  spi_device_acquire_bus(config.bus, portMAX_DELAY);
  spi_device_polling_transmit(config.bus, &t);
  spi_device_release_bus(config.bus);
}

void spi_read(struct spi_config config, uint8_t len, uint8_t *data) {
  spi_transaction_t t = {
      .rxlength = len *8,
      .flags = SPI_TRANS_USE_RXDATA,
      .user = 0,
  };
  spi_device_acquire_bus(config.bus, portMAX_DELAY);
  esp_err_t err = spi_device_polling_transmit(config.bus, &t);
  spi_device_release_bus(config.bus);
  memcpy(data, t.rx_data, len);
}

void spi_transfer(struct spi_config config, uint8_t receive_data, uint8_t len,
                  uint8_t *data) {

  spi_write(config, len, data);
  if (receive_data) {
    spi_read(config, len, data);
  }
}
