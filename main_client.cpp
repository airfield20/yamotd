#include <iostream>
#include <algorithm>
#include <pthread.h>

#include "include/client/client.h"

enum Command{
    MSGGET,
    MSGSTORE,
    LOGIN,
    LOGOUT,
    SHUTDOWN,
    QUIT,
    NONE,
    UNKNOWN
};

/**
 * @brief Just prints some helpful strings for the user
 */
void print_help(){
    std::cout << std::endl << "Please choose from the following commands:" << std::endl;
    std::cout << "MSGGET MSGSTORE LOGIN LOGOUT SHUTDOWN QUIT" << std::endl;
    std::cout << "   0       1             3       4      5" << std::endl;
    std::cout << "You may type the full command or the corresponding number" << std::endl;
}

/**
 * @brief propmts the user for a command, there are only 6 valid commands. Choose one from the help statement
 * @return a string with the users intended command, must be checked for validity
 */
std::string query_user(){
    std::string command;
    while(command == "" ) {
        std::cout << "> ";
        std::getline(std::cin, command);
    }
    if(!std::isdigit(command.at(0))) {
        return command;
    }
    else{
        int num = static_cast<int>(command.at(0)) - 48;
        switch (num) {
            case 0:
                return "MSGGET";
            case 1:
                return "MSGSTORE";
            case 3:
                return "LOGOUT";
            case 4:
                return "SHUTDOWN";
            case 5:
                return "QUIT";
            default:
                break;
        }
    }
    return "UNKNOWN";
}

/**
 * Gets a string from the user that contains a quote.
 * @return quote string
 */
std::string query_quote(){
    std::cout << "Quote > ";
    std::string quote;
    std::getline(std::cin,quote);
    quote = "!" + quote;
    return quote;
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        std::cout << "Usage: client <Server IP Address>" << std::endl;
        exit(1);
    }


    cis427::Client client((std::string(argv[1]))); //connect to ip address given as argument
    if(!client.client_connect()){
        return(1); //exit app if you cant connect
    }
    print_help(); //show user how to use app
    bool quit = false;
    cis427::Response response;
    while(!quit && client.is_connected) { //when the quit flag is set to true the app will exit. Also if the client loses connectivity, the app will exit
        std::string command = query_user(); //get user command
        std::string command_upper = command;
        std::transform(command_upper.begin(), command_upper.end(), command_upper.begin(), ::toupper); //convert to upper case to check command contents easily
        if (client.is_connected) { //only send if client is connected to avoid deadlock
            client.send_command(cis427::to_buff(command), command.size()); //send command as char buffer
            response = client.client_recieve(); //wait until you receive something from server, this should really be threaded
            std::cout << "Return Code: " << response.code << std::endl << response.buff << std::endl; //print server response
        }
        if(command_upper == "MSGSTORE" && response.code == 200 && client.is_connected){ //MSGSTORE is special because it involves a sequence of transactions
            std::string quote = query_quote(); //get the quote after requesting a store
            client.send_command(cis427::to_buff(quote), quote.size()); //send the quote with a ! in front of it to imply its being sent as a quote database modification
            response = client.client_recieve(); //wait for acknowledgment from server, this should be threaded too
            std::cout << "Return Code: " << response.code << std::endl << response.buff << std::endl;
        }
        else if(command_upper == "QUIT"){ //shutdown client app if you type quit, socket will disconnect first though
            break;
        }
        else if(command_upper == "SHUTDOWN"){
            if(response.code == 200){ //only quit client app if server acknowledges the shutdown command
                break;
            }
        }
        if(response.code != 200){
            print_help(); //you screwed up, you must need help
        }
    }
    client.disconnect(); //make sure client is disconnected prior to exiting app

    return 0;
}
