#pragma once

#include <vector>

#include "../system/api_params.h"

static const std::vector<std::string> api_paths_broadcast_enabled = {PATH_SYNC_REQUEST,
                                                                     PATH_TEST_ECHO,
                                                                     PATH_SYNC_GADGET,
                                                                     PATH_SYNC_EVENT,
                                                                     PATH_UPDATE_GADGET};

static const std::vector<std::string> api_paths_incoming = {PATH_SYNC_REQUEST,
                                                            PATH_TEST_ECHO,
                                                            PATH_SYNC_GADGET,
                                                            PATH_SYNC_EVENT,
                                                            PATH_CLIENT_CONFIG_DELETE,
                                                            PATH_CLIENT_GADGET_CONFIG_WRITE,
                                                            PATH_CLIENT_EVENT_CONFIG_WRITE,
                                                            PATH_CLIENT_SYSTEM_CONFIG_WRITE,
                                                            PATH_UPDATE_GADGET};
