cmake_minimum_required(VERSION 3.7.2)

project(fort-robotics-embbedded-eval)

# Use standard C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# Use standard C++1
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C11 and C++11 support have been enabled by default")

if (NOT DEBUG)
    message("-- Debug mode is off")
    # Compiler flags for binary size optimization
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-psabi -s -fstack-protector")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-psabi -s -fstack-protector")
    message("-- Setting build type as Release")
    set(CMAKE_BUILD_TYPE Release)
else()
    message ("-- Debug mode is ON")
    # Compiler flags for unoptimized debug data
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-psabi -fno-stack-protector -g -Og -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-psabi -fno-stack-protector -g -Og -ggdb")
    message("-- Setting build type as Debug")
    set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()

# Go into the msg_pdu dir for compilation
add_subdirectory(msg_pdu)
