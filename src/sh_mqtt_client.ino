
// Settings
#include "user_settings.h"
#include "system_settings.h"

// Network Gadgets
#include "network_library.h"
#include "protocol_paths.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// Tools
#include "console_logger.h"
#include "system_timer.h"
#include "boot_mode.h"

// External Dependencies
#include "Client.h"
#include "ArduinoJson.h"

// Remotes
#include "remote_library.h"

#include "gadget_collection.h"
#include "system_storage.h"

#include "pin_profile.h"
#include "color.h"

/**
 * Reboots the chip and prints out the given message
 * @param reason The reason to print to the terminal
 */
static void rebootChip(const std::string& reason) {
    if (!reason.empty()) {
        logger.print("Rebooting Chip because: '");
        logger.print(reason);
        logger.print("' in ");
    } else {
        logger.println("Rebooting Chip in ");
    }
    for (byte k = 0; k < 5; k++) {
        logger.print(5 - k);
        logger.print(" ");
        delay(1000);
    }
    ESP.restart();
}

/**
 * Reads the gadget with the selected index
 * @param index index of the gadget to load
 * @return the gadget
 */
static gadget_tuple readGadget(uint8_t index) {
    auto gadget = System_Storage::readGadget(index);
    gadget_tuple err_gadget(0, {false, false, false, false, false, false}, {0, 0, 0, 0, 0}, "", "", "");
    auto type = std::get<0>(gadget);
    if (type) {
        auto ports = std::get<2>(gadget);
        for (int i = 0; i < GADGET_PIN_BLOCK_LEN; i++) {
            auto buf_pin = getPinForPort(ports[i]);
            if (!buf_pin) {
                return err_gadget;
            }
            ports[i] = buf_pin;
        }

        return gadget_tuple(type, std::get<1>(gadget), ports, std::get<3>(gadget), std::get<4>(gadget), std::get<5>(gadget));
    } else {
        return err_gadget;
    }
}

/**
 * Writes a gadget to the eeprom
 * @param gadget_type type of the gadget
 * @param remote_bf bitfield for the remotes
 * @param ports ports used by the gadget to connect hardware
 * @param gadget_config base config for the gadget
 * @param code_config config for the code mapping
 * @return whether writing was successful
 */
static status_tuple writeGadget(uint8_t gadget_type, bitfield_set remote_bf, pin_set ports, const std::string& name, const std::string& gadget_config, const std::string& code_config) {
    return System_Storage::writeGadget(gadget_type, remote_bf, ports, name, gadget_config, code_config);
}

// ========= STATIC METHODS END ============



std::string client_id_;

std::shared_ptr<IR_Gadget> ir_gadget;
std::shared_ptr<Radio_Gadget> radio_gadget;

std::shared_ptr<Request_Gadget> network_gadget;

Gadget_Collection gadgets;

std::shared_ptr<CodeRemote> code_remote;

BootMode system_mode = BootMode::Unknown_Mode;

bool eeprom_active_;

TaskHandle_t main_task;
TaskHandle_t network_task;

bool lock_gadget_updates = false;

// ===== REMOTE METHODS =====

/**
 * Prevents gadgets from getting updates
 */
void lockGadgetUpdates() {
    logger.println("Locking Updates");
    lock_gadget_updates = true;
}

/**
 * Lets gadgets receive characteristic updates
 */
void unlockGadgetUpdates() {
    logger.println("Unlocking Updates");
    lock_gadget_updates = false;
}

/**
 * returns whether gadgets are currently allowed to receive updates
 * @return Whether gadgets are currently allowed to receive updates
 */
bool gadgetUpdatesAreLocked() { return lock_gadget_updates; }

/**
 * forwards an event to all gadgets
 * @param event
 */
void forwardEvent(const std::shared_ptr<Event>& event){
    for (int i = 0; i < gadgets.getGadgetCount(); i++){
        gadgets[i]->handleEvent(event->getSender(), event->getType());
    }
}

// ===== START METHODS =====

void updateCharacteristicOnBridge(const std::string& gadget_name, GadgetCharacteristic characteristic, int value) {
    if (gadgetUpdatesAreLocked()) return;
    auto target_gadget = gadgets.getGadget(gadget_name);

    if (!target_gadget) {
        return;
    }

    DynamicJsonDocument req_doc(2000);

    req_doc["name"] = gadget_name;
    req_doc["type"] = int(target_gadget->getType());
    req_doc["characteristic"] = int(characteristic);
    req_doc["value"] = value;

    auto timestamp = int(micros() % 7554);

    auto out_req = std::make_shared<Request>("smarthome/remotes/gadget/update", timestamp, client_id_, "<remote>", req_doc);

    network_gadget->sendRequest(out_req);
}

