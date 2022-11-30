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

std::string server::get_possible_type(std::string type, bool first)
{
	std::map<std::string, std::string>::iterator it = possible_types.find(type);
	
	if(first && it != possible_types.end())
	{
		return (*it).first;
	}else if (it != possible_types.end()){
		return (*it).second;
	}
	return NULL;
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
	possible_return_code = 
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
}


config	&server::getConfig( void )
{
	return (servConfig);
}


bool	server::getConfigOutcome( void )
{
	return(servConfig.getOutcome());
}
