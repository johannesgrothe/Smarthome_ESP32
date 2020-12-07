#include "smarthome_gadget_remote.h"

#include <utility>

bool
SmarthomeGadgetRemote::registerGadget(const std::string& gadget_name, GadgetType gadget_type, vector<GadgetCharacteristicSettings> characteristics) {

  DynamicJsonDocument payload(2000);
  unsigned long ident = micros() % 7023;

  payload["gadget_type"] = int(gadget_type);
  payload["gadget_name"] = gadget_name;

  for (auto characteristic_data: characteristics) {
    payload[int(characteristic_data.characteristic)] = JsonObject();
    payload[int(characteristic_data.characteristic)]["max"] = characteristic_data.max;
    payload[int(characteristic_data.characteristic)]["min"] = characteristic_data.min;
    payload[int(characteristic_data.characteristic)]["step"] = characteristic_data.step;
  }

  auto register_req = new Request("smarthome/remotes/gadget/register", int(ident), chip_name, "<bridge>", payload);

  auto resp = req_gadget->sendRequestAndWaitForResponse(register_req, 5000);

  if (resp == nullptr) {
    logger.println(LOG_TYPE::ERR, "No response received");
    delete resp;
    return false;
  }

  auto resp_info = resp->getAck();
  if (!std::get<0>(resp_info)) {
    logger.printfln(LOG_TYPE::ERR, "Registering gadget failed: %s", std::get<1>(resp_info).c_str());
    delete resp;
    return false;
  }

  logger.println("Adding gadget was successful.");
  delete resp;
  return true;
}

bool SmarthomeGadgetRemote::removeGadget(const std::string& gadget_name) {
  return false;
}

void SmarthomeGadgetRemote::handleRequest(std::shared_ptr<Request> req) {
  if (req->getPath() == "smarthome/from/set") {
    auto req_body = req->getPayload();
    if (req_body.containsKey("name") && req_body.containsKey("characteristic") && req_body.containsKey("value")) {
      logger.print("System / Gadget-Remote", "Received characteristic update");
      auto target_gadget = gadgets.getGadget(req_body["name"]);
      if (target_gadget != nullptr) {
        auto characteristic = getCharacteristicFromInt(req_body["characteristic"].as<int>());
        if (characteristic != GadgetCharacteristic::None) {
          logger.incIndent();
          lockUpdates();
          int value = req_body["value"].as<int>();
          target_gadget->handleCharacteristicUpdate(characteristic, value);
          unlockUpdates();
          logger.decIndent();
        } else {
          logger.print(LOG_TYPE::ERR, "Illegal err_characteristic 0");
        }
      } else {
        logger.print("Unknown Gadget");
      }
    }
  } else {
    logger.println("System / Homebridge-Remote", "Received uncomplete Request");
  }
}

SmarthomeGadgetRemote::SmarthomeGadgetRemote(std::shared_ptr<Request_Gadget> gadget) :
  GadgetRemote(std::move(gadget)) {};

void
SmarthomeGadgetRemote::updateCharacteristic(std::string gadget_name, GadgetCharacteristic characteristic, int value) {
  if (updatesAreLocked()) return;
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

  auto out_req = new Request("smarthome/remotes/gadget/update", timestamp, chip_name, "<remote>", req_doc);

  req_gadget->sendRequest(out_req);
}
