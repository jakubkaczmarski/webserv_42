#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP
#include "webserv.hpp"
// #include <iostream>

typedef struct t_request
{
	std::string								method;
	std::string								URI;
	std::string								httpVers;
	std::map<std::string, std::string>		headers;
}	s_request;

class placeholder
{
	private:
		int					serverSocket;
		int					connectionSocket;
		struct sockaddr_in	serverAddress;
		int					sizeOfServerAddress = sizeof(serverAddress);
		std::string			fullRequest;
		s_request			currRequest;


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

		void	fillRequestStruct(std::string &fullRequest)
		{
			// std::string		firstLine = fullRequest.substr(0, fullRequest.find('\n'));
			currRequest.method = fullRequest.substr(0, fullRequest.find(' '));
			std::string		tempstr = fullRequest.substr(fullRequest.find(' ') + 1, fullRequest.size());
			currRequest.URI = tempstr.substr(0, tempstr.find(' '));
			currRequest.httpVers = tempstr.substr(tempstr.find(' ') + 1, tempstr.size());

			std::string		workstr = fullRequest.substr(fullRequest.find('\n') + 1, fullRequest.find("\n\n"));

			// cout << workstr << endl;
			std::vector<string> outputArray;
			// while(1)
			while (workstr.size() > 1)
			{
				std::string		tempy = workstr.substr(0, workstr.find('\n'));
				outputArray = split(tempy, ':', 1);
				currRequest.headers.insert(std::make_pair(outputArray[0], outputArray[1]));
				workstr = workstr.substr(workstr.find('\n') + 1, workstr.size());
			}
			

			// {
			// 	std::pair<std::string, std::string> tmp_pair(workstr.substr(0, workstr.find(':')), workstr.substr(workstr.find(':') + 2, workstr.find('\n')));
			// 	currRequest.headers.insert(tmp_pair);
			// 	// currRequest.headers.insert(
			// }

			// typedef typename std::map<std::string, std::string>::iterator		map_it;
			// map_it		it		= currRequest.headers.begin();
			// map_it		it_e	= currRequest.headers.end();


			// cout << "here we start the map pritn" << endl;
			// for (; it != it_e; it++)
			// {
			// 	cout << it->first << ": ";
			// 	cout << it->second << endl;
			// }

			// cout << RED << "trying to find stuff    " << currRequest.headers.at("DNT") << RESET_LINE;


			// cout << "here we end the map pritn" << endl;
			// for (auto i: currRequest.headers)
			// {
			// 	cout << i.first << " ";
			// 	cout << i.second << endl;
			// }

			

		}

		void		handleRequest(std::string &fullRequest)
		{
			fillRequestStruct(fullRequest);
			if (currRequest.method.compare("GET") == 0)
			{
				// fillHeadersMap(fullRequest);
				cout << "its a get request :)" << endl;
			}
			else if (currRequest.method.compare("POST") == 0)
			{
				cout << "its a post request :)" << endl;
			}
			else if (currRequest.method.compare("DELETE") == 0)
			{
				cout << "its a delete request :)" << endl;
			}
			else
			{
				cout << "We should throw an error code with a message that we do not support this method" << endl;
			}


			// cout << "method = " << currRequest.method << endl;
			// cout << "URI = " << currRequest.URI << endl;
			// cout << "httpVers = " << currRequest.httpVers << endl;

		}




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
			fullRequest.clear();
			while(((recvReturn = recv(requestSocket, receivingBuffer, MAX_LINE, 0)) > 0))
			{
				failTest(recvReturn, "Reading into receivingBuffer out of requestSocket");
				fullRequest.append(receivingBuffer);
				if (receivingBuffer[recvReturn - 1] == '\n' && receivingBuffer[recvReturn - 2] == '\r')
					break;
				memset(receivingBuffer, 0, MAX_LINE);
			}

			handleRequest(fullRequest);

			// funciton to determine what kind of request

			// here we call the corresponding request funciton

			// proccess the request here to get an answer

			failTest(send(requestSocket, sendingBuffer, strlen(sendingBuffer), 0),
						"Sending answer to Request to requestSocket");

			failTest(close(requestSocket),
						"Sending answer to Request to requestSocket");
			cout << "This is the full Request" << RESET_LINE;
			cout << endl << fullRequest << RED << "<<here is the end>>" << RESET_LINE;
		}
};

#endif