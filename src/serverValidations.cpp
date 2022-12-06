#include "../includes/server.hpp"

void	Server::URIisDirectory( s_request &req )
{
	std::vector<std::string>			dirs;

	dirs.push_back("/database/intraPictures");
	dirs.push_back("/database/forms");
	dirs.push_back("/uploads");
	dirs.push_back("/database");
	std::vector<std::string>::iterator	DirIt = dirs.begin();
	std::vector<std::string>::iterator	DirIt_e = dirs.end();
	for (; DirIt != DirIt_e; DirIt++)
	{
		if(req.URI.compare(*DirIt) == 0)
		{
			req.method = "GET";
			req.URI = "/database/DirRequestError.png";
		}
	}
}

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
	if (servConfig.validateDirectoryListing(servConfig.getDirectoryListing()) == false)
	{
		cout << RED << "Request rejected because of wrong DirectoryListing Option: " << (*it)->request.httpVers << RESET_LINE;
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
	URIisDirectory((*it)->request);
	return (true);
}