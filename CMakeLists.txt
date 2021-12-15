cmake_minimum_required(VERSION 3.17)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(This NetSim)

set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/types.cpp
        )

add_executable(netsim ${SOURCE_FILES} main.cpp)

enable_testing()

add_subdirectory(googletest)

add_subdirectory(NetSim_tests)