//
// Created by aaron on 2020-09-30.
//

#include "../include/server/server.h"

cis427::Server::Server(const unsigned int &port) :
        m_port(port),
        m_socklen{},
        m_sockaddr_in{}{
    memset(reinterpret_cast<char *>(&m_sockaddr_in), 0, sizeof(m_sockaddr_in));
    m_sockaddr_in.sin_family = AF_INET;
    m_sockaddr_in.sin_addr.s_addr = INADDR_ANY;
    m_sockaddr_in.sin_port = htons(m_port);

    m_socket = socket(AF_INET, SOCK_STREAM, 0); //0 means choose protocol automatically
    if(m_socket < 0){
        perror("socket");
        exit(1);
    }
//    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<void *>(true), sizeof(int)) < 0) {
//        perror("setsockopt(SO_REUSEADDR) failed");
//        exit(0);
//    }
}

int cis427::Server::start_server(Response(*callback_function)(Connection&)) {
    if((bind(m_socket, reinterpret_cast<sockaddr *>(&m_sockaddr_in), sizeof(m_sockaddr_in))) < 0)
    {
        perror("bind");
        return 1; //1 means error
    }
    m_callback_function = callback_function;
    listen(m_socket, MAX_PENDING_CONNECTIONS);
    m_socklen = sizeof(m_sockaddr_in);
    cout << "The server is up, waiting for connection on port " << m_port << endl;
    Connection conn;
    conn.addr = "";
    clients.push_back(conn);
    connection_handler(clients.at(0));
    return 0;
}

int cis427::Server::stop_server(const string &exit_reason) {
    return 0;
}

int cis427::Server::connection_handler(Connection &conn) {

    /* wait for connection, then receive and print text */
    int len;
    bool stop = false;
    while (!stop) {
        if ((conn.socket_fd = accept(m_socket, (struct sockaddr *)&m_sockaddr_in, &m_socklen)) < 0) {
            perror("accept");
            exit(1);
        }
        cout << "new connection from " << inet_ntoa(m_sockaddr_in.sin_addr) << endl;
        conn.addr = std::string(inet_ntoa(m_sockaddr_in.sin_addr));
        while (!stop && (recv(conn.socket_fd, conn.buff.data(), conn.buff.size(), 0))) {
            Response output = m_callback_function(conn);
            std::cout << "GOT: " << std::string(conn.buff.data()) << " SENDING: " << std::string(output.buff.data()) << std::endl;
            send (conn.socket_fd, output.buff.data(), strlen(output.buff.data()) + 1, 0);
            std::transform(output.buff.begin(), output.buff.end(),output.buff.begin(), ::toupper);
            if(conn.shutdown_command){
                stop = true;
            }
        }

        close(conn.socket_fd);
        cout << "Connection closed" << endl;
    }

    return 0;
}

cis427::Server::~Server() {
//close all socket connections here
    for(const auto& conn: clients){
        close(conn.socket_fd);
    }
}

std::array<char, MAX_COMMAND_LENGTH> cis427::to_buff(const std::string& str){
    std::array<char, MAX_COMMAND_LENGTH> arr{};
    std::copy(str.begin(),str.begin() + MAX_COMMAND_LENGTH, arr.begin());
    return arr;
}