#pragma once

#include <vector>

#include "../system/api_definitions.h"

static const std::vector<std::string> api_paths_broadcast_enabled = {api_definitions::uris::sync_request,
                                                                     api_definitions::uris::test_echo,
                                                                     api_definitions::uris::sync_event,
                                                                     api_definitions::uris::update_gadget};

static const std::vector<std::string> api_paths_incoming = {api_definitions::uris::sync_request,
                                                            api_definitions::uris::test_echo,
                                                            api_definitions::uris::sync_event,
                                                            api_definitions::uris::client_config_delete,
                                                            api_definitions::uris::client_system_config_write,
                                                            api_definitions::uris::client_event_config_write,
                                                            api_definitions::uris::client_gadget_config_write,
                                                            api_definitions::uris::update_gadget};
