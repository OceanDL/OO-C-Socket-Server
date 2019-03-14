// Server.cpp
// Created by Ocean Lee 3/12/19


#include <string>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Sock.h"
#include "tinyxml2.h"
#include <chrono>
#include <ctime> 
#include <thread>


// Create response takes the client request as a string, parses the request's XML format,
// and formulates an XML response while printing the request to the console. 
std::string createResponse(std::string request) {
    std::string response;
    std::string status;
    const time_t ctt = time(0);
    tinyxml2::XMLDocument requestDoc;
    tinyxml2::XMLDocument responseDoc;
    int success = requestDoc.Parse(request.c_str()); 
    if(success != tinyxml2::XML_SUCCESS) {
        response = "Unknown Command";
    } else {
        tinyxml2::XMLNode* reqRoot = requestDoc.FirstChildElement("request");
        tinyxml2::XMLElement* commandElement = reqRoot->FirstChildElement("command");
        tinyxml2::XMLNode* dataNode = reqRoot->FirstChildElement("data");
        if (reqRoot == nullptr || commandElement == nullptr){
            status = "Incomplete: Error parsing XML elements";
        } else{
            std::cout << "Command: " << std::endl << commandElement->GetText() << std::endl << "Data Rows: " << std::endl;
            if(dataNode->FirstChild() != nullptr) {
                for (tinyxml2::XMLElement* elem = dataNode->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
                    std::cout << elem->Attribute("type") << ": " << elem->GetText() << std::endl;
                }
            } 
            status = "Complete";
        }
        tinyxml2::XMLNode* resRoot = responseDoc.NewElement("response");
        responseDoc.InsertFirstChild(resRoot);
        tinyxml2::XMLElement* resCommandElement = responseDoc.NewElement("command");
        tinyxml2::XMLElement* statusElement = responseDoc.NewElement("status");
        tinyxml2::XMLElement* dateElement = responseDoc.NewElement("date");
        resRoot->InsertEndChild(resCommandElement);
        resRoot->InsertEndChild(statusElement);
        resRoot->InsertEndChild(dateElement);
        resCommandElement->SetText(commandElement->GetText());
        statusElement->SetText(status.c_str());
        dateElement->SetText(asctime(localtime(&ctt)));
        tinyxml2::XMLDocument* docPtr = &responseDoc;
        tinyxml2::XMLPrinter printer;
        docPtr->Accept(&printer);
        response = (std::string)"<?xml version = '1.0' encoding = 'UTF-8'?>\n" + printer.CStr();
    }              
    return response;
}

//Handles thread for established client connection
void threadHandler(Sock* clientSock){
    std::string request; 
    int requestLength = 4096;
    clientSock->Read(request, requestLength);
    clientSock->Write(createResponse(request));
    clientSock->~Sock();
}

// Defaults to 127.0.0.1 and port 5000
int main(int argc, char **argv)
{
    std::string ip = "";
    int port = 0;
    std::string tempArg = "";
    char *ptr;
    switch (argc) {
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
    Sock *s = new Sock();
    s->Bind(ip, port);
    s->Listen(10);
    

    // Loop to accept client connection and loop through read/writing. 
    while(true){
        Sock* clientSock = s->Accept();
        if(clientSock->sock < 0) {
            //do nothing
        } else {
            std::thread t(threadHandler, clientSock);
            t.detach();            
        }
    }
    
    return 0;
}


