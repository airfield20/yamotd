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
    pthread_mutex_init(&m_msg_buff_lock, NULL);
    pthread_create(&m_listen_thread, NULL, cis427::Client::client_listen, &this->m_socket);
    return is_connected;
}

bool cis427::Client::send_command(const char * buff, const unsigned int &len) {
    int sent = send(m_socket, buff, len, 0);
    if(sent == -1){
        is_connected = false;
        return false;
    }
    return true;
}

cis427::Response cis427::Client::client_recieve() {
    bool flag = true;
    while(flag) {
        pthread_mutex_lock(&m_msg_buff_lock);
        for (int i = 0; i < m_msg_buff.size(); i++) {
            if (m_msg_buff.at(i)[MAX_COMMAND_LENGTH - 1] != '\a') {
                Response ret(m_msg_buff.at(i));
                m_msg_buff.erase(m_msg_buff.begin() + i);
                pthread_mutex_unlock(&m_msg_buff_lock);
                return ret;
            }
        }
        pthread_mutex_unlock(&m_msg_buff_lock);
    }
}

std::vector<char *> cis427::Client::m_msg_buff;
pthread_mutex_t cis427::Client::m_msg_buff_lock;

void *  cis427::Client::client_listen(void * sock) {
    char rbuf[MAX_COMMAND_LENGTH];
    while(true){
        int nb = recv(*(int *)sock, rbuf, sizeof(rbuf), 0);
        if(nb > 0){
            pthread_mutex_lock(&m_msg_buff_lock);
            m_msg_buff.push_back(rbuf);
            pthread_mutex_unlock(&m_msg_buff_lock);
            if(rbuf[0] == '\a'){
                std::string sender;
                std::string message;
                bool modfound = false;
                for(int i=1;i<nb;i++){
                    if(rbuf[i] == '%'){
                        modfound = true;
                        continue;
                    }
                    if(!modfound) {
                        sender.push_back(rbuf[i]);
                    }
                    else{
                        message.push_back(rbuf[i]);
                    }
                }

                std::cout << "Message from " << sender << "\n" <<sender<<": " << message << std::endl;
            }
        }
    }
}

void cis427::Client::disconnect() {
    is_connected = false;
    close(m_socket);
}

cis427::Client::~Client() {
    disconnect();
}




