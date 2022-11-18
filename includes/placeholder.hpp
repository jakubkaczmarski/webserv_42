#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP
#include "webserv.hpp"
// #include <iostream>
#include <fstream>
#include <vector>

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

class placeholder
{
	private:
		int					serverSocket;
		int					connectionSocket;
		struct sockaddr_in	serverAddress;
		int					sizeOfServerAddress = sizeof(serverAddress);
		std::string			fullRequest;
		s_request			currRequest;
		s_response			currResponse;


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

			// print body
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

		int getFileSize(const std::string &fileName)
		{
			ifstream file(fileName.c_str(), ifstream::in | ifstream::binary);

			if(!file.is_open())
			{
				return -1;
			}

			file.seekg(0);
			int fileSize = file.tellg();
			file.close();

			return fileSize;
		}
		void sendImage(int requestSocket)
		{
			ifstream		imageToSend;
			char			readBuffer[MAX_LINE + 1];

			imageToSend.open("./database/UFF.png");
			if (!imageToSend.is_open())
			{
				cout << "Error opening the image" << endl;
				// exit(69); does
			}
			// std::string tempp = getBinary("./database/Error_404.png");
			// std::string tempp = getBinary("./database/index.html");
			// std::string tempp = getImage("./database/Error_404.png");
			// const char * temp2 = tempp.c_str();
			// send(requestSocket, temp2, tempp.size(), 0);
			std::string temp = ("./database/Error_404.png");
			sendResponse(requestSocket, temp);
		}


		// std::vector<char>	getBinary(std::string path, std::string file_type)
		std::string		getBinary(std::string &path, long *size)
		{
			FILE	*file_stream = fopen(path.c_str(), "rb");
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



		std::string getImage(const std::string &full_path)
		{
			const char* file_name = full_path.c_str();
			FILE* file_stream = fopen(file_name, "rb");
			std::string file_str;
			size_t file_size;
			if(file_stream != nullptr)
			{
				fseek(file_stream, 0, SEEK_END);
				long file_length = ftell(file_stream);
				rewind(file_stream);
				char* buffer = (char*) malloc(sizeof(char) * file_length);
				if(buffer != nullptr)
				{
					file_size = fread(buffer, 1, file_length, file_stream);
					stringstream out;
					for(int i = 0; i < file_size; i++)
					{
						out << buffer[i];
					}
					file_str = out.str();
				}
				else
				{
					printf("buffer is null!");
				}
			}
			else
			{
				printf("file_stream is null! file name -> %s\n", file_name);
			}
			std::string html = "HTTP/1.1 200 Okay\r\nContent-Type: text/html; charset=ISO-8859-4 \r\n\r\n" + string("FILE NOT FOUND!!");
			if(file_str.length() > 0)
			{
				// HTTP/1.0 200 OK
				// Server: cchttpd/0.1.0
				// Content-Type: image/gif
				// Content-Transfer-Encoding: binary
				// Content-Length: 41758
				std::string file_size_str = std::to_string(file_str.length());
				html = "HTTP/1.1 200 Okay\r\nContent-Type: image/png; Content-Transfer-Encoding: binary; Content-Length: " + file_size_str + ";charset=ISO-8859-4 \r\n\r\n" + file_str;
				printf("\n\nHTML -> %s\n\nfile_str -> %ld\n\n\n", html.c_str(), file_str.length());
			}
			return html;
		}


		void sendFile(int requestSocket)
		{
			ifstream fileToSend;
			char readBuffer[MAX_LINE];
			// fileToSend.open("/home/kis619/Desktop/webserv_42/database/cute.png");
			fileToSend.open("./database/index.html");
			if (!fileToSend.is_open())
			{
				cout << "Error opening the file" << endl;
				// exit(69); does
			}

			// char	sendingBuffer[MAX_LINE + 1] = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: \r\n\r\n"; 
			char		sendingBuffer[MAX_LINE + 1] = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: 331777\r\n\r\n"; 

			// HTTP/1.0 200 Ok
			// Content-Type: image/png
			// Content-Length: 14580053
			failTest(send(requestSocket, sendingBuffer, strlen(sendingBuffer), 0),
						"Sending answer to Request to requestSocket");
			int i = 0;
			while(fileToSend)
			{
				fileToSend.read(readBuffer, MAX_LINE);
				// cout << PURPLE << readBuffer << RESET_LINE;
				// cout << PURPLE << strlen(readBuffer) << RESET_LINE;
				// fileToSend.
				failTest(send(requestSocket, readBuffer, strlen(readBuffer), 0),
						"Sending answer to Request to requestSocket");
				memset(readBuffer, 0, MAX_LINE);
				i++;

			}
			cout << "this is i =  " << i << endl;
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

			failTest(send(requestSocket, responsy, strlen(responsy), 0),
						"Sending answer to Request to requestSocket");
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
			// sendFile(requestSocket);
			sendImage(requestSocket);
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