void updateEventOnBridge(const string& sender, EventType type) {
    if (sender.empty()) {
        logger.println("no sender specified, no event send");
        return;
    }
    unsigned long long timestamp = millis();
    auto event_buf = std::make_shared<Event>(sender, timestamp, type);

    DynamicJsonDocument req_doc(2000);

    req_doc["name"] = sender;
    req_doc["timestamp"] = timestamp;
    req_doc["event_type"] = int(type);

    auto out_req = std::make_shared<Request>("smarthome/remotes/event/send", timestamp, client_id_, "<remote>", req_doc);

    network_gadget->sendRequest(out_req);

    forwardEvent(event_buf);
}

bool initGadgets() {

    logger.print("Initializing Gadgets: ");

    auto eeprom_gadgets = System_Storage::readAllGadgets();

    logger.println(eeprom_gadgets.size());
    logger.incIndent();

    for (auto gadget: eeprom_gadgets) {
        auto gadget_ident = (GadgetIdentifier) std::get<0>(gadget);
        auto remote_bf = std::get<1>(gadget);
        auto pins = std::get<2>(gadget);
        auto name = std::get<3>(gadget);
        auto gadget_config_str = std::get<4>(gadget);
        auto code_config_str = std::get<5>(gadget);

        logger.printfln("Initializing %s", name.c_str());
        logger.incIndent();

        DynamicJsonDocument gadget_config(2500);
        DynamicJsonDocument code_config(2500);

        bool deserialization_ok = true;

        DeserializationError err;

        if (!gadget_config_str.empty()) {
            err = deserializeJson(gadget_config, gadget_config_str);
            if (err != DeserializationError::Ok) {
                deserialization_ok = false;
            }
        }

        if (!gadget_config_str.empty()) {
            err = deserializeJson(code_config, code_config_str);
            if (err != DeserializationError::Ok) {
                deserialization_ok = false;
            }
        }

        if (deserialization_ok) {
            logger.println(LOG_TYPE::DATA, "Creating Gadget");
            logger.incIndent();
            auto buf_gadget = createGadget(gadget_ident, pins, name, gadget_config.as<JsonObject>());
            logger.decIndent();

            if (buf_gadget != nullptr) {
                // Gadget Remote
                if (remote_bf[0]) {
                    logger.println(LOG_TYPE::DATA, "Linking Gadget Remote");
                    logger.incIndent();

                    using std::placeholders::_1;
                    using std::placeholders::_2;
                    using std::placeholders::_3;

                    buf_gadget->setGadgetRemoteCallback(std::bind(&updateCharacteristicOnBridge, _1, _2, _3));
                    buf_gadget->setEventRemoteCallback(std::bind(&updateEventOnBridge, _1, _2));

                    logger.decIndent();
                }

                // Code Remote
                if (remote_bf[1]) {
                    logger.println(LOG_TYPE::DATA, "Linking Code Remote");
                    logger.incIndent();

                    logger.decIndent();

                    for (int method_index = 0; method_index < GadgetMethodCount; method_index++) {
                        std::stringstream ss;
                        ss << method_index;
                        std::string method_str = ss.str();
                        if (code_config.containsKey(method_str)) {
                            JsonArray codes = code_config[method_str].as<JsonArray>();
                            for (int i = 0; i < codes.size(); i++) {
                                unsigned long code = codes[i].as<unsigned long>();
                                buf_gadget->setMethodForCode((GadgetMethod) method_index, code);
                            }
                        }
                    }

                    buf_gadget->printMapping();
                }

                // Event Remote
                if (remote_bf[2]) {
                    logger.println(LOG_TYPE::DATA, "Linking Gadget Remote");
                    logger.incIndent();
                    // TODO: init event remote on gadgets
                    logger.println(LOG_TYPE::ERR, "Not Implemented");

                    logger.decIndent();
                }

                // IR Gadget
                bool ir_ok = true;
                if (gadgetRequiresIR(gadget_ident)) {
                    if (ir_gadget != nullptr) {
                        logger.println(LOG_TYPE::DATA, "Linking IR gadget");
                        buf_gadget->setIR(ir_gadget);
                    } else {
                        logger.println(LOG_TYPE::ERR, "No IR gadget available");
                        ir_ok = false;
                    }
                } else {
                    logger.println(LOG_TYPE::DATA, "No IR required");
                }

                // Radio
                // TODO: check when radio is implemented
                bool radio_ok = true;
                if (gadgetRequiresRadio(gadget_ident)) {
                    if (radio_gadget != nullptr) {
                        logger.println(LOG_TYPE::DATA, "Linking radio gadget");
                        buf_gadget->setRadio(radio_gadget);
                    } else {
                        logger.println(LOG_TYPE::DATA, "No radio gadget available");
                        radio_ok = false;
                    }
                } else {
                    logger.println(LOG_TYPE::DATA, "No radio required");
                }

                // Add created gadget to the list
                if (ir_ok && radio_ok) {
                    gadgets.addGadget(buf_gadget);
                } else {
                    logger.println(LOG_TYPE::DATA, "Gadget initialization failed due to ir/radio problems");
                }
            } else {
                logger.println(LOG_TYPE::ERR, "Gadget could not be created");
            }
        } else {
            logger.println(LOG_TYPE::ERR, "Error in config deserialization process");
        }
        logger.decIndent();
    }
    logger.decIndent();
    return true;
}

