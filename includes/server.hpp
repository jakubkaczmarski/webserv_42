#ifndef server_HPP
#define server_HPP
#include "webserv.hpp"
#include <fstream>
#include "config.hpp"


typedef struct t_request
{
	std::string								raw;
	std::string								method;
	std::string								URI;
	std::string								httpVers;
	std::map<std::string, std::string>		headers;
	std::string								body; //for now string
	int										already_sent = 0;
	int										fd = 0;
}	s_request;

typedef struct t_response
{
	std::string								httpVers;
	std::string								statusMessage;			// number + message
	std::string								headers;
	int										content_lenght;
	// std::string								body;					// getBinary()
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
};

class server
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
		config								servConfig;




		void			failTest( int check, std::string message );
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
		void 			handle_post( std::vector<connecData*>::iterator it);
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
		void			parseRequest( struct epoll_event ev );
		void			responseHeader( std::vector<connecData*>::iterator it, struct epoll_event	ev );
		
		// void			removeFromEpoll( struct epoll_event ev );
		


	public:
		server(): servConfig()
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
		server(char * confPath): servConfig(confPath)
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
		~server() {
		};
		void	handleRequest( void )
		{
		};


		config		&getConfig( void );
		bool		getConfigOutcome( void );
		void		requestLoop( void );
};

#endif