cmake_minimum_required(VERSION 3.2)
project(Static_Tests)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")

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
        ../src/api/api_encoder.cpp
        ../src/api/api_decoder.cpp
        )

set(TEST_SOURCES)

message("Scanning for test files...")
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(filename ${files})
    set(f_test "")
    cmake_path(GET filename FILENAME f_test)
    if (${f_test} MATCHES "^test_[a-z_]+\\.cpp")
        set(TEST_SOURCES ${TEST_SOURCES} ${filename})
        message("Detected test-file '${filename}'")
    endif()
endforeach()
message("")

add_executable(CatchTests ${TEST_SOURCES} ${ADDITIONAL_SOURCES})
#add_executable(CatchTests ${TEST_SOURCES})
target_link_libraries(CatchTests ArduinoJson)
