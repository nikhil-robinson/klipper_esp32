# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(klipper)

# Create output directory and copy build artifacts after build completion
add_custom_target(copy_build_artifacts ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/components/klipper/klipper/out
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${CMAKE_BINARY_DIR}/klipper.bin
        ${CMAKE_SOURCE_DIR}/components/klipper/klipper/out/klipper.bin
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${CMAKE_BINARY_DIR}/klipper.elf
        ${CMAKE_SOURCE_DIR}/components/klipper/klipper/out/klipper.elf
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${CMAKE_BINARY_DIR}/esp-idf/klipper/compile_time_request.c
        ${CMAKE_SOURCE_DIR}/components/klipper/klipper/out/compile_time_request.c
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${CMAKE_BINARY_DIR}/esp-idf/klipper/compile_time_request.txt
        ${CMAKE_SOURCE_DIR}/components/klipper/klipper/out/compile_time_request.txt
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${CMAKE_BINARY_DIR}/esp-idf/klipper/klipper.dict
        ${CMAKE_SOURCE_DIR}/components/klipper/klipper/out/klipper.dict
    COMMENT "Copying build artifacts to output directory"
    VERBATIM
)

# Make sure this runs after the main project is built
add_dependencies(copy_build_artifacts klipper.elf)
