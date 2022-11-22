#ifndef server_HPP
#define server_HPP
#include "webserv.hpp"
#include "config.hpp"


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
		int					serverSocket;
		int					connectionSocket;
		struct sockaddr_in	serverAddress;
		int					sizeOfServerAddress = sizeof(serverAddress);
		std::string			fullRequest;
		s_request			currRequest;
		s_response			currResponse;
		std::map<std::string, std::string> possible_types;
		int					port;
		config				servConfig;
		// int					host;  // what is this???
		size_t				max_client_body_size;




		void			failTest( int check, std::string message );
		void			servAddressInit( void );
		void			fillRequestLineItems(string &fullRequest);
		void			fillRequestHeaders(string &fullRequest);
		void			fillRequestBody(std::string &fullRequest);
		void			fillRequestStruct(std::string &fullRequest);
		int				checkGetRequest(int requestSocket);
		int				checkRequestErrors(int requestSocket);
		void			handleRequest(int requestSocket, std::string &fullRequest);
		std::string		getBinary(std::string &path, long *size, int request_soc);
		std::string		makeHeader(long bodySize, std::string &path);									//prolly other stuff too
		void			fillResponseStructBinary(std::string &path, int request_soc);
		void			sendResponse(int requestSocket, std::string &path);			// im writing this with a get request in mind
		void			fillInPossibleTypes();
	public:
		server(): servConfig()
		{
			fillInPossibleTypes();
			servAddressInit();
		};
		server(char * confPath): servConfig(confPath)
		{
			fillInPossibleTypes();
			servAddressInit();
		};
		~server() {
		};
		void	handleRequest( void )
		{
		};


		config	getConfig( void );

		void		request( void );
};

#endif