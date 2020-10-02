//
// Created by aaron on 2020-09-30.
//

#ifndef P1_CONNECTION_H
#define P1_CONNECTION_H

#include <string>

#define MAX_COMMAND_LENGTH 500

namespace cis427{
    struct Connection{
        std::string user;
        std::string addr;
        int socket_fd;
        std::array<char, MAX_COMMAND_LENGTH> buff;
        bool wait_for_follow_up_command = false;
        bool shutdown_command = false;
    };

    struct Response{
        int code;
        std::array<char, MAX_COMMAND_LENGTH> buff;
    };
}

#endif //P1_CONNECTION_H
