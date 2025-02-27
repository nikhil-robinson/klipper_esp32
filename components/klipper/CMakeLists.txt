idf_component_register(SRCS "klipper/src/adccmds.c" 
                            "klipper/src/basecmd.c" 
                            "klipper/src/buttons.c" 
                            "klipper/src/command.c" 
                            "klipper/src/debugcmds.c" 
                            "klipper/src/endstop.c" 
                            "klipper/src/gpiocmds.c" 
                            "klipper/src/i2c_software.c" 
                            "klipper/src/i2ccmds.c" 
                            "klipper/src/initial_pins.c" 
                            "klipper/src/lcd_hd44780.c" 
                            "klipper/src/lcd_st7920.c" 
                            "klipper/src/neopixel.c" 
                            "klipper/src/pulse_counter.c" 
                            "klipper/src/pwmcmds.c" 
                            "klipper/src/sched.c"
                            "klipper/src/sensor_ldc1612.c"
                            "klipper/src/sensor_adxl345.c" 
                            "klipper/src/sensor_angle.c" 
                            "klipper/src/sensor_bulk.c" 
                            "klipper/src/sensor_lis2dw.c" 
                            "klipper/src/sensor_mpu9250.c" 
                            "klipper/src/spi_software.c" 
                            "klipper/src/spicmds.c" 
                            "klipper/src/stepper.c" 
                            "klipper/src/thermocouple.c"   
                            "klipper/src/tmcuart.c" 
                            "klipper/src/trsync.c"
                            "klipper/src/generic/alloc.c"
                            "klipper/src/generic/crc16_ccitt.c" 
                            "klipper/src/generic/timer_irq.c"
                            "board/adc.c" 
                            "board/gpio.c" 
                            "board/timer.c"
                            "board/internal.c"
                            "board/console.c"
                            "board/hard_pwm.c"
                            "board/i2c.c"
                            "board/spi.c"
                    INCLUDE_DIRS "klipper/src/" "board/include"
                    REQUIRES driver esp_adc)


# Add custom command to generate compile_time_request.c
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/compile_time_request.c
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/klipper/ python3 ./scripts/buildcommands.py -d ${CMAKE_CURRENT_SOURCE_DIR}/klipper.dict ${CMAKE_CURRENT_SOURCE_DIR}/compile_time_request.txt ${CMAKE_CURRENT_SOURCE_DIR}/compile_time_request.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/klipper.dict ${CMAKE_CURRENT_SOURCE_DIR}/compile_time_request.txt
    COMMENT "Generating compile_time_request.c"
    VERBATIM
)

# Define custom target to ensure compile_time_request.c is generated
add_custom_target(compile_time_request_target DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/compile_time_request.c)

# Make sure the component library depends on the generated file
add_dependencies(${COMPONENT_LIB} compile_time_request_target)

# Add the generated file to the sources of the component target
target_sources(${COMPONENT_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/compile_time_request.c)
