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
    bool stop = false;
    Connection conn;
    conn.thread = 0;
    conn.shutdown_command = false;
    while (!stop) {
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



        pthread_create(&conn.thread, NULL, cis427::Server::thread_loop, &conn);

        while (!stop && (recv(conn.socket_fd, conn.buff, sizeof(conn.buff), 0))) {
            Response output = m_callback_function(conn);
            std::cout << "GOT: " << std::string(conn.buff) << " SENDING: " << std::string(output.buff.data()) << std::endl;
            send (conn.socket_fd, output.to_buff(), strlen(output.to_buff()) + 1, 0);
            if(conn.shutdown_command){
                stop = true;
            }
            cis427::clear_buff(conn.buff);
        }
        close(conn.socket_fd);
        cout << "Connection closed" << endl;
    }

    return 0;
}

void * cis427::Server::thread_loop(void *) {
    return 0;
}

cis427::Server::~Server() {
//close all socket connections here
    for(int i=0;i<clients.size();i++){
        close(clients.at(i).socket_fd);
    }
}

void cis427::Server::set_port(unsigned int port) {
    Server::m_port = m_port;
}



