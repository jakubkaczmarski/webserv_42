#include "../includes/server.hpp"
	//Request-Line				= Method SP Request-URI SP HTTP-Version CRLF
	//CR = Carriage return		= \r
	//LF = Line Feed			= \n


//register events of fd to epfd
static void epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		cout << "Error number: " << errno << " - " << strerror(errno) << endl;
		exit(1);
	}
}

// void		server::requestLoop( void )
// {
// 	int		idx;
// 	int		requestSocket;
// 	int		recvReturn;
// 	char	receivingBuffer[MAX_LINE + 1];
	
	
// 	int epollFD = epoll_create1(0);
// 	failTest(epollFD, "epoll_create1");

// 	struct epoll_event events[MAX_EVENTS]; //should be a define
// 	epoll_ctl_add(epollFD, serverSocket, EPOLLIN | EPOLLOUT | EPOLLET);

// 	int nfds;
// 	while(true)
// 	{
// 		nfds = epoll_wait(epollFD, events, MAX_EVENTS, -1); //-1 blocks forever, 0 never blocks in both examples they use -1? 
// 		// cout << YELLOW << "Number of fds ready: " << nfds << RESET_LINE;
// 		for (idx = 0; idx < nfds; idx++)
// 		{
// 			// cout << RED << "IDX: " << idx << RESET_LINE;
//  			if (events[idx].data.fd == serverSocket) //ready to accept a new connection
// 			{
// 				cout << "Waiting for a connection on PORT: " << PORT_NBR << endl;
// 				requestSocket = accept(serverSocket, (SA *) NULL, NULL);
// 				failTest(requestSocket, "accept() Socket");
// 				epoll_ctl_add(epollFD, requestSocket, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
// 			}
// 			else if(events[idx].events & EPOLLIN) /* handle EPOLLIN event */
// 			{
// 				// cout << RED << "ELSE IF" << RESET_LINE;
// 				memset(receivingBuffer, 0, MAX_LINE + 1);	
// 				fullRequest.clear();
// 				while(((recvReturn = recv(events[idx].data.fd , receivingBuffer, MAX_LINE, 0)) > 0))
// 				// while(((recvReturn = recv(requestSocket , receivingBuffer, MAX_LINE, 0)) > 0))
// 				{
// 					fullRequest.append(receivingBuffer, recvReturn);
// 					if (recvReturn < MAX_LINE)
// 					{
// 						memset(receivingBuffer, 0, MAX_LINE);
// 						break;
// 					}
// 					memset(receivingBuffer, 0, MAX_LINE);
// 					cout << PURPLE << "Inside else if: " << recvReturn << RESET_LINE;
// 				}
// 				failTest(recvReturn, "recv() call to read from requestSocket");
// 				handleRequest(events[idx].data.fd , fullRequest);
// 				// handleRequest(requestSocket, fullRequest);
// 				// failTest(close(events[idx].data.fd ), "Sending answer to Request to requestSocket");
// 				// epoll_ctl(epollFD, EPOLL_CTL_DEL, events[idx].data.fd, NULL);
// 				failTest(close(requestSocket), "Sending answer to Request to requestSocket");
// 			}

// 			/* check if the connection is closing */
// 			// if (events[idx].events & (EPOLLRDHUP | EPOLLHUP))
// 			// {
// 			// 	printf("[+] connection closed\n");
// 			// 	epoll_ctl(epollFD, EPOLL_CTL_DEL, events[idx].data.fd, NULL);
// 			// 	close(events[idx].data.fd);
// 			// 	continue;
// 			// }


// 		//printing the request
// 		cout << "This is the full Request" << RESET_LINE;
// 		// cout << endl << fullRequest << RED << "<<here is the end>>" << RESET_LINE;
// 		}
// 	}
// }

// void	server::sendResponse(int requestSocket, std::string &path)					// im writing this with a get request in mind
// {
// 	std::string		outie;

// 	fillResponseStructBinary(path,requestSocket);
// 	outie.append(currResponse.httpVers + " " + 
// 				currResponse.statusMessage + "\r\n" +
// 				currResponse.headers + "\r\n\r\n" +
// 				currResponse.body);

// 	const char *	responsy = outie.c_str();

// 	failTest(send(requestSocket, responsy, outie.size(), 0),
// 				"Sending answer to Request to requestSocket");
// }



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



