
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

# Component name with ESP-IDF prefix
set(COMPONENT_TARGET __idf_klipper)

# Find Python executable (ESP-IDF usually sets this)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# First, register the component WITHOUT compile_time_request.c
idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS
        klipper/src
        klipper/src/generic
        .
        board
    REQUIRES driver esp_adc esp_vfs_console esp_driver_usb_serial_jtag
)

# Path to the extraction script
set(EXTRACT_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/extract_compile_time_requests.py)
set(CTR_FILE ${CMAKE_CURRENT_BINARY_DIR}/compile_time_request.c)

# Generate compile_time_request.c after all main source object files are built
add_custom_command(
    OUTPUT ${CTR_FILE}
    COMMAND ${Python3_EXECUTABLE} ${EXTRACT_SCRIPT}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${COMPONENT_TARGET}
        "${CMAKE_C_COMPILER};${CMAKE_ASM_COMPILER};${CMAKE_LINKER};${CMAKE_OBJCOPY};${CMAKE_OBJDUMP};${CMAKE_STRIP}"
        --python ${Python3_EXECUTABLE}
        --objcopy ${CMAKE_OBJCOPY}
    DEPENDS ${COMPONENT_LIB}
    COMMENT "Generating compile_time_request.c from object files"
    VERBATIM
)

# Create a separate library for compile_time_request.c
add_library(${COMPONENT_TARGET}_ctr STATIC ${CTR_FILE})

# Set up include directories for the CTR library
target_include_directories(${COMPONENT_TARGET}_ctr PRIVATE
    klipper/src
    klipper/src/generic
    .
    board
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Link the CTR library to the main component
target_link_libraries(${COMPONENT_LIB} PRIVATE ${COMPONENT_TARGET}_ctr)