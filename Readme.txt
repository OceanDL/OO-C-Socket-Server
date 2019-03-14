Navigate to main directory (C++_Socket_Server) in a terminal and run make.
This will generate two executable programs, Client and Server in the bin folder.
These can be ran by running "bin/Client" and "bin/Server" respectively

Client takes 3 optional arguments (Address, Port, and XML file) which are accepted in the following format:
"bin/Client 127.0.0.1 5000 request.txt"

Server takes 2 optional arguments (Address and Port) which are accepted in the following format:
"bin/Server 127.0.0.1 5000"

The server must be started before the client for connection to be established. 
The server will display the command and data rows of the request.
The client will display the server response upon a successfully received request and then terminate connection. 

