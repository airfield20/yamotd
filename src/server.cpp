//
// Created by aaron on 2020-09-30.
//

#include "server.h"

cis427::Server::Server(const unsigned int &port, const std::string &path_to_messages) :
        m_message_file_path(path_to_messages),
        m_port(port),
        m_buff({}),
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

}

int cis427::Server::start_server() {
    if((bind(m_socket, reinterpret_cast<sockaddr *>(&m_sockaddr_in), sizeof(m_sockaddr_in))) < 0)
    {
        perror("bind");
        return 1; //1 means error
    }
    listen(m_socket, MAX_PENDING_CONNECTIONS);
    m_socklen = sizeof(m_sockaddr_in);
    cout << "The server is up, waiting for connection on port " << m_port << endl;
    Connection conn;
    conn.addr = "";
    conn.isLoggedIn = false;
    clients.push_back(conn);
    connection_handler(clients.at(0));
    return 0;
}

int cis427::Server::stop_server(const string &exit_reason) {
    return 0;
}

string cis427::Server::connection_handler(Connection &conn) {

    /* wait for connection, then receive and print text */
    int new_s;
    int len;
    while (1) {
        if ((new_s = accept(m_socket, (struct sockaddr *)&m_sockaddr_in, &m_socklen)) < 0) {
            perror("accept");
            exit(1);
        }
        cout << "new connection from " << inet_ntoa(m_sockaddr_in.sin_addr) << endl;

        while ((recv(new_s, m_buff.data(), m_buff.size(), 0))) {
            cout << std::string(m_buff.data());
            send (new_s, m_buff.data(), strlen(m_buff.data()) + 1, 0);
        }

        close(new_s);
    }

    return std::__cxx11::string("");
}

cis427::Server::~Server() {
//close all socket connections here
}
