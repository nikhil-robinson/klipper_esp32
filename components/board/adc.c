// ADC functions on rp2040
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // timer_from_us
#include "command.h" // shutdown
#include "board/gpio.h" // gpio_adc_setup
#include "sched.h" // sched_shutdown

DECL_CONSTANT("ADC_MAX", 4095);

typedef struct {
    adc_unit_t unit;
    adc_channel_t channel;
} adc_mapping_t;

const adc_mapping_t adc_lookup[] = {
#if CONFIG_IDF_TARGET_ESP32C3
    { ADC_UNIT_1, ADC_CHANNEL_0 }, // GPIO_NUM_0
    { ADC_UNIT_1, ADC_CHANNEL_1 }, // GPIO_NUM_1
    { ADC_UNIT_1, ADC_CHANNEL_2 }, // GPIO_NUM_2
    { ADC_UNIT_1, ADC_CHANNEL_3 }, // GPIO_NUM_3
    { ADC_UNIT_1, ADC_CHANNEL_4 }, // GPIO_NUM_4
    { ADC_UNIT_2, ADC_CHANNEL_0 }, // GPIO_NUM_5
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
    {ADC_UNIT_1, ADC_CHANNEL_0}, //GPIO_NUM_1},
    {ADC_UNIT_1, ADC_CHANNEL_1}, //GPIO_NUM_2},
    {ADC_UNIT_1, ADC_CHANNEL_2}, //GPIO_NUM_3},
    {ADC_UNIT_1, ADC_CHANNEL_3}, //GPIO_NUM_4},
    {ADC_UNIT_1, ADC_CHANNEL_4}, //GPIO_NUM_5},
    {ADC_UNIT_1, ADC_CHANNEL_5}, //GPIO_NUM_6},
    {ADC_UNIT_1, ADC_CHANNEL_6}, //GPIO_NUM_7},
    {ADC_UNIT_1, ADC_CHANNEL_7}, //GPIO_NUM_8},
    {ADC_UNIT_1, ADC_CHANNEL_8}, //GPIO_NUM_9},
    {ADC_UNIT_1, ADC_CHANNEL_9}, //GPIO_NUM_10},
    {ADC_UNIT_2, ADC_CHANNEL_0}, //GPIO_NUM_11},
    {ADC_UNIT_2, ADC_CHANNEL_1}, //GPIO_NUM_12},
    {ADC_UNIT_2, ADC_CHANNEL_2}, //GPIO_NUM_13},
    {ADC_UNIT_2, ADC_CHANNEL_3}, //GPIO_NUM_14},
    {ADC_UNIT_2, ADC_CHANNEL_4}, //GPIO_NUM_15},
    {ADC_UNIT_2, ADC_CHANNEL_5}, //GPIO_NUM_16},
    {ADC_UNIT_2, ADC_CHANNEL_6}, //GPIO_NUM_17},
    {ADC_UNIT_2, ADC_CHANNEL_7}, //GPIO_NUM_18},
    {ADC_UNIT_2, ADC_CHANNEL_8}, //GPIO_NUM_19},
    {ADC_UNIT_2, ADC_CHANNEL_9}, //PIO_NUM_20},

#elif CONFIG_IDF_TARGET_ESP32H2
    { ADC_UNIT_1, ADC_CHANNEL_0 }, // GPIO_NUM_0
    { ADC_UNIT_1, ADC_CHANNEL_1 }, // GPIO_NUM_1
    { ADC_UNIT_1, ADC_CHANNEL_2 }, // GPIO_NUM_2
    { ADC_UNIT_1, ADC_CHANNEL_3 }, // GPIO_NUM_3
    { ADC_UNIT_1, ADC_CHANNEL_4 }, // GPIO_NUM_4
    { ADC_UNIT_2, ADC_CHANNEL_0 }, // GPIO_NUM_5

#endif
};

// #define ADC_TEMPERATURE_PIN 0xfe
// DECL_ENUMERATION("pin", "ADC_TEMPERATURE", ADC_TEMPERATURE_PIN);

struct gpio_adc
gpio_adc_setup(uint32_t pin)
{
    const adc_mapping_t adc = adc_lookup[pin];
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = adc.unit,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, adc.channel, &config));

    return (struct gpio_adc){ .handle = adc_handle };
}

enum { ADC_DUMMY=0xff };
static uint8_t last_analog_read = ADC_DUMMY;

// Try to sample a value. Returns zero if sample ready, otherwise
// returns the number of clock ticks the caller should wait before
// retrying this function.
uint32_t
gpio_adc_sample(struct gpio_adc g)
{
    return timer_from_us(5); // Sample takes 2us but provide extra time
}

// Read a value; use only after gpio_adc_sample() returns zero
uint16_t
gpio_adc_read(struct gpio_adc g)
{
    int adc_raw =0;
    ESP_ERROR_CHECK(adc_oneshot_read(g.handle, g.chan, &adc_raw));
    last_analog_read = ADC_DUMMY;
    return (uint16_t)adc_raw;
}

// Cancel a sample that may have been started with gpio_adc_sample()
void
gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (last_analog_read == g.chan)
        last_analog_read = ADC_DUMMY;
}



