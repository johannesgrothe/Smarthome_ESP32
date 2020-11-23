#include "event_remote.h"

void EventRemote::forwardEvent(std::shared_ptr<Event> event){
  for (int i = 0; i < gadgets.getGadgetCount(); i++){
    //TODO gadgets[i].handleEvent(event)
  }
}

EventRemote::EventRemote() :
    Remote() {};

EventRemote::EventRemote(std::shared_ptr<Request_Gadget> gadget) :
    Remote(gadget, std::__cxx11::string()) {};


