
/* DO NOT EDIT! This is an autogenerated file. See scripts/buildcommands.py. */

#include "board/irq.h"
#include "board/pgm.h"
#include "command.h"
#include "compiler.h"
#include "initial_pins.h"

void
ctr_run_initfuncs(void)
{
    extern void alloc_init(void);
    alloc_init();
    extern void initial_pins_setup(void);
    initial_pins_setup();
    extern void timer_init(void);
    timer_init();
}

void
ctr_run_shutdownfuncs(void)
{
    extern void sendf_shutdown(void);
    sendf_shutdown();
    extern void move_reset(void);
    move_reset();
    extern void digital_out_shutdown(void);
    digital_out_shutdown();
    extern void stepper_shutdown(void);
    stepper_shutdown();
    extern void trsync_shutdown(void);
    trsync_shutdown();
    extern void analog_in_shutdown(void);
    analog_in_shutdown();
    extern void tmcuart_shutdown(void);
    tmcuart_shutdown();
    extern void st7920_shutdown(void);
    st7920_shutdown();
    extern void hd44780_shutdown(void);
    hd44780_shutdown();
}

void
ctr_run_taskfuncs(void)
{
    irq_poll();
    extern void trsync_task(void);
    trsync_task();
    irq_poll();
    extern void analog_in_task(void);
    analog_in_task();
    irq_poll();
    extern void buttons_task(void);
    buttons_task();
    irq_poll();
    extern void tmcuart_task(void);
    tmcuart_task();
    irq_poll();
    extern void counter_task(void);
    counter_task();
    irq_poll();
    extern void console_task(void);
    console_task();
    irq_poll();
}

uint8_t __always_inline
ctr_lookup_static_string(const char *str)
{
    if (__builtin_strcmp(str, "Shutdown cleared when not shutdown") == 0)
        return 2;
    if (__builtin_strcmp(str, "Timer too close") == 0)
        return 3;
    if (__builtin_strcmp(str, "sentinel timer called") == 0)
        return 4;
    if (__builtin_strcmp(str, "Invalid command") == 0)
        return 5;
    if (__builtin_strcmp(str, "Message encode error") == 0)
        return 6;
    if (__builtin_strcmp(str, "Command parser error") == 0)
        return 7;
    if (__builtin_strcmp(str, "Command request") == 0)
        return 8;
    if (__builtin_strcmp(str, "config_reset only available when shutdown") == 0)
        return 9;
    if (__builtin_strcmp(str, "oids already allocated") == 0)
        return 10;
    if (__builtin_strcmp(str, "Can't assign oid") == 0)
        return 11;
    if (__builtin_strcmp(str, "Invalid oid type") == 0)
        return 12;
    if (__builtin_strcmp(str, "Already finalized") == 0)
        return 13;
    if (__builtin_strcmp(str, "Invalid move request size") == 0)
        return 14;
    if (__builtin_strcmp(str, "Move queue overflow") == 0)
        return 15;
    if (__builtin_strcmp(str, "alloc_chunks failed") == 0)
        return 16;
    if (__builtin_strcmp(str, "alloc_chunk failed") == 0)
        return 17;
    if (__builtin_strcmp(str, "update_digital_out not valid with active queue") == 0)
        return 18;
    if (__builtin_strcmp(str, "Scheduled digital out event will exceed max_duration") == 0)
        return 19;
    if (__builtin_strcmp(str, "Can not set soft pwm cycle ticks while updates pending") == 0)
        return 20;
    if (__builtin_strcmp(str, "Missed scheduling of next digital out event") == 0)
        return 21;
    if (__builtin_strcmp(str, "Can't reset time when stepper active") == 0)
        return 22;
    if (__builtin_strcmp(str, "Invalid count parameter") == 0)
        return 23;
    if (__builtin_strcmp(str, "Stepper too far in past") == 0)
        return 24;
    if (__builtin_strcmp(str, "Can't add signal that is already active") == 0)
        return 25;
    if (__builtin_strcmp(str, "ADC out of range") == 0)
        return 26;
    if (__builtin_strcmp(str, "Invalid buttons retransmit count") == 0)
        return 27;
    if (__builtin_strcmp(str, "Set button past maximum button count") == 0)
        return 28;
    if (__builtin_strcmp(str, "Max of 8 buttons") == 0)
        return 29;
    if (__builtin_strcmp(str, "tmcuart data too large") == 0)
        return 30;
    if (__builtin_strcmp(str, "Invalid neopixel update command") == 0)
        return 31;
    if (__builtin_strcmp(str, "Invalid neopixel data_size") == 0)
        return 32;
    if (__builtin_strcmp(str, "Rescheduled timer in the past") == 0)
        return 33;
    if (__builtin_strcmp(str, "Force shutdown command") == 0)
        return 34;
    if (__builtin_strcmp(str, "Error on analog read") == 0)
        return 35;
    if (__builtin_strcmp(str, "Unable to open adc device") == 0)
        return 36;
    if (__builtin_strcmp(str, "Unable to open in GPIO chip line") == 0)
        return 37;
    if (__builtin_strcmp(str, "Unable to open out GPIO chip line") == 0)
        return 38;
    if (__builtin_strcmp(str, "Unable to open GPIO chip device") == 0)
        return 39;
    if (__builtin_strcmp(str, "GPIO chip device not found") == 0)
        return 40;
    return 0xff;
}

const struct initial_pin_s initial_pins[] PROGMEM = {
};
const int initial_pins_size PROGMEM = ARRAY_SIZE(initial_pins);

static const uint8_t command_parameters0[] PROGMEM = {
    PT_uint16 };
static const uint8_t command_parameters1[] PROGMEM = {
    PT_uint32, PT_uint16 };
static const uint8_t command_parameters2[] PROGMEM = {
    PT_uint32, PT_progmem_buffer };
static const uint8_t command_parameters3[] PROGMEM = {
    PT_uint32, PT_uint32, PT_uint32 };
static const uint8_t command_parameters4[] PROGMEM = {
    PT_uint32, PT_uint32 };
static const uint8_t command_parameters5[] PROGMEM = {
    PT_uint32 };
static const uint8_t command_parameters6[] PROGMEM = {
    PT_byte, PT_uint32, PT_byte, PT_uint16 };
static const uint8_t command_parameters7[] PROGMEM = {
    PT_buffer };
static const uint8_t command_parameters8[] PROGMEM = {
    PT_byte, PT_int32 };
static const uint8_t command_parameters9[] PROGMEM = {
    PT_byte, PT_byte, PT_uint32, PT_byte };
static const uint8_t command_parameters10[] PROGMEM = {
    PT_byte, PT_byte, PT_byte, PT_uint32 };
static const uint8_t command_parameters11[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint16 };
static const uint8_t command_parameters12[] PROGMEM = {
    PT_byte, PT_byte, PT_buffer };
static const uint8_t command_parameters13[] PROGMEM = {
    PT_byte, PT_buffer };
static const uint8_t command_parameters14[] PROGMEM = {
    PT_byte, PT_byte };
static const uint8_t command_parameters15[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32, PT_uint32 };
static const uint8_t command_parameters16[] PROGMEM = {
    PT_uint32, PT_byte };
