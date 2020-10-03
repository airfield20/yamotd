OBJS	= main_server.o server.o quotedb.o main_client.o client.o
OUT	= server client

OBJS0	= main_server.o server.o quotedb.o
SOURCE0	= main_server.cpp src/server.cpp src/quotedb.cpp src/helper.hpp
HEADER0	= ./include/server.h ./include/quotedb.h
OUT0	= server_app

OBJS1	= main_client.o client.o
SOURCE1	= main_client.cpp src/client.cpp src/helper.hpp
HEADER1	= ./include/client.h
OUT1	= client_app

CC	 = g++
FLAGS	 = -g -c -Iinclude/
LFLAGS	 =

all: server client tests

server: $(OBJS0) $(LFLAGS)
	$(CC) -g $(OBJS0) -o $(OUT0)

client: $(OBJS1) $(LFLAGS)
	$(CC) -g $(OBJS1) -o $(OUT1)

main_server.o: main_server.cpp
	$(CC) $(FLAGS) main_server.cpp

server.o: src/server.cpp
	$(CC) $(FLAGS) src/server.cpp

quotedb.o: src/quotedb.cpp
	$(CC) $(FLAGS) src/quotedb.cpp

main_client.o: main_client.cpp
	$(CC) $(FLAGS) main_client.cpp

client.o: src/client.cpp
	$(CC) $(FLAGS) src/client.cpp

clean:
	rm -f $(OBJS) $(OUT)