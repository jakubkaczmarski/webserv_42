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
			req.URI = DIRERRORPATH;
		}
	}
}

bool	Server::validateRequest( struct epoll_event ev )
{
	cout << GREEN << __func__ << RESET_LINE;
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	// print headers to terminal

	std::map<string, string>::iterator	itt = (*it)->request.headers.begin();
	std::map<string, string>::iterator	it_e = (*it)->request.headers.end();
	cout << (*it)->request.URI << " " <<  (*it)->request.method << endl;
	for (; itt != it_e; itt++)
	{
		cout << PURPLE << itt->first << ": " << itt->second << RESET_LINE;
	}
	if (servConfig.allowedMETHOD((*it)->request.method) == false)
	{
		cerr << RED << "Request rejected because of Invalid Method: " << (*it)->request.method << RESET_LINE;
		stopInvaldiRequest(ev); // stop request because illegal
		return (false);
	}
	cout << "this is before expansion" << (*it)->request.URI << endl;
	if((*it)->request.URI == "/")
	{
		cout << "got into if" << endl;
		(*it)->request.URI = DEFAULTPAGE;
	}
	cout << "this is after expansion" << (*it)->request.URI << endl;
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