
set(SOURCES
    # Board specific sources
    board/adc.c
    board/console.c
    board/gpio.c
    board/hard_pwm.c
    board/i2c.c
    board/internal.c
    board/spi.c
    board/timer.c
    # Klipper sources
    klipper/src/adccmds.c
    klipper/src/basecmd.c
    klipper/src/buttons.c
    klipper/src/command.c
    klipper/src/debugcmds.c
    klipper/src/endstop.c
    klipper/src/gpiocmds.c
    klipper/src/i2ccmds.c
    klipper/src/i2c_software.c
    klipper/src/initial_pins.c
    klipper/src/lcd_hd44780.c
    klipper/src/lcd_st7920.c
    klipper/src/neopixel.c
    klipper/src/pulse_counter.c
    klipper/src/pwmcmds.c
    klipper/src/sched.c
    klipper/src/sensor_adxl345.c
    klipper/src/sensor_angle.c
    klipper/src/sensor_bulk.c
    klipper/src/sensor_ldc1612.c
    klipper/src/sensor_lis2dw.c
    klipper/src/sensor_mpu9250.c
    klipper/src/spicmds.c
    klipper/src/spi_software.c
    klipper/src/stepper.c
    klipper/src/thermocouple.c
    klipper/src/tmcuart.c
    klipper/src/trsync.c
    klipper/src/generic/alloc.c
    klipper/src/generic/crc16_ccitt.c
    klipper/src/generic/serial_irq.c
    klipper/src/generic/timer_irq.c
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(EXTRACT_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/extract_compile_time_requests.py)
set(CTR_FILE ${CMAKE_CURRENT_BINARY_DIR}/compile_time_request.c)

if(NOT EXISTS ${CTR_FILE})
    file(WRITE ${CTR_FILE} "// Placeholder CTR file\nvoid ctr_placeholder() {}\n")
endif()

list(APPEND SOURCES ${CTR_FILE})

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS
        klipper/src
        klipper/src/generic
        .
        board
    REQUIRES driver esp_adc esp_vfs_console esp_driver_usb_serial_jtag esp_system
)

add_custom_command(
    TARGET ${COMPONENT_LIB} PRE_BUILD
    COMMAND ${Python3_EXECUTABLE} ${EXTRACT_SCRIPT}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${COMPONENT_LIB}
        "${CMAKE_C_COMPILER};${CMAKE_ASM_COMPILER};${CMAKE_LINKER};${CMAKE_OBJCOPY};${CMAKE_OBJDUMP};${CMAKE_STRIP}"
        --python ${Python3_EXECUTABLE}
        --objcopy ${CMAKE_OBJCOPY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating compile_time_request.c from object files"
    VERBATIM
)