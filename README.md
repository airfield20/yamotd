# Yet another message of the day

CIS 427 Program 1

This project uses catchv1 to support the compiler installed on the UMD servers.

UMD uses g++ v 4.4.7 which has very limited c++11 features and does not support std::threading

The sole author of this project is Aaron Cofield. I did not have a partner

## Building
There are two methods provided to build the project. Cmake (included for CLion and Kdevelop compatability) and Makefile (Submission requirement)
 - To build with make just type `make` in the projects root directory
 - Cmake usage is standard:
```
mkdir build -p; cd build
cmake ..
make
```
 ## Usage
 
 - The makefile build generates two binary execitables, server and client. 
 - To run the server just run `./server`
 - To run the client you must provide an ip address Ex: `./client 127.0.0.1`
 
 ## Tests
 - To run tests you must build with cmake, tests are created using legacy catch to maintain compatability with c++03 used by UMD servers
 
 ## Documentation is here
 [Docs](docs/html/index.html)
 
 Per submission requirements a list of functions is required
 - 
 
 ## Submission Requirements
 ### Known bugs
  - None
  
 ### Sample outputs
 #### Server
  ```
 "/home/aaron/Documents/School/CIS 427/P1/cmake-build-debug/P1_server"
 The server is up, waiting for connection on port 8081
 new connection from 127.0.0.1
 GOT: MSGGET SENDING: OK
 The greatest of all weaknesses is the fear of appearing weak.
 -- J. B. Bossuet, Politics from Holy Writ, 1709
 GOT: MSGSTORE SENDING: You are not currently logged in, login first
 GOT: login root root01 SENDING: OK
 GOT: msgstore SENDING: OK
 GOT: !The universe expands left SENDING: OK
 GOT: logout SENDING: OK
 GOT: shutdown SENDING: User not allowed to execute this command
 GOT: login root root01 SENDING: OK
 GOT: shutdown SENDING: OK
 Connection closed
 ```

 #### Client:
 ```
"/home/aaron/Documents/School/CIS 427/P1/cmake-build-debug/P1_client" 127.0.0.1

Please choose from the following commands:
MSGGET MSGSTORE LOGIN LOGOUT SHUTDOWN QUIT
   0       1             3       4      5
You may type the full command or the corresponding number
> MSGGET
Return Code: 200
OK
The greatest of all weaknesses is the fear of appearing weak.
-- J. B. Bossuet, Politics from Holy Writ, 1709
> MSGSTORE
Return Code: 401
You are not currently logged in, login first

Please choose from the following commands:
MSGGET MSGSTORE LOGIN LOGOUT SHUTDOWN QUIT
   0       1             3       4      5
You may type the full command or the corresponding number
> login root root01
Return Code: 200
OK
> msgstore
Return Code: 200
OK
Quote > The universe expands left
Return Code: 200
OK
> logout
Return Code: 200
OK
> shutdown
Return Code: 402
User not allowed to execute this command

Please choose from the following commands:
MSGGET MSGSTORE LOGIN LOGOUT SHUTDOWN QUIT
   0       1             3       4      5
You may type the full command or the corresponding number
> login root root01
Return Code: 200
OK
> shutdown
Return Code: 200
OK

Process finished with exit code 0
```