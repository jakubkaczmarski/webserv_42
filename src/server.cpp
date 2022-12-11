#include "../includes/server.hpp"

void	Server::failTest( int check, string message )
{
	if (check < 0)
	{
		cerr << RED << message << " < 0! Abort!" << RESET_LINE;
		cerr << RED << "Errno message : " << strerror(errno) << RESET_LINE;
		exit(1);
	}
}

void	Server::acceptConnection( int epollFD )
{
	cout << SKY << __func__ << RESET_LINE;
	
	connecData			*tmp = new connecData();
	struct epoll_event	ev;

	tmp->request.already_sent = 0;
	tmp->request.fd = 0;
	tmp->isCGI = false;
	tmp->socket = accept(serverSocket, (SA *) NULL, NULL);
	failTest(tmp->socket, "accept new Socket");
	tmp->finishedResponse = false;
	tmp->finishedRequest = false;
	connections.push_back(tmp);
	ev = createEpollStruct(tmp->socket, EPOLLIN );
	epoll_ctl(epollFD, EPOLL_CTL_ADD, ev.data.fd, &ev);
}

void	Server::servAddressInit( void )
{
	cout << PINK << __func__ << RESET_LINE;
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);			// SOCK_STREAM == TCP

	int	option = 1;
	
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));

	serverAddress.sin_family		= AF_INET;							// means IPv4 / AD_INET6 =IPv6 
	// serverAddress.sin_port			= htons(PORT_NBR);					// Used port
	serverAddress.sin_port			= htons(ft_atoi(servConfig.getPort().c_str()));					// Used port
	// serverAddress.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);			// Address this socket is litening to
	// serverAddress.sin_addr.s_addr	= htonl((unsigned int)ft_atoi(servConfig.getHost().c_str()));			// Address this socket is litening to
	serverAddress.sin_addr.s_addr	= inet_addr(servConfig.getHost().c_str());			// Address this socket is litening to

	cout << INADDR_LOOPBACK << endl;

	failTest(serverSocket, "Server Socket");

	failTest(bind(serverSocket, (SA *) &serverAddress, sizeof(serverAddress)),
				"Binding Server Socket");

	failTest(listen(serverSocket, SOCKET_BACKLOG),						// SOMAXCONN???
				"listen() of Server Socket");
}; // probably here address

string	Server::getPossibleType(string type, bool first)
{
	cout << PINK << __func__ << RESET_LINE;
	std::map<string, string>::iterator it = possibleTypes.find(type);

	if(first && it != possibleTypes.end())
	{
		return (*it).first;
	}else if (it != possibleTypes.end()){
		return (*it).second;
	}
	string n;
	return n;
}

