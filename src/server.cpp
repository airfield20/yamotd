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
cis427::Response (*cis427::Server::m_callback_function)(Connection*);
int cis427::Server::m_fdmax;
int cis427::Server::m_socket;
unsigned int cis427::Server::m_port;

int cis427::Server::start_server(Response(*callback_function)(Connection*)) {
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
    while (!m_stop) {
        Connection * conn = new Connection;
        conn->thread = 0;
        conn->shutdown_command = false;

        if ((conn->socket_fd = accept(m_socket, (struct sockaddr *)&m_sockaddr_in, &m_socklen)) < 0) {
            perror("accept");
            exit(1);
        }
        FD_SET(conn->socket_fd, &m_master);
        if(conn->socket_fd > m_fdmax){
            m_fdmax = conn->socket_fd;
        }
        cout << "new connection from " << inet_ntoa(m_sockaddr_in.sin_addr) << endl;
        conn->addr = std::string(inet_ntoa(m_sockaddr_in.sin_addr));

        if(pthread_create(&conn->thread, NULL, cis427::Server::thread_loop, conn) != 0){
            perror("pthread_create");
            delete conn;
            exit(1);
        }
    }
    return 0;
}

void * cis427::Server::thread_loop(void * connection) {
    int num_bytes_received = 0;
    struct Connection * conn = (Connection *) connection;
    Connection * arg = new Connection;
    arg->socket_fd = conn->socket_fd;
    arg->addr = conn->addr;
    arg->callback_function = conn->callback_function;
    arg->shutdown_command = conn->shutdown_command;
    arg->thread = conn->thread;
    arg->user = conn->user;
    for(int i=0;i<MAX_COMMAND_LENGTH;i++)
    {
        arg->buff[i] = conn->buff[i];
    }

    while(!m_stop) {
        cis427::clear_buff(arg->buff);
        if ((num_bytes_received = recv(arg->socket_fd, arg->buff, sizeof(arg->buff), 0)) <= 0) {
            if (num_bytes_received == 0) {
                cout << "Server: socket " << conn->socket_fd << " hung up" << endl;
            } else {
                perror("recv");
            }

            close(conn->socket_fd);
            FD_CLR(conn->socket_fd, &m_master);
            cout << "Connection closed" << endl;
            arg->buff[0] = 'E';
            arg->buff[1] = 'N';
            arg->buff[2] = 'D';
            m_callback_function(arg);
            pthread_exit(0);
        }
        else{

            int lk = 2;
            Response output = m_callback_function(arg);

            std::cout << "GOT: " << std::string(arg->buff) << " SENDING: " << std::string(output.buff.data()) << std::endl;
            send(arg->socket_fd, output.to_buff(), strlen(output.to_buff()) + 1, 0);
            if (arg->shutdown_command) {
                for(int i=0;i<=m_fdmax;i++){
                    if(FD_ISSET(i,&m_master)){
                        if(i != m_socket){
                            std::string buff = "210 the server is about to shut down";
                            Message m;
                            m.message = "210 the server is about to shut down";
                            m.sender.user = "server";
                            m.recipient.socket_fd = i;
                            send_to(m);
                            //send(i, buff.c_str(),buff.size(), 0);
                        }
                    }
                }
                m_stop = true;
                pthread_exit(0);

            }
            cis427::clear_buff(arg->buff);
        }
    }
    close(conn->socket_fd);
    cout << "Connection closed" << endl;
    return 0;
}

bool cis427::Server::send_to(cis427::Message msg) {
    std::string serialized = "\a" + msg.sender.user + "%" + msg.message;
    int success = send(msg.recipient.socket_fd, serialized.c_str(), strlen(serialized.c_str()) + 1, 0);
    if(success > 0) return true;
    perror("send");
    std::cerr << "Failed to send message" << std::endl;
    return false;
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





