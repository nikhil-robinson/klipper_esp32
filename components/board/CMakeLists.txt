idf_component_register(SRCS "console.c" "adc.c" 
                            "gpio.c" 
                            "hard_pwm.c" 
                            "timer.c" 
                            "compile_time_request.c"
                            "internal.c"
                    INCLUDE_DIRS "include"
                    REQUIRES klipper driver esp_adc)
