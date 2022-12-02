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

#include <netinet/in.h>



#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void	server::servAddressInit( void )
{
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
	if(path.compare("/") == 0)
	{
		//Root path for welcome page
		file_stream = fopen(DEFAULT_PATH , "rb");
	}else if(path.compare("/favicon.ico") == 0)
	{
		//Favicon for now streamlined
		file_stream = fopen(FAV_ICON_PATH, "rb");
	}
	else{
		//If there is a different file user wants to open
		file_stream = fopen(("."+path).c_str(), "rb");
	}
	if(file_stream == nullptr)
	{
		//For errors
		file_stream = fopen(ERROR_404_PATH, "rb");
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
void	server::create_response_and_send(std::vector<connecData*>::iterator it)
{
	(*it)->response.statusMessage = possible_return_code[(*it)->response.status_code];
	(*it)->response.headers = "HTTP/1.1 ";
	(*it)->response.headers.append((*it)->response.status_code);
	(*it)->response.headers.append(" ");
	(*it)->response.headers.append((*it)->response.statusMessage);
	(*it)->response.headers.append("\n");
	(*it)->response.headers.append("Content-Length: ");
	(*it)->response.headers.append((*it)->response.content_lenght_str);
	(*it)->response.headers.append("\n");
	(*it)->response.headers.append("Connection: close\n");
	(*it)->response.headers.append("Content-Type: ");
	(*it)->response.headers.append((*it)->response.content_type);
	(*it)->response.headers.append("\n");
	send((*it)->socket, (*it)->response.headers.c_str(), (*it)->response.headers.length(), 0);
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
	std::string n;
	return n;
}

void			server::fillInPossibleTypes()
{
	std::string s = ".aac";
	possible_types.insert(std::make_pair(std::string(".aac"), std::string("audio/aac\r\n")));
	possible_types.insert(std::make_pair(std::string(".aac"),  std::string("audio/aac\r\n")));
	possible_types.insert(std::make_pair(std::string(".aac" )     ,std::string("audio/aac\r\n")));
	possible_types.insert(std::make_pair(std::string(".aac" ),   std::string("audio/aac\r\n")));
	possible_types.insert(std::make_pair(std::string(".mp4"), std::string("video/mp4\r\n")));
	possible_types.insert(std::make_pair(std::string(".mp4" ) ,std::string("video/mp4\r\n")));
	possible_types.insert(std::make_pair(std::string(".mp4" )  ,std::string("video/mp4\r\n")));
	possible_types.insert(std::make_pair(std::string(".abw" )  , std::string("application/x-abiword\r\n")));
	possible_types.insert(std::make_pair(std::string(".abw" )  , std::string("application/x-abiword\r\n")));
	possible_types.insert(std::make_pair(std::string(".arc" )     ,std::string("application/octet-stream\r\n")));
	possible_types.insert(std::make_pair(std::string(".arc" )     , std::string("application/octet-stream\r\n")));
	possible_types.insert(std::make_pair(std::string(".avi" )     ,std::string("video/x-msvideo\r\n")));
	possible_types.insert(std::make_pair(std::string(".avi" )     ,std::string("video/x-msvideo\r\n")));
	possible_types.insert(std::make_pair(std::string(".azw" ) ,std::string("application/vnd.amazon.ebook\r\n")));
	possible_types.insert(std::make_pair(std::string(".bin"), std::string("application/octet-stream\r\n")));
	possible_types.insert(std::make_pair(std::string(".bz" ),std::string("application/x-bzip\r\n")));
	possible_types.insert(std::make_pair(std::string(".bz2" )     ,	std::string("application/x-bzip2\r\n")));
	possible_types.insert(std::make_pair(std::string(".csh" )     ,	std::string("application/x-csh\r\n")));
	possible_types.insert(std::make_pair(std::string(".css" )     , std::string("text/css\r\n")));
	possible_types.insert(std::make_pair(std::string(".csv" )    ,	std::string("text/csv\r\n")));
	possible_types.insert(std::make_pair(std::string(".doc" )     , std::string("application/msword\r\n")));
	possible_types.insert(std::make_pair(std::string(".epub")     ,	std::string("application/epub+zip\r\n")));
	possible_types.insert(std::make_pair(std::string(".gif" )     ,	std::string("image/gif\r\n")));
	possible_types.insert(std::make_pair(std::string(".htm" )     , std::string("text/html\r\n")));
	possible_types.insert(std::make_pair(std::string(".html")     , std::string("text/html\r\n")));
	possible_types.insert(std::make_pair(std::string(".ico" )     , std::string("image/x-icon\r\n")));
	possible_types.insert(std::make_pair(std::string(".ics" )     ,	std::string("text/calendar\r\n")));
	possible_types.insert(std::make_pair(std::string(".jar" )   ,	std::string("Temporary Redirect\r\n")));
	possible_types.insert(std::make_pair(std::string(".jpeg")   ,	std::string("image/jpeg\r\n")));
	possible_types.insert(std::make_pair(std::string(".jpg"	),		std::string("image/jpeg\r\n")));
	possible_types.insert(std::make_pair(std::string(".js"  )     ,std::string( "application/js\r\n")));
	possible_types.insert(std::make_pair(std::string(".json")     ,std::string( "application/json\r\n")));
	possible_types.insert(std::make_pair(std::string(".mid" )     ,std::string("audio/midi\r\n")));
	possible_types.insert(std::make_pair(std::string(".midi")     ,std::string("audio/midi\r\n")));
	possible_types.insert(std::make_pair(std::string(".mpeg")     ,std::string("video/mpeg\r\n")));
	possible_types.insert(std::make_pair(std::string(".mpkg")     ,std::string("application/vnd.apple.installer+xm)l\r\n")));
	possible_types.insert(std::make_pair(std::string(".odp" )     ,std::string("application/vnd.oasis.opendocument.pr)esentation\r\n")));
	possible_types.insert(std::make_pair(std::string(".ods" )     ,std::string( "application/vnd.oasis.opendocument.spreadsheet\r\n")));
	possible_types.insert(std::make_pair(std::string(".odt" )     ,std::string( "application/vnd.oasis.opendocument.text\r\n")));
	possible_types.insert(std::make_pair(std::string(".oga" )     ,std::string("audio/ogg\r\n")));
	possible_types.insert(std::make_pair(std::string(".ogv" )     ,std::string( "video/ogg\r\n")));
	possible_types.insert(std::make_pair(std::string(".ogx" )     ,std::string("application/ogg\r\n")));
	possible_types.insert(std::make_pair(std::string(".png" )     ,std::string("image/png\r\n")));
	possible_types.insert(std::make_pair(std::string(".pdf" )     ,std::string("application/pdf\r\n")));
	possible_types.insert(std::make_pair(std::string(".ppt" )     ,std::string( "application/vnd.ms-powerpoint\r\n")));
	possible_types.insert(std::make_pair(std::string(".rar" )     ,std::string( "application/x-rar-compressed\r\n")));
	possible_types.insert(std::make_pair(std::string(".rtf" )     ,std::string( "application/rtf\r\n")));
	possible_types.insert(std::make_pair(std::string(".sh"  )     ,std::string( "application/x-sh\r\n")));
	possible_types.insert(std::make_pair(std::string(".svg" )     ,std::string( "image/svg+xml\r\n")));
	possible_types.insert(std::make_pair(std::string(".swf" )     ,std::string("application/x-shockwave-flash\r\n")));
	possible_types.insert(std::make_pair(std::string(".tar" )     ,std::string( "application/x-tar\r\n")));
	possible_types.insert(std::make_pair(std::string(".tif" )     ,std::string( "image/tiff\r\n")));
	possible_types.insert(std::make_pair(std::string(".tiff")     ,std::string( "image/tiff\r\n")));
	possible_types.insert(std::make_pair(std::string(".ttf" )     ,std::string( "application/x-font-ttf\r\n")));
	possible_types.insert(std::make_pair(std::string(".txt" )     ,std::string( "text/plain\r\n")));
	possible_types.insert(std::make_pair(std::string(".vsd" )     ,std::string( "application/vnd.visio\r\n")));
	possible_types.insert(std::make_pair(std::string(".wav" )     ,std::string( "audio/x-wav\r\n")));
	possible_types.insert(std::make_pair(std::string(".weba")     ,std::string( "audio/webm\r\n")));
	possible_types.insert(std::make_pair(std::string(".webm")     ,std::string( "video/webm\r\n")));
	possible_types.insert(std::make_pair(std::string(".webp")     ,std::string( "image/webp\r\n")));
	possible_types.insert(std::make_pair(std::string(".woff")     ,std::string( "application/x-font-woff\r\n")));
	possible_types.insert(std::make_pair(std::string(".xhtml")    ,std::string( "application/xhtml+xml\r\n")));
	possible_types.insert(std::make_pair(std::string(".xls" )     ,std::string( "application/vnd.ms-excel\r\n")));
	possible_types.insert(std::make_pair(std::string(".xml" )     ,std::string( "application/xml\r\n")));
	possible_types.insert(std::make_pair(std::string(".xul" )     ,std::string( "application/vnd.mozilla.xul+xml\r\n")));
	possible_types.insert(std::make_pair(std::string(".zip" )     ,std::string( "application/zip\r\n")));
	possible_types.insert(std::make_pair(std::string(".3gp" )     ,std::string( "video/3gpp audio/3gpp\r\n")));
	possible_types.insert(std::make_pair(std::string(".3g2" )     ,std::string( "video/3gpp2 audio/3gpp2\r\n")));
	possible_types.insert(std::make_pair(std::string(".7z"), std::string("application/x-7z-compressed\r\n")));
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
