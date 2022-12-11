#ifndef server_HPP
#define server_HPP
#include "webserv.hpp"
#include <fstream>
#include "config.hpp"
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct t_request
{
	std::string								raw;
	std::string								method;
	std::string								URI;
	std::string								httpVers;
	std::map<std::string, std::string>		headers;
	std::string								body; //for now string
	int										already_sent = 0;
	char*									body_in_char;
	int										fd = 0;
	long long								content_size;
	std::string								cgi_data;
	FILE	*								file_one;
	FILE	*								file_two;

}	s_request;

typedef struct t_response
{
	std::string								httpVers;
	std::string								status_code;
	std::string								statusMessage;			// number + message
	std::string								headers;
	int										content_lenght;
	std::string										content_lenght_str;
	// std::string								body;
	std::string								content_type;				// getBinary()
	int										body_fd;
}	s_response;

class connecData
{
	public:
		bool									finishedRequest;
		bool									finishedResponse;
		int										socket;
		s_request								request;
		s_response								response;
		bool									isCGI = false;
		string									fileNameCGI;
		struct epoll_event						*ev_p;
};

#include "CGI.hpp"
class Server
{
	friend class CGI;
	private:
	
		int								serverSocket;
		std::vector<connecData*>		connections;
		int								epollFD;

		// do we need this shit?
		struct sockaddr_in	serverAddress;
		int					sizeOfServerAddress = sizeof(serverAddress);

		

		int									connectionSocket;
		std::string							fullRequest;
		s_request							currRequest;
		s_response							currResponse;
		std::map<std::string, std::string>	possibleTypes;
		std::map<std::string, std::string>	possibleReturnCode;
		std::map<std::string, std::string>	possibleCGIPaths;
		config								servConfig;
		CGI									objectCGI;
		std::vector<std::string>			scriptsCGI;

		///serverFillRequest.cpp
		bool			parseRequest( struct epoll_event ev );
		void			fillRequestLineItems(std::vector<connecData*>::iterator	it);
		void			fillRequestHeaders(std::vector<connecData*>::iterator	it);
		void			fillRequestBody(std::vector<connecData*>::iterator	it);

		///serverHandlers.cpp
		void 			handlePost( std::vector<connecData*>::iterator it, struct epoll_event ev);
		void			handleDelete(std::vector<connecData*>::iterator it ,struct epoll_event	ev);
		void 			handleGet( std::vector<connecData*>::iterator it, struct epoll_event	ev);
		void			handleCGI( struct epoll_event	ev, std::vector<connecData*>::iterator it);

		bool			checkCGIPaths(std::vector<connecData*>::iterator it, struct epoll_event	ev);
		void			fillScriptsCGI(void);
		std::string 	getPossibleType(std::string type, bool first);
		void			failTest( int check, std::string message );
		std::string		getExtensionFromRequestGet(std::vector<connecData*>::iterator it);
		std::string		getExtensionFromRequestPost(std::vector<connecData*>::iterator it);
		void			servAddressInit( void );
		int				checkRequestErrors(int requestSocket);
		std::string		getBinary(std::string &path, long *size, int request_soc);
		std::string		makeHeader(long bodySize, std::string &path);									//prolly other stuff too
		void			fillResponseStructBinary(std::string &path, int request_soc);
		void			sendResponse(int requestSocket, std::string &path);			// im writing this with a get request in mind
		void			fillInPossibleTypes();
		void			acceptConnection( int epollFD );
		void			closeAndRemoveFromEpoll( struct epoll_event ev );
		void			readRequest( struct epoll_event ev );
		void			sendResponse( struct epoll_event ev );
		std::vector<connecData*>::iterator				findStructVectorIt( struct epoll_event ev);
		bool			doneReadingRequest( struct epoll_event ev, std::vector<connecData*>::iterator it );
		void			endConnection( struct epoll_event ev );
		void			confusedEpoll( struct epoll_event ev );
		void			prepareResponseHeader( std::vector<connecData*>::iterator it, struct epoll_event	ev );
		void			createAndSendResponseHeaders(struct epoll_event	ev, std::vector<connecData*>::iterator it);
		void			handle_cgi(std::vector<connecData *>::iterator it);
		bool			validateRequest( struct epoll_event ev );
		void			URIisDirectory(s_request &req);
		void			setErrorStatusCodeAndRespond(struct epoll_event	ev, std::vector<connecData*>::iterator it, std::string err);





	public:
		Server(): servConfig()
		{
			if(servConfig.getOutcome() == false)
			{
				// servConfig.~config();
				return ;
			}
			
			fillScriptsCGI();
			fillInPossibleTypes();
			servAddressInit();
			// currConnections = 0;
		};
		Server(char * confPath): servConfig(confPath)
		{
			if(servConfig.getOutcome() == false)
			{
				// servConfig.~config();
				cout << "returning " << endl;
				return ;
			}
			fillScriptsCGI();
			fillInPossibleTypes();
			servAddressInit();
			// currConnections = 0;
		};
		~Server() {
		};
		void	handleRequest( void )
		{
		};


		config		&getConfig( void );
		bool		getConfigOutcome( void );
		void		requestLoop( void );
};

#endif