static const uint8_t command_parameters17[] PROGMEM = {
    PT_byte };
static const uint8_t command_parameters18[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32 };
static const uint8_t command_parameters19[] PROGMEM = {
    PT_byte, PT_uint32 };
static const uint8_t command_parameters20[] PROGMEM = {
    PT_byte, PT_uint32, PT_byte, PT_byte, PT_uint32 };
static const uint8_t command_parameters21[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint16, PT_int16 };
static const uint8_t command_parameters22[] PROGMEM = {
    PT_byte, PT_byte, PT_byte, PT_byte, PT_uint32 };
static const uint8_t command_parameters23[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32, PT_byte, PT_uint32, PT_byte, PT_byte, PT_byte };
static const uint8_t command_parameters24[] PROGMEM = {
    PT_byte, PT_byte, PT_byte };
static const uint8_t command_parameters25[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32, PT_byte };
static const uint8_t command_parameters26[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32, PT_byte, PT_uint32, PT_uint16, PT_uint16, PT_byte };
static const uint8_t command_parameters27[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32, PT_byte, PT_byte };
static const uint8_t command_parameters28[] PROGMEM = {
    PT_byte, PT_buffer, PT_byte };
static const uint8_t command_parameters29[] PROGMEM = {
    PT_byte, PT_uint32, PT_byte, PT_uint32, PT_uint32 };
static const uint8_t command_parameters30[] PROGMEM = {
    PT_byte, PT_uint16, PT_buffer };
static const uint8_t command_parameters31[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint16, PT_uint32, PT_uint32 };
static const uint8_t command_parameters32[] PROGMEM = {
    PT_byte, PT_uint32, PT_byte };
static const uint8_t command_parameters33[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32, PT_uint32, PT_uint32, PT_uint32 };
static const uint8_t command_parameters34[] PROGMEM = {
    PT_byte, PT_uint32, PT_uint32, PT_uint32, PT_uint32, PT_uint32, PT_uint32, PT_uint32 };

const struct command_encoder command_encoder_51 PROGMEM = {    
    // starting
    .msg_id=51,
    .num_params=0,
    .param_types = 0,
    .max_size=6,
};
const struct command_encoder command_encoder_52 PROGMEM = {    
    // is_shutdown static_string_id=%hu
    .msg_id=52,
    .num_params=1,
    .param_types = command_parameters0,
    .max_size=9,
};
const struct command_encoder command_encoder_53 PROGMEM = {    
    // shutdown clock=%u static_string_id=%hu
    .msg_id=53,
    .num_params=2,
    .param_types = command_parameters1,
    .max_size=14,
};
const struct command_encoder command_encoder_0 PROGMEM = {    
    // identify_response offset=%u data=%.*s
    .msg_id=0,
    .num_params=2,
    .param_types = command_parameters2,
    .max_size=64,
};
const struct command_encoder command_encoder_54 PROGMEM = {    
    // stats count=%u sum=%u sumsq=%u
    .msg_id=54,
    .num_params=3,
    .param_types = command_parameters3,
    .max_size=21,
};
const struct command_encoder command_encoder_55 PROGMEM = {    
    // uptime high=%u clock=%u
    .msg_id=55,
    .num_params=2,
    .param_types = command_parameters4,
    .max_size=16,
};
const struct command_encoder command_encoder_56 PROGMEM = {    
    // clock clock=%u
    .msg_id=56,
    .num_params=1,
    .param_types = command_parameters5,
    .max_size=11,
};
const struct command_encoder command_encoder_57 PROGMEM = {    
    // config is_config=%c crc=%u is_shutdown=%c move_count=%hu
    .msg_id=57,
    .num_params=4,
    .param_types = command_parameters6,
    .max_size=18,
};
const struct command_encoder command_encoder_58 PROGMEM = {    
    // pong data=%*s
    .msg_id=58,
    .num_params=1,
    .param_types = command_parameters7,
    .max_size=64,
};
const struct command_encoder command_encoder_59 PROGMEM = {    
    // debug_result val=%u
    .msg_id=59,
    .num_params=1,
    .param_types = command_parameters5,
    .max_size=11,
};
const struct command_encoder command_encoder_60 PROGMEM = {    
    // stepper_position oid=%c pos=%i
    .msg_id=60,
    .num_params=2,
    .param_types = command_parameters8,
    .max_size=13,
};
const struct command_encoder command_encoder_61 PROGMEM = {    
    // endstop_state oid=%c homing=%c next_clock=%u pin_value=%c
    .msg_id=61,
    .num_params=4,
    .param_types = command_parameters9,
    .max_size=17,
};
const struct command_encoder command_encoder_62 PROGMEM = {    
    // trsync_state oid=%c can_trigger=%c trigger_reason=%c clock=%u
    .msg_id=62,
    .num_params=4,
    .param_types = command_parameters10,
    .max_size=17,
};
const struct command_encoder command_encoder_63 PROGMEM = {    
    // analog_in_state oid=%c next_clock=%u value=%hu
    .msg_id=63,
    .num_params=3,
    .param_types = command_parameters11,
    .max_size=16,
};
const struct command_encoder command_encoder_64 PROGMEM = {    
    // buttons_state oid=%c ack_count=%c state=%*s
    .msg_id=64,
    .num_params=3,
    .param_types = command_parameters12,
    .max_size=64,
};
const struct command_encoder command_encoder_65 PROGMEM = {    
    // tmcuart_response oid=%c read=%*s
    .msg_id=65,
    .num_params=2,
    .param_types = command_parameters13,
    .max_size=64,
};
const struct command_encoder command_encoder_66 PROGMEM = {    
    // neopixel_result oid=%c success=%c
    .msg_id=66,
    .num_params=2,
    .param_types = command_parameters14,
    .max_size=10,
};
const struct command_encoder command_encoder_67 PROGMEM = {    
    // counter_state oid=%c next_clock=%u count=%u count_clock=%u
    .msg_id=67,
    .num_params=4,
    .param_types = command_parameters15,
    .max_size=23,
};

