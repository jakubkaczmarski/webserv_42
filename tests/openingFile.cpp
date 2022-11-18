#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sstream> //for the split function
#include <vector> // to store token when splitting
#include <map> //to store the headers
#include <fstream> //for opening files

#include <unistd.h>


// #include "includes/colours.h"


using std::cout;
using std::cerr;
using std::endl;
using std::strerror;
using std::string;
using std::stringstream;
using std::ifstream;

#define PORT_NBR 60000 
#define MAX_LINE 4096
#define SA struct sockaddr
#define SOCKET_BACKLOG 1024


int main(void)
{
	cout << "WHAT" << endl;
	ifstream fileToSend;
	char readBuffer[MAX_LINE];
	cout << getcwd(readBuffer, 500) << endl;
	fileToSend.open("./Desktop/webserv_42/database/index.html");
	if (!fileToSend.is_open())
	{
		cout << "Error opening the file" << endl;
		// exit(69); 
	}

	// char	sendingBuffer[MAX_LINE + 1] = "HTTP/1.0 200 OK\r\n\r\n"; 
	// failTest(send(requestSocket, readBuffer, strlen(readBuffer), 0),
	// 			"Sending answer to Request to requestSocket");
	// while(fileToSend)
	// {
	// 	fileToSend.read(readBuffer, MAX_LINE);
	// 	// failTest(send(requestSocket, readBuffer, strlen(readBuffer), 0),
	// 	// 		"Sending answer to Request to requestSocket");
	// 	write(2, readBuffer, strlen(readBuffer));
	// 	memset(readBuffer, 0, MAX_LINE);
	// }
}