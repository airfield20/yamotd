//
// Created by aaron on 2020-09-30.
//

#ifndef P1_SERVER_H
#define P1_SERVER_H

#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <array>

#include "connection.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

using cis427::Connection;

#define MAX_COMMAND_LENGTH 500

namespace cis427 {
    class Server {
    public:
        Server(const unsigned int& port, const std::string& path_to_messages);
        int start_server();

    private:
        int stop_server(const std::string& exit_reason);
        int load_messages(const std::string& path);

        std::string m_message_file_path;
        std::vector<string> m_messages;
        unsigned int m_port;
        struct sockaddr_in m_sockaddr_in;
        socklen_t m_socklen;
        std::array<char, MAX_COMMAND_LENGTH> m_buff;
    };
}

#endif //P1_SERVER_H
