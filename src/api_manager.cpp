#include "api_manager.h"

ApiManager::ApiManager(ApiManagerDelegate *delegate) :
    delegate_(delegate) {

}

void ApiManager::receive(std::shared_pointer<Request> req) {

}