bool initConnectors() {
    logger.println("Initializing Connectors: ");

    int ir_recv = System_Storage::readIRrecvPin();
    int ir_send = System_Storage::readIRsendPin();
    int radio = 0;

    logger.println("Creating IR-Gadget: ");
    logger.incIndent();
    if (ir_recv || ir_send) {
        ir_gadget = std::make_shared<IR_Gadget>(ir_recv, ir_send);
    } else {
        logger.println("No IR Configured");
        ir_gadget = nullptr;
    }
    logger.decIndent();

    logger.println("Creating Radio-Gadget:");
    logger.incIndent();
    if (radio) {
        logger.println("Radio Configured bot not implemented");
    } else {
        logger.println("No Radio Configured");
        radio_gadget = nullptr;
    }
    logger.decIndent();

    logger.decIndent();
    return true;
}

bool initNetwork(NetworkMode mode) {
    if (mode == NetworkMode::None) {
        logger.println(LOG_TYPE::ERR, "No network configured.");
        return false;
    }

    // initialize Network
    if (mode == NetworkMode::MQTT) {

        if (!System_Storage::hasValidWifiSSID()) {
            logger.println(LOG_TYPE::ERR, "Config has no valid wifi ssid");
            return false;
        }

        if (!System_Storage::hasValidWifiPW()) {
            logger.println(LOG_TYPE::ERR, "Config has no valid wifi password");
            return false;
        }

        if (!System_Storage::hasValidMQTTIP()) {
            logger.println(LOG_TYPE::ERR, "Config has no valid mqtt ip");
            return false;
        }

        if (!System_Storage::hasValidMQTTPort()) {
            logger.println(LOG_TYPE::ERR, "Config has no valid mqtt port");
            return false;
        }

        std::string ssid = System_Storage::readWifiSSID();
        std::string wifi_pw = System_Storage::readWifiPW();
        uint16_t port = System_Storage::readMQTTPort();
        IPAddress ip = System_Storage::readMQTTIP();
        std::string user = System_Storage::readMQTTUsername();
        std::string mqtt_pw = System_Storage::readMQTTPassword();

        network_gadget = std::make_shared<MQTT_Gadget>(client_id_,
                                                       ssid,
                                                       wifi_pw,
                                                       ip,
                                                       port,
                                                       user,
                                                       mqtt_pw);

    } else if (mode == NetworkMode::Serial) {
        network_gadget = std::make_shared<Serial_Gadget>();
    } else {
        logger.println(LOG_TYPE::ERR, "Unknown Network Settings");
        return false;
    }
    logger.decIndent();
    return true;
}

void handleCodeConnector(const std::shared_ptr<Code_Gadget> &gadget) {
    if (gadget == nullptr) {
        return;
    }
    if (gadget->hasNewCommand()) {
        CodeCommand *com = gadget->getCommand();
        logger.print("Command: ");
        logger.println(com->getCode());

        if (code_remote != nullptr) {
            logger.incIndent();
            code_remote->handleNewCodeFromGadget(com);
            logger.decIndent();
        }
    }
}

