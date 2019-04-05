#OO-C-Socket-Server#


##Description:##
	This project consists of 2 executable programs, the server and client respectively. The server can accept 2 command line arguments for either/both the address and port for the server. The server is then setup on that configuration. The client shares the same command line argument configuration for setting up connection to the server as well as an additional argument for the XML request file location. The four source files (excluding the TinyXML2 dependency) and their descriptions are outlined below.

##Sock.h:##
	This is a header file for the Sock class for socket connection and setup functionality, a Sock class has the following:
	-int socket descriptor
	-Port
	-IP address
	-char buffer for messages
	-address struct for storing address configuration
	-Function definitions for the Socket class

##Sock.cpp:##
	This socket class encapsulates all of the socket creation and connection. There are 6 functions outside of Sock’s constructors/deconstructor:
	-int Bind(ip, port): Binds the socket to the specified address and port.
	-int Listen(queue): Puts socket in a passive listen mode, queue is number of pending connections allowed.
	-Sock* Accept(): Returns a Sock* to the first pending connection and establishes it with the client.
	int Read(&buffer, bufferLength): Reads the incoming message, saves it to passed in string reference.
	-int Write(message): Writes the message string to the client.
	-int Connect(ip, port): Socket connection function, used in Client executable

##Server.cpp##
	This server utilizes the Sock class to setup the server sockets and establish connection with the client. The server setups the socket, binds it to the passed in address and port, sets the socket to listen, then enters a loop for checking accepted connections before passing them to the threadHandler() to process their request and send pack an XML formatted response. This allows us to have simultaneous clients by utilizing threads. The server displays the command and data rows of the request and writes a request back to the client. The expected request/response format is given below:

(Request)
<?xml version = '1.0' encoding = 'UTF-8'?>
<request>
  <command>Print</command>
  <data>
     <row type="name">Mr. Joe Chase</row>
     <row type="address">123 Anywhere Lane</row>
  </data>
</request>

(Response)
<?xml version = '1.0' encoding = 'UTF-8'?>
<response>
    <command>Print</command>
    <status>Complete</status>
    <date>Wed Mar 13 14:25:13 2019
</date>
</response>


##Client.cpp:##
	The client is a simple program that also utilizes the Sock class to start a connection with the server and write a message, the client reads the XML request from a file, sends the request to the server, then prints out the response to the console. It takes the address, port, and file location of the XML request as command line parameters.


##Design:##
	The Sock class was necessary to properly encapsulate the socket functionality while allowing for easy thread handling of the socket objects. Having this easy thread management allows the server to
handle the waiting client connections in the queue for simultaneous client connections. Tinyxml2 was used to parse and construct the XML messages as it is lightweight solution for the relatively small XML processing requirements. Overall, the design chosen doesn’t have many limitations outside of general abstraction of the direct socket manipulation, but since the socket descriptor can still be accessed, most socket functionality still remains. Currently, the server listening socket limits the queue to 10 waiting connections. However, this queue size could be changed if it was determined that a greater amount of connections could be waiting and resolved in an acceptable time frame.  
	


 

