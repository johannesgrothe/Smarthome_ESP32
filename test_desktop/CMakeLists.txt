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

FetchContent_Declare(nlohmann
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.10.5
        )

FetchContent_Declare(json_validator
        GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
        GIT_TAG        2.1.0
        )

FetchContent_MakeAvailable(ArduinoJson nlohmann json_validator)

set(ADDITIONAL_SOURCES
        ../src/connectors/request.cpp
        ../src/gadgets/abstract/gadget.cpp
        ../src/connectors/ir_gadget.cpp
        ../src/connectors/code_gadget.cpp
        ../src/connectors/code_command.cpp
        ../src/system_timer.cpp
        ../src/connectors/event.cpp
        ../src/color.cpp
        ../src/mapping_reference.cpp
        ../src/gadgets/abstract/lamp_rgb.cpp
        ../src/gadgets/abstract/fan.cpp
        ../src/gadgets/abstract/doorbell.cpp
        ../src/gadgets/abstract/sensor_motion.cpp
        ../src/gadgets/abstract/sensor_temperature.cpp
        ../src/gadgets/abstract/wallswitch.cpp
        ../src/api/api_manager.cpp
        ../src/api/api_manager_delegate.cpp
        ../src/api/dtos/gadget_dto.cpp
        ../src/api/dtos/client_dto.cpp
        ../src/api/characteristic_meta.cpp
        ../src/client_main.cpp
        ../src/connectors/request_gadget.cpp
        ../src/connectors/request_queue.cpp
        ../src/connectors/split_request_buffer.cpp
        ../src/api/coders/api_encoder.cpp
        ../src/api/coders/api_decoder.cpp
        ../src/storage/system_storage.cpp
        ../src/storage/eeprom_storage.cpp
        ../src/storage/static_storage.cpp
        ../src/ip_container.cpp
        ../src/network_loader.cpp
        ../src/hardware_controller.cpp
        ../src/storage/config.cpp
        ../src/connectors/serial_gadget.cpp
        ../src/crc_calculator.cpp
        ../src/storage/system_config.cpp
        ../src/storage/gadget_config.cpp
        ../src/storage/event_config.cpp
        ../src/storage/eeprom_manager.cpp
        ../src/storage/base_config.cpp
        ../src/gadget_manager.cpp
        ../src/event_manager.cpp
        ../src/gadget_factory.cpp
        ../src/gadgets/impl/lamp_basic.cpp
        ../src/gadgets/impl/doorbell_basic.cpp
        ../src/gadgets/impl/wallswitch_basic.cpp
        ../src/gadgets/impl/fan_westinghouse_ir.cpp
        ../src/gadgets/impl/lamp_westinghouse_ir.cpp
        ../src/gadgets/characteristic.cpp
        ../src/gadgets/simple_hardware_gadget.cpp
        ../src/gadgets/abstract/lamp_onoff.cpp
        ../src/connectors/radio_gadget.cpp
        ../src/storage/eeprom_gadget_mapping_coder.cpp
        ../src/scheduled_messages_manager.cpp
        ../src/json_validator.cpp
        ../src/api/coders/config_decoder.cpp
        ../src/api/gadget_meta_coder.cpp
        ../src/api/dtos/gadget_update_dto.cpp
        ../src/api/characteristic_update_meta.cpp
        test_helpers/validator.cpp
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

set(TEST_COUNT)
list(LENGTH TEST_SOURCES TEST_COUNT)
message("Detected " ${TEST_COUNT} " test files")
message("")

add_executable(CatchTests ${TEST_SOURCES} ${ADDITIONAL_SOURCES})
target_link_libraries(CatchTests nlohmann_json_schema_validator)
target_link_libraries(CatchTests ArduinoJson)
