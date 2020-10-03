//
// Created by aaron on 2020-09-30.
//

#include "../include/client/client.h"

cis427::Client::Client(const std::string &server_addr) {
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    memset(reinterpret_cast<char *>(&m_sockaddr_in), 0, sizeof(m_sockaddr_in));
    m_sockaddr_in.sin_family = AF_INET;
    m_sockaddr_in.sin_addr.s_addr = inet_addr(server_addr.c_str());
    m_sockaddr_in.sin_port = htons(SERVER_PORT);
}

bool cis427::Client::client_connect() {
    if (connect(m_socket, reinterpret_cast<struct sockaddr *>(&m_sockaddr_in), sizeof(m_sockaddr_in)) < 0) {
        perror("connect");
        close(m_socket);
        return false;
    }
    is_connected = true;
    return is_connected;
}

bool cis427::Client::send_command(const std::array<char, MAX_COMMAND_LENGTH> &buff, const unsigned int &len) {
    int sent = send(m_socket, buff.data(), len, 0);
    if(sent == -1){
        is_connected = false;
        return false;
    }
    return true;
}

cis427::Response cis427::Client::client_recieve() {
    std::array<char, MAX_COMMAND_LENGTH> rbuf{};
    recv (m_socket, rbuf.data(), sizeof(rbuf), 0);
    return Response(rbuf);
}

void cis427::Client::disconnect() {
    is_connected = false;
    close(m_socket);
}

cis427::Client::~Client() {
    disconnect();
}


