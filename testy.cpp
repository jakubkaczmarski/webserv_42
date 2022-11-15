#include "webserv.hpp"

#include "placeholder.hpp"

int main( void )
{
	placeholder placey();

	cout << "hiello" << endl;
	while(true)
	{
		placey.request();
	}
}
