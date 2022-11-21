#ifndef server_HPP
#define server_HPP
#include "webserv.hpp"


typedef struct t_request
{
	std::string								method;
	std::string								URI;
	std::string								httpVers;
	std::map<std::string, std::string>		headers;
	std::string								body; //for now string
}	s_request;

typedef struct t_response
{
	std::string								httpVers;
	std::string								statusMessage;			// number + message
	std::string								headers;
	std::string								body;					// getBinary()
}	s_response;

class server
{
	private:
		const std::map<string, string> statusCodesAndMessages
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
		s_response			currResponse;
		int					port;
		// int					host;  // what is this???
		size_t				max_client_body_size;


		void	failTest( int check, std::string message )
		{
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
				std::string	body = fullRequest.substr(begin, size);
				currRequest.body = body;
			}
			// else if (currRequest.headers.end() != currRequest.headers.find("Content-Length"))

			// print body
			cout << PURPLE << currRequest.body << RESET_LINE;
		}

		void	fillRequestStruct(std::string &fullRequest)
		{
			fillRequestLineItems(fullRequest);
			fillRequestHeaders(fullRequest);
			fillRequestBody(fullRequest);
		}

		void		handleRequest(int requestSocket, std::string &fullRequest)
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
			sendResponse(requestSocket, currRequest.URI);
			currRequest.headers.clear();
			currRequest.body.clear();

			// currRequest.httpVers.clear();
			// currRequest.method.clear();
			// currRequest.URI.clear();
		}

		std::ifstream::pos_type fileSize(const char* filename)
        {
            std::ifstream fileStream(filename, std::ifstream::ate | std::ifstream::binary);
			if(!fileStream.is_open())
			{
				cout << RED << "error message for filestream in getBinary!" << RESET_LINE;
			}
            return fileStream.tellg(); 
        }

		std::string		getBinary(std::string &path, long *size)
		{
			cout << "Path is = " << path << endl;

			ifstream fileStream;
			*size = fileSize(("." + path).c_str());
			fileStream.open(("." + path), std::ios_base::in | std::ios_base::binary);

			if(!fileStream.is_open())
			{
				cout << RED << "error message for filestream in getBinary!" << RESET_LINE;
			}
			else
			{
				cout << RED << *size << RESET_LINE;
				std::vector<char>  binaryVector;
				binaryVector.resize(*size);
				fileStream.read(&binaryVector[0], *size);
				std::string binaryString;
				for(long i = 0; i < *size; i++)
				{
					binaryString.push_back(binaryVector[i]);
				}
				cout << GREEN << binaryString.size() << RESET_LINE;
				return binaryString;
			}
			fileStream.close();
			exit(-1);
		}
		std::string makeHeader(long bodySize) //prolly other stuff too
		{
			std::string		out;
			out = "Content-Type: image/png; Content-Transfer-Encoding: binary; Content-Length: " + std::to_string(bodySize) + ";charset=ISO-8859-4 ";
			return (out);
		}

		void fillResponseStructBinary(std::string &path)
		{
			long		bodyLength;
			currResponse.httpVers = HTTPVERSION;
			currResponse.statusMessage = "200 Everything is A-Ok";// still have to do
			cerr << "\n\n\nHEYYYYYYYY\n\n\n" << endl;
			currResponse.body = getBinary(path, &bodyLength);
			currResponse.headers = makeHeader(bodyLength); // still have to do
		}

		void	sendResponse(int requestSocket, std::string &path)					// im writing this with a get request in mind
		{
			std::string		outie;

			fillResponseStructBinary(path);
			outie.append(currResponse.httpVers + " " + 
						currResponse.statusMessage + "\r\n" +
						currResponse.headers + "\r\n\r\n" +
						currResponse.body);

			const char *	responsy = outie.c_str();

			failTest(send(requestSocket, responsy, outie.size(), 0),
						"Sending answer to Request to requestSocket");
		}
	public:
		server()
		{
			servAddressInit();
		};
		~server() {
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

			handleRequest(requestSocket, fullRequest);
			failTest(close(requestSocket),
						"Sending answer to Request to requestSocket");
			cout << "This is the full Request" << RESET_LINE;
			cout << endl << fullRequest << RED << "<<here is the end>>" << RESET_LINE;
		}
};

#endif