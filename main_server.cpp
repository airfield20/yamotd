#include "include/server/server.h"
#include "include/server/quotedb.h"

#define SERVER_PORT 8081
#define MESSAGES_PATH "../messages.txt"

cis427::QuoteDB quote_db(MESSAGES_PATH);


bool login(const std::string& username, const std::string& password){
    std::array<std::string, 5> users = {"root", "john", "david", "mary", "notfound"};
    std::array<std::string, 4> passwords = {"root01", "john01", "david01", "mary01"};

    int user_index = std::distance(users.begin(), std::find(users.begin(),users.end(), username));
    if(user_index < 5 && password == passwords.at(user_index) ){
        return true;
    }
    return false;
}

cis427::Response callback(cis427::Connection& conn){
    cis427::Response response{};
    std::string client_input = std::string(conn.buff.data());
    if(client_input.at(client_input.size() - 1) == '\n' ){
        client_input = client_input.substr(0, client_input.size() - 1);
    }
    std::string lowercase = client_input;
    std::transform(client_input.begin(), client_input.end(), client_input.begin(), ::toupper);
    if(client_input == "MSGGET"){
        response.code = 200;
        std::string response_string = "OK" + quote_db.get_random_quote();
        response.buff = cis427::to_buff(response_string);
    }
    else if(client_input == "MSGSTORE"){
        if(conn.user == ""){
            response.code = 401;
            response.buff = cis427::to_buff("You are not currently logged in, login first");
        }
        else{
            response.code = 200;
            response.buff = cis427::to_buff("OK");
            conn.wait_for_follow_up_command = true;
        }
    }
    else if(client_input == "SHUTDOWN"){

        if(conn.user != "root"){
            response.code = 402;
            response.buff = cis427::to_buff("User not allowed to execute this command");
        }
        else{
            response.code = 200;
            response.buff = cis427::to_buff("OK");
        }
    }
    else if(client_input.substr(0, 5) == "LOGIN"){
        std::string credentials = lowercase.substr(6);
        size_t delim = credentials.find(' ');
        std::string username = credentials.substr(0,delim);
        std::string pass = credentials.substr(delim);
        if(login(username,pass)){
            response.code = 200;
            response.buff = cis427::to_buff("OK");
            conn.user = username;
        }
        else{
            response.code = 410;
            response.buff = cis427::to_buff("Wrong UserID or Password");
        }

    }
    else if(client_input == "LOGOUT"){

    }
    else if(client_input == "QUIT"){

    }
    else{
        response.code = 300;
        response.buff = cis427::to_buff("message format error");
    }
    return response;
}

int main() {
    cis427::Server server(SERVER_PORT);
    server.start_server(callback);
    return 0;
}
