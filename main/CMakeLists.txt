# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources 
${CMAKE_SOURCE_DIR}/main/main.cpp
${CMAKE_SOURCE_DIR}/components/soilsensor/sensor.cpp
)

idf_component_register(
    SRCS ${app_sources}
    PRIV_REQUIRES tinyusb
)
