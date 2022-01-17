cmake_minimum_required(VERSION 3.16.0)
set(EXTRA_COMPONENT_DIRS main components/sensor)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(tomatomon)

idf_build_set_property(COMPILE_OPTIONS "-Wmissing-field-initializers" APPEND)