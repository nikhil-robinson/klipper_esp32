
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
    extern void serial_init(void);
    serial_init();
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
    extern void clear_active_irq(void);
    clear_active_irq();
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
    extern void timer_task(void);
    timer_task();
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
    if (__builtin_strcmp(str, "Not an input pin") == 0)
        return 33;
    if (__builtin_strcmp(str, "Not an output pin") == 0)
        return 34;
    if (__builtin_strcmp(str, "Not a valid ADC pin") == 0)
        return 35;
    if (__builtin_strcmp(str, "Rescheduled timer in the past") == 0)
        return 36;
    return 0xff;
}

const struct initial_pin_s initial_pins[] PROGMEM = {
};
const int initial_pins_size PROGMEM = ARRAY_SIZE(initial_pins);

extern void DefaultHandler(void);
extern uint32_t _stack_end;
extern void ResetHandler(void);

const void *VectorTable[] __visible __section(".vector_table") = {
    &_stack_end,
    ResetHandler,
};

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
extern void command_reset(uint32_t*);
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
    // reset
    .msg_id=50,
    .num_params=0,
    .param_types = 0,
    .num_args=0,
    .flags=0x01,
    .func=command_reset
},
};

const uint8_t command_index_size PROGMEM = ARRAY_SIZE(command_index);

// version: v0.12.0-159-g2425a746-20240417_140117-pop-os
// build_versions: gcc:  binutils: 

