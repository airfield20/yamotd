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
#include <cstdio>
#include <unistd.h>
#include <algorithm>

#include "connection.h"
#include "../../src/helper.hpp"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

using cis427::Connection;

#define MAX_PENDING_CONNECTIONS 5

namespace cis427 {

    /**
     * @class Server class is a wrapper around c socket server implementation provided by professor
     */
    class Server {
    public:
        /**
         *
         * @param port Port that the sever will wait for connections on
         */
        Server(const unsigned int& port);
        /**
         * This function starts waiting for incoming connections from the specified port,
         * when a message is received, it will call the callback function param and pass
         * in a connection object, which contains the input buffer and information about
         * the connection
         * @param callback_function function that will be called whenever data is received
         * @return 0 when the server is no longer running, 1 if the server is unable to start
         */
        int start_server(Response(*callback_function)(Connection&));
        /**
         * Allows you to set the port again if for some reason you want to
         * @param port new port you want to use
         */
        void set_port(unsigned int port);
        /**
         * @brief destroys the server and calls close on all socket file descriptors
         */
        virtual ~Server();

    private:
        /**
         * Loop that handles sending and receiving data and calls callback function
         * @param conn Connection object to use, NOTE: this is only suited for 1 active connection
         * @return 0 when server is no longer active
         */
        int connection_handler(Connection& conn);

        std::vector<Connection> clients;
        unsigned int m_port;
        struct sockaddr_in m_sockaddr_in;
        socklen_t m_socklen;
        Response (*m_callback_function)(Connection&);
        int m_socket;
    };
}

#endif //P1_SERVER_H
