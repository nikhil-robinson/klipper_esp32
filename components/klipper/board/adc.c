// Read analog values from Esp32 chips
//
// Copyright (C) 2024  Nikhil Robinson <nikhil@techprogeny.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

// Unless required by applicable law or agreed to in writing, this
// software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.

#include "gpio.h" // gpio_adc_setup
#include "misc.h" // timer_from_us
#include "command.h"    // shutdown
#include "sched.h"      // sched_shutdown

DECL_CONSTANT("ADC_MAX", 4095);

typedef struct
{
  adc_unit_t unit;
  adc_channel_t channel;
  gpio_num_t pin
} adc_mapping_t;

static const adc_mapping_t adc_lookup[] = {
#if CONFIG_IDF_TARGET_ESP32
    {ADC_UNIT_1, ADC_CHANNEL_0, GPIO_NUM_36},
    {ADC_UNIT_1, ADC_CHANNEL_1, GPIO_NUM_37},
    {ADC_UNIT_1, ADC_CHANNEL_2, GPIO_NUM_38},
    {ADC_UNIT_1, ADC_CHANNEL_3, GPIO_NUM_39},
    {ADC_UNIT_1, ADC_CHANNEL_4, GPIO_NUM_32},
    {ADC_UNIT_1, ADC_CHANNEL_5, GPIO_NUM_33},
    {ADC_UNIT_1, ADC_CHANNEL_6, GPIO_NUM_34},
    {ADC_UNIT_1, ADC_CHANNEL_7, GPIO_NUM_35},
    {ADC_UNIT_2, ADC_CHANNEL_0, GPIO_NUM_4},
    {ADC_UNIT_2, ADC_CHANNEL_1, GPIO_NUM_0},
    {ADC_UNIT_2, ADC_CHANNEL_2, GPIO_NUM_2},
    {ADC_UNIT_2, ADC_CHANNEL_3, GPIO_NUM_15},
    {ADC_UNIT_2, ADC_CHANNEL_4, GPIO_NUM_13},
    {ADC_UNIT_2, ADC_CHANNEL_5, GPIO_NUM_12},
    {ADC_UNIT_2, ADC_CHANNEL_6, GPIO_NUM_14},
    {ADC_UNIT_2, ADC_CHANNEL_7, GPIO_NUM_27},
    {ADC_UNIT_2, ADC_CHANNEL_8, GPIO_NUM_25},
    {ADC_UNIT_2, ADC_CHANNEL_9, GPIO_NUM_26},
#elif CONFIG_IDF_TARGET_ESP32C3
    {ADC_UNIT_1, ADC_CHANNEL_0, GPIO_NUM_0},
    {ADC_UNIT_1, ADC_CHANNEL_1, GPIO_NUM_1},
    {ADC_UNIT_1, ADC_CHANNEL_2, GPIO_NUM_2},
    {ADC_UNIT_1, ADC_CHANNEL_3, GPIO_NUM_3},
    {ADC_UNIT_1, ADC_CHANNEL_4, GPIO_NUM_4},
    {ADC_UNIT_2, ADC_CHANNEL_0, GPIO_NUM_5},
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
    {ADC_UNIT_1, ADC_CHANNEL_0, GPIO_NUM_1},
    {ADC_UNIT_1, ADC_CHANNEL_1, GPIO_NUM_2},
    {ADC_UNIT_1, ADC_CHANNEL_2, GPIO_NUM_3},
    {ADC_UNIT_1, ADC_CHANNEL_3, GPIO_NUM_4},
    {ADC_UNIT_1, ADC_CHANNEL_4, GPIO_NUM_5},
    {ADC_UNIT_1, ADC_CHANNEL_5, GPIO_NUM_6},
    {ADC_UNIT_1, ADC_CHANNEL_6, GPIO_NUM_7},
    {ADC_UNIT_1, ADC_CHANNEL_7, GPIO_NUM_8},
    {ADC_UNIT_1, ADC_CHANNEL_8, GPIO_NUM_9},
    {ADC_UNIT_1, ADC_CHANNEL_9, GPIO_NUM_10},
    {ADC_UNIT_2, ADC_CHANNEL_0, GPIO_NUM_11},
    {ADC_UNIT_2, ADC_CHANNEL_1, GPIO_NUM_12},
    {ADC_UNIT_2, ADC_CHANNEL_2, GPIO_NUM_13},
    {ADC_UNIT_2, ADC_CHANNEL_3, GPIO_NUM_14},
    {ADC_UNIT_2, ADC_CHANNEL_4, GPIO_NUM_15},
    {ADC_UNIT_2, ADC_CHANNEL_5, GPIO_NUM_16},
    {ADC_UNIT_2, ADC_CHANNEL_6, GPIO_NUM_17},
    {ADC_UNIT_2, ADC_CHANNEL_7, GPIO_NUM_18},
    {ADC_UNIT_2, ADC_CHANNEL_8, GPIO_NUM_19},
    {ADC_UNIT_2, ADC_CHANNEL_9, GPIO_NUM_20},

#endif
};

// #define ADC_TEMPERATURE_PIN 0xfe
// DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);

static adc_mapping_t *adc_serach(uint32_t pin)
{

  for (size_t i = 0; i < (sizeof(adc_lookup) / sizeof((adc_lookup)[0])); i++)
  {
    adc_mapping_t *adc = &adc_lookup[pin];
    if (adc->pin == pin)
    {
      return adc;
    }
  }
  return NULL;
}

adc_oneshot_unit_handle_t adc_handles[2] = {NULL};

struct gpio_adc gpio_adc_setup(uint32_t pin)
{
  const adc_mapping_t *adc = adc_serach(pin);
  if (adc == NULL)
  {
    shutdown("Not a valid ADC pin");
  }

  // adc_oneshot_unit_handle_t adc_handle;
  adc_oneshot_unit_init_cfg_t init_config1 = {
      .unit_id = adc->unit,
  };

  if (condition(adc_handles[adc->unit] == NULL))
  {
    esp_err_t ret = adc_oneshot_new_unit(&init_config1, &adc_handles[adc->unit]);
    if (ret != ESP_OK)
    {
      report_errno("analog open", ret);
      goto fail;
    }
  }
  //-------------ADC1 Config---------------//
  adc_oneshot_chan_cfg_t config = {
      .bitwidth = ADC_BITWIDTH_DEFAULT,
      .atten = ADC_ATTEN_DB_12,
  };

  esp_err_t ret = adc_oneshot_config_channel(adc_handles[adc->unit], adc->channel, &config);
  if (ret != ESP_OK)
  {
    report_errno("analog config", ret);
    goto fail;
  }

  return (struct gpio_adc){.handle = adc_handles[adc->unit], .chan = adc->channel};

fail:
  shutdown("Failed to initialize ADC unit");
}

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t gpio_adc_sample(struct gpio_adc g) { return 0; }

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t gpio_adc_read(struct gpio_adc g)
{
  int adc_raw = 0;
  esp_err_t ret = adc_oneshot_read(g.handle, g.chan, &adc_raw);
  if (ret != ESP_OK)
  {
    report_errno("analog read", ret);
    try_shutdown("Error on analog read");
    return 0;
  }
  return (uint16_t)adc_raw;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void gpio_adc_cancel_sample(struct gpio_adc g) {}
