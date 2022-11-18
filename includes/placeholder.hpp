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
	std::string								body; //for now string
}	s_request;

class placeholder
{
	private:
		const map<string, string> statusCodesAndMessages
		{
			{"100", "Continue"},
			{"101", "Switching protocols"},
			{"102", "Processing"},
			{"103", "Early Hints"},
			{"200", "OK"},
			{"201", "Created"},
			{"202", "Accepted"},
			{"203", "Non-Authoritative Information"},
			{"204", "No Content"},
			{"205", "Reset Content"},
			{"206", "Partial Content"},
			{"207", "Multi-Status"},
			{"208", "Already Reported"},
			{"226", "IM Used"},
			{"300", "Multiple Choices"},
			{"301", "Moved Permanently"},
			{"302", "Found"},
			{"303", "See Other"},
			{"304", "Not Modified"},
			{"305", "Use Proxy"},
			{"306", "Switch Proxy"},
			{"307", "Temporary Redirect"},
			{"308", "Permanent Redirect"},
			{"400", "Bad Request"},
			{"401", "Unauthorized"},
			{"402", "Payment Required"},
			{"403", "Forbidden"},
			{"404", "Not Found"},
			{"405", "Method Not Allowed"},
			{"406", "Not Acceptable"},
			{"407", "Proxy Authentication Required"},
			{"408", "Request Timeout"},
			{"409", "Conflict"},
			{"410", "Gone"},
			{"411", "Length Required"},
			{"412", "Precondition Failed"},
			{"413", "Payload Too Large"},
			{"414", "URI Too Long"},
			{"415", "Unsupported Media Type"},
			{"416", "Range Not Satisfiable"},
			{"417", "Expectation Failed"},
			{"418", "I'm a Teapot"},
			{"421", "Misdirected Request"},
			{"422", "Unprocessable Entity"},
			{"423", "Locked"},
			{"424", "Failed Dependency"},
			{"425", "Too Early"},
			{"426", "Upgrade Required"},
			{"428", "Precondition Required"},
			{"429", "Too Many Requests"},
			{"431", "Request Header Fields Too Large"},
			{"451", "Unavailable For Legal Reasons"},
			{"500", "Internal Server Error"},
			{"501", "Not Implemented"},
			{"502", "Bad Gateway"},
			{"503", "Service Unavailable"},
			{"504", "Gateway Timeout"},
			{"505", "HTTP Version Not Supported"},
			{"506", "Variant Also Negotiates"},
			{"507", "Insufficient Storage"},
			{"508", "Loop Detected"},
			{"510", "Not Extended"},
			{"511", "Network Authentication Required"},
		};
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

		void fillRequestLineItems(string &fullRequest)
		{
			//get request line
			std::string	requestLine = fullRequest.substr(0, fullRequest.find('\n'));
			std::vector<string> requestLineV = split(requestLine, ' ');
			currRequest.method		= requestLineV[0];
			currRequest.URI			= requestLineV[1];
			currRequest.httpVers	= requestLineV[2];

			// print out request line
			cout << YELLOW << "method = " << currRequest.method << endl;
			cout << "URI = " << currRequest.URI << endl;
			cout << "httpVers = " << currRequest.httpVers << RESET_LINE;
		}

		void	fillRequestHeaders(string &fullRequest)
		{
			//get headers
			int begin	= fullRequest.find('\n') + 1;
			int size	= fullRequest.find("\r\n\r\n") - begin;
			std::string	headers = fullRequest.substr(begin, size);
			std::vector<string> headersVector = split(headers, '\n');
			std::vector<string> key_value;
			size_t vectorSize = headersVector.size();
			for (size_t i = 0; i < vectorSize; i++)
			{
				key_value = split(headersVector[i], ':', 1);
				currRequest.headers.insert(std::make_pair(key_value[0], key_value[1]));
			}
			
			// print headers to terminal
			for (auto i : currRequest.headers)
			{
				cout << RED << i.first << ": " << i.second << RESET_LINE;
			}
		}

		void fillRequestBody(std::string &fullRequest)
		{
			//get body
			size_t	begin;
			size_t	size;
			if (currRequest.headers.end() != currRequest.headers.find("Content-Length")) // or 
			{
				begin		= fullRequest.find("\r\n\r\n") + 4;
				size		= stoi(currRequest.headers.at("Content-Length"));
				// cout << begin << endl;
				// cout << size << endl;
				std::string	body = fullRequest.substr(begin, size);
				currRequest.body = body;
				// cout << RED << body << RESET_LINE;
				// cout << RED << "BODY" << fullRequest[149] << RESET_LINE;
			}
			// else if (currRequest.headers.end() != currRequest.headers.find("Content-Length"))

			//print body
			cout << PURPLE << currRequest.body << RESET_LINE;
		}

		void	fillRequestStruct(std::string &fullRequest)
		{
			fillRequestLineItems(fullRequest);
			fillRequestHeaders(fullRequest);
			fillRequestBody(fullRequest);
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


			currRequest.headers.clear();
			currRequest.body.clear();
			// currRequest.httpVers.clear();
			// currRequest.method.clear();
			// currRequest.URI.clear();
		}

		void sendFile(int requestSocket)
		{
			ifstream fileToSend;
			char readBuffer[MAX_LINE];
			// fileToSend.open("/home/kis619/Desktop/webserv_42/database/cute.png");
			fileToSend.open("/home/kis619/Desktop/webserv_42/database/index.html");
			if (!fileToSend.is_open())
			{
				cout << "Error opening the file" << endl;
				// exit(69); does
			}

			char	sendingBuffer[MAX_LINE + 1] = "HTTP/1.1 200 OK\r\n\r\n"; 
			failTest(send(requestSocket, sendingBuffer, strlen(sendingBuffer), 0),
						"Sending answer to Request to requestSocket");
			while(fileToSend)
			{
				fileToSend.read(readBuffer, MAX_LINE);
				cout << PURPLE << readBuffer << RESET_LINE;
				cout << PURPLE << strlen(readBuffer) << RESET_LINE;
				failTest(send(requestSocket, readBuffer, strlen(readBuffer), 0),
						"Sending answer to Request to requestSocket");
				memset(readBuffer, 0, MAX_LINE);
			}
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
			char	sendingBuffer[MAX_LINE + 1] = "HTTP/1.1 200 OK\r\n\r\n<html>This is a welcoming message version 2!<h1>NOOO</h1></html>"; // example answer for request

			cout << "Waiting for a connection on PORT: " << PORT_NBR << endl;
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
			sendFile(requestSocket);
			// funciton to determine what kind of request

			// here we call the corresponding request funciton

			// proccess the request here to get an answer

			// failTest(send(requestSocket, sendingBuffer, strlen(sendingBuffer), 0),
			// 			"Sending answer to Request to requestSocket");
			// cout << strlen("HTTP/1.1 200 OK\r\n\r\n") << endl;
			failTest(close(requestSocket),
						"Sending answer to Request to requestSocket");
			cout << "This is the full Request" << RESET_LINE;
			cout << endl << fullRequest << RED << "<<here is the end>>" << RESET_LINE;
		}
};

#endif