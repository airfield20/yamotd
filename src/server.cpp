//
// Created by aaron on 2020-09-30.
//

#include "../include/server/server.h"

cis427::Server::Server(const unsigned int &port){
    m_port = port;
    FD_ZERO(&m_master);

    memset(reinterpret_cast<char *>(&m_sockaddr_in), 0, sizeof(m_sockaddr_in));
    m_sockaddr_in.sin_family = AF_INET;
    m_sockaddr_in.sin_addr.s_addr = INADDR_ANY;
    m_sockaddr_in.sin_port = htons(m_port);

    m_socket = socket(AF_INET, SOCK_STREAM, 0); //0 means choose protocol automatically
    if(m_socket < 0){
        perror("socket");
        exit(1);
    }
    int yes=1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(1);
    }
}

bool cis427::Server::m_stop = false;
fd_set cis427::Server::m_master;
cis427::Response (*cis427::Server::m_callback_function)(Connection&);

int cis427::Server::start_server(Response(*callback_function)(Connection&)) {
    if((bind(m_socket, reinterpret_cast<sockaddr *>(&m_sockaddr_in), sizeof(m_sockaddr_in))) < 0)
    {
        perror("bind");
        return 1; //1 means error
    }
    m_callback_function = callback_function;
    listen(m_socket, MAX_PENDING_CONNECTIONS);
    FD_SET(m_socket, &m_master);
    m_fdmax = m_socket;
    m_socklen = sizeof(m_sockaddr_in);
    cout << "The server is up, waiting for connection on port " << m_port << endl;
    connection_handler();
    return 0;
}

int cis427::Server::connection_handler() {

    /* wait for connection, then receive and print text */
    int len;
    Connection conn;
    conn.thread = 0;
    conn.shutdown_command = false;
    while (!m_stop) {
        if ((conn.socket_fd = accept(m_socket, (struct sockaddr *)&m_sockaddr_in, &m_socklen)) < 0) {
            perror("accept");
            exit(1);
        }
        FD_SET(conn.socket_fd, &m_master);
        if(conn.socket_fd > m_fdmax){
            m_fdmax = conn.socket_fd;
        }
        cout << "new connection from " << inet_ntoa(m_sockaddr_in.sin_addr) << endl;
        conn.addr = std::string(inet_ntoa(m_sockaddr_in.sin_addr));

        if(pthread_create(&conn.thread, NULL, cis427::Server::thread_loop, &conn) != 0){
            perror("pthreaed_create");
            exit(1);
        }
    }
    return 0;
}

[[noreturn]] void * cis427::Server::thread_loop(void * connection) {
    int num_bytes_received = 0;
    struct Connection * conn = (Connection *) connection;

    while(true) {
        if ((num_bytes_received = recv(conn->socket_fd, conn->buff, sizeof(conn->buff), 0)) <= 0) {
            if (num_bytes_received == 0) {
                cout << "Server: socket " << conn->socket_fd << " hung up" << endl;
            } else {
                perror("recv");
            }

            close(conn->socket_fd);
            FD_CLR(conn->socket_fd, &m_master);
            pthread_exit(0);
        }
        else{
            Connection arg = *conn;
            arg.socket_fd = conn->socket_fd;
            arg.addr = conn->addr;
            arg.callback_function = conn->callback_function;
            arg.shutdown_command = conn->shutdown_command;
            arg.thread = conn->thread;
            arg.user = conn->user;
            for(int i=0;i<MAX_COMMAND_LENGTH;i++)
            {
                arg.buff[i] = conn->buff[i];
            }
            int lk = 2;
            Response output = m_callback_function(arg);

            std::cout << "GOT: " << std::string(conn->buff) << " SENDING: " << std::string(output.buff.data()) << std::endl;
            send(conn->socket_fd, output.to_buff(), strlen(output.to_buff()) + 1, 0);
            if (conn->shutdown_command) {
                m_stop = true;
            }
            cis427::clear_buff(conn->buff);
        }

        close(conn->socket_fd);
        cout << "Connection closed" << endl;
    }
    return 0;
}

cis427::Server::~Server() {
//close all socket connections here
    for(int i=0;i<clients.size();i++){
        close(clients.at(i).socket_fd);
    }
}

void cis427::Server::set_port(unsigned int port){
    Server::m_port = m_port;
}



