#pragma once

#include "../gadgets/sh_lamp.h"

class DummyLamp : public SH_Lamp {
public:
  DummyLamp(std::string name, SHLampType lamp_type);

  void refresh();
};

DummyLamp::DummyLamp(std::string name, SHLampType lamp_type):
  SH_Lamp(name, lamp_type) {}

void DummyLamp::refresh(){}