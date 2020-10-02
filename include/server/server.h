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
#include <arpa/inet.h>
#include <array>
#include <cstdio>
#include <unistd.h>
#include <algorithm>

#include "connection.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

using cis427::Connection;

#define MAX_PENDING_CONNECTIONS 5

namespace cis427 {
    class Server {
    public:
        Server(const unsigned int& port);
        int start_server(Response(*callback_function)(Connection&));

        virtual ~Server();

    private:
        int stop_server(const std::string& exit_reason);
        int connection_handler(Connection& conn);

        std::vector<Connection> clients;
        unsigned int m_port;
        struct sockaddr_in m_sockaddr_in;
        socklen_t m_socklen;
        Response (*m_callback_function)(Connection&);
        int m_socket;
    };

    std::array<char, MAX_COMMAND_LENGTH> to_buff(const std::string& str);
}

#endif //P1_SERVER_H
