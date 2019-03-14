// Sock.h
// Created by Ocean Lee 3/12/19

#define SOCK_H
#include <string>
#include <string.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib> 



class Sock
{
public:
    int sock;
    Sock();
    Sock(int domain, int type, int protocol);
    int Bind(std::string ip, int port);
    int Listen(int queue);
    Sock* Accept();
    int Read(std::string &buf, int bufLength);
    int Write(std::string request);
    int Connect(std::string ip, int port);
    ~Sock();
private:
    int domain;
    int type;
    int protocol;
    std::string ip;
    std::string port;
    char buffer[4096];
    struct sockaddr_in address; 
};









