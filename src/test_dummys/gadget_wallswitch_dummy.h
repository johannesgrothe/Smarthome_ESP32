#pragma once

#include "../gadgets/wallswitch.h"

class GadgetWallswitchDummy : public Switch {
public:
  GadgetWallswitchDummy(std::string name);

  void refresh();
};

GadgetWallswitchDummy::GadgetWallswitchDummy(std::string name):
    Switch(name) {}

void GadgetWallswitchDummy::refresh() {}