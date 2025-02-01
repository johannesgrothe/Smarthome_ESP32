#include "api/dtos/gadget_update_dto.h"
#include "gadget_manager.h"

#include <console_logger.h>

static auto TAG = "GadgetManager";

GadgetManager::GadgetManager() = default;

bool GadgetManager::addGadget(const std::shared_ptr<Gadget> &new_gadget) {
    gadgets_.push_back(new_gadget);
    return true;
}

std::shared_ptr<Gadget> GadgetManager::getGadget(const std::string &gadget_name) {
    for (auto gadget: gadgets_) {
        if (gadget->getName() == gadget_name) {
            return gadget;
        }
    }
    return nullptr;
}

std::shared_ptr<Gadget> GadgetManager::getGadget(const uint8_t index) const {
    if (index >= gadgets_.size() || index < 0) {
        logger_e(TAG, "invalid index %d (gadget count: %d)", index, gadgets_.size());
        return nullptr;
    }
    return gadgets_[index];
}

std::shared_ptr<Gadget> GadgetManager::operator[](const int index) const {
    return getGadget(index);
}

uint8_t GadgetManager::getGadgetCount() const {
    return static_cast<uint8_t>(gadgets_.size());
}

void GadgetManager::loop() {
    for (const auto &gadget: gadgets_) {
        if (gadget != nullptr) {
            gadget->refresh();
        } else {
            logger_e(TAG, "gadget is nullptr");
        }
    }
}

void GadgetManager::forwardUpdate(const GadgetUpdateDTO &data) {
    for (const auto &gadget: gadgets_) {
        if (gadget->getName() == data.id) {
            logger_i(TAG, "Updating status for gadget '%s'", data.id.c_str());
            gadget->applyChanges(data.properties);
            return;
        }
    }
}
