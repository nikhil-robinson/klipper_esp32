idf_component_register(SRCS "watchdog.c" "spi.c" "i2c.c" "adc.c" 
                            "gpio.c" 
                            "hard_pwm.c" 
                            "timer.c" 
                            "compile_time_request.c"
                            "internal.c"
                            "serial.c"
                    INCLUDE_DIRS "include"
                    REQUIRES klipper driver esp_adc)
