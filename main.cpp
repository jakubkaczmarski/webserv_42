#include "./src/includes/webserv.hpp"


//Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
//CR = Carriage return	= \r
//LF = Line Feed 		= \n


void	successCheck(int check, std::string message)
{
	if (check < 0)
	{
		cout << RED << message << " was unsuccessful!" << RESET_LINE;
		cout << RED << "Errno message : " << strerror(errno) << RESET_LINE;
		exit(-1);
	}
	else
		cout << GREEN << message << " was successful!" << RESET_LINE;
	cout << endl;
}


int	main( void )
{
	int					listenSocket;
	int					connectionSocket;
	int					returnChecks;
	struct sockaddr_in	serverAddress;
	int					sizeOfServerAddress = sizeof(serverAddress);
	char				sendingBuffer[MAX_LINE + 1] = "HTTP/1.0 200 OK\r\n\r\nThis is a welcoming message version 2!";
	char				receivingBuffer[MAX_LINE + 1];
	std::string			fullReceivedRequest;



	cout << PURPLE << "Starting with creating listenSocket" << RESET_LINE;

	listenSocket = socket(PF_INET, SOCK_STREAM, 0);

	successCheck(listenSocket, "Creating listenSocket");

	memset(&serverAddress, 0, sizeOfServerAddress);
	serverAddress.sin_family		= PF_INET;
	serverAddress.sin_port			= htons(PORT_NBR);
	serverAddress.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);		// = localhost

	returnChecks = bind(listenSocket, (SA *) &serverAddress, sizeOfServerAddress);

	successCheck(returnChecks, "Binding listenSocket");

	returnChecks = listen(listenSocket, SOCKET_BACKLOG);

	successCheck(returnChecks, "Setting up listening for listenSocket");


	while (1)
	{
		cout << PURPLE << "Awaiting Connection on Port Nr.: " << PORT_NBR << "!" << RESET_LINE << endl;

		connectionSocket = accept(listenSocket, (SA *)NULL, NULL);

		memset(receivingBuffer, 0, MAX_LINE + 1);
		while(((returnChecks = recv(connectionSocket, receivingBuffer, MAX_LINE, 0)) > 0))
		{
			successCheck(returnChecks, "Reading into receivingBuffer out of connectionSocket");
			fullReceivedRequest.append(receivingBuffer);
			if (receivingBuffer[returnChecks - 1] == '\n')
				break;
			memset(receivingBuffer, 0, MAX_LINE);
		}

		cout << "This is the full Request" << RESET_LINE;
		cout << endl << fullReceivedRequest << endl << endl;

		// distributeRequests(fullReceivedRequest);

		successCheck(connectionSocket, "Creating ConnectionSocket");

		returnChecks = send(connectionSocket, sendingBuffer, strlen(sendingBuffer), 0);
		
		successCheck(returnChecks, "Sending message into connectionSocket");
		
		returnChecks = close(connectionSocket);

		successCheck(returnChecks, "Closing connectionSocket");
	}
}
