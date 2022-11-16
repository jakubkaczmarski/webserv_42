#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP
#include "webserv.hpp"

class placeholder
{
	private:
		int					serverSocket;
		int					connectionSocket;
		struct sockaddr_in	serverAddress;
		int					sizeOfServerAddress = sizeof(serverAddress);
		std::string			currRequest;


		void	failTest( int check, std::string message )
		{
			// if (serverSocket < 0)
			// {
			// 	cerr << RED << "Server Socket < 0! Abort!" << endl;
			// 	cerr << "Errno message: " << strerror(errno) << RESET_LINE;
			// 	exit(-1);
			// }
			if (check < 0)
			{
				cerr << RED << message << " < 0! Abort!" << RESET_LINE;
				cerr << RED << "Errno message : " << strerror(errno) << RESET_LINE;
				exit(-1);
			}
		}

		void	servAddressInit( void )
		{
			serverSocket = socket(AF_INET, SOCK_STREAM, 0);			// SOCK_STREAM == TCP

			int	option = 1;
			
			setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));

			serverAddress.sin_family		= AF_INET;							// means IPv4 / AD_INET6 =IPv6 
			serverAddress.sin_port			= htons(PORT_NBR);					// Used port
			serverAddress.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);			// Address this socket is litening to


			failTest(serverSocket, "Server Socket");

			failTest(bind(serverSocket, (SA *) &serverAddress, sizeof(serverAddress)),
						"Binding Server Socket");

			failTest(listen(serverSocket, SOCKET_BACKLOG),						// SOMAXCONN???
						"listen() of Server Socket");
		}; // probably here address


	public:
		placeholder()
		{
			servAddressInit();
		};
		~placeholder() {
		};
		void	handleRequest( void )
		{
		};

		void		request( void )
		{
			//Request-Line				= Method SP Request-URI SP HTTP-Version CRLF
			//CR = Carriage return		= \r
			//LF = Line Feed			= \n
			int		requestSocket;
			int		recvReturn;
			char	receivingBuffer[MAX_LINE + 1];
			char	sendingBuffer[MAX_LINE + 1] = "HTTP/1.0 200 OK\r\n\r\nThis is a welcoming message version 2!"; // example answer for request

			requestSocket = accept(serverSocket, (SA *) NULL, NULL);
			failTest(requestSocket, "accept() Socket");
			memset(receivingBuffer, 0, MAX_LINE + 1);	
			currRequest.clear();
			while(((recvReturn = recv(requestSocket, receivingBuffer, MAX_LINE, 0)) > 0))
			{
				failTest(recvReturn, "Reading into receivingBuffer out of requestSocket");
				currRequest.append(receivingBuffer);
				if (receivingBuffer[recvReturn - 1] == '\n' && receivingBuffer[recvReturn - 2] == '\r')
					break;
				memset(receivingBuffer, 0, MAX_LINE);
			}

			// proccess the request here to get an answer

			failTest(send(requestSocket, sendingBuffer, strlen(sendingBuffer), 0),
						"Sending answer to Request to requestSocket");

			failTest(close(requestSocket),
						"Sending answer to Request to requestSocket");
			cout << "This is the full Request" << RESET_LINE;
			cout << endl << currRequest << endl << endl;
		}
};

#endif