//
// Created by aaron on 2020-09-30.
//

#ifndef P1_CLIENT_H
#define P1_CLIENT_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <cstdlib>
#include <pthread.h>

#include "../../src/helper.hpp"
#include "../server/connection.h"

#define SERVER_PORT 8081
#define MAX_COMMAND_LENGTH 5000

namespace cis427 {
    /**
     * @Class This is wrapper class around the c socket interface example provided by the instructor
     */
    class Client {
    public:
        /**
         * @brief Create a single client that connects to a socket server
         * @param server_addr the ip address of the server in X.X.X.X format 127.0.0.1 is local server
         */
        Client(const std::string &server_addr);

        /**
         * @brief Connects the client to the server
         * @return false if connection fails, true if it succeeds
         */
        bool client_connect();

        /**
         * @brief sends a char buffer to the server
         * @param m_buff buffer to be sent to the server
         * @param len number of bytes being sent
         * @return true if something was sent to the server, false if send fails
         */
        bool send_command(const char * buff, const unsigned int &len);

        /**
         * @brief Blocking wait for socket to receive data from server
         * @return a response object that contains the response code and char buffer from the server
         */
        Response client_recieve();

        static void * client_listen(void * sock);

        /**
         * @brief closes the connection to the server
         */
        void disconnect();

        /**
         * Destructor just calls the disconnect function to make sure clients disconnect properly
         */
        virtual ~Client();

        //TODO Make this private, user should not be able to set this
        bool is_connected;

    private:
        struct sockaddr_in m_sockaddr_in;
        std::vector<std::string> m_command_history;
        std::vector<Response> m_response_history;
        int m_socket;
        std::string m_server_addr;
        pthread_t m_listen_thread;
        static std::vector<char *> m_msg_buff;
        static pthread_mutex_t m_msg_buff_lock;

    };

}

#endif //P1_CLIENT_H