int	server::checkGetRequest(int requestSocket)
{
	if(file_exists( currRequest.URI) == false)
	{
		// sendResponse(requestSocket, servConfig.getConfigMap().at(ERROR404));
		return (-1);
	}
	return (0);
}


// void server::fillResponseStructBinary(std::string &path, int request_soc)
// {
// 	long		bodyLength;
// 	currResponse.httpVers = HTTPVERSION;
// 	currResponse.statusMessage = "200 Everything is A-Ok";// still have to do
// 	currResponse.body = getBinary(path, &bodyLength, request_soc);
// 	currResponse.headers = makeHeader(bodyLength, path); // still have to do
// }

int	server::checkRequestErrors(int requestSocket)
{
	// if (currRequest.method.compare("GET") == 0)
	// 	return (checkGetRequest(requestSocket));
	// else if (currRequest.method.compare("POST") == 0)
	// 	return (checkPostRequest());
	// else if (currRequest.method.compare("DELETE") == 0)
	// 	return (checkDeleteRequest());
	return (-1);
}
// void		server::handle_post(int requestSocket, std::string &path, std::string &fullrequest)
// {
// 	//Content-type deduction 
// 	size_t index = fullRequest.find("Content-Type:");
// 	std::string content_type;
// 	// file_name.append(content_type)
// 	int start = index + 8;
// 	index = index + 8;
// 	while(fullRequest[index] && fullRequest[index] != '\n')
// 	{
// 		index++;
// 	}
// 	content_type = fullRequest.substr(start, index);
// 	std::cout << content_type << std::endl;
// 	if(path.compare(0, 8,"/uploads") == 0)
// 	{
// 		std::string file_name = "file";
// 		std::string extension;
// 		int i;
// 		for(i = content_type.length() - 1; content_type[i - 1] && content_type[i] && content_type[i - 1] != '/'; i--)
// 		{
// 		}
// 		extension = content_type.substr(i, content_type.length() - 1);
// 		// std::ofstream file("./uploads/" + file_name + "." + extension,  std::ios::out | std::ios::binary);
// 		std::ofstream file("./uploads/file.png",  std::ios::out | std::ios::binary);
// 		file << currRequest.body;
// 		write(requestSocket, "200 OK\r\n", 7);
// 		file.close();
// 		// std::cout << "tak tutaj mozesz wrzucac " << extension <<  std::endl;

// 	}else{
// 		std::cout << "Wrong path mate" << std::endl;
// 	}
// }

// void		server::handleRequest(int requestSocket, std::string &fullRequest)
// {
// 	// fillRequestStruct(fullRequest);
// 	// if (checkRequestErrors(requestSocket) != 0)
// 		// return ;
// 	if (currRequest.method.compare("GET") == 0)
// 	{
// 		// test for errors
// 		// send response
// 		sendResponse(requestSocket, currRequest.URI);
// 		cout << "its a get request :)" << endl;
// 	}
// 	else if (currRequest.method.compare("POST") == 0)
// 	{
// 		// test for errors
// 		// send response
// 		handle_post(requestSocket, currRequest.URI, fullRequest);
// 		cout << "its a post request :)" << endl;
// 	}
// 	else if (currRequest.method.compare("DELETE") == 0)
// 	{
// 		// test for errors
// 		// send response
// 		cout << "its a delete request :)" << endl;
// 	}
// 	else
// 	{
// 		cout << "We should throw an error code with a message that we do not support this method" << endl;
// 	}
// 	currRequest.headers.clear();
// 	currRequest.body.clear();
// 	// currRequest.httpVers.clear();
// 	// currRequest.method.clear();
// 	// currRequest.URI.clear();
// }

std::string		server::getBinary(std::string &path, long *size, int request_soc)
{

	FILE	*file_stream;
	std::string def_path("./database/default_index.html");
	std::string fav_path("./database/favicon.ico");
	std::string err_path("./database/Error_404.png");
	if(path.compare("/") == 0)
	{
		//Root path for welcome page
		file_stream = fopen(def_path.c_str() , "rb");
	}else if(path.compare("/favicon.ico") == 0)
	{
		//Favicon for now streamlined
		file_stream = fopen(fav_path.c_str(), "rb");
	}
	else{
		//If there is a different file user wants to open
		file_stream = fopen(("."+path).c_str(), "rb");
	}
	if(file_stream == nullptr)
	{
		//For errors
		file_stream = fopen(err_path.c_str(), "rb");
		path = "/database/Error_404.png";
	}
	std::string binaryString;
	fseek(file_stream, 0, SEEK_END);
	*size = ftell(file_stream);
	cout << YELLOW << *size << RESET_LINE;
	rewind(file_stream);
	std::vector<char>  binaryVector;
	binaryVector.resize(*size);
	fread(&binaryVector[0], 1, *size, file_stream);
	
	for(long i = 0; i < *size; i++)
	{
		binaryString.push_back(binaryVector[i]);
	}
	cout << GREEN << binaryString.size() << RESET_LINE;
	return binaryString;
}

