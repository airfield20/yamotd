//
// Created by aaron on 2020-09-30.
//

#ifndef P1_CONNECTION_H
#define P1_CONNECTION_H

#include <string>
#include <pthread.h>

#include "../../src/helper.hpp"

namespace cis427{
    struct Connection{
        std::string user;
        std::string addr;
        int socket_fd;
        pthread_t thread;
        char buff[MAX_COMMAND_LENGTH];
        bool shutdown_command;
        Response (*callback_function)(Connection&);
    };
}

#endif //P1_CONNECTION_H
