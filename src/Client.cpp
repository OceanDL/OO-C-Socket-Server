// Sock.h
// Created by Ocean Lee 3/12/19

#include <string>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Sock.h"
#include <netinet/in.h> 
#include "tinyxml2.h"


// Reads XML from specified file and returns as string to write to socket
std::string createRequest(std::string file){
    std::string request;
    tinyxml2::XMLDocument requestDoc;
    tinyxml2::XMLError status = requestDoc.LoadFile(file.c_str());
    if(status != tinyxml2::XML_SUCCESS){
        std::cerr << "XML could not be loaded from file";
        exit(EXIT_FAILURE);
    }
    tinyxml2::XMLDocument* docPtr = &requestDoc;
    tinyxml2::XMLPrinter printer;
    docPtr->Accept(&printer);
    request = printer.CStr();
    return request;
}



// Connects to client on specified address and port. Also takes a new xml file as a third parameter for the request message. 
int main(int argc, char **argv)
{
    std::string ip = "";
    int port = 0;
    std::string tempArg = "";
    char *ptr;
    std::string file = "res/request.txt";
    switch (argc) {
        case 4:
            ip = argv[1];
            port = atoi(argv[2]);
            file = argv[3];
            break;
        case 3:
            ip = argv[1];
            port = atoi(argv[2]);
            break;
        case 2: 
            tempArg = argv[1];
            if (tempArg.find('.') != std::string::npos) {
                ip = argv[1];
                port = 5000;
            } else {
                port = strtol(argv[1], &ptr, 10);
                ip = "127.0.0.1";
            }
            break;
        default:
            ip = "127.0.0.1";
            port = 5000;
            break;
    } 
    Sock *clientSock = new Sock();
    clientSock->Connect(ip, port);
    clientSock->Write(createRequest(file));
    std::string buf;
    clientSock->Read(buf, 4096);
    std::cout << buf << std::endl;

    return 0;
}


