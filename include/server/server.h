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
#include <stdio.h>
#include <unistd.h>

#include "connection.h"
#include "quotedb.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

using cis427::Connection;

#define MAX_COMMAND_LENGTH 500
#define MAX_PENDING_CONNECTIONS 5

namespace cis427 {
    class Server {
    public:
        Server(const unsigned int& port, const std::string& path_to_messages);
        int start_server();

        virtual ~Server();

    private:
        int stop_server(const std::string& exit_reason);
        string connection_handler(Connection& conn);

        std::string m_message_file_path;
        cis427::QuoteDB m_quote_db;
        std::vector<Connection> clients;
        unsigned int m_port;
        struct sockaddr_in m_sockaddr_in;
        socklen_t m_socklen;
        std::array<char, MAX_COMMAND_LENGTH> m_buff;
        int m_socket;
    };
}

#endif //P1_SERVER_H
