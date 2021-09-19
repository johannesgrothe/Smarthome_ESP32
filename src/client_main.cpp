#include "client_main.h"

ClientMain::ClientMain() :
    ApiManagerDelegate(),
    api_manager_(this, nullptr) {

}
