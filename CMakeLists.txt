cmake_minimum_required(VERSION 3.17)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Werror=switch)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/factory.cpp
        src/reports.cpp
        src/simulation.cpp
        )

add_executable(netsim ${SOURCE_FILES})
target_compile_definitions(netsim PUBLIC EXERCISE_ID=EXERCISE_ID_FACTORY)

set(SOURCES_FILES_TESTS
        tests/test_nodes.cpp
        tests/test_factory_io.cpp
        tests/test_Factory.cpp
        tests/test_reports.cpp
        tests/test_simulate.cpp
        tests/test_storage_types.cpp
        tests/test_package.cpp
        )

add_executable(netsim_test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} tests/main_gtest.cpp)
target_compile_definitions(netsim_test PUBLIC EXERCISE_ID=EXERCISE_ID_FACTORY)

target_include_directories(netsim_test PUBLIC
        googletest/googlemock/include
        googletest/googletest/include
        mocks
        )

add_subdirectory(googletest)

target_link_libraries(netsim_test gmock)