/**
 * Method to check if request payload contains all of the selected keys. respondes a false ack if any of them misses
 * @param req Request to ckeck payload off
 * @param key_list The list of all the keys that need to be present
 * @return Whether all keys were present
 */
static bool checkPayloadForKeys(const std::shared_ptr<Request>& req, const std::vector<std::string>& key_list) {
    DynamicJsonDocument json_body = req->getPayload();

    for (const auto& key: key_list) {
        if (!json_body.containsKey(key)) {
            logger.printfln(LOG_TYPE::ERR, "'%s' missing in request", key);
            std::stringstream sstr;
            sstr << "Key missing in payload: '" << key << "'." << std::endl;
            req->respond(false, sstr.str());
            return false;
        }
    }
    return true;
}

/**
 * Method that handles a request to receive a characteristic update
 * @param req Request that demands the characteristic update
 */
void handleGadgetCharacteristicUpdateRequest(const std::shared_ptr<Request>& req) {

    // Check payload for missing keys
    if (!checkPayloadForKeys(req, {"name", "characteristic", "value"})) {
        return;
    }

    auto req_body = req->getPayload();

    logger.print("System / Gadget-Remote", "Received characteristic update");
    auto target_gadget = gadgets.getGadget(req_body["name"]);
    if (target_gadget != nullptr) {
        auto characteristic = getCharacteristicFromInt(req_body["characteristic"].as<int>());
        if (characteristic != GadgetCharacteristic::None) {
            logger.incIndent();
            lockGadgetUpdates();
            int value = req_body["value"].as<int>();
            target_gadget->handleCharacteristicUpdate(characteristic, value);
            unlockGadgetUpdates();
            logger.decIndent();
        } else {
            logger.print(LOG_TYPE::ERR, "Illegal err_characteristic 0");
        }
    } else {
        logger.print("Unknown Gadget");
    }
}

void handleEventUpdateRequest(const std::shared_ptr<Request>& req) {

    // Check payload for missing keys
    if (!checkPayloadForKeys(req, {"name", "timestamp", "event_type"})) {
        return;
    }

    auto req_body = req->getPayload();

    logger.print("System / Event-Remote", "Received event_type update");
    logger.incIndent();
    auto sender = req_body["name"].as<string>();
    auto timestamp = req_body["timestamp"].as<unsigned long long>();
    auto type = EventType(req_body["event_type"].as<int>());
    auto event_buf = std::make_shared<Event>(sender, timestamp, type);
    forwardEvent(event_buf);
    logger.decIndent();
}

void handleBroadcastRequest(const std::shared_ptr<Request>& req) {
    logger.println("Broadcast");
    DynamicJsonDocument doc(10);
    req->respond("smarthome/broadcast/res", doc);
}

void handleSystemControlRequest(const std::shared_ptr<Request>& req) {

    // Check payload for missing keys
    if (!checkPayloadForKeys(req, {"subject"})) {
        return;
    }

    DynamicJsonDocument json_body = req->getPayload();

    auto subject = json_body["subject"].as<std::string>();
    if (subject == "reboot") {
        req->respond(true);
        auto payload = req->getPayload();
        if (payload.containsKey("message")) {
            rebootChip(payload["message"]);
        }
        rebootChip("Network Request");
    }
    req->respond(false);
}

void handleConfigResetRequest(const std::shared_ptr<Request>& req) {

    // Check payload for missing keys
    if (!checkPayloadForKeys(req, {"reset_option"})) {
        return;
    }

    DynamicJsonDocument json_body = req->getPayload();

    // part of config to reset
    auto reset_option = json_body["reset_option"].as<std::string>();

    bool success = false;

    // reset complete config
    if (reset_option == "erase") {
        System_Storage::writeTestEEPROM();
        System_Storage::resetContentFlag();
        System_Storage::resetGadgets();
        success = true;
    }

    // reset the complete config
    else if (reset_option == "complete") {
        System_Storage::resetContentFlag();
        System_Storage::resetGadgets();
        success = true;
    }

    // reset the system config only
    else if (reset_option == "config") {
        System_Storage::resetContentFlag();
        success = true;
    }

    // reset the complete config
    else if (reset_option == "gadgets") {
        System_Storage::resetGadgets();
        success = true;
    }

    req->respond(success);
}

