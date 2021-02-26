# Yet another message of the day

## CIS 427 Program 2

This project uses catchv1 to support the compiler installed on the UMD servers.

UMD uses g++ v 4.4.7 which has very limited c++11 features and does not support std::threading

The sole author of this project is Aaron Cofield. I did not have a partner

## Note:

All code is thoroughly documented with doxygen style comments and inline comments

## Building

There are two methods provided to build the project. cmake \(included for CLion and Kdevelop compatibility\) and Makefile \(Submission requirement\)

* To build with make just type `make` in the projects root directory
* cmake usage is standard:

  ```text
  mkdir build -p; cd build
  cmake ..
  make
  ```

  **Usage**

* The makefile build generates two binary executables, server and client.
* To run the server **cd into build directory first** then run `./server`
* To run the client **cd into build directory first** and you must provide an ip address Ex: `./client 127.0.0.1`

  **Tests**

* To run tests you must build with cmake, tests are created using legacy catch to maintain compatability with c++03 used by UMD servers

  Per submission requirements a list of functions is required

* print\_help
* query\_user
* query\_quote
* login
* callback
* Response\(\)
* Response\(const int& icode, const std::string& ibuff\)3
* Response\(const std::array& ibuff\)
* std::array to\_buff\(\)
* inline std::string buff\_to\_string\(const std::array& buff\){
  * inline void clear\_buff\(std::array& buff\){
  * inline std::array to\_buff\(const std::string &str\) {
  * Client\(const std::string &server\_addr\);
  * bool client\_connect\(\);
  * bool send\_command\(const std::array &buff, const unsigned int &len\);
  * Response client\_recieve\(\);
  * void disconnect\(\);
  * virtual ~Client\(\);
  * Server\(const unsigned int& port\);
  * int start\_server\(Response\(\*callback\_function\)\(Connection&\)\);
  * void set\_port\(unsigned int port\);
  * virtual ~Server\(\);
  * explicit QuoteDB\(const std::string &path\_to\_quotes\);
  * QuoteDB\(\);
  * std::string get\_random\_quote\(\);
  * std::string get\_quote\(const unsigned long &index\);
  * int get\_num\_quotes\(\);
  * bool add\_quote\(const std::string& quote, const std::string& author\);
  * bool remove\_last\_quote\(\);
* write\_quotes\_to\_file

## Submission Requirements

### Known bugs

* when shutting down server, the server will not exit until another client attempts to connect.

  **Sample outputs**

  **Server**

  ```text
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

  **Client:**

  \`\`\` "/home/aaron/Documents/School/CIS 427/P1/cmake-build-debug/P1\_client" 127.0.0.1

Please choose from the following commands: MSGGET MSGSTORE LOGIN LOGOUT SHUTDOWN QUIT 0 1 3 4 5 You may type the full command or the corresponding number

> MSGGET Return Code: 200 OK The greatest of all weaknesses is the fear of appearing weak. -- J. B. Bossuet, Politics from Holy Writ, 1709 MSGSTORE Return Code: 401 You are not currently logged in, login first

Please choose from the following commands: MSGGET MSGSTORE LOGIN LOGOUT SHUTDOWN QUIT 0 1 3 4 5 You may type the full command or the corresponding number

> login root root01 Return Code: 200 OK msgstore Return Code: 200 OK Quote &gt; The universe expands left Return Code: 200 OK logout Return Code: 200 OK shutdown Return Code: 402 User not allowed to execute this command

Please choose from the following commands: MSGGET MSGSTORE LOGIN LOGOUT SHUTDOWN QUIT 0 1 3 4 5 You may type the full command or the corresponding number

> login root root01 Return Code: 200 OK shutdown Return Code: 200 OK

Process finished with exit code 0 \`\`\`

