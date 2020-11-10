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
        int start_server(Response(*callback_function)(Connection*));
        /**
         * Allows you to set the port again if for some reason you want to
         * @param port new port you want to use
         */
        void set_port(unsigned int port);
        /**
         * @brief destroys the server and calls close on all socket file descriptors
         */
        virtual ~Server();
        /**
         * @brief this function enables clients to send messages to each other through the server
         * @param msg this object contains a string message and the sender and recipient information
         * @return true if successful, otherwise false
         */
        static bool send_to(Message msg);


    private:

        /**
         * @brief This function accepts new connections and spawns new threads to manage those connections
         * @return 0 when finished
         */
        int connection_handler();

        /**
         * Loop that handles sending and receiving data and calls callback function, it does all this in a separate thread
         * @param conn Connection object to use that has information about the clien
         * @return nothing
         */
        static void * thread_loop(void * connection);

        std::vector<Connection> clients;
        static unsigned int m_port;
        struct sockaddr_in m_sockaddr_in;
        socklen_t m_socklen;
        static Response (*m_callback_function)(Connection*);
        static int m_socket;
        static bool m_stop;
        static fd_set m_master;   // master file descriptor list
        static int m_fdmax;
    };
}

#endif //P1_SERVER_H