void handleConfigWriteRequest(const std::shared_ptr<Request>& req) {

    // Check payload for missing keys
    if (!checkPayloadForKeys(req, {"param", "value"})) {
        return;
    }

    DynamicJsonDocument json_body = req->getPayload();

    // Parameter to write
    auto param_name = json_body["param"].as<std::string>();
    // Value to write as std::string
    auto param_val = json_body["value"].as<std::string>();
    // Value to write as uint8_t
    auto param_val_uint = json_body["value"].as<uint8_t>();

    logger.printfln("Write param '%s'", param_name.c_str());
    bool write_successful = false;

    // write ID
    if (param_name == "id") {
        write_successful = System_Storage::writeID(param_val);
        if (write_successful) {
            client_id_ = param_val;
        }
    }

    // Write Wifi SSID
    else if (param_name == "wifi_ssid") {
        write_successful = System_Storage::writeWifiSSID(param_val);
    }

    // Write Wifi PW
    else if (param_name == "wifi_pw") {
        write_successful = System_Storage::writeWifiPW(param_val);
    }

    // Write MQTT IP
    else if (param_name == "mqtt_ip") {
        if (param_val == "null") {
            write_successful = System_Storage::writeMQTTIP(IPAddress(0, 0, 0, 0));
        } else {
            IPAddress buf_ip;
            buf_ip.fromString(param_val.c_str());
            write_successful = System_Storage::writeMQTTIP(buf_ip);
        }
    }

    // Write MQTT Port
    else if (param_name == "mqtt_port") {
        write_successful = System_Storage::writeMQTTPort((uint16_t) atoi(param_val.c_str()));
    }

    // Write MQTT User
    else if (param_name == "mqtt_user") {
        write_successful = System_Storage::writeMQTTUsername(param_val);
    }

    // Write MQTT PW
    else if (param_name == "mqtt_pw") {
        write_successful = System_Storage::writeMQTTPassword(param_val);
    }

    // Write IR recv
    else if (param_name == "irrecv_pin") {
        write_successful = System_Storage::writeIRrecvPin(param_val_uint);
    }

    // Write IR send
    else if (param_name == "irsend_pin") {
        write_successful = System_Storage::writeIRsendPin(param_val_uint);
    }

    // Write radio receiver pin
    else if (param_name == "radio_recv_pin") {
        write_successful = System_Storage::writeRadioRecvPin(param_val_uint);
    }

    // Write radio sender pin
    else if (param_name == "radio_send_pin") {
        write_successful = System_Storage::writeRadioSendPin(param_val_uint);
    }

    // Write network mode
    else if (param_name == "network_mode") {
        if (param_val_uint < NetworkModeCount) {
            write_successful = System_Storage::writeNetworkMode((NetworkMode) param_val_uint);
        } else {
            write_successful = false;
        }
    }

    // Write gadget remote
    else if (param_name == "gadget_remote") {
        if (param_val_uint < GadgetRemoteModeCount) {
            write_successful = System_Storage::writeGadgetRemote((GadgetRemoteMode) param_val_uint);
        } else {
            write_successful = false;
        }
    }

    // Write code remote
    else if (param_name == "code_remote") {
        if (param_val_uint < CodeRemoteModeCount) {
            write_successful = System_Storage::writeCodeRemote((CodeRemoteMode) param_val_uint);
        } else {
            write_successful = false;
        }
    }

    // Write event remote
    else if (param_name == "event_remote") {
        if (param_val_uint < EventRemoteModeCount) {
            write_successful = System_Storage::writeEventRemote((EventRemoteMode) param_val_uint);
        } else {
            write_successful = false;
        }
    }

    req->respond(write_successful);

    if (param_name == "id" && write_successful) {
        client_id_ = param_val;
    }
}

