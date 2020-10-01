#include "server.h"

#define SERVER_PORT 8081

int main() {
    cis427::Server server(SERVER_PORT, "../messages.txt");
    server.start_server();
    return 0;
}
