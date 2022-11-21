#include "./includes/webserv.hpp"

#include "./includes/server.hpp"
#include "./includes/config.hpp"

int main( int argc, char **argv )
{
	if(argc == 1)
	{
		server	servy;

		cout << servy.getConfig().getConfigMap().at("errorPage") << endl;
		while(true)
		{
			servy.request();
		}
	}
	else if (argc == 2)
	{
		server			servy(argv[1]);


		cout << servy.getConfig().getConfigMap().at("errorPage") << endl;
		while(true)
		{
			servy.request();
		}
	}
	else if (argc > 2)
	{
		cerr << RED << "Too many arguments!" << RESET_LINE;
		return (-1);
	}
	// cout << confy.getConfigMap().at("errorPage") << endl;

}



// • Choose the port and host of each ’server’.
// • Setup the server_names or not.
// • The first server for a host:port will be the default for this host:port (that means
// it will answer to all the requests that don’t belong to an other server).
// • Setup default error pages.
// • Limit client body size.