const __always_inline struct command_encoder *
ctr_lookup_encoder(const char *str)
{
    if (__builtin_strcmp(str, "starting") == 0)
        return &command_encoder_51;
    if (__builtin_strcmp(str, "is_shutdown static_string_id=%hu") == 0)
        return &command_encoder_52;
    if (__builtin_strcmp(str, "shutdown clock=%u static_string_id=%hu") == 0)
        return &command_encoder_53;
    if (__builtin_strcmp(str, "identify_response offset=%u data=%.*s") == 0)
        return &command_encoder_0;
    if (__builtin_strcmp(str, "stats count=%u sum=%u sumsq=%u") == 0)
        return &command_encoder_54;
    if (__builtin_strcmp(str, "uptime high=%u clock=%u") == 0)
        return &command_encoder_55;
    if (__builtin_strcmp(str, "clock clock=%u") == 0)
        return &command_encoder_56;
    if (__builtin_strcmp(str, "config is_config=%c crc=%u is_shutdown=%c move_count=%hu") == 0)
        return &command_encoder_57;
    if (__builtin_strcmp(str, "pong data=%*s") == 0)
        return &command_encoder_58;
    if (__builtin_strcmp(str, "debug_result val=%u") == 0)
        return &command_encoder_59;
    if (__builtin_strcmp(str, "stepper_position oid=%c pos=%i") == 0)
        return &command_encoder_60;
    if (__builtin_strcmp(str, "endstop_state oid=%c homing=%c next_clock=%u pin_value=%c") == 0)
        return &command_encoder_61;
    if (__builtin_strcmp(str, "trsync_state oid=%c can_trigger=%c trigger_reason=%c clock=%u") == 0)
        return &command_encoder_62;
    if (__builtin_strcmp(str, "analog_in_state oid=%c next_clock=%u value=%hu") == 0)
        return &command_encoder_63;
    if (__builtin_strcmp(str, "buttons_state oid=%c ack_count=%c state=%*s") == 0)
        return &command_encoder_64;
    if (__builtin_strcmp(str, "tmcuart_response oid=%c read=%*s") == 0)
        return &command_encoder_65;
    if (__builtin_strcmp(str, "neopixel_result oid=%c success=%c") == 0)
        return &command_encoder_66;
    if (__builtin_strcmp(str, "counter_state oid=%c next_clock=%u count=%u count_clock=%u") == 0)
        return &command_encoder_67;
    return NULL;
}

const __always_inline struct command_encoder *
ctr_lookup_output(const char *str)
{
    
    return NULL;
}

extern void command_allocate_oids(uint32_t*);
extern void command_buttons_ack(uint32_t*);
extern void command_buttons_add(uint32_t*);
extern void command_buttons_query(uint32_t*);
extern void command_clear_shutdown(uint32_t*);
extern void command_config_analog_in(uint32_t*);
extern void command_config_buttons(uint32_t*);
extern void command_config_counter(uint32_t*);
extern void command_config_digital_out(uint32_t*);
extern void command_config_endstop(uint32_t*);
extern void command_config_hd44780(uint32_t*);
extern void command_config_neopixel(uint32_t*);
extern void command_config_reset(uint32_t*);
extern void command_config_st7920(uint32_t*);
extern void command_config_stepper(uint32_t*);
extern void command_config_tmcuart(uint32_t*);
extern void command_config_trsync(uint32_t*);
extern void command_debug_nop(uint32_t*);
extern void command_debug_ping(uint32_t*);
extern void command_debug_read(uint32_t*);
extern void command_debug_write(uint32_t*);
extern void command_emergency_stop(uint32_t*);
extern void command_endstop_home(uint32_t*);
extern void command_endstop_query_state(uint32_t*);
extern void command_finalize_config(uint32_t*);
extern void command_get_clock(uint32_t*);
extern void command_get_config(uint32_t*);
extern void command_get_uptime(uint32_t*);
extern void command_hd44780_send_cmds(uint32_t*);
extern void command_hd44780_send_data(uint32_t*);
extern void command_identify(uint32_t*);
extern void command_neopixel_send(uint32_t*);
extern void command_neopixel_update(uint32_t*);
extern void command_query_analog_in(uint32_t*);
extern void command_query_counter(uint32_t*);
extern void command_queue_digital_out(uint32_t*);
extern void command_queue_step(uint32_t*);
extern void command_reset_step_clock(uint32_t*);
extern void command_set_digital_out(uint32_t*);
extern void command_set_digital_out_pwm_cycle(uint32_t*);
extern void command_set_next_step_dir(uint32_t*);
extern void command_st7920_send_cmds(uint32_t*);
extern void command_st7920_send_data(uint32_t*);
extern void command_stepper_get_position(uint32_t*);
extern void command_stepper_stop_on_trigger(uint32_t*);
extern void command_tmcuart_send(uint32_t*);
extern void command_trsync_set_timeout(uint32_t*);
extern void command_trsync_start(uint32_t*);
extern void command_trsync_trigger(uint32_t*);
extern void command_update_digital_out(uint32_t*);