void handleConfigReadRequest(const std::shared_ptr<Request>& req) {

    // Check payload for missing keys
    if (!checkPayloadForKeys(req, {"param"})) {
        return;
    }

    DynamicJsonDocument json_body = req->getPayload();

    auto param_name = json_body["param"].as<std::string>();
    bool read_successful = false;
    std::string read_val_str;
    uint8_t read_val_uint;

    logger.printfln("Read param '%s'", param_name.c_str());

    // read wifi ssid
    if (param_name == "wifi_ssid") {
        read_successful = true;
        read_val_str = System_Storage::readWifiSSID();
    }

    // read mqtt ip
    if (param_name == "mqtt_ip") {
        read_successful = true;
        std::stringstream sstr;
        sstr << System_Storage::readMQTTIP().toString().c_str();
        read_val_str = sstr.str();
    }

    // read mqtt port
    if (param_name == "mqtt_port") {
        read_successful = true;
        std::stringstream sstr;
        sstr << System_Storage::readMQTTPort();
        read_val_str = sstr.str();
    }

    // read mqtt username
    if (param_name == "mqtt_user") {
        read_successful = true;
        read_val_str = System_Storage::readMQTTUsername();
    }

    // send response if read was successful
    if (read_successful) {
        DynamicJsonDocument doc(100);
        doc["value"] = read_val_str;
        req->respond(doc);
        return;
    }

    // read irrecv pin
    if (param_name == "irrecv_pin") {
        read_successful = true;
        read_val_uint = System_Storage::readIRrecvPin();
    }

    // read irsend pin
    else if (param_name == "irsend_pin") {
        read_successful = true;
        read_val_uint = System_Storage::readIRsendPin();
    }

    // read radio recv pin
    else if (param_name == "radio_recv_pin") {
        read_successful = true;
        read_val_uint = System_Storage::readRadioRecvPin();
    }

    // read radio send pin
    else if (param_name == "radio_send_pin") {
        read_successful = true;
        read_val_uint = System_Storage::readRadioSendPin();
    }

    // read network mode
    else if (param_name == "network_mode") {
        read_successful = true;
        read_val_uint = (uint8_t) System_Storage::readNetworkMode();
    }

    // read gadget remote
    else if (param_name == "gadget_remote") {
        read_successful = true;
        read_val_uint = (uint8_t) System_Storage::readGadgetRemote();
    }

    // read code remote
    else if (param_name == "code_remote") {
        read_successful = true;
        read_val_uint = (uint8_t) System_Storage::readCodeRemote();
    }

    // read event_remote
    else if (param_name == "event_remote") {
        read_successful = true;
        read_val_uint = (uint8_t) System_Storage::readEventRemote();
    }

    // send response if read was successful
    if (read_successful) {
        DynamicJsonDocument doc(100);
        doc["value"] = read_val_uint;
        req->respond(doc);
        return;
    }
}

void handleGadgetWriteRequest(const std::shared_ptr<Request>& req) {
    // Check payload for missing keys
    if (!checkPayloadForKeys(req, {"type", "name"})) {
        return;
    }

    DynamicJsonDocument json_body = req->getPayload();

    auto type = json_body["type"].as<uint8_t>();

    auto name = json_body["name"].as<std::string>();

    uint8_t port0 = 0;
    uint8_t port1 = 0;
    uint8_t port2 = 0;
    uint8_t port3 = 0;
    uint8_t port4 = 0;

    if (json_body.containsKey("ports")) {
        JsonObject ports = json_body["ports"].as<JsonObject>();
        if (ports.containsKey("port0")) {
            port0 = ports["port0"].as<uint8_t>();
        }
        if (ports.containsKey("port1")) {
            port1 = ports["port1"].as<uint8_t>();
        }
        if (ports.containsKey("port2")) {
            port2 = ports["port2"].as<uint8_t>();
        }
        if (ports.containsKey("port3")) {
            port3 = ports["port3"].as<uint8_t>();
        }
        if (ports.containsKey("port4")) {
            port4 = ports["port4"].as<uint8_t>();
        }
    }

    pin_set pins = {port0, port1, port2, port3, port4};

    std::string gadget_config;
    std::string code_config;

    if (json_body.containsKey("config")) {
        gadget_config = json_body["config"].as<std::string>();
    }
    if (json_body.containsKey("codes")) {
        code_config = json_body["codes"].as<std::string>();
    }

    // Create bitfield
    bitfield_set remote_bf = {false, false, false, false, false, false, false, false};

    if (json_body.containsKey("remotes")) {
        JsonObject remote_json = json_body["remotes"].as<JsonObject>();
        if (remote_json.containsKey("gadget")) {
            remote_bf[0] = remote_json["gadget"].as<bool>();
        }
        if (remote_json.containsKey("code")) {
            remote_bf[1] = remote_json["code"].as<bool>();
        }
        if (remote_json.containsKey("event")) {
            remote_bf[2] = remote_json["event"].as<bool>();
        }
    }

    auto success_tuple = writeGadget(type, remote_bf, pins, name, gadget_config, code_config);
    bool success = std::get<0>(success_tuple);
    if (success) {
        req->respond(true);
    } else {
        auto err_msg = std::get<1>(success_tuple);
        req->respond(false, err_msg);
    }
    return;
}

