#pragma once

#include <mutex>
#include <ArduinoJson.h>
#include <api/dtos/gadget_dto.h>


// List of all Gadget Types
enum class GadgetType {
    None, Lightbulb, Fan, Doorbell, Switch, Motion_Sensor, Temp_Humm_Sensor
};

/**
 * Base class for all gadgets in the system
 */
class Gadget {
    // stores whether initializing was successful
    bool init_error_;

    // Name of the gadget
    const std::string name_;

    // Whether the status of the gadget has changed by internal things since the last refresh
    bool has_changed_;

    // Whether the status of the gadget was changed by external things since the last refresh
    bool was_changed_;

protected:
    // Type of the gadget
    const GadgetType type_;

    /**
     * Updates the initialization error. If status_update is true, the error will not change. If its false, the error will now be false.
     * @param status_update The update for the init_error
     */
    void updateInitStatus(bool status_update);

    /**
     * Checks if any characteristic of the gadget was changed by external
     * controlling input since last check (by API or whatever)
     * @return Whether any characteristic of the gadget has changed since last check
     */
    bool wasChanged();

    /**
     * Registers a change to the gadget by anything but public setter methods
     */
    void registerInternalChange();

    /**
     * Registers a change to the gadget by external setter methods
     */
    void registerExternalChange();

    /**
     * Encodes the gadgets properties for the api
     */
    virtual DynamicJsonDocument encodeProperties() = 0;

    /**
     * Encodes the gadgets type for the api
     */
    virtual gadget_definitions::GadgetIdentifier encodeType() = 0;

public:
    virtual ~Gadget() = default;

    /**
     * Constructor for the base gadget
     * @param name Name of the gadget. Should be unique in the system.
     * @param type Type of the gadget
     */
    explicit Gadget(std::string name, GadgetType type);

    /**
     * Returns the type of the gadget
     * @return the type of the gadget
     */
    GadgetType getType() const;

    /**
     * Returns the unique name of the gadget
     * @return the name of the gadget
     */
    std::string getName() const;

    /**
     * Checks if the gadget was successfully initialized
     * @return whether the gadget was successfully initialized
     */
    bool hasInitError() const;

    /**
     * Checks if any characteristic of the gadget was changed by internal changes (NOT external setters)
     * @return Whether any characteristic of the gadget has changed since last check
     */
    bool hasChanged();

    /**
     * Refresh the gadget and its hardware. Used as loop method.
     */
    virtual void refresh() = 0;

    /**
     * Applies changes from the data given by the api to the gadget
     *
     * @param data Data containing the new property values
     */
    virtual void applyChanges(const DynamicJsonDocument &data) = 0;

    /**
     * Encodes the complete gadget into json for the api to consume
     *
     * @return The full gadget encoded in json
     */
    GadgetDTO encode();
};
