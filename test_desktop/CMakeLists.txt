cmake_minimum_required(VERSION 3.2)
project(Static_Tests)

enable_testing()

set(CMAKE_CXX_STANDARD "11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")
set(PROJECT_VERSION "1.0.0")

add_compile_definitions(UNIT_TEST)

include(FetchContent)
FetchContent_Declare(ArduinoJson
        GIT_REPOSITORY https://github.com/bblanchon/ArduinoJson.git
        GIT_TAG        v6.18.3
        )
FetchContent_MakeAvailable(ArduinoJson)

set(ADDITIONAL_SOURCES
        ../src/connectors/request.cpp
        ../src/gadgets/sh_gadget.cpp
        ../src/connectors/ir_gadget.cpp
        ../src/connectors/code_gadget.cpp
        ../src/connectors/code_command.cpp
        ../src/system_timer.cpp
        ../src/connectors/event.cpp
        ../src/color.cpp
        ../src/mapping_reference.cpp
        ../src/gadgets/sh_lamp.cpp
        ../src/gadgets/sh_fan.cpp
        ../src/gadgets/sh_doorbell.cpp
        ../src/gadgets/sh_sensor_motion.cpp
        ../src/gadgets/sh_sensor_temperature.cpp
        ../src/gadgets/sh_wallswitch.cpp
        ../src/api/api_manager.cpp
        ../src/api/api_manager_delegate.cpp
        ../src/api/gadget_meta.cpp
        ../src/api/client_meta.cpp
        ../src/client_main.cpp
        ../src/connectors/request_gadget.cpp
        ../src/connectors/request_queue.cpp
        ../src/connectors/split_request_buffer.cpp
        )

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
include (gcovr)

message("Scanning for test files...")
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(filename ${files})
    set(f_test "")
    cmake_path(GET filename FILENAME f_test)
    if (${f_test} MATCHES "test_[a-z_]+\\.cpp")
        if (${f_test} MATCHES "test_example.cpp")
            message("${f_test} is a test template, not worth testing")
        else()
            set(SOURCE_FILES ${filename} ${ADDITIONAL_SOURCES})
            string(REPLACE ".cpp" "" f_name ${f_test})
            add_executable(${f_name} ${SOURCE_FILES})
            add_test(NAME ${f_name} COMMAND ${f_name})
#            ctest_coverage(${f_name} "/Users/honneka/Documents/Projekte/Programmierung/Smarthome_ESP32/test_desktop/build")

            target_link_libraries(${f_name} ArduinoJson)

            message("Added test '${f_name}' with executable '${f_name}' from '${filename}'")
        endif()
    endif()
endforeach()
message("")
