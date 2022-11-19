#include "./includes/webserv.hpp"

#include "./includes/server.hpp"

int main( void )
{
	server servy;

	while(true)
	{
		servy.request();
	}
}
