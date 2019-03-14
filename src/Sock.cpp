// Sock.cpp
// Created by Ocean Lee 3/12/19

#include "Sock.h"
#include <unistd.h>

// Default constructor, assumes standard ipv4, TCP, and IP protocal
Sock::Sock()
{
    domain = AF_INET;
    type =  SOCK_STREAM;
    protocol = 0; 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        std::cerr << "Failed to open Socket";
    }
}

// Constructor for ipv6, udp, or other internet protocals
Sock::Sock(int domain, int type, int protocol)
{
    this->domain = domain;
    this->type = type;
    this->protocol = protocol;
    sock = socket(domain, type, protocol);
    if(sock < 0) {
        std::cerr << "Failed to open Socket" << std::endl;
    }
}

// Binds Socket to IP and port
int Sock::Bind(std::string ip, int port)
{
    int sleepTime = 1;
    this->ip = ip;
    this->port = port;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &address.sin_addr);

    int success = bind(sock, (sockaddr*)&address, sizeof(address));

    while(success < 0) {
        std::cerr << "Failed to bind Socket" << std::endl;
        std::cerr << "Retrying in "  << sleepTime << " seconds..." << std::endl;
        
        sleep(sleepTime);
        sleepTime *= 2;
        sock = socket(domain, type, protocol);
        success = bind(sock, (sockaddr*)&address, sizeof(address));
    }
    std::cout << "Socket bound succesfully" << std::endl;
    
    return success; 
}

// Puts socket in listen mode, queue specifies how many connections can be waited upon
int Sock::Listen(int queue)
{
    int success = listen(sock, queue);
    if(success < 0) {
        std::cerr << "Failed to listen on Socket" << std::endl;
    }

    return success;
}

// Returns pointer to accepted client socket, establishes connection between client and server
Sock* Sock::Accept(){
    int aLength = sizeof(address);
    int clientSock = accept(sock, (struct sockaddr *)&address, (socklen_t*)&aLength);
    if(clientSock < 0) {
        std::cerr <<  "Could not accept Client connection" << std::endl;
    }
    Sock *clientSocket = new Sock();
    clientSocket->sock = clientSock;
    clientSocket->port = port;

    return clientSocket;
}

// Takes string and buffer length for message to be read
int Sock::Read(std::string &buf, int bufLength){
    char readBuf[bufLength];
    memset(readBuf, 0, bufLength);
    int success = read(sock, readBuf, bufLength);
    if(success < 0){
        std::cerr << "Message could not be read" << std::endl;
    }
    buf = std::string(readBuf);
    return success;
}

// Takes message to send to client
int Sock::Write(std::string request){
     const char * msg = request.c_str();
     int length = (int)strlen(msg);
     int success = (int)send(sock, msg, length, 0);
     if(success < 0){
         std::cerr << "Could not write request" << std::endl;
     }
     return success;
}

// Connect for client 
int Sock::Connect(std::string ip, int port){
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; 
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
    int success = ::connect(sock, (struct sockaddr *)&address, sizeof(address));
    if (success < 0) {
            std::cerr << "Could not connect" << std::endl;
        }
    return success;
}

// Deconstructor to close socket and terminate connection with client
Sock::~Sock()
{
    shutdown(sock,SHUT_RDWR);
    close(sock);
}
