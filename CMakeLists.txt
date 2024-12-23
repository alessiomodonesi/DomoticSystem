cmake_minimum_required(VERSION 2.84 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(Domotica VERSION 1.0)
include_directories(include)
set(SOURCES
    src/main.cpp
    src/DomoticDevice.cpp
    src/ManualDevice.cpp
    src/FixedCycleDevice.cpp
)
add_executable(domotica ${SOURCES})
target_link_libraries(main ${PROJECT_NAME})