const struct command_parser command_index[] PROGMEM = {
{
}, {
    // identify offset=%u count=%c
    .msg_id=1,
    .num_params=2,
    .param_types = command_parameters16,
    .num_args=2,
    .flags=0x01,
    .func=command_identify
}, {
    // clear_shutdown
    .msg_id=2,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_clear_shutdown
}, {
    // emergency_stop
    .msg_id=3,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_emergency_stop
}, {
    // get_uptime
    .msg_id=4,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_get_uptime
}, {
    // get_clock
    .msg_id=5,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_get_clock
}, {
    // finalize_config crc=%u
    .msg_id=6,
    .num_params=1,
    .param_types = command_parameters5,
    .num_args=1,
    .flags=0,
    .func=command_finalize_config
}, {
    // get_config
    .msg_id=7,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_get_config
}, {
    // allocate_oids count=%c
    .msg_id=8,
    .num_params=1,
    .param_types = command_parameters17,
    .num_args=1,
    .flags=0,
    .func=command_allocate_oids
}, {
    // debug_nop
    .msg_id=9,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_debug_nop
}, {
    // debug_ping data=%*s
    .msg_id=10,
    .num_params=1,
    .param_types = command_parameters7,
    .num_args=2,
    .flags=0x01,
    .func=command_debug_ping
}, {
    // debug_write order=%c addr=%u val=%u
    .msg_id=11,
    .num_params=3,
    .param_types = command_parameters18,
    .num_args=3,
    .flags=0x01,
    .func=command_debug_write
}, {
    // debug_read order=%c addr=%u
    .msg_id=12,
    .num_params=2,
    .param_types = command_parameters19,
    .num_args=2,
    .flags=0x01,
    .func=command_debug_read
}, {
    // set_digital_out pin=%u value=%c
    .msg_id=13,
    .num_params=2,
    .param_types = command_parameters16,
    .num_args=2,
    .flags=0,
    .func=command_set_digital_out
}, {
    // update_digital_out oid=%c value=%c
    .msg_id=14,
    .num_params=2,
    .param_types = command_parameters14,
    .num_args=2,
    .flags=0,
    .func=command_update_digital_out
}, {
    // queue_digital_out oid=%c clock=%u on_ticks=%u
    .msg_id=15,
    .num_params=3,
    .param_types = command_parameters18,
    .num_args=3,
    .flags=0,
    .func=command_queue_digital_out
}, {
    // set_digital_out_pwm_cycle oid=%c cycle_ticks=%u
    .msg_id=16,
    .num_params=2,
    .param_types = command_parameters19,
    .num_args=2,
    .flags=0,
    .func=command_set_digital_out_pwm_cycle
}, {
    // config_digital_out oid=%c pin=%u value=%c default_value=%c max_duration=%u
    .msg_id=17,
    .num_params=5,
    .param_types = command_parameters20,
    .num_args=5,
    .flags=0,
    .func=command_config_digital_out
}, {
    // stepper_stop_on_trigger oid=%c trsync_oid=%c
    .msg_id=18,
    .num_params=2,
    .param_types = command_parameters14,
    .num_args=2,
    .flags=0,
    .func=command_stepper_stop_on_trigger
}, {
    // stepper_get_position oid=%c
    .msg_id=19,
    .num_params=1,
    .param_types = command_parameters17,
    .num_args=1,
    .flags=0,
    .func=command_stepper_get_position
}, {
    // reset_step_clock oid=%c clock=%u
    .msg_id=20,
    .num_params=2,
    .param_types = command_parameters19,
    .num_args=2,
    .flags=0,
    .func=command_reset_step_clock
}, {
    // set_next_step_dir oid=%c dir=%c
    .msg_id=21,
    .num_params=2,
    .param_types = command_parameters14,
    .num_args=2,
    .flags=0,
    .func=command_set_next_step_dir
}, {
    // queue_step oid=%c interval=%u count=%hu add=%hi
    .msg_id=22,
    .num_params=4,
    .param_types = command_parameters21,
    .num_args=4,
    .flags=0,
    .func=command_queue_step
}, {
    // config_stepper oid=%c step_pin=%c dir_pin=%c invert_step=%c step_pulse_ticks=%u
    .msg_id=23,
    .num_params=5,
    .param_types = command_parameters22,
    .num_args=5,
    .flags=0,
    .func=command_config_stepper
}, {
    // endstop_query_state oid=%c
    .msg_id=24,
    .num_params=1,
    .param_types = command_parameters17,
    .num_args=1,
    .flags=0,
    .func=command_endstop_query_state
}, {
    // endstop_home oid=%c clock=%u sample_ticks=%u sample_count=%c rest_ticks=%u pin_value=%c trsync_oid=%c trigger_reason=%c
    .msg_id=25,
    .num_params=8,
    .param_types = command_parameters23,
    .num_args=8,
    .flags=0,
    .func=command_endstop_home
}, {
    // config_endstop oid=%c pin=%c pull_up=%c
    .msg_id=26,
    .num_params=3,
    .param_types = command_parameters24,
    .num_args=3,
    .flags=0,
    .func=command_config_endstop
}, {
    // trsync_trigger oid=%c reason=%c
    .msg_id=27,
    .num_params=2,
    .param_types = command_parameters14,
    .num_args=2,
    .flags=0,
    .func=command_trsync_trigger
}, {
    // trsync_set_timeout oid=%c clock=%u
    .msg_id=28,
    .num_params=2,
    .param_types = command_parameters19,
    .num_args=2,
    .flags=0,
    .func=command_trsync_set_timeout
}, {
    // trsync_start oid=%c report_clock=%u report_ticks=%u expire_reason=%c
    .msg_id=29,
    .num_params=4,
    .param_types = command_parameters25,
    .num_args=4,
    .flags=0,
    .func=command_trsync_start
}, {
    // config_trsync oid=%c
    .msg_id=30,
    .num_params=1,
    .param_types = command_parameters17,
    .num_args=1,
    .flags=0,
    .func=command_config_trsync
}, {
    // query_analog_in oid=%c clock=%u sample_ticks=%u sample_count=%c rest_ticks=%u min_value=%hu max_value=%hu range_check_count=%c
    .msg_id=31,
    .num_params=8,
    .param_types = command_parameters26,
    .num_args=8,
    .flags=0,
    .func=command_query_analog_in
}, {
    // config_analog_in oid=%c pin=%u
    .msg_id=32,
    .num_params=2,
    .param_types = command_parameters19,
    .num_args=2,
    .flags=0,
    .func=command_config_analog_in
}, {
    // buttons_ack oid=%c count=%c
    .msg_id=33,
    .num_params=2,
    .param_types = command_parameters14,
    .num_args=2,
    .flags=0,
    .func=command_buttons_ack
}, {
    // buttons_query oid=%c clock=%u rest_ticks=%u retransmit_count=%c invert=%c
    .msg_id=34,
    .num_params=5,
    .param_types = command_parameters27,
    .num_args=5,
    .flags=0,
    .func=command_buttons_query
}, {
    // buttons_add oid=%c pos=%c pin=%u pull_up=%c
    .msg_id=35,
    .num_params=4,
    .param_types = command_parameters9,
    .num_args=4,
    .flags=0,
    .func=command_buttons_add
}, {
    // config_buttons oid=%c button_count=%c
    .msg_id=36,
    .num_params=2,
    .param_types = command_parameters14,
    .num_args=2,
    .flags=0,
    .func=command_config_buttons
}, {
    // tmcuart_send oid=%c write=%*s read=%c
    .msg_id=37,
    .num_params=3,
    .param_types = command_parameters28,
    .num_args=4,
    .flags=0,
    .func=command_tmcuart_send
}, {
    // config_tmcuart oid=%c rx_pin=%u pull_up=%c tx_pin=%u bit_time=%u
    .msg_id=38,
    .num_params=5,
    .param_types = command_parameters29,
    .num_args=5,
    .flags=0,
    .func=command_config_tmcuart
}, {
    // neopixel_send oid=%c
    .msg_id=39,
    .num_params=1,
    .param_types = command_parameters17,
    .num_args=1,
    .flags=0,
    .func=command_neopixel_send
}, {
    // neopixel_update oid=%c pos=%hu data=%*s
    .msg_id=40,
    .num_params=3,
    .param_types = command_parameters30,
    .num_args=4,
    .flags=0,
    .func=command_neopixel_update
}, {
    // config_neopixel oid=%c pin=%u data_size=%hu bit_max_ticks=%u reset_min_ticks=%u
    .msg_id=41,
    .num_params=5,
    .param_types = command_parameters31,
    .num_args=5,
    .flags=0,
    .func=command_config_neopixel
}, {
    // query_counter oid=%c clock=%u poll_ticks=%u sample_ticks=%u
    .msg_id=42,
    .num_params=4,
    .param_types = command_parameters15,
    .num_args=4,
    .flags=0,
    .func=command_query_counter
}, {
    // config_counter oid=%c pin=%u pull_up=%c
    .msg_id=43,
    .num_params=3,
    .param_types = command_parameters32,
    .num_args=3,
    .flags=0,
    .func=command_config_counter
}, {
    // st7920_send_data oid=%c data=%*s
    .msg_id=44,
    .num_params=2,
    .param_types = command_parameters13,
    .num_args=3,
    .flags=0,
    .func=command_st7920_send_data
}, {
    // st7920_send_cmds oid=%c cmds=%*s
    .msg_id=45,
    .num_params=2,
    .param_types = command_parameters13,
    .num_args=3,
    .flags=0,
    .func=command_st7920_send_cmds
}, {
    // config_st7920 oid=%c cs_pin=%u sclk_pin=%u sid_pin=%u sync_delay_ticks=%u cmd_delay_ticks=%u
    .msg_id=46,
    .num_params=6,
    .param_types = command_parameters33,
    .num_args=6,
    .flags=0,
    .func=command_config_st7920
}, {
    // hd44780_send_data oid=%c data=%*s
    .msg_id=47,
    .num_params=2,
    .param_types = command_parameters13,
    .num_args=3,
    .flags=0,
    .func=command_hd44780_send_data
}, {
    // hd44780_send_cmds oid=%c cmds=%*s
    .msg_id=48,
    .num_params=2,
    .param_types = command_parameters13,
    .num_args=3,
    .flags=0,
    .func=command_hd44780_send_cmds
}, {
    // config_hd44780 oid=%c rs_pin=%u e_pin=%u d4_pin=%u d5_pin=%u d6_pin=%u d7_pin=%u delay_ticks=%u
    .msg_id=49,
    .num_params=8,
    .param_types = command_parameters34,
    .num_args=8,
    .flags=0,
    .func=command_config_hd44780
}, {
    // config_reset
    .msg_id=50,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_config_reset
},
};

