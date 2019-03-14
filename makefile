CC = g++
CFLAGS= -std=c++14 -pthread 
CLIENT_OUT = Client
SERVER_OUT = Server
PROG = Server Client




mainmake: Server Client

Client: out/Client.o out/Sock.o
	$(CC) $(CFLAGS) -o bin/$(CLIENT_OUT) out/Client.o out/Sock.o out/tinyxml2.o

Server: out/Server.o out/Sock.o out/tinyxml2.o
	$(CC) $(CFLAGS) -o bin/$(SERVER_OUT) out/Server.o out/Sock.o out/tinyxml2.o 

out/Client.o: src/Client.cpp
	$(CC) $(CFLAGS) -o out/Client.o -c src/Client.cpp

out/Server.o: src/Server.cpp
	$(CC) $(CFLAGS) -o out/Server.o -c src/Server.cpp

out/Sock.o: src/Sock.cpp
	$(CC) $(CFLAGS) -o out/Sock.o -c src/Sock.cpp

out/tinyxml2.o: src/tinyxml2.cpp
	$(CC) $(CFLAGS) -o out/tinyxml2.o -c src/tinyxml2.cpp

.PHONY: clean

clean:
	rm -f out/* bin/*