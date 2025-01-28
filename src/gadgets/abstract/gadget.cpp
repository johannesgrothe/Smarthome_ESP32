#include "gadget.h"

#include <console_logger.h>
#include <utility>

Gadget::Gadget(std::string name, const GadgetType type) : init_error_(false),
                                                          name_(std::move(name)),
                                                          has_changed_(true),
                                                          was_changed_(true),
                                                          type_(type) {
}

GadgetType Gadget::getType() const {
    return type_;
}

std::string Gadget::getName() const {
    return name_;
}

void Gadget::updateInitStatus(const bool status_update) {
    if (!status_update) {
        init_error_ = true;
    }
}

bool Gadget::hasInitError() const {
    return init_error_;
}

bool Gadget::hasChanged() {
    const bool buf = has_changed_;
    has_changed_ = false;
    if (buf) {
        logger_i(getName(), "Internal change detected.");
    }
    return buf;
}

bool Gadget::wasChanged() {
    const bool buf = was_changed_;
    was_changed_ = false;
    if (buf) {
        logger_i(getName(), "External change detected.");
    }
    return buf;
}

void Gadget::registerInternalChange() {
    has_changed_ = true;
}


void Gadget::registerExternalChange() {
    was_changed_ = true;
}

GadgetDTO Gadget::encode() {
    logger_e(TAG, "GadgetDTO::encode()");
    const auto properties = encodeProperties();
    const auto type = encodeType();
    return {type, name_, properties};
}
