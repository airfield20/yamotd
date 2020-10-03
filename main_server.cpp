#include "include/server/server.h"
#include "include/server/quotedb.h"

#define SERVER_PORT 8081
#define MESSAGES_PATH "../messages.txt"

cis427::QuoteDB quote_db(MESSAGES_PATH);

bool login(const std::string& username, const std::string& password){
    std::string users[5] = {"root", "john", "david", "mary", "notfound"};
    std::string passwords[4] = {"root01", "john01", "david01", "mary01"};

    int user_index = std::distance(users, std::find(users,users, username));
    if(user_index < 5 && password.compare(passwords[user_index]) == 0 ){
        return true;
    }
    return false;
}

cis427::Response callback(cis427::Connection& conn){
    cis427::Response response;
    std::string client_input = std::string(conn.buff);
    if(client_input.at(client_input.size() - 1) == '\n' ){
        client_input = client_input.substr(0, client_input.size() - 1);
    }
    std::string lowercase = client_input;
    std::transform(client_input.begin(), client_input.end(), client_input.begin(), ::toupper);
    if(client_input == "MSGGET"){
        response.code = 200;
        std::string response_string = "OK\n" + quote_db.get_random_quote();
        response.buff = response_string;
    }
    else if(client_input == "MSGSTORE"){
        if(conn.user.empty()){
            response.code = 401;
            response.buff = ("You are not currently logged in, login first");
        }
        else{
            response.code = 200;
            response.buff = ("OK");
        }
    }
    else if(client_input == "SHUTDOWN"){

        if(conn.user != "root"){
            response.code = 402;
            response.buff = ("User not allowed to execute this command");
        }
        else{
            response.code = 200;
            response.buff = ("OK");
            conn.shutdown_command = true;
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
                conn.user = username;
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
        conn.user = "";
    }
    else if(client_input == "QUIT"){
        response.code = 200;
        response.buff = ("OK");
        conn.user = "";
    }
    else if(client_input.at(0) == '!'){
        if(conn.user.empty())
        {
            response.code = 401;
            response.buff = ("You are not currently logged in, login first");
        }
        else{
            quote_db.add_quote(std::string(conn.buff).substr(1), conn.user);
            response.code = 200;
            response.buff = ("OK");
        }
    }
    else{
        response.code = 300;
        response.buff = ("message format error");
    }
    return response;
}

int main() {
    cis427::Server server(SERVER_PORT);
    int stat = server.start_server(callback);
    if(stat == 1){
        server.set_port(SERVER_PORT + 1);
        server.start_server(callback);
    }
    return 0;
}
