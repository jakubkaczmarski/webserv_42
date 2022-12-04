#include "../includes/server.hpp"

bool	Server::validateRequest( struct epoll_event ev )
{
	cout << GREEN << __func__ << RESET_LINE;
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	if (servConfig.allowedMETHOD((*it)->request.method) == false)
	{
		cerr << RED << "Request rejected because of Invalid Method: " << (*it)->request.method << RESET_LINE;
		stopInvaldiRequest(ev); // stop request because illegal
		return (false);
	}
	if (servConfig.allowedURI((*it)->request.URI, (*it)->request.method) == false)
	{
		cerr << RED << "Request rejected because of Invalid URI: " << (*it)->request.URI << RESET_LINE;
		stopInvaldiRequest(ev);
		return (false);
	}
	if ((*it)->request.httpVers.compare((std::string)HTTPVERSION) != 0)
	{
		cout << RED << "Request rejected because of Invalid HTTP Version: " << (*it)->request.httpVers << RESET_LINE;
		stopInvaldiRequest(ev);
		return (false);
	}
	try
	{
		if (ft_atoi((*it)->request.headers.at("Content-Length").c_str()) > servConfig.getClientMaxBody())
			stopInvaldiRequest(ev);
			return (false);
	}
	catch(const std::exception& e)
	{
	}
	return (true);
}