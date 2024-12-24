cmake_minimum_required(VERSION 2.84 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(Domotica)
include_directories(include)
add_library(
    ${PROJECT_NAME} SHARED
    src/DomoticSystem.cpp
    src/DomoticDevice.cpp
    src/FixedCycleDevice.cpp
    src/ManualDevice.cpp
)
add_executable(main src/main.cpp)
target_link_libraries(main ${PROJECT_NAME})