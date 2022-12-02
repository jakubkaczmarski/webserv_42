#ifndef server_HPP
#define server_HPP
#include "webserv.hpp"
#include <fstream>
#include "config.hpp"
 #include <sys/wait.h>


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
		string									fileCGI;
};

#include "CGI.hpp"
class Server
{
	private:
	
		int								serverSocket;
		std::vector<connecData*>		connections;
		int								epollFD;
		// size_t							currConnections;  // no need because connecctions is a vector now

		// do we need this shit?
		struct sockaddr_in	serverAddress;
		int					sizeOfServerAddress = sizeof(serverAddress);

		

		int									connectionSocket;
		std::string							fullRequest;
		s_request							currRequest;
		s_response							currResponse;
		std::map<std::string, std::string>	possible_types;
		std::map<std::string, std::string>	possible_return_code;
		std::map<std::string, std::string>	possible_cgi_paths;
		config								servConfig;
		CGI									objectCGI;	

		std::map<std::string, std::string>	get_cgi_env(std::vector<connecData*>::iterator it);
		std::string 	get_possible_type(std::string type, bool first);
		void			failTest( int check, std::string message );
		std::string		get_extension_from_request_get(std::vector<connecData*>::iterator it);
		std::string		get_extension_from_request_post(std::vector<connecData*>::iterator it);
		void			servAddressInit( void );
		void			fillRequestLineItems(std::vector<connecData*>::iterator	it);
		void			fillRequestHeaders(std::vector<connecData*>::iterator	it);
		void			fillRequestBody(std::vector<connecData*>::iterator	it);
		void			fillRequestStruct(std::vector<connecData*>::iterator it);
		int				checkGetRequest(int requestSocket);
		int				checkRequestErrors(int requestSocket);
		void			handleRequest(int requestSocket, std::string &fullRequest);
		std::string		getBinary(std::string &path, long *size, int request_soc);
		std::string		makeHeader(long bodySize, std::string &path);									//prolly other stuff too
		void			fillResponseStructBinary(std::string &path, int request_soc);
		void			sendResponse(int requestSocket, std::string &path);			// im writing this with a get request in mind
		void			fillInPossibleTypes();
		void 			handle_post( std::vector<connecData*>::iterator it, struct epoll_event ev);
		void 			handleGet( std::vector<connecData*>::iterator it);
		void			handle_delete(std::vector<connecData*>::iterator it ,struct epoll_event	ev);
		void			acceptConnection( int epollFD );
		void			closeAndRemoveFromEpoll( struct epoll_event ev );
		void			doRequestStuff( struct epoll_event ev );
		void			doResponseStuff( struct epoll_event ev );
		std::vector<connecData*>::iterator				findStructVectorIt( struct epoll_event ev);
		void			endRequest( struct epoll_event ev, std::vector<connecData*>::iterator it );
		void			endResponse( struct epoll_event ev );
		void			confusedEpoll( struct epoll_event ev );
		bool			parseRequest( struct epoll_event ev );
		void			responseHeader( std::vector<connecData*>::iterator it, struct epoll_event	ev );
		void			create_response_and_send(std::vector<connecData*>::iterator it);
		void			handle_cgi(std::vector<connecData *>::iterator it);
		// void			removeFromEpoll( struct epoll_event ev );
		bool			validateRequest( struct epoll_event ev );
		void			stopInvaldiRequest( struct epoll_event ev );


	public:
		Server(): servConfig()
		{
			if(servConfig.getOutcome() == false)
			{
				// servConfig.~config();
				return ;
			}
			
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