set(CMAKE_CXX_COMPILER "/usr/local/bin/clang++")
set(CMAKE_C_COMPILER "/usr/local/bin/clang")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -ldl -DCHAISCRIPT_NO_THREADS")

cmake_minimum_required(VERSION 3.3)
project(amvmodel)

find_package(Boost 1.60.0 COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

set(SOURCE_FILES test/test.cpp
        test/ProcessTest.hxx
        src/model/process_node.hxx
        src/model/process.hxx
        src/model/event.hxx
        src/model/context.hxx
        src/collection/name_value_pair.hxx
        test/ContextTest.hxx
        src/model/event_callback_container.hxx
        test/EventCallbackTest.h
        test/jstest.h
        src/script/chaiscript_gateway.h
        src/script/chaiscript_gateway.cpp
        src/serial/serial.hxx
        src/gps/gps.h
        src/gps/gps.cpp
        src/gps/nmea.h
        src/gps/nmea.cpp
        src/collection/name_value_pair.cpp
	src/main.cpp)

add_executable(amvmodel ${SOURCE_FILES})
target_link_libraries(amvmodel ${Boost_LIBRARIES} ev)