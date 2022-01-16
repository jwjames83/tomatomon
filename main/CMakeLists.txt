# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources 
    ${CMAKE_SOURCE_DIR}/components/dotstar/src/dotstar.cpp
    ${CMAKE_SOURCE_DIR}/components/sensors/sensor.cpp
    ${CMAKE_SOURCE_DIR}/main/main.cpp
)

FILE(GLOB_RECURSE app_includes 
    ${CMAKE_SOURCE_DIR}/components/sensors
    ${CMAKE_SOURCE_DIR}/components/webserver
    ${CMAKE_SOURCE_DIR}/components/dotstar/include
)

idf_component_register(
    SRCS ${app_sources}
    INCLUDE_DIRS ${app_includes}
)
