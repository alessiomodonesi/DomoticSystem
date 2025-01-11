cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_BUILD_TYPE Release)
project(Domotica)
include_directories(include)
add_library(
    ${PROJECT_NAME} SHARED
    src/Time.cpp
    src/DomoticSystem.cpp
    src/DomoticDevice.cpp
    src/FixedCycleDevice.cpp
    src/UnifiedLogger.cpp
)
add_executable(main src/main.cpp)
target_link_libraries(main ${PROJECT_NAME})