void	Server::fillInPossibleTypes()
{
	cout << PINK << __func__ << RESET_LINE;

	possibleTypes.insert(make_pair(string(".aac"), string("audio/aac\r\n")));
	possibleTypes.insert(make_pair(string(".aac"), string("audio/aac\r\n")));
	possibleTypes.insert(make_pair(string(".aac"), string("audio/aac\r\n")));
	possibleTypes.insert(make_pair(string(".aac"), string("audio/aac\r\n")));
	possibleTypes.insert(make_pair(string(".mp4"), string("video/mp4\r\n")));
	possibleTypes.insert(make_pair(string(".mp4"), string("video/mp4\r\n")));
	possibleTypes.insert(make_pair(string(".mp4"), string("video/mp4\r\n")));
	possibleTypes.insert(make_pair(string(".abw"), string("application/x-abiword\r\n")));
	possibleTypes.insert(make_pair(string(".abw"), string("application/x-abiword\r\n")));
	possibleTypes.insert(make_pair(string(".arc"), string("application/octet-stream\r\n")));
	possibleTypes.insert(make_pair(string(".arc"), string("application/octet-stream\r\n")));
	possibleTypes.insert(make_pair(string(".avi"), string("video/x-msvideo\r\n")));
	possibleTypes.insert(make_pair(string(".avi"), string("video/x-msvideo\r\n")));
	possibleTypes.insert(make_pair(string(".azw"), string("application/vnd.amazon.ebook\r\n")));
	possibleTypes.insert(make_pair(string(".bin"), string("application/octet-stream\r\n")));
	possibleTypes.insert(make_pair(string(".bz" ), string("application/x-bzip\r\n")));
	possibleTypes.insert(make_pair(string(".bz2"), string("application/x-bzip2\r\n")));
	possibleTypes.insert(make_pair(string(".csh"), string("application/x-csh\r\n")));
	possibleTypes.insert(make_pair(string(".css"), string("text/css\r\n")));
	possibleTypes.insert(make_pair(string(".csv"), string("text/csv\r\n")));
	possibleTypes.insert(make_pair(string(".doc"), string("application/msword\r\n")));
	possibleTypes.insert(make_pair(string(".epub"), string("application/epub+zip\r\n")));
	possibleTypes.insert(make_pair(string(".gif" ), string("image/gif\r\n")));
	possibleTypes.insert(make_pair(string(".htm" ), string("text/html\r\n")));
	possibleTypes.insert(make_pair(string(".html"), string("text/html\r\n")));
	possibleTypes.insert(make_pair(string(".ico" ), string("image/x-icon\r\n")));
	possibleTypes.insert(make_pair(string(".ics" ), string("text/calendar\r\n")));
	possibleTypes.insert(make_pair(string(".jar" ), string("Temporary Redirect\r\n")));
	possibleTypes.insert(make_pair(string(".jpeg"), string("image/jpeg\r\n")));
	possibleTypes.insert(make_pair(string(".jpg"),  string("image/jpeg\r\n")));
	possibleTypes.insert(make_pair(string(".js"  ), string( "application/js\r\n")));
	possibleTypes.insert(make_pair(string(".json"), string( "application/json\r\n")));
	possibleTypes.insert(make_pair(string(".mid" ), string("audio/midi\r\n")));
	possibleTypes.insert(make_pair(string(".midi"), string("audio/midi\r\n")));
	possibleTypes.insert(make_pair(string(".mpeg"), string("video/mpeg\r\n")));
	possibleTypes.insert(make_pair(string(".mpkg"), string("application/vnd.apple.installer+xm)l\r\n")));
	possibleTypes.insert(make_pair(string(".odp" ), string("application/vnd.oasis.opendocument.pr)esentation\r\n")));
	possibleTypes.insert(make_pair(string(".ods" ), string( "application/vnd.oasis.opendocument.spreadsheet\r\n")));
	possibleTypes.insert(make_pair(string(".odt" ), string( "application/vnd.oasis.opendocument.text\r\n")));
	possibleTypes.insert(make_pair(string(".oga" ), string("audio/ogg\r\n")));
	possibleTypes.insert(make_pair(string(".ogv" ), string( "video/ogg\r\n")));
	possibleTypes.insert(make_pair(string(".ogx" ), string("application/ogg\r\n")));
	possibleTypes.insert(make_pair(string(".png" ), string("image/png\r\n")));
	possibleTypes.insert(make_pair(string(".pdf" ), string("application/pdf\r\n")));
	possibleTypes.insert(make_pair(string(".ppt" ), string( "application/vnd.ms-powerpoint\r\n")));
	possibleTypes.insert(make_pair(string(".rar" ), string( "application/x-rar-compressed\r\n")));
	possibleTypes.insert(make_pair(string(".rtf" ), string( "application/rtf\r\n")));
	possibleTypes.insert(make_pair(string(".sh"  ), string( "application/x-sh\r\n")));
	possibleTypes.insert(make_pair(string(".svg" ), string( "image/svg+xml\r\n")));
	possibleTypes.insert(make_pair(string(".swf" ), string("application/x-shockwave-flash\r\n")));
	possibleTypes.insert(make_pair(string(".tar" ), string( "application/x-tar\r\n")));
	possibleTypes.insert(make_pair(string(".tif" ), string( "image/tiff\r\n")));
	possibleTypes.insert(make_pair(string(".tiff"), string( "image/tiff\r\n")));
	possibleTypes.insert(make_pair(string(".ttf" ), string( "application/x-font-ttf\r\n")));
	possibleTypes.insert(make_pair(string(".txt" ), string( "text/plain\r\n")));
	possibleTypes.insert(make_pair(string(".vsd" ), string( "application/vnd.visio\r\n")));
	possibleTypes.insert(make_pair(string(".wav" ), string( "audio/x-wav\r\n")));
	possibleTypes.insert(make_pair(string(".weba"), string( "audio/webm\r\n")));
	possibleTypes.insert(make_pair(string(".webm"), string( "video/webm\r\n")));
	possibleTypes.insert(make_pair(string(".webp"), string( "image/webp\r\n")));
	possibleTypes.insert(make_pair(string(".woff"), string( "application/x-font-woff\r\n")));
	possibleTypes.insert(make_pair(string(".xhtml"), string( "application/xhtml+xml\r\n")));
	possibleTypes.insert(make_pair(string(".xls" ), string( "application/vnd.ms-excel\r\n")));
	possibleTypes.insert(make_pair(string(".xml" ), string( "application/xml\r\n")));
	possibleTypes.insert(make_pair(string(".xul" ), string( "application/vnd.mozilla.xul+xml\r\n")));
	possibleTypes.insert(make_pair(string(".zip" ), string( "application/zip\r\n")));
	possibleTypes.insert(make_pair(string(".3gp" ), string( "video/3gpp audio/3gpp\r\n")));
	possibleTypes.insert(make_pair(string(".3g2" ), string( "video/3gpp2 audio/3gpp2\r\n")));
	possibleTypes.insert(make_pair(string(".7z"), string("application/x-7z-compressed\r\n")));
	
	possibleReturnCode = 
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
	possibleCGIPaths = 
	{
		{"py", "/usr/bin/python3"},
		{"sh", "/bin/sh"},
		{"pl", "/usr/bin/perl"},
		{"php", "/usr/bin/php"}
	};
}

void Server::fillScriptsCGI(void)
{
	cout << PINK << __func__ << RESET_LINE;
	scriptsCGI.insert(scriptsCGI.end(), "/cgi-bin/i_stole_your_password.py");
	scriptsCGI.insert(scriptsCGI.end(), "/cgi-bin/magic.py");
	scriptsCGI.insert(scriptsCGI.end(), "/cgi-bin/perl.pl");
	scriptsCGI.insert(scriptsCGI.end(), "/cgi-bin/print_env_variables.py");
	scriptsCGI.insert(scriptsCGI.end(), "/cgi-bin/put_photo_in_cat.py");
	scriptsCGI.insert(scriptsCGI.end(), "/cgi-bin/directory_listing.sh");
}

config	&Server::getConfig( void )
{
	cout << PINK << __func__ << RESET_LINE;
	return (servConfig);
}


bool	Server::getConfigOutcome( void )
{
	cout << PINK << __func__ << RESET_LINE;
	return(servConfig.getOutcome());
}

