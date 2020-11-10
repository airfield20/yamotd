#include "include/server/server.h"
#include "include/server/quotedb.h"

#define SERVER_PORT 8081
#define MESSAGES_PATH "../messages.txt"

cis427::Server server(SERVER_PORT);
cis427::QuoteDB quote_db(MESSAGES_PATH);
std::vector<cis427::Connection> active_users;
std::vector<cis427::Message> messages;

bool login(const std::string& username, const std::string& password){
    std::string users[5] = {"root", "john", "david", "mary", "notfound"};
    std::string passwords[4] = {"root01", "john01", "david01", "mary01"};

    int user_index = 0;
    for(user_index=0;user_index<5;user_index++){
        if(users[user_index] == username){
            break;
        }
    }
    if(user_index < 4 && password.compare(passwords[user_index]) == 0 ){
        return true;
    }
    return false;
}

cis427::Connection copy(const cis427::Connection * conn){
    cis427::Connection ret;
    ret.user = conn->user;
    ret.addr = conn->addr;
    ret.socket_fd = conn->socket_fd;
    ret.thread = conn->thread;
    for(int i=0; i<MAX_COMMAND_LENGTH;i++){
        ret.buff[i] = conn->buff[i];
    }
    ret.shutdown_command = conn->shutdown_command;
    ret.callback_function = conn->callback_function;
    ret.helper = conn->helper;
    return ret;
}

cis427::Response callback(cis427::Connection* conn){
    cis427::Response response;
    std::string client_input = std::string(conn->buff);
    if(client_input.at(client_input.size() - 1) == '\n' ){
        client_input = client_input.substr(0, client_input.size() - 1);
    }
    std::string lowercase = client_input;
    std::transform(client_input.begin(), client_input.end(), client_input.begin(), ::toupper);
    if(conn->helper.substr(0, 4) == "SEND"){
        std::string target = conn->helper.substr(5);
        cis427::Message msg;
        msg.read = false;
        msg.sender = copy(conn);
        if(msg.sender.user.empty()){
            msg.sender.user = "anonymous";
        }
        msg.message = lowercase;
        for(int i=0;i<active_users.size();i++){
            if(active_users.at(i).user == target){
                msg.recipient = copy(&active_users.at(i));
            }
        }
        server.send_to(msg);
        conn->helper.clear();
        response.buff = "OK";
        response.code = 200;
        return response;
    }
    if(client_input == "MSGGET"){
        response.code = 200;
        std::string response_string = "OK\n" + quote_db.get_random_quote();
        response.buff = response_string;
    }
    else if(client_input == "MSGSTORE"){
        if(conn->user.empty()){
            response.code = 401;
            response.buff = ("You are not currently logged in, login first");
        }
        else{
            response.code = 200;
            response.buff = ("OK");
        }
    }
    else if(client_input.substr(0,4) == "SEND"){
        std::string target = client_input.substr(5);
        bool target_found = false;
        for(int i=0;i<active_users.size();i++){
            if(active_users.at(i).user == target){
                target_found = true;
                break;
            }
        }
        if(target_found) {
            std::cout << "sending to " << target << std::endl;
            conn->helper = "SEND " + target;
            response.code = 200;
            response.buff = "OK";
        }
        else{
            response.code = 420;
            response.buff = "Either the user does not exist or is not logged in";
        }
    }
    else if(client_input == "SHUTDOWN"){

        if(conn->user != "ROOT"){
            response.code = 402;
            response.buff = ("User not allowed to execute this command");
        }
        else{
            response.code = 200;
            response.buff = ("OK");
            conn->shutdown_command = true;
        }
    }
    else if(client_input.substr(0, 5) == "LOGIN"){
        try {
            std::string credentials = lowercase.substr(6);
            size_t delim = credentials.find(' ');
            std::string username = credentials.substr(0, delim);
            std::string pass = credentials.substr(delim+1);
            if(login(username,pass)){
                response.code = 200;
                response.buff = ("OK");
                std::transform(username.begin(), username.end(), username.begin(), ::toupper);
                conn->user = username;
                active_users.push_back(*conn);
            }
            else{
                response.code = 410;
                response.buff = ("Wrong UserID or Password");
            }
        } catch (...) {
            response.code = 410;
            response.buff = ("Error: Unable to parse login, Usage > login username password");
        }


    }
    else if(client_input == "LOGOUT"){
        response.code = 200;
        response.buff = ("OK");
        if(!conn->user.empty()){
            for(int i=0; i<active_users.size();i++){
                if(active_users.at(i).user == conn->user){
                    active_users.erase(active_users.begin() + i);
                }
            }
        }
        conn->user = "";
    }
    else if(client_input == "WHO"){
        response.code = 200;
        response.buff="OK\n";
        for(int i=0;i<active_users.size();i++){
            response.buff.append(active_users.at(i).user + "       " + conn->addr + "\n");
        }
    }
    else if(client_input == "END"){
        if(!conn->user.empty()){
            for(int i=0; i<active_users.size();i++){
                if(active_users.at(i).user == conn->user){
                    active_users.erase(active_users.begin() + i);
                }
            }
        }
    }
    else if(client_input == "QUIT"){
        response.code = 200;
        response.buff = ("OK");
        conn->user = "";
    }
    else if(client_input.at(0) == '!'){
        if(conn->user.empty())
        {
            response.code = 401;
            response.buff = ("You are not currently logged in, login first");
        }
        else{
            quote_db.add_quote(std::string(conn->buff).substr(1), conn->user);
            response.code = 200;
            response.buff = ("OK");
        }
    }
    else{
        response.code = 300;
        response.buff = ("message format error");
    }
    int x=2;
    return response;
}

int main(int argc, char * argv[]) {
    std::cout << "Creating server" << std::endl;
    std::cout << "Starting server" << std::endl;
    server.start_server(callback);
    return 0;
}
