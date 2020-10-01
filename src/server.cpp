//
// Created by aaron on 2020-09-30.
//

#include "server.h"

cis427::Server::Server(const unsigned int &port, const std::string &path_to_messages) :
        m_message_file_path(path_to_messages),
        m_port(port),
        m_messages({}) {

    int err = load_messages(path_to_messages);
    if (err) {
        cerr << "Unable to load messages. Error Code: " << err << endl;
        throw std::runtime_error("MESSAGES NOT FOUND");
    }

    memset(reinterpret_cast<char *>(&m_sockaddr_in), 0, sizeof(m_sockaddr_in));
    m_sockaddr_in.sin_family = AF_INET;
    m_sockaddr_in.sin_addr.s_addr = INADDR_ANY;
    m_sockaddr_in.sin_port = htons(m_port);
}

int cis427::Server::start_server() {
    return 0;
}

int cis427::Server::stop_server(const string &exit_reason) {
    return 0;
}

int cis427::Server::load_messages(const string &path) {
    return 0;
}