std::string server::makeHeader(long bodySize, std::string &path) //prolly other stuff too
{
	std::string		out;
	std::string extension;


	int i = path.length() - 1;
	while(path[i] && path[i] != '.')
	{
		i--;
	}
	if(!path[i])
	{
		//Default extension if there is none 
		extension = "text";
	}else{
		extension.append(path.substr(i, path.length() - i));
	}
	std::cout << "extension thingy " << extension << std::endl; 
	// out.append(path);
	out = "Content-Type: " + extension + " ; Content-Transfer-Encoding: binary; Content-Length: " + std::to_string(bodySize) + ";charset=ISO-8859-4 ";
	return (out);
}


void			server::fillInPossibleTypes()
{

	possible_types[".aac"]      = "audio/aac\r\n";
	possible_types[".aac"]      = "audio/aac\r\n";
	possible_types[".aac"]      = "audio/aac\r\n";
	possible_types[".aac"]      = "audio/aac\r\n";
	possible_types[".mp4"]      = "video/mp4\r\n";
	possible_types[".mp4"]      = "video/mp4\r\n";
	possible_types[".mp4"]      = "video/mp4\r\n";
	possible_types[".mp4"]      = "video/mp4\r\n";
	possible_types[".abw"]      = "application/x-abiword\r\n";
	possible_types[".abw"]      = "application/x-abiword\r\n";
	possible_types[".abw"]      = "application/x-abiword\r\n";
	possible_types[".abw"]      = "application/x-abiword\r\n";
	possible_types[".arc"]      = "application/octet-stream\r\n";
	possible_types[".arc"]      = "application/octet-stream\r\n";
	possible_types[".arc"]      = "application/octet-stream\r\n";
	possible_types[".arc"]      = "application/octet-stream\r\n";
	possible_types[".avi"]      = "video/x-msvideo\r\n";
	possible_types[".avi"]      = "video/x-msvideo\r\n";
	possible_types[".avi"]      = "video/x-msvideo\r\n";
	possible_types[".avi"]      = "video/x-msvideo\r\n";
	possible_types[".azw"]      = "application/vnd.amazon.ebook\r\n";
	possible_types[".azw"]      = "application/vnd.amazon.ebook\r\n";
	possible_types[".azw"]      = "application/vnd.amazon.ebook\r\n";
	possible_types[".azw"]      = "application/vnd.amazon.ebook\r\n";
	possible_types[".bin"]      = "application/octet-stream\r\n";
	possible_types[".bin"]      = "application/octet-stream\r\n";
	possible_types[".bin"]      = "application/octet-stream\r\n";
	possible_types[".bin"]      = "application/octet-stream\r\n";
	possible_types[".bz"]       = "application/x-bzip\r\n";
	possible_types[".bz"]       = "application/x-bzip\r\n";
	possible_types[".bz"]       = "application/x-bzip\r\n";
	possible_types[".bz"]       = "application/x-bzip\r\n";
	possible_types[".bz2"]      = "application/x-bzip2\r\n";
	possible_types[".bz2"]      = "application/x-bzip2\r\n";
	possible_types[".bz2"]      = "application/x-bzip2\r\n";
	possible_types[".bz2"]      = "application/x-bzip2\r\n";
	possible_types[".csh"]      = "application/x-csh\r\n";
	possible_types[".csh"]      = "application/x-csh\r\n";
	possible_types[".csh"]      = "application/x-csh\r\n";
	possible_types[".csh"]      = "application/x-csh\r\n";
	possible_types[".css"]      = "text/css\r\n";
	possible_types[".css"]      = "text/css\r\n";
	possible_types[".css"]      = "text/css\r\n";
	possible_types[".css"]      = "text/css\r\n";
	possible_types[".csv"]      = "text/csv\r\n";
	possible_types[".csv"]      = "text/csv\r\n";
	possible_types[".csv"]      = "text/csv\r\n";
	possible_types[".csv"]      = "text/csv\r\n";
	possible_types[".doc"]      = "application/msword\r\n";
	possible_types[".doc"]      = "application/msword\r\n";
	possible_types[".doc"]      = "application/msword\r\n";
	possible_types[".doc"]      = "application/msword\r\n";
	possible_types[".epub"]     = "application/epub+zip\r\n";
	possible_types[".epub"]     = "application/epub+zip\r\n";
	possible_types[".epub"]     = "application/epub+zip\r\n";
	possible_types[".epub"]     = "application/epub+zip\r\n";
	possible_types[".gif"]      = "image/gif\r\n";
	possible_types[".gif"]      = "image/gif\r\n";
	possible_types[".gif"]      = "image/gif\r\n";
	possible_types[".gif"]      = "image/gif\r\n";
	possible_types[".htm"]      = "text/html\r\n";
	possible_types[".htm"]      = "text/html\r\n";
	possible_types[".htm"]      = "text/html\r\n";
	possible_types[".htm"]      = "text/html\r\n";
	possible_types[".html"]     = "text/html\r\n";
	possible_types[".html"]     = "text/html\r\n";
	possible_types[".html"]     = "text/html\r\n";
	possible_types[".html"]     = "text/html\r\n";
	possible_types[".ico"]      = "image/x-icon\r\n";
	possible_types[".ics"]      = "text/calendar\r\n";
	possible_types[".jar"]      = "Temporary Redirect\r\n";
	possible_types[".jpeg"]     = "image/jpeg\r\n";
	possible_types[".jpg"]      = "image/jpeg\r\n";
	possible_types[".js"]       = "application/js\r\n";
	possible_types[".json"]     = "application/json\r\n";
	possible_types[".mid"]      = "audio/midi\r\n";
	possible_types[".midi"]     = "audio/midi\r\n";
	possible_types[".mpeg"]     = "video/mpeg\r\n";
	possible_types[".mpkg"]     = "application/vnd.apple.installer+xml\r\n";
	possible_types[".odp"]      = "application/vnd.oasis.opendocument.presentation\r\n";
	possible_types[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet\r\n";
	possible_types[".odt"]      = "application/vnd.oasis.opendocument.text\r\n";
	possible_types[".oga"]      = "audio/ogg\r\n";
	possible_types[".ogv"]      = "video/ogg\r\n";
	possible_types[".ogx"]      = "application/ogg\r\n";
	possible_types[".png"]      = "image/png\r\n";
	possible_types[".pdf"]      = "application/pdf\r\n";
	possible_types[".ppt"]      = "application/vnd.ms-powerpoint\r\n";
	possible_types[".rar"]      = "application/x-rar-compressed\r\n";
	possible_types[".rtf"]      = "application/rtf\r\n";
	possible_types[".sh"]       = "application/x-sh\r\n";
	possible_types[".svg"]      = "image/svg+xml\r\n";
	possible_types[".swf"]      = "application/x-shockwave-flash\r\n";
	possible_types[".tar"]      = "application/x-tar\r\n";
	possible_types[".tif"]      = "image/tiff\r\n";
	possible_types[".tiff"]     = "image/tiff\r\n";
	possible_types[".ttf"]      = "application/x-font-ttf\r\n";
	possible_types[".txt"]      = "text/plain\r\n";
	possible_types[".vsd"]      = "application/vnd.visio\r\n";
	possible_types[".wav"]      = "audio/x-wav\r\n";
	possible_types[".weba"]     = "audio/webm\r\n";
	possible_types[".webm"]     = "video/webm\r\n";
	possible_types[".webp"]     = "image/webp\r\n";
	possible_types[".woff"]     = "application/x-font-woff\r\n";
	possible_types[".xhtml"]    = "application/xhtml+xml\r\n";
	possible_types[".xls"]      = "application/vnd.ms-excel\r\n";
	possible_types[".xml"]      = "application/xml\r\n";
	possible_types[".xul"]      = "application/vnd.mozilla.xul+xml\r\n";
	possible_types[".zip"]      = "application/zip\r\n";
	possible_types[".3gp"]      = "video/3gpp audio/3gpp\r\n";
	possible_types[".3g2"]      = "video/3gpp2 audio/3gpp2\r\n";
	possible_types[".7z"]		= "application/x-7z-compressed\r\n";

}


config	&server::getConfig( void )
{
	return (servConfig);
}


bool	server::getConfigOutcome( void )
{
	return(servConfig.getOutcome());
}
