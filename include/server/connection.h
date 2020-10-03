//
// Created by aaron on 2020-09-30.
//

#ifndef P1_CONNECTION_H
#define P1_CONNECTION_H

#include <string>
#include "../../src/helper.hpp"

namespace cis427{
    struct Connection{
        std::string user;
        std::string addr;
        int socket_fd;
        char buff[MAX_COMMAND_LENGTH];
        bool shutdown_command;
    };
}

#endif //P1_CONNECTION_H
