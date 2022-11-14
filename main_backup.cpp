#include "webserv.hpp"

int main(void)
{
	int					socketFD;										//writing
	int					acceptSocket;									//reading
	int					errNum;											//error handling
	struct sockaddr_in	serverAddress;									//adress
	int					sizeServerAddress = sizeof(serverAddress);		//size of in bytes of server address
	char				lineToReturn[MAX_LINE+1] = "HTTP/1.0 200 OK\r\n\r\nThis is a welcoming message!";						//buffer
	char				receivedLine[MAX_LINE+1];						//buffer 2

	cout << "^^^ Stage 1: socket creation ^^^" << endl;
	
	socketFD = socket(AF_INET, SOCK_STREAM, 0);							//SOCK_NONBLOCK
	
	if (socketFD == -1)
	{
		cout << "Socket error: " << std::strerror(errno) << '\n';
		return (-1);
	}
	else
	{
		cout << "Socket " << socketFD << " created!" << endl;
	}
	
	cout << "^^^ Stage 2: socket binding ^^^" << endl;

	memset(&serverAddress, 0, sizeServerAddress);
	serverAddress.sin_family		= AF_INET;
	serverAddress.sin_addr.s_addr	= htonl(INADDR_ANY);
	serverAddress.sin_port			= htons(PORT_NBR);
	errNum = bind(socketFD, (SA *) &serverAddress, sizeServerAddress);
	
	if (errNum == -1)
	{
		cout << "Acutal errno: " << errno << endl;
		cout << "Socket error: " << std::strerror(errno) << '\n';
		return (-2);
	}
	else
	{
		cout << "Socket bound!"<< endl;
	}
	
	cout << "^^^ Stage 3: socket listening ^^^" << endl;
	errNum = listen(socketFD, SOCKET_BACKLOG);
	if (errNum == -1)
	{
		cout << "Socket error: " << std::strerror(errno) << '\n';
		return (-3);
	}
	else
	{
		cout << "Socket listening!"<< endl;
	}

	cout << "^^^ Stage 4: socket accept ^^^" << endl;

	while(1)
	{
		struct sockaddr_in	addr;
		socklen_t			addr_len;

		cout << "Waiting for a connection on Port: " << PORT_NBR << endl;
		
		acceptSocket = accept(socketFD, (SA *)NULL, NULL);
		if (acceptSocket == -1)
		{
			cout << "Socket error: " << std::strerror(errno) << '\n';
			return (-4);
		}
		else
		{
			cout << "Connection " << acceptSocket << " established!" << endl;

		}
		
		memset(receivedLine, 0, MAX_LINE);
		while(((errNum = read(acceptSocket, receivedLine, MAX_LINE - 1)) > 0))
		{
			if (receivedLine[errNum - 1] == '\n')
				break;
			memset(receivedLine, 0, MAX_LINE);
		}

		if (errNum == -1)
		{
			cout << "Read error: " << std::strerror(errno) << '\n';
			return (-1);
		}
		
		// lineToReturn = "HTTP/1.0 200 OK\r\n\r\nThis is a welcoming message!";
		errNum = send(acceptSocket, lineToReturn, strlen(lineToReturn), 0);
		if (errNum == -1)
		{
			cout << "Error with errno Nr.: " << errno << endl << "Meaning : " << std::strerror(errno) << endl;
		}
		else
		{
			cout << "everythign is A-OK" << endl;
		}

		close(acceptSocket);
	}
	return 0;
}