void xddd(const std::shared_ptr<Request>& req) {

}

void handleSystemRequest(const std::shared_ptr<Request>& req) {

    DynamicJsonDocument json_body = req->getPayload();

    if (!eeprom_active_) {
        logger.print(LOG_TYPE::ERR, "EEPROM is broken, cannot deal with system requests.");
        req->respond(false);
        return;
    }

    logger.println("System command received");

    // React to broadcast
    if (req->getPath() == PATH_BROADCAST) {
        handleBroadcastRequest(req);
        return;
    }

    // All directed Requests
    logger.println("Directed Request");

    // system commands
    if (req->getPath() == PATH_SYSTEM_CONTROL) {
        handleSystemControlRequest(req);
        return;
    }

    // Reset config
    if (req->getPath() == PATH_CONFIG_RESET) {
        handleConfigResetRequest(req);
        return;
    }

    // Write parameters
        if (req->getPath() == PATH_CONFIG_WRITE) {
        handleConfigWriteRequest(req);
        return;
    }

    // Read parameters
    if (req->getPath() == PATH_CONFIG_READ) {
        handleConfigReadRequest(req);
        return;
    }

    // Write gadget
    if (req->getPath() == PATH_GADGET_WRITE) {
        handleGadgetWriteRequest(req);
        return;
    }

    req->respond(false);
}

/**
 * Handles a request gotten from the network gadget
 * @param req Request to handle
 */
void handleRequest(const std::shared_ptr<Request>& req) {
    std::string req_path = req->getPath();
    if (!req->hasReceiver()) {
        req->updateReceiver(client_id_);
        for (const auto& list_path: broadcast_request_paths) {
            if (req_path == list_path) {
                // Handle broadcasts which do not have an receiver
                handleSystemRequest(req);
                return;
            }
        }
        return;
    } else if (client_id_ != req->getReceiver()) {
        // Return if the client is not the receiver of the message
        return;
    }

    // Check if the received request is a system request
    for (const auto& list_path: system_request_paths) {
        // TODO: check if last character is # and then only check 0 - (n-2)
        if (req_path.compare(0, list_path.length(), list_path) == 0) {
            handleSystemRequest(req);
            return;
        }
    }

    // Checks for characteristic update request
    if (req->getPath() == PATH_CHARACTERISTIC_UPDATE_FROM_BRIDGE) {
        handleGadgetCharacteristicUpdateRequest(req);
        return;
    }

    // Check for request containing new event
    if (req->getPath() == PATH_EVENT_UPDATE_FROM_BRIDGE) {
        handleEventUpdateRequest(req);
        return;
    }

    logger.printfln(LOG_TYPE::ERR, "Received request to unconfigured path");
}

/**
 * Gets new requests from the network gadget.
 */
void handleNetwork() {
    if (network_gadget == nullptr) {
        return;
    }
    if (network_gadget->hasRequest()) {
        std::string type;
        std::shared_ptr<Request> req = network_gadget->getRequest();
        RequestGadgetType g_type = network_gadget->getGadgetType();
        if (g_type == RequestGadgetType::MQTT_G)
            type = "MQTT";
        else if (g_type == RequestGadgetType::SERIAL_G)
            type = "Serial";
        else
            type = "<o.O>";
        logger.printfln("[%s] '%s': %s", type.c_str(), req->getPath().c_str(), req->getBody().c_str());
        handleRequest(req);
    }
}

/**
 * Test-Fuction for debugging
 */
void testStuff() {
    logger.println("Testing Stuff");
    logger.incIndent();

    if (eeprom_active_) {
        logger.println("testing eeprom:");

        logger.println("Status-Byte:");
        logger.println(System_Storage::hasValidID());
        logger.println(System_Storage::hasValidWifiSSID());
        logger.println(System_Storage::hasValidWifiPW());
        logger.println(System_Storage::hasValidMQTTIP());
        logger.println(System_Storage::hasValidMQTTPort());
        logger.println(System_Storage::hasValidMQTTUsername());
        logger.println(System_Storage::hasValidMQTTPassword());

        logger.println("Done");

    } else {
        logger.println(LOG_TYPE::FATAL, "eeprom isn't initialized");
    }

    logger.decIndent();
}

