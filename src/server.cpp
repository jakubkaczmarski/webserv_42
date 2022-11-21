#include "../includes/server.hpp"

void		server::request( void )
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

config	server::getConfig( void )
{
	return (servConfig);
}

void	server::sendResponse(int requestSocket, std::string &path)					// im writing this with a get request in mind
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

void server::fillResponseStructBinary(std::string &path)
{
	long		bodyLength;
	currResponse.httpVers = HTTPVERSION;
	currResponse.statusMessage = "200 Everything is A-Ok";// still have to do
	currResponse.body = getBinary(path, &bodyLength);
	currResponse.headers = makeHeader(bodyLength); // still have to do
}

void	server::failTest( int check, std::string message )
{
	if (check < 0)
	{
		cerr << RED << message << " < 0! Abort!" << RESET_LINE;
		cerr << RED << "Errno message : " << strerror(errno) << RESET_LINE;
		exit(-1);
	}
}

void	server::servAddressInit( void )
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

void server::fillRequestLineItems(string &fullRequest)
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

void	server::fillRequestHeaders(string &fullRequest)
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

void server::fillRequestBody(std::string &fullRequest)
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

void	server::fillRequestStruct(std::string &fullRequest)
{
	fillRequestLineItems(fullRequest);
	fillRequestHeaders(fullRequest);
	fillRequestBody(fullRequest);
}

int	server::checkGetRequest(int requestSocket)
{
	if(file_exists( currRequest.URI) == false)
	{
		sendResponse(requestSocket, servConfig.getConfigMap().at(ERROR404));
		return (-1);
	}
	return (0);
}

int	server::checkRequestErrors(int requestSocket)
{
	if (currRequest.method.compare("GET") == 0)
		return (checkGetRequest(requestSocket));
	// else if (currRequest.method.compare("POST") == 0)
	// 	return (checkPostRequest());
	// else if (currRequest.method.compare("DELETE") == 0)
	// 	return (checkDeleteRequest());
	return (-1);
}

void		server::handleRequest(int requestSocket, std::string &fullRequest)
{
	fillRequestStruct(fullRequest);
	if (checkRequestErrors(requestSocket) != 0)
		return ;
	if (currRequest.method.compare("GET") == 0)
	{
		// test for errors

		// send response
		cout << "its a get request :)" << endl;
	}
	else if (currRequest.method.compare("POST") == 0)
	{
		// test for errors
		// send response
		cout << "its a post request :)" << endl;
	}
	else if (currRequest.method.compare("DELETE") == 0)
	{
		// test for errors
		// send response
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

std::string		server::getBinary(std::string &path, long *size)
{
	cout << "Path is = " << path << endl;
	FILE	*file_stream = fopen(("."+path).c_str(), "rb");
	if(file_stream == nullptr)
	{
		cout << RED << "errormessage for filestream in getBinary!" << RESET_LINE;
	}
	else
	{
		fseek(file_stream, 0, SEEK_END);
		*size = ftell(file_stream);
		cout << RED << *size << RESET_LINE;
		rewind(file_stream);
		std::vector<char>  binaryVector;
		binaryVector.resize(*size);
		fread(&binaryVector[0], 1, *size, file_stream);
		std::string binaryString;
		for(long i = 0; i < *size; i++)
		{
			binaryString.push_back(binaryVector[i]);
		}
		cout << GREEN << binaryString.size() << RESET_LINE;
		return binaryString;
	}
	exit(-1);
}

std::string server::makeHeader(long bodySize) //prolly other stuff too
{
	std::string		out;
	out = "Content-Type: image/png; Content-Transfer-Encoding: binary; Content-Length: " + std::to_string(bodySize) + ";charset=ISO-8859-4 ";
	return (out);
}