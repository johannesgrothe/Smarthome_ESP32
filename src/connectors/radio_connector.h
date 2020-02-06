#ifndef RADIO_CONNECTOR_H
#define RADIO_CONNECTOR_H

class Radio_Gadget {
protected:

  bool is_initialized;

public:
  Radio_Gadget();

};

class Radio_Connector {
protected:

  Radio_Gadget *radiogadget;

public:
  Radio_Connector();

};
#endif //RADIO_CONNECTOR_H