const uint8_t command_identify_data[] PROGMEM = {
    0x78, 0xda, 0x9d, 0x58, 0xdb, 0x6e, 0x23, 0x37,
    0x12, 0xfd, 0x15, 0x42, 0xc0, 0x60, 0x81, 0xc0,
    0x1e, 0xe8, 0x2e, 0xcb, 0xc0, 0x3c, 0x38, 0x1e,
    0x65, 0x62, 0x64, 0x34, 0x33, 0xb1, 0xec, 0x4d,
    0x80, 0x45, 0x40, 0x70, 0xba, 0x29, 0x89, 0x70,
    0xdf, 0x42, 0xb2, 0x7d, 0xd9, 0xc0, 0xff, 0xbe,
    0xa7, 0xc8, 0x26, 0xd9, 0x92, 0x9c, 0x3c, 0xac,
    0x5f, 0xc4, 0x26, 0x8b, 0x55, 0xc5, 0xaa, 0x53,
    0xc5, 0x43, 0xff, 0x35, 0x10, 0x4d, 0x33, 0xb8,
    0x1c, 0xfc, 0x52, 0xa8, 0xa6, 0x91, 0x7a, 0x70,
    0x36, 0xf8, 0xde, 0xaa, 0x22, 0xe7, 0x8f, 0x52,
    0x1b, 0x55, 0x57, 0x06, 0x4b, 0xbb, 0x2c, 0xbb,
    0x64, 0xec, 0xbb, 0xaa, 0x5a, 0xab, 0x0a, 0x73,
    0xc9, 0x20, 0x93, 0xd5, 0x65, 0x29, 0xaa, 0x1c,
    0xab, 0x7f, 0x0d, 0x44, 0x51, 0xd4, 0x99, 0xb0,
    0x92, 0xd7, 0x2a, 0x37, 0x2c, 0xab, 0xdb, 0xca,
    0x7e, 0x78, 0x97, 0x0d, 0x2e, 0x2f, 0x48, 0x95,
    0xb5, 0xd0, 0xc1, 0x45, 0xf6, 0xc0, 0xb0, 0x8a,
    0xe9, 0xde, 0xfa, 0x64, 0xd2, 0x13, 0xc8, 0xf3,
    0x20, 0xd0, 0xd4, 0xc6, 0xfd, 0xa8, 0xea, 0xc3,
    0xbb, 0x96, 0x35, 0x6d, 0x51, 0xf0, 0xb6, 0xf1,
    0x1b, 0x66, 0x69, 0xc3, 0x9f, 0xad, 0xd4, 0x2f,
    0x51, 0x27, 0x1c, 0x78, 0x20, 0x69, 0x2d, 0x8d,
    0xe5, 0x56, 0x65, 0x0f, 0xc6, 0x7f, 0x59, 0x2d,
    0x2a, 0x53, 0x2a, 0xcb, 0x83, 0x55, 0xa6, 0x2a,
    0x1c, 0xac, 0xb3, 0x3f, 0xc5, 0x39, 0x0a, 0x29,
    0x34, 0x37, 0xfb, 0xd6, 0xe6, 0xf5, 0x53, 0x35,
    0xb8, 0x1c, 0xd3, 0xd1, 0xaa, 0xad, 0xda, 0x71,
    0x51, 0x89, 0xa2, 0xde, 0x71, 0x55, 0x45, 0xbf,
    0x9c, 0x43, 0xd8, 0x96, 0x64, 0x3a, 0x67, 0x82,
    0x84, 0xff, 0xe4, 0xbd, 0x13, 0xce, 0xa3, 0xa8,
    0x9b, 0x94, 0xfa, 0x50, 0xd9, 0xc1, 0xe9, 0xa6,
    0x93, 0x28, 0x9c, 0xab, 0x9d, 0xb2, 0xa2, 0xe0,
    0x75, 0x6b, 0x8f, 0x36, 0x3c, 0x8a, 0xa2, 0x95,
    0xf4, 0x9d, 0xcb, 0xad, 0x68, 0x0b, 0xcb, 0xe3,
    0x44, 0x29, 0x9e, 0x79, 0xde, 0x6a, 0x61, 0x91,
    0x34, 0xe7, 0xe7, 0x68, 0x11, 0xf5, 0x49, 0x64,
    0xca, 0xd6, 0xcd, 0x81, 0xae, 0xec, 0xc0, 0xf8,
    0x38, 0x79, 0xba, 0xcf, 0xa7, 0xd3, 0xc5, 0xc5,
    0x30, 0x08, 0x6b, 0xc3, 0x3b, 0xdb, 0x32, 0x0c,
    0xf2, 0x69, 0x1c, 0xcd, 0xe2, 0x68, 0x1e, 0x47,
    0x8b, 0x38, 0x92, 0x85, 0x78, 0x89, 0xe9, 0xc0,
    0x09, 0x97, 0xd1, 0x48, 0x25, 0xeb, 0x46, 0x3d,
    0xcb, 0xe2, 0xe8, 0x78, 0xb9, 0xb0, 0x82, 0x1b,
    0xf5, 0x5f, 0x9c, 0x68, 0xdf, 0x02, 0x71, 0x96,
    0xd3, 0xb1, 0x7a, 0x09, 0x35, 0x12, 0x53, 0xaa,
    0xea, 0x2b, 0x1d, 0x45, 0xa5, 0xc6, 0x2e, 0x96,
    0xe3, 0xe8, 0x78, 0x16, 0x1d, 0x37, 0x59, 0xf1,
    0x10, 0xc7, 0x2a, 0x8f, 0xc3, 0x97, 0x2a, 0xe3,
    0x87, 0x3e, 0xb2, 0xac, 0xcc, 0xf9, 0x89, 0xdb,
    0xf3, 0x9e, 0x05, 0x49, 0x45, 0x12, 0x4c, 0xd0,
    0x27, 0xef, 0xa2, 0x99, 0x2b, 0x1d, 0x86, 0x1e,
    0x63, 0x4e, 0x38, 0x49, 0xb5, 0x85, 0x91, 0x3d,
    0xa5, 0xe3, 0x94, 0x6d, 0x5b, 0x66, 0xad, 0xd0,
    0x31, 0xd3, 0xfa, 0x99, 0x9f, 0xa0, 0x83, 0xd9,
    0x38, 0x49, 0x41, 0xb1, 0xaa, 0x94, 0x1e, 0x8b,
    0x17, 0x49, 0x8b, 0xa6, 0xf3, 0x74, 0x4a, 0xb0,
    0x32, 0x3c, 0x1b, 0xe4, 0xf2, 0x7b, 0x8b, 0x50,
    0xd7, 0x28, 0xef, 0x65, 0xf8, 0x82, 0x96, 0x9d,
    0x0b, 0xf3, 0x87, 0x77, 0x3f, 0xa0, 0x7a, 0x47,
    0x51, 0x4c, 0x4b, 0x81, 0x12, 0xd4, 0xb9, 0xd4,
    0x64, 0x0f, 0xf5, 0xa8, 0x3d, 0x8a, 0xc6, 0x41,
    0xe0, 0x49, 0x2b, 0x2b, 0x4f, 0x24, 0x08, 0x91,
    0x5e, 0x10, 0x79, 0x90, 0xa5, 0xd4, 0x3b, 0x59,
    0x65, 0x2f, 0x9c, 0xe0, 0x06, 0x27, 0x30, 0xe5,
    0xa1, 0xc7, 0xf7, 0x75, 0x29, 0x4f, 0xca, 0xd5,
    0x88, 0xb2, 0x29, 0x52, 0x54, 0xc2, 0x77, 0x2c,
    0xd6, 0xc3, 0x72, 0x86, 0xeb, 0x09, 0xee, 0xfe,
    0xb8, 0xbc, 0xd3, 0x68, 0xb5, 0xda, 0xed, 0xa4,
    0xa6, 0x43, 0x18, 0x82, 0x3f, 0x21, 0x7a, 0x96,
    0x8c, 0xbb, 0x66, 0x01, 0x9f, 0x84, 0x95, 0x31,
    0x40, 0x63, 0x54, 0xff, 0x56, 0xa1, 0xc8, 0x81,
    0x36, 0xee, 0x63, 0xc8, 0x32, 0x9d, 0xb9, 0xb3,
    0x20, 0xe1, 0x3b, 0x00, 0xcd, 0xf9, 0x39, 0xb8,
    0x9c, 0x75, 0x5f, 0x4e, 0x66, 0x70, 0xb9, 0xf0,
    0x9f, 0x6d, 0x43, 0x59, 0x00, 0x3a, 0xce, 0x06,
    0x5d, 0xc5, 0x70, 0x03, 0x7b, 0x1c, 0x08, 0x8a,
    0x0d, 0x81, 0xc6, 0x3e, 0xce, 0xd3, 0x8b, 0x23,
    0x31, 0x4a, 0x41, 0x10, 0x4b, 0xe9, 0x98, 0x42,
    0xb9, 0xca, 0x65, 0x65, 0xd5, 0x16, 0xcd, 0x6d,
    0xbb, 0x05, 0xdc, 0x1d, 0x2a, 0x63, 0x47, 0x41,
    0x90, 0x43, 0xe9, 0x38, 0x3d, 0x29, 0xdf, 0xcb,
    0xde, 0x4a, 0xdb, 0xe4, 0xe9, 0xa8, 0xbe, 0xa1,
    0xee, 0xdb, 0xbe, 0x19, 0x64, 0xdd, 0xc7, 0xe4,
    0xa4, 0xcb, 0xfd, 0x9f, 0xb9, 0x29, 0x53, 0x6e,
    0x60, 0x89, 0x8a, 0x36, 0x7d, 0xa1, 0x07, 0xef,
    0xb0, 0x71, 0x2f, 0xb3, 0x87, 0x7e, 0x6f, 0x1c,
    0x05, 0x1f, 0x8e, 0x5a, 0x63, 0xf4, 0xa0, 0xa9,
    0x01, 0xff, 0x63, 0xfb, 0xbd, 0xb2, 0x1c, 0x3b,
    0x05, 0xad, 0x7c, 0xab, 0x5d, 0x46, 0x25, 0x75,
    0xbf, 0x57, 0x8c, 0x66, 0x61, 0x0b, 0xd5, 0x64,
    0x90, 0x55, 0x64, 0xdd, 0xc3, 0x38, 0x44, 0x1a,
    0x5e, 0x03, 0xdf, 0xf8, 0x55, 0x00, 0x0a, 0xec,
    0xb8, 0xc6, 0x03, 0x24, 0x0c, 0xbb, 0xa1, 0xdb,
    0xef, 0xf1, 0x71, 0x6c, 0x11, 0x1b, 0x20, 0x45,
    0x32, 0x7d, 0xb7, 0x8e, 0xda, 0x37, 0x5c, 0x99,
    0x9c, 0x08, 0xf1, 0xe6, 0xa9, 0xe4, 0xd9, 0x0b,
    0xae, 0xa4, 0xa8, 0x93, 0x3e, 0xfa, 0xfe, 0xcf,
    0xfd, 0xa6, 0x4a, 0x3e, 0x77, 0x2e, 0xa0, 0xeb,
    0x44, 0x14, 0x29, 0xed, 0x81, 0x8d, 0xc0, 0xfa,
    0x4e, 0xf8, 0x8f, 0x80, 0x9c, 0x1d, 0x4a, 0xfd,
    0x0d, 0x1e, 0xa7, 0x24, 0xe5, 0x7a, 0x1e, 0x27,
    0xd0, 0x03, 0x4a, 0x8a, 0x2e, 0x98, 0x08, 0xbb,
    0xd1, 0x32, 0x09, 0xb8, 0x52, 0xa3, 0x78, 0xfb,
    0x62, 0x64, 0xb1, 0x36, 0x7b, 0x95, 0x8a, 0x1d,
    0xa8, 0x84, 0xae, 0xe3, 0xf5, 0x11, 0xcc, 0x5c,
    0x7b, 0x21, 0xab, 0x8c, 0x3a, 0x91, 0x87, 0x08,
    0xaa, 0xa1, 0xdb, 0x4d, 0xa7, 0xa6, 0x7a, 0x7b,
    0x23, 0xc5, 0x38, 0xf1, 0x45, 0x92, 0xb3, 0xfd,
    0x4e, 0x2a, 0x9b, 0x5a, 0x77, 0x65, 0xec, 0xaf,
    0x0f, 0xf7, 0x1d, 0x21, 0x25, 0x9f, 0x1b, 0xa5,
    0xe5, 0x41, 0xd3, 0x58, 0x46, 0x4d, 0x47, 0xa7,
    0xe8, 0x0b, 0xc1, 0x2d, 0x5f, 0x61, 0x6f, 0x21,
    0xaf, 0x97, 0xe2, 0xe9, 0x6b, 0xe8, 0xce, 0x44,
    0x93, 0xae, 0x3e, 0x5e, 0xf3, 0xf5, 0xd5, 0xef,
    0x54, 0x7d, 0x4b, 0x04, 0xff, 0xfa, 0xf3, 0xd7,
    0xeb, 0x5f, 0xf8, 0x4f, 0xb7, 0xab, 0x5f, 0xa9,
    0x57, 0x0d, 0xdd, 0xdf, 0xd9, 0x60, 0x7d, 0x7d,
    0x0f, 0xc2, 0x25, 0x4d, 0x33, 0x19, 0x83, 0x65,
    0xdd, 0xae, 0xae, 0x57, 0x37, 0xff, 0x5e, 0xf1,
    0xdf, 0x6e, 0xbe, 0x7c, 0xfc, 0xfa, 0x1b, 0x45,
    0x1b, 0x58, 0xdc, 0xac, 0x6e, 0x6f, 0xae, 0x3e,
    0xf3, 0x1f, 0xaf, 0xee, 0x3f, 0x86, 0x9d, 0x98,
    0xbc, 0xbb, 0xba, 0xdb, 0xf0, 0xcd, 0xfd, 0x7a,
    0xf3, 0x2b, 0x56, 0x36, 0x2b, 0x5a, 0x99, 0xd3,
    0xf4, 0xea, 0xdb, 0xb7, 0xd5, 0x2d, 0xff, 0xf1,
    0xeb, 0xdd, 0xcf, 0x7c, 0xf5, 0xf1, 0x13, 0xe6,
    0x47, 0xaf, 0xd4, 0x17, 0x5b, 0x74, 0x6a, 0x47,
    0x15, 0x1c, 0x83, 0x03, 0x3a, 0x83, 0x87, 0x77,
    0xab, 0x35, 0x36, 0x5c, 0xdd, 0xdd, 0xdf, 0x3a,
    0x1d, 0xc8, 0xff, 0xa7, 0x6f, 0x37, 0x5f, 0xf9,
    0x97, 0xfb, 0x35, 0x1f, 0x0e, 0x2e, 0xff, 0x33,
    0x3c, 0x9b, 0x0c, 0xff, 0x78, 0xa5, 0x9c, 0x63,
    0x37, 0x85, 0x5b, 0xe3, 0x3e, 0xe1, 0x2a, 0xef,
    0xb6, 0x33, 0x17, 0x88, 0xad, 0x2f, 0x79, 0xcf,
    0x2a, 0xae, 0x0a, 0xca, 0xe7, 0x0b, 0x0b, 0xbd,
    0x36, 0xf7, 0xd8, 0xbf, 0x16, 0x15, 0xab, 0x6a,
    0xcb, 0x90, 0x58, 0x66, 0xea, 0x2d, 0x2a, 0xe4,
    0xa9, 0xf4, 0x78, 0x67, 0x2e, 0x3f, 0xec, 0x69,
    0xaf, 0x30, 0xf6, 0x71, 0x36, 0xac, 0x01, 0x52,
    0x60, 0xc9, 0x17, 0x17, 0xf6, 0xfe, 0xcb, 0x52,
    0x81, 0xe2, 0x36, 0xdf, 0x41, 0x2b, 0xb3, 0x7b,
    0x61, 0x99, 0x32, 0x4c, 0x74, 0xb6, 0x44, 0x66,
    0xd5, 0xa3, 0xf4, 0x77, 0x40, 0x27, 0x6c, 0x48,
    0x94, 0x72, 0xe4, 0x6f, 0x2a, 0x3f, 0xeb, 0x6a,
    0x99, 0x11, 0xae, 0x60, 0x4e, 0x56, 0x2c, 0x5c,
    0xef, 0x71, 0x3f, 0xc2, 0x7d, 0xed, 0x99, 0x2e,
    0x6b, 0x84, 0x36, 0x58, 0x92, 0x5a, 0xd7, 0xda,
    0x5d, 0x02, 0x61, 0x41, 0x4b, 0xb4, 0x14, 0x63,
    0x1d, 0xdf, 0xbd, 0xa9, 0x90, 0x7e, 0x95, 0xb3,
    0x40, 0x0c, 0x13, 0x03, 0xf5, 0x9d, 0xc5, 0x83,
    0x27, 0x48, 0x75, 0x24, 0xda, 0xdd, 0x30, 0x69,
    0x0e, 0x62, 0x64, 0x4c, 0x94, 0x12, 0x6d, 0xc9,
    0x13, 0x85, 0xb0, 0x58, 0xd6, 0x8f, 0x32, 0xd8,
    0x63, 0x44, 0x93, 0x08, 0x64, 0x69, 0x39, 0x92,
    0xaa, 0x48, 0xa3, 0x3c, 0x5b, 0x3d, 0x59, 0xef,
    0xee, 0x87, 0x68, 0x9f, 0x7a, 0x71, 0x10, 0x42,
    0x88, 0x98, 0x7d, 0x69, 0xa4, 0xbf, 0xfa, 0xd7,
    0xe2, 0x99, 0xf2, 0x79, 0x11, 0x4e, 0xe4, 0x4b,
    0x64, 0x2d, 0x8d, 0x11, 0x3b, 0xc9, 0x70, 0xd5,
    0xd7, 0xb9, 0x0c, 0x31, 0x41, 0xb2, 0xd7, 0xca,
    0x18, 0x89, 0xb4, 0xa0, 0xd9, 0xe7, 0x6d, 0x41,
    0x54, 0x03, 0x9b, 0xa9, 0x5b, 0xb1, 0xae, 0x52,
    0x1c, 0x40, 0xe4, 0xa3, 0x74, 0xa1, 0x80, 0xd5,
    0x35, 0x9d, 0xc8, 0xb5, 0x64, 0x86, 0x91, 0xde,
    0x16, 0xf5, 0x93, 0x6f, 0xd3, 0x5f, 0x80, 0x0d,
    0xc1, 0xbc, 0x4b, 0x04, 0x2c, 0x07, 0xd1, 0x49,
    0x58, 0xa8, 0xd0, 0xb4, 0x1b, 0xdf, 0x56, 0xfd,
    0x3b, 0xa2, 0x9b, 0x85, 0xf6, 0x34, 0x8d, 0xc8,
    0xdc, 0xca, 0xce, 0x15, 0x38, 0x45, 0x69, 0xd6,
    0xd8, 0x07, 0xb4, 0x48, 0x04, 0x98, 0x32, 0x46,
    0xfc, 0x7c, 0x13, 0x05, 0x4e, 0x5c, 0x64, 0x4f,
    0xaa, 0x28, 0xd0, 0x25, 0x32, 0x89, 0xd5, 0x3e,
    0xbf, 0xf6, 0x7d, 0x6f, 0x03, 0xec, 0xf8, 0xb0,
    0x38, 0x75, 0x24, 0xa1, 0xca, 0xb6, 0x0c, 0x73,
    0x21, 0xe1, 0x80, 0xc5, 0xa6, 0x7b, 0x5f, 0x30,
    0xf7, 0xdc, 0x80, 0x32, 0x87, 0x36, 0x07, 0xff,
    0xfe, 0xcb, 0x63, 0xd3, 0xc1, 0xcf, 0xd6, 0x35,
    0xdb, 0x0a, 0xe7, 0xab, 0xf7, 0x93, 0xb8, 0xca,
    0x9d, 0x73, 0x9f, 0x96, 0xd0, 0xcf, 0x8c, 0x74,
    0xe4, 0xca, 0xbd, 0xbc, 0x70, 0xb3, 0xb6, 0xd5,
    0x03, 0x36, 0xa0, 0x5e, 0x72, 0x4f, 0xfb, 0x7b,
    0xf3, 0x26, 0x2d, 0x24, 0x1a, 0xeb, 0x71, 0x5f,
    0x57, 0x05, 0x4a, 0xe5, 0x11, 0xcb, 0xe2, 0x7b,
    0x11, 0x2a, 0x20, 0xfa, 0x83, 0x03, 0xba, 0xf7,
    0x5c, 0xac, 0xa9, 0xee, 0x95, 0x97, 0x7b, 0xd2,
    0x68, 0x88, 0xa4, 0x54, 0xc0, 0x92, 0x0f, 0x6b,
    0x86, 0x65, 0x5a, 0x9a, 0xc6, 0xe6, 0xee, 0x40,
    0xe8, 0xfc, 0x2d, 0x84, 0xa6, 0x66, 0x40, 0x8c,
    0xf4, 0x8d, 0xb6, 0x49, 0x51, 0xf0, 0x69, 0x7e,
    0x52, 0x76, 0xdf, 0x95, 0x9e, 0x87, 0x04, 0x5d,
    0x16, 0xaf, 0x68, 0x36, 0x85, 0xca, 0x64, 0x45,
    0x47, 0x1e, 0x7c, 0xfa, 0x72, 0xcf, 0x3e, 0x7d,
    0xfb, 0xfc, 0x38, 0x19, 0xb8, 0x8b, 0xb8, 0x01,
    0x22, 0xa5, 0x7f, 0x83, 0x06, 0x1e, 0x73, 0xc0,
    0xf2, 0x1c, 0xf4, 0x52, 0xff, 0x8f, 0x94, 0x04,
    0x58, 0xed, 0xbd, 0x3d, 0x0f, 0x76, 0x88, 0x1e,
    0x49, 0x61, 0x6e, 0xc5, 0xdf, 0x84, 0x73, 0xf7,
    0x58, 0xa4, 0xfb, 0x3e, 0xdd, 0x3b, 0xb3, 0x18,
    0x51, 0x34, 0x9f, 0x8e, 0x1f, 0xba, 0x9b, 0xc9,
    0xd1, 0x48, 0x9a, 0x0b, 0xe1, 0x74, 0x0f, 0x34,
    0x00, 0x9c, 0x47, 0x7e, 0x81, 0xdd, 0xee, 0x7d,
    0xe6, 0xa8, 0xcf, 0x3f, 0x39, 0xdd, 0xed, 0xe8,
    0x06, 0x3c, 0x59, 0x9f, 0x2f, 0x12, 0x75, 0x37,
    0x78, 0x0a, 0x46, 0x26, 0x3e, 0x5b, 0x26, 0xe6,
    0x7b, 0xa0, 0x18, 0x1c, 0x1c, 0x25, 0x79, 0x6a,
    0xa2, 0x4f, 0xac, 0xa1, 0x77, 0x94, 0x38, 0x28,
    0x0f, 0x41, 0xee, 0x91, 0x51, 0xcf, 0x0e, 0xde,
    0x53, 0x50, 0x90, 0xd1, 0xde, 0x21, 0xd9, 0xf1,
    0xa5, 0xd0, 0x9d, 0x73, 0xdc, 0x63, 0xa6, 0x9d,
    0xab, 0xe1, 0xfd, 0xd4, 0x66, 0x19, 0x5a, 0x89,
    0x37, 0x8b, 0x60, 0xc2, 0x54, 0xff, 0x71, 0x32,
    0x43, 0xe9, 0x98, 0x54, 0x3a, 0x81, 0x98, 0xbe,
    0x6d, 0x65, 0xe2, 0x2e, 0x25, 0x6d, 0xdd, 0x2d,
    0x31, 0x1b, 0xf9, 0x2b, 0xca, 0xa4, 0xf0, 0x99,
    0xb6, 0xec, 0x7e, 0xcc, 0x9f, 0x3e, 0x4c, 0x3d,
    0x6e, 0x73, 0xc4, 0x6b, 0x3c, 0x65, 0x06, 0xf7,
    0x9b, 0x0f, 0x13, 0x59, 0x49, 0xa1, 0x88, 0x8c,
    0x20, 0xef, 0xb0, 0x31, 0xeb, 0xf3, 0x8f, 0x14,
    0xef, 0x4c, 0x44, 0x32, 0xf4, 0xe6, 0x23, 0xa5,
    0x07, 0xa5, 0xf9, 0x98, 0x8a, 0xc3, 0xdd, 0x44,
    0x7b, 0xb5, 0xdb, 0xbb, 0x7c, 0x27, 0x9c, 0xcd,
    0x50, 0x04, 0xdd, 0xff, 0x63, 0x50, 0x04, 0x8f,
    0xc3, 0xf7, 0xa3, 0xf1, 0xfb, 0xe1, 0xf9, 0x68,
    0xb6, 0x3c, 0xdf, 0x8d, 0xa7, 0xe3, 0x99, 0x58,
    0x4c, 0xe7, 0xe7, 0xe3, 0xe1, 0x78, 0x3a, 0x9c,
    0x8e, 0x16, 0x7c, 0x34, 0x1d, 0x8e, 0x46, 0x8b,
    0xf3, 0xa6, 0x6e, 0xce, 0x6b, 0x33, 0x78, 0xfd,
    0x1f, 0xfe, 0x08, 0x0b, 0x09,
};

// Identify size = 1829 (4576 uncompressed)
const uint32_t command_identify_size PROGMEM
    = ARRAY_SIZE(command_identify_data);