const uint8_t command_index_size PROGMEM = ARRAY_SIZE(command_index);

// version: v0.12.0-158-g36f9b26e-dirty-20240413_093852-raspberrypi
// build_versions: gcc: (Debian 10.2.1-6) 10.2.1 20210110 binutils: (GNU Binutils for Debian) 2.35.2

const uint8_t command_identify_data[] PROGMEM = {
    0x78, 0xda, 0x9d, 0x58, 0x6b, 0x6f, 0xe3, 0xb6,
    0x12, 0xfd, 0x2b, 0x84, 0x81, 0x45, 0x1f, 0x48,
    0x72, 0x2d, 0xd9, 0x92, 0xed, 0x00, 0xfb, 0x21,
    0x4d, 0xb7, 0x8b, 0xa2, 0x4d, 0x5f, 0xde, 0x05,
    0x0a, 0x5c, 0x5c, 0x10, 0x8a, 0x44, 0xdb, 0x44,
    0xf4, 0x2a, 0x49, 0x39, 0x71, 0x8b, 0xfc, 0xf7,
    0x9e, 0x21, 0x25, 0x52, 0x96, 0xd3, 0x7e, 0xb8,
    0xf9, 0x62, 0x49, 0x33, 0x9c, 0x17, 0xcf, 0x0c,
    0x0f, 0xf3, 0xd7, 0x2c, 0x6b, 0xdb, 0xd9, 0xed,
    0xec, 0x87, 0x52, 0xb6, 0xad, 0x50, 0xb3, 0xab,
    0xd9, 0x63, 0x27, 0xcb, 0x82, 0x1f, 0x85, 0xd2,
    0xb2, 0xa9, 0x35, 0x44, 0xfb, 0x3c, 0xbf, 0x65,
    0x5f, 0x7e, 0x2b, 0x1e, 0x65, 0x56, 0xb3, 0x68,
    0x7e, 0x13, 0xdf, 0x44, 0xd7, 0xe9, 0x57, 0xfd,
    0x13, 0x8b, 0xe7, 0x71, 0x34, 0x8f, 0xa2, 0x39,
    0x7b, 0x94, 0x75, 0x67, 0x64, 0xa9, 0xa1, 0xfb,
    0xf1, 0xa7, 0xcf, 0xec, 0x9b, 0xfe, 0x95, 0xed,
    0x1a, 0xc5, 0xdc, 0xe2, 0xaf, 0x58, 0x7c, 0xb3,
    0x48, 0x6e, 0x62, 0x38, 0xc9, 0x9b, 0xaa, 0xca,
    0xea, 0x02, 0xe6, 0xff, 0x9a, 0x65, 0x65, 0xd9,
    0xe4, 0x99, 0x11, 0xbc, 0x91, 0x85, 0x66, 0x79,
    0xd3, 0xd5, 0xe6, 0xfd, 0xbb, 0x7c, 0x76, 0xbb,
    0xa6, 0x58, 0x8c, 0x41, 0x10, 0x3c, 0xcb, 0x9f,
    0x18, 0xa4, 0xf8, 0x3c, 0x92, 0x2f, 0x16, 0x23,
    0x85, 0xa2, 0x18, 0x14, 0xda, 0x46, 0xdb, 0x1f,
    0x59, 0xbf, 0x7f, 0xd7, 0xb1, 0xb6, 0x2b, 0x4b,
    0xde, 0xb5, 0x6e, 0x41, 0x12, 0x16, 0xfc, 0xd1,
    0x09, 0x75, 0xf2, 0x36, 0x11, 0xc0, 0x13, 0x69,
    0x2b, 0xa1, 0x0d, 0x37, 0x32, 0x7f, 0xd2, 0xee,
    0xcd, 0xa8, 0xac, 0xd6, 0x95, 0x34, 0x7c, 0xf0,
    0xca, 0x64, 0x8d, 0xca, 0xf4, 0xfe, 0x97, 0xc8,
    0xa3, 0x14, 0x99, 0xe2, 0xfa, 0xd0, 0x99, 0xa2,
    0x79, 0xae, 0x67, 0xb7, 0x31, 0xa5, 0x56, 0xef,
    0xe4, 0x9e, 0x67, 0x75, 0x56, 0x36, 0x7b, 0x2e,
    0x6b, 0x1f, 0x97, 0x0d, 0x08, 0xcb, 0x82, 0x4e,
    0x1f, 0xcc, 0xa0, 0xe1, 0x5e, 0xf9, 0x28, 0xc3,
    0xd4, 0xab, 0xda, 0x8f, 0x42, 0x9d, 0x1b, 0x3b,
    0xcb, 0x6e, 0xb9, 0xf0, 0xca, 0x85, 0xdc, 0x4b,
    0x93, 0x95, 0xbc, 0xe9, 0xcc, 0x64, 0xc1, 0x31,
    0x2b, 0x3b, 0x41, 0xef, 0x85, 0xd8, 0x65, 0x5d,
    0x69, 0xb8, 0xff, 0x50, 0x65, 0x2f, 0xbc, 0xe8,
    0x54, 0x66, 0xb0, 0xeb, 0x36, 0xce, 0x68, 0xe5,
    0xed, 0x09, 0xec, 0x94, 0x69, 0xda, 0x33, 0x5b,
    0xf9, 0x99, 0xf3, 0x38, 0x44, 0x7a, 0x28, 0x96,
    0xcb, 0xd5, 0x7a, 0x3e, 0x28, 0x2b, 0xcd, 0x7b,
    0xdf, 0x62, 0x78, 0x28, 0x96, 0xfe, 0x29, 0xf1,
    0x4f, 0xa9, 0x7f, 0x5a, 0xf9, 0x27, 0x51, 0x66,
    0x27, 0xbf, 0x1d, 0xc8, 0x70, 0xe3, 0x9d, 0xd4,
    0xa2, 0x69, 0xe5, 0x8b, 0x28, 0x27, 0xe9, 0x15,
    0x99, 0xc9, 0xb8, 0x96, 0x7f, 0x22, 0xa3, 0x43,
    0x07, 0x44, 0x1a, 0x4e, 0x69, 0x8d, 0x36, 0x54,
    0x0b, 0x7c, 0x92, 0xf5, 0xd8, 0x68, 0xe4, 0x8d,
    0x5a, 0xf1, 0xec, 0x36, 0x99, 0xfb, 0x2f, 0xda,
    0xac, 0x36, 0xb1, 0x4f, 0x25, 0xf7, 0xa9, 0xe8,
    0xbc, 0x7c, 0xf2, 0xcf, 0xb2, 0xf0, 0x8f, 0xa7,
    0x3a, 0xe7, 0xe7, 0x51, 0xb3, 0xbc, 0x2a, 0xf8,
    0x45, 0x22, 0xe9, 0xc8, 0x83, 0xa0, 0xbe, 0x1b,
    0x5c, 0xd0, 0x2b, 0xef, 0xeb, 0x5b, 0x48, 0x35,
    0x3c, 0x3a, 0xd4, 0x59, 0xe5, 0xa0, 0xd5, 0x95,
    0x5a, 0x8c, 0x8c, 0xc6, 0x61, 0xff, 0x4d, 0x95,
    0x77, 0x99, 0xf2, 0x7b, 0xaf, 0x5e, 0xf8, 0x05,
    0x5e, 0x98, 0xf1, 0x1f, 0xa9, 0x4c, 0x46, 0x56,
    0xc2, 0xa1, 0x73, 0x1d, 0xac, 0x28, 0xca, 0xa7,
    0x37, 0x02, 0x09, 0xca, 0x52, 0x88, 0xc7, 0x0e,
    0xc5, 0x6f, 0x30, 0x31, 0x36, 0xc3, 0x1b, 0xac,
    0xec, 0x6d, 0xe1, 0xdf, 0xbf, 0xfb, 0x1a, 0xfd,
    0x1c, 0x79, 0x35, 0x25, 0x32, 0x34, 0xa5, 0x2a,
    0x84, 0x22, 0x7f, 0xe8, 0x50, 0xe5, 0x70, 0x15,
    0x0f, 0x0a, 0xcf, 0x4a, 0x1a, 0x71, 0xa1, 0x41,
    0x18, 0x75, 0x8a, 0xd8, 0x19, 0x51, 0x09, 0xb5,
    0x17, 0x75, 0x7e, 0xe2, 0x04, 0x40, 0x04, 0x81,
    0x4f, 0x0e, 0x8c, 0xfc, 0xd0, 0x54, 0xe2, 0xa2,
    0x81, 0x75, 0x56, 0xb5, 0x65, 0xa8, 0xca, 0xf0,
    0xee, 0xdb, 0xf7, 0xbc, 0xc1, 0x11, 0x7a, 0x68,
    0x00, 0x97, 0x2e, 0xef, 0x2d, 0x1a, 0x25, 0xf7,
    0x7b, 0xa1, 0x28, 0x09, 0x4d, 0x0d, 0x41, 0x18,
    0x4f, 0x82, 0x73, 0x3b, 0x3e, 0x10, 0x53, 0x66,
    0x84, 0x2f, 0x50, 0x8c, 0x79, 0xb0, 0x93, 0x68,
    0x7b, 0xe0, 0x8f, 0xbb, 0x1a, 0xb2, 0x5c, 0xe5,
    0x36, 0x17, 0x6c, 0xf8, 0x1e, 0xd0, 0xb3, 0x71,
    0x02, 0x61, 0xfd, 0x9b, 0xd5, 0x99, 0xdd, 0xae,
    0xdc, 0x6b, 0xd7, 0xd2, 0x2e, 0x00, 0x1d, 0x57,
    0xb3, 0xbe, 0x87, 0xb8, 0x86, 0x3f, 0x0e, 0x04,
    0xf9, 0x11, 0x41, 0xcf, 0xae, 0xce, 0xcb, 0xf5,
    0x44, 0x8d, 0xb6, 0x60, 0x50, 0x0b, 0xdb, 0xb1,
    0x84, 0x71, 0x59, 0x88, 0xda, 0xc8, 0x1d, 0xc6,
    0xdd, 0x6e, 0x07, 0x84, 0x5b, 0x54, 0xfa, 0x19,
    0x83, 0x22, 0x0f, 0xcd, 0x64, 0xed, 0x84, 0xfd,
    0xde, 0x8c, 0x24, 0x5d, 0x5b, 0x84, 0x54, 0xdd,
    0x88, 0x3d, 0x74, 0x63, 0x37, 0xd8, 0x75, 0x57,
    0x93, 0x8b, 0xb9, 0xf7, 0x7f, 0xee, 0x4d, 0x15,
    0xf6, 0x06, 0x9e, 0xa8, 0x8d, 0xc3, 0x1b, 0xa6,
    0xf2, 0x1e, 0x0b, 0x0f, 0x22, 0x7f, 0x1a, 0x4f,
    0xcb, 0x68, 0x88, 0x61, 0x32, 0x2c, 0x7d, 0x04,
    0x6d, 0x03, 0xf8, 0x4f, 0xfd, 0x8f, 0xda, 0x32,
    0xb6, 0x06, 0x3a, 0xf1, 0xd6, 0x00, 0xf5, 0x46,
    0x9a, 0xf1, 0xf4, 0x88, 0x92, 0x61, 0x09, 0xf5,
    0xe4, 0xa0, 0x2b, 0xc9, 0xbb, 0x83, 0xf1, 0x50,
    0x69, 0x44, 0x0d, 0x7c, 0xe3, 0x57, 0x02, 0x28,
    0xf0, 0xe3, 0x46, 0x91, 0x6d, 0x64, 0x6b, 0x79,
    0xea, 0x06, 0x5a, 0x28, 0x29, 0xe9, 0x8c, 0x63,
    0x99, 0x4c, 0x71, 0xf8, 0x5f, 0x5c, 0x28, 0xf1,
    0xf6, 0xb9, 0xe2, 0xf9, 0x09, 0x27, 0x93, 0xb7,
    0x49, 0x2f, 0xe3, 0xa0, 0x53, 0xb7, 0xa8, 0x16,
    0x2f, 0x7d, 0x08, 0x18, 0x35, 0x1e, 0x3a, 0x52,
    0x39, 0x34, 0xa3, 0x9a, 0x6e, 0xfc, 0xfd, 0x2b,
    0x0a, 0x93, 0x73, 0xad, 0x7f, 0x00, 0xe1, 0x92,
    0xb4, 0xec, 0xa0, 0xe3, 0x84, 0x74, 0xe0, 0x47,
    0xd2, 0x39, 0xe3, 0xb1, 0x16, 0x6d, 0x82, 0x82,
    0xed, 0x2f, 0x2a, 0xb2, 0xeb, 0x40, 0xe6, 0x1b,
    0x72, 0xd4, 0x9e, 0x58, 0x01, 0xf8, 0xf7, 0x63,
    0x6e, 0x0c, 0x5b, 0x66, 0x67, 0x0a, 0x79, 0x65,
    0x34, 0x7e, 0x1c, 0x2e, 0xd0, 0x02, 0xfd, 0x6a,
    0xca, 0x9a, 0x9a, 0xec, 0x8d, 0x7d, 0x45, 0xc6,
    0xeb, 0xa0, 0x67, 0xc6, 0xe3, 0x53, 0xb4, 0x8d,
    0xea, 0x7b, 0xd7, 0x9d, 0x22, 0xf6, 0xdd, 0xe3,
    0x48, 0xbc, 0xb4, 0x52, 0x89, 0xb3, 0x49, 0xb1,
    0xf1, 0x96, 0x26, 0x59, 0x8c, 0x95, 0x10, 0x96,
    0x6b, 0xab, 0xb7, 0xe0, 0x36, 0xda, 0xe2, 0xe5,
    0xeb, 0x30, 0x92, 0x89, 0x2d, 0xdd, 0x7d, 0x7b,
    0xcf, 0x1f, 0xee, 0x7e, 0xa7, 0x96, 0xdb, 0xa0,
    0xf8, 0xf7, 0x3f, 0xfe, 0x7c, 0xff, 0x03, 0xff,
    0xee, 0xb7, 0x0f, 0xbf, 0xd2, 0xc1, 0xe5, 0xfe,
    0xae, 0x66, 0x0f, 0xf7, 0x9f, 0x41, 0xdc, 0x4a,
    0x50, 0xb0, 0x17, 0x90, 0xad, 0xed, 0xa7, 0xbb,
    0x4f, 0x5b, 0xbe, 0xfd, 0xfc, 0xb0, 0xfd, 0x95,
    0x7f, 0x73, 0xb7, 0xfd, 0x40, 0xa3, 0x2c, 0x7d,
    0xa5, 0x61, 0xd6, 0x61, 0xbc, 0xda, 0x13, 0xdf,
    0x12, 0x31, 0xa0, 0xcb, 0xf2, 0x31, 0xdb, 0xc1,
    0xf3, 0xd9, 0xed, 0x7f, 0xe1, 0x22, 0xbd, 0x5a,
    0xff, 0x0f, 0xe3, 0xa9, 0x95, 0x0d, 0x7d, 0x98,
    0x5f, 0xc5, 0xeb, 0xe1, 0x3d, 0x3f, 0xc8, 0x76,
    0xfe, 0x9f, 0x7f, 0x92, 0x44, 0x5e, 0x82, 0xef,
    0x13, 0x59, 0xec, 0x65, 0xc9, 0x2a, 0x9d, 0xc8,
    0x16, 0x5e, 0xb6, 0x4e, 0x97, 0x13, 0xd9, 0xd2,
    0xcb, 0xa2, 0x28, 0x89, 0x27, 0xc2, 0x24, 0x08,
    0x97, 0xcb, 0x69, 0x34, 0x69, 0x10, 0xae, 0xe2,
    0x69, 0x38, 0xab, 0x10, 0xea, 0x3c, 0x9a, 0xc6,
    0xb3, 0x0e, 0xc2, 0xc5, 0xdc, 0x05, 0xf4, 0x4a,
    0x58, 0x45, 0xd5, 0x08, 0x26, 0x0a, 0x87, 0x1f,
    0x97, 0x45, 0xbf, 0x31, 0xcc, 0x6e, 0xe0, 0xce,
    0xcd, 0x27, 0x47, 0x8a, 0xee, 0x4a, 0xc2, 0xe1,
    0x89, 0x0d, 0x07, 0x43, 0xe1, 0x7a, 0xf6, 0x1e,
    0x54, 0xba, 0x6e, 0x0c, 0x03, 0x20, 0x99, 0x6e,
    0x76, 0xe8, 0xec, 0xe7, 0xca, 0xf5, 0x29, 0xb3,
    0xb8, 0x62, 0xcf, 0x07, 0x89, 0x67, 0x87, 0x0f,
    0xcd, 0x5a, 0x20, 0x1c, 0x9e, 0xdc, 0x50, 0xc0,
    0xda, 0x2f, 0x0c, 0x4d, 0x13, 0x50, 0x8f, 0x3d,
    0xac, 0x32, 0x73, 0xc8, 0x0c, 0x93, 0x9a, 0x65,
    0xbd, 0xaf, 0x2c, 0x37, 0xf2, 0x28, 0xdc, 0x81,
    0xd5, 0x2b, 0x6b, 0x52, 0x25, 0x6c, 0xb9, 0x63,
    0xd5, 0x7d, 0xb5, 0x33, 0x88, 0x51, 0x3f, 0xc0,
    0x9d, 0xa8, 0xd9, 0xc0, 0x45, 0xfc, 0x7a, 0xcc,
    0xa9, 0x7b, 0x47, 0xd4, 0x59, 0x9b, 0x29, 0x0d,
    0x91, 0x50, 0xaa, 0x51, 0xf6, 0xc4, 0x1a, 0x04,
    0x4a, 0x60, 0xfe, 0x69, 0x63, 0xe9, 0xfa, 0x07,
    0x92, 0x62, 0x40, 0x32, 0x07, 0x21, 0xdb, 0x82,
    0x8e, 0x76, 0x7f, 0xd7, 0xa8, 0x5c, 0xb0, 0x81,
    0x27, 0xb3, 0x9e, 0xfe, 0x3b, 0x0e, 0xfd, 0xf1,
    0x97, 0xef, 0x7f, 0x66, 0x54, 0x6b, 0x10, 0xbe,
    0xa3, 0x84, 0x1a, 0x15, 0x66, 0x87, 0xb1, 0x59,
    0xb8, 0x73, 0xe5, 0xfb, 0x1a, 0xcd, 0x20, 0x0b,
    0x36, 0xb0, 0xe5, 0x40, 0xcb, 0xdd, 0x70, 0x75,
    0xad, 0x34, 0x68, 0x79, 0xd3, 0xc9, 0xf8, 0x1b,
    0xd4, 0x28, 0x85, 0xac, 0x12, 0x98, 0xcc, 0x8e,
    0x2b, 0x0d, 0xc2, 0xaa, 0x39, 0x8a, 0x21, 0x0b,
    0x46, 0xdc, 0x91, 0x5a, 0x2e, 0x88, 0x3d, 0xd3,
    0xf4, 0xdc, 0xd2, 0x51, 0xf8, 0x0b, 0x79, 0x7f,
    0x44, 0x86, 0xd4, 0xa2, 0xa0, 0x84, 0xc2, 0x33,
    0x73, 0x6a, 0x85, 0x63, 0x3f, 0x0f, 0xd9, 0x0b,
    0xa1, 0x64, 0x3d, 0x64, 0xe4, 0x06, 0xc6, 0x83,
    0xd0, 0x3a, 0xdb, 0x0b, 0x06, 0xb6, 0xd3, 0x14,
    0x62, 0xa8, 0x34, 0x20, 0xf4, 0x20, 0xb5, 0x16,
    0xd8, 0x6c, 0x9c, 0x77, 0x45, 0x57, 0x12, 0xdb,
    0xc2, 0x62, 0x9a, 0xdd, 0xac, 0x9f, 0x1b, 0x16,
    0x76, 0xe2, 0x28, 0x6c, 0x29, 0xe0, 0xf5, 0x81,
    0x32, 0xb2, 0xa7, 0x12, 0xc3, 0x93, 0xda, 0x95,
    0xcd, 0xb3, 0x3b, 0xa9, 0x7e, 0x13, 0xbd, 0x11,
    0x98, 0xa3, 0x6d, 0x57, 0x38, 0xa9, 0x80, 0x1e,
    0x81, 0xd2, 0xd0, 0x0e, 0xd2, 0x85, 0x6a, 0xeb,
    0x15, 0x2e, 0x8c, 0xb3, 0x67, 0x59, 0x96, 0x98,
    0x76, 0xb9, 0x80, 0x74, 0x7c, 0x5d, 0x70, 0xf3,
    0x7b, 0x0b, 0x2c, 0xb9, 0x84, 0xac, 0x39, 0xd2,
    0x90, 0x55, 0x57, 0x0d, 0xdf, 0x86, 0xad, 0x02,
    0x4c, 0xb6, 0x1e, 0x06, 0x74, 0x7b, 0x82, 0x31,
    0x8b, 0x3e, 0xdb, 0x0e, 0xe3, 0x8b, 0xd4, 0xb6,
    0x87, 0xa3, 0x69, 0x1a, 0xb6, 0xcb, 0x6c, 0xac,
    0x2e, 0x4e, 0x22, 0x5a, 0x9f, 0x6c, 0xf8, 0x24,
    0xc2, 0x5c, 0xd6, 0xc2, 0x32, 0xc3, 0xcf, 0x75,
    0xf6, 0x48, 0x0d, 0xd4, 0xb0, 0x06, 0x0d, 0xc3,
    0xa6, 0xb8, 0x72, 0x94, 0x66, 0xa2, 0x94, 0x15,
    0x79, 0x10, 0xa7, 0x17, 0x62, 0x39, 0x36, 0x83,
    0xda, 0x0b, 0x77, 0x9c, 0x4c, 0xb4, 0xa8, 0x44,
    0x17, 0x6a, 0x48, 0xd4, 0xde, 0x6c, 0xc1, 0x53,
    0xba, 0xfa, 0x09, 0x19, 0xa0, 0xa1, 0x0b, 0x77,
    0xad, 0x1a, 0x7d, 0xd7, 0x41, 0x90, 0x9e, 0x5f,
    0x44, 0xd0, 0x47, 0x25, 0x7a, 0xf9, 0x08, 0xb1,
    0x75, 0xe6, 0x5a, 0xd4, 0x17, 0x08, 0xa9, 0xd8,
    0xfb, 0xb2, 0x6f, 0xfa, 0xfe, 0x16, 0x5d, 0x38,
    0x0a, 0xae, 0x89, 0xf2, 0xd5, 0x80, 0xa5, 0xdb,
    0xe7, 0x1c, 0x62, 0x12, 0x2d, 0xfd, 0xa9, 0x69,
    0xf1, 0x6c, 0x0b, 0x58, 0x66, 0x8a, 0xa6, 0x15,
    0xf1, 0xfb, 0x37, 0xce, 0x23, 0xda, 0x16, 0x07,
    0xe2, 0x67, 0x69, 0x0e, 0xfd, 0x6c, 0x70, 0xe8,
    0xa2, 0x53, 0xf8, 0x15, 0xd3, 0xb0, 0x44, 0xf5,
    0x6a, 0xda, 0x83, 0x19, 0x5d, 0xfc, 0x3f, 0xfe,
    0xf2, 0xe3, 0x71, 0x31, 0xb3, 0x0c, 0xa7, 0x05,
    0xb8, 0x85, 0x0e, 0x67, 0x0a, 0x58, 0xe1, 0x19,
    0x67, 0xb6, 0x28, 0x0e, 0x07, 0xab, 0x27, 0x78,
    0x80, 0xfd, 0xe8, 0x6e, 0x7f, 0xb6, 0x22, 0x1b,
    0x51, 0x3e, 0x66, 0x25, 0x8e, 0x62, 0xa4, 0xf6,
    0x32, 0x4e, 0x44, 0x2a, 0x1c, 0xe8, 0x89, 0xaf,
    0x28, 0xa6, 0x63, 0xcf, 0xb6, 0xed, 0x91, 0x6f,
    0x49, 0x39, 0x7d, 0x1b, 0xca, 0x69, 0x2f, 0xc0,
    0xe8, 0x15, 0xee, 0xd9, 0x1a, 0x56, 0xdb, 0xfb,
    0xaf, 0x25, 0x92, 0xff, 0x16, 0x74, 0xbf, 0xa2,
    0x7f, 0xe0, 0xc1, 0x7b, 0xba, 0x0a, 0x17, 0x21,
    0x8d, 0xab, 0xb6, 0xbf, 0xd7, 0x24, 0x9b, 0x70,
    0x8f, 0x38, 0x33, 0x8c, 0x1b, 0x0d, 0xba, 0xfb,
    0xd2, 0xc5, 0xf8, 0x9a, 0x02, 0xbb, 0x51, 0x60,
    0xf4, 0x7c, 0x28, 0xf2, 0x88, 0xda, 0x3b, 0xda,
    0x75, 0x43, 0x45, 0xc1, 0x8e, 0x8e, 0x92, 0x64,
    0xd3, 0x53, 0xab, 0xcf, 0x33, 0x1e, 0xf1, 0xfc,
    0x3e, 0xd4, 0xe1, 0x36, 0xda, 0xe5, 0x39, 0xa6,
    0x92, 0x73, 0x8b, 0x62, 0xc2, 0xd5, 0xf8, 0xaa,
    0x97, 0x00, 0xe2, 0x61, 0xa4, 0x7b, 0x9a, 0xff,
    0xb6, 0x97, 0x85, 0x3d, 0x35, 0x95, 0xb1, 0xc7,
    0x58, 0x12, 0xb9, 0x33, 0x54, 0x87, 0xf2, 0xe9,
    0xae, 0xea, 0x7f, 0xf4, 0x1f, 0xae, 0x4c, 0x23,
    0xd2, 0x38, 0x21, 0x8c, 0xee, 0x02, 0x02, 0x26,
    0x9d, 0xce, 0x03, 0x0b, 0x0c, 0xa5, 0xf0, 0x54,
    0xab, 0xe8, 0xb1, 0x91, 0x8c, 0x89, 0x5d, 0xa8,
    0x77, 0x9e, 0x79, 0x96, 0xf9, 0xe6, 0x95, 0x6f,
    0x04, 0xa5, 0x34, 0xa6, 0xe6, 0xb0, 0x47, 0xe5,
    0x41, 0xee, 0x0f, 0x76, 0xbf, 0x03, 0xce, 0x12,
    0x34, 0x41, 0xff, 0x0f, 0x33, 0x34, 0xc1, 0x71,
    0x7e, 0x13, 0xc5, 0x37, 0xf3, 0xeb, 0x28, 0x59,
    0x5f, 0xef, 0x17, 0xe9, 0x6e, 0xf3, 0x18, 0xa7,
    0xe2, 0x1a, 0xbc, 0xda, 0x9c, 0xae, 0xe3, 0x79,
    0xbc, 0x9c, 0x2f, 0xa3, 0x05, 0x9f, 0x6f, 0x16,
    0xeb, 0x24, 0xbe, 0x56, 0x99, 0x6e, 0x1f, 0x31,
    0xea, 0x4f, 0xad, 0x9c, 0xbd, 0xfe, 0x0d, 0x35,
    0x2a, 0x93, 0x9f,
};

// Identify size = 1939 (5004 uncompressed)
const uint32_t command_identify_size PROGMEM
    = ARRAY_SIZE(command_identify_data);
