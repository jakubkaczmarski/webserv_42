#include "./includes/webserv.hpp"

#include "./includes/server.hpp"
#include "./includes/config.hpp"

int main( int argc, char **argv )
{
	server	servy;
	config	confy;	

	if (argc == 2)
		config	confy(argv[1]);
	else if (argc > 2)
	{
		cerr << RED << "Too many arguments!" << RESET_LINE;
		return (-1);
	}
	cout << confy.getConfigMap().at("errorPage") << endl;

	while(true)
	{
		servy.request();
	}
}



// • Choose the port and host of each ’server’.
// • Setup the server_names or not.
// • The first server for a host:port will be the default for this host:port (that means
// it will answer to all the requests that don’t belong to an other server).
// • Setup default error pages.
// • Limit client body size.