/**
 * Initializes the chip in network only mode using the serial network
 */
void initModeSerial() {
    bool status = initNetwork(NetworkMode::Serial);
    if (status) {
        logger.println("Serial network initialized");
    } else {
        logger.println(LOG_TYPE::ERR, "serial network was not initialized");
    }
}

/**
 * Initializes the chip in network only mode using the network mode loaded from eeprom
 */
void initModeNetwork() {
    if (eeprom_active_) {
        auto mode = System_Storage::readNetworkMode();
        initNetwork(mode);
    } else {
        logger.println(LOG_TYPE::ERR, "network type could not be loaded");
    }
}

/**
 * Initializes the chip with all gadgets and connectors loaded
 */
void initModeComplete() {
    if (eeprom_active_) {
        auto mode = System_Storage::readNetworkMode();
        initNetwork(mode);
    } else {
        logger.println(LOG_TYPE::ERR, "network type could not be loaded");
        return;
    }

    initConnectors();

    initGadgets();
}

// ===== REFRESHERS =====
/**
 * The refresh method that is supposed to be performed in serial only mode
 */
void refreshModeSerial() {
    handleNetwork();
}

/**
 * The refresh method that is supposed to be performed in network only mode
 */
void refreshModeNetwork() {
    handleNetwork();
}

/**
 * The refresh method that is supposed to be performed in full operation mode
 */
void refreshModeComplete() {

    handleNetwork();

    ir_gadget->refresh();
    handleCodeConnector(ir_gadget);

//  handleCodeRemote();

    for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
        gadgets.getGadget(c)->refresh();
    }
}

/**
 * Refresh method for the main task
 */
void refresh() {
    switch (system_mode) {
        case BootMode::Serial_Ony:
            refreshModeSerial();
            break;
        case BootMode::Network_Only_EEPROM:
            refreshModeNetwork();
            break;
        case BootMode::Full_Operation:
            refreshModeComplete();
            break;
        default:
            delay(1000);
            break;
    }
}

/**
 * Refresh method for network operation
 */
void refreshNetwork() {
    if (network_gadget == nullptr) {
        return;
    }
    network_gadget->refresh();
}
// ===== END REFRESHERS =====

// ===== TASKS =====
/**
 * Function for the main task refresing the main content
 * @param args Unused
 */
[[noreturn]] static void mainTask(void *args) {
  while (true) {
    refresh();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

/**
 * Function for the network tasks receiving and sending requests
 * @param args Unused
 */
[[noreturn]] static void networkTask(void *args) {
  while (true) {
    refreshNetwork();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
// ===== END TASKS =====

// ===== TASK CREATION =====
/**
 * Creates and starts the tasks used by the system
 */
static void createTasks() {
  xTaskCreatePinnedToCore(
    mainTask,     /* Task function. */
    "Smarthome_Main",       /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &main_task,
    0);            /* Task handle. */

  xTaskCreatePinnedToCore(
    networkTask,     /* Task function. */
    "Smarthome_MQTT",       /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &network_task,
    1);            /* Task handle. */

  //    vTaskSuspend(main_task);
  //    vTaskResume(main_task);
}
// ===== END TASK CREATION =====

// ===== MAIN FUNCTIONS =====
void setup() {
    Serial.begin(SERIAL_SPEED);
    logger.println(LOG_TYPE::INFO, "Launching...");

    eeprom_active_ = System_Storage::initEEPROM();
    if (eeprom_active_) {
        client_id_ = System_Storage::readID();
        logger.printfln("Client ID: '%s'", client_id_.c_str());
    }

    testStuff();

    logger.print(LOG_TYPE::INFO, "Boot Mode: ");
    system_mode = getBootMode();

    switch (system_mode) {
        case BootMode::Serial_Ony:
            logger.println("Serial Only");
            initModeSerial();
            break;
        case BootMode::Network_Only_EEPROM:
            logger.println("Network Only: EEPROM");
            initModeNetwork();
            break;
        case BootMode::Full_Operation:
            logger.println("Full Operation");
            initModeComplete();
            break;
        default:
            logger.println("Unknown Boot Mode");
            break;
    }

    logger.printfln("EEPROM usage: %d / %d bytes\n", System_Storage::getEEPROMUsage(), EEPROM_SIZE);
    logger.printfln("Free Heap: %d", ESP.getFreeHeap());

    createTasks();
}

void loop() {}
