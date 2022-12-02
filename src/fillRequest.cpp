#include "../includes/server.hpp"

void	Server::fillRequestStruct(std::vector<connecData*>::iterator	it)
{
	fillRequestLineItems(it);
	fillRequestHeaders(it);
	fillRequestBody(it);
}

void Server::fillRequestLineItems(std::vector<connecData*>::iterator	it)
{

	//get request line
	std::string	requestLine = (*it)->request.raw.substr(0, (*it)->request.raw.find('\n'));
	std::vector<string> requestLineV = split(requestLine, ' ');
	(*it)->request.method		= requestLineV[0];
	(*it)->request.URI			= requestLineV[1];
	(*it)->request.httpVers		= requestLineV[2];

	// print out request line
	// cout << YELLOW << "method = " << (*it)->request.method << endl;
	// cout << "URI = " << (*it)->request.URI << endl;
	// cout << "httpVers = " << (*it)->request.httpVers << RESET_LINE;
}


void	Server::fillRequestHeaders(std::vector<connecData*>::iterator	it)
{
	//get headers
	int begin	= (*it)->request.raw.find('\n') + 1;
	int size	= (*it)->request.raw.find("\r\n\r\n") - begin;
	std::string	headers = (*it)->request.raw.substr(begin, size);
	std::vector<string> headersVector = split(headers, '\n');
	std::vector<string> key_value;
	size_t vectorSize = headersVector.size();
	for (size_t i = 0; i < vectorSize; i++)
	{
		key_value = split(headersVector[i], ':', 1);
		(*it)->request.headers.insert(std::make_pair(key_value[0], key_value[1]));
	}
	
	// print headers to terminal
	for (auto i : (*it)->request.headers)
	{
		cout << RED << i.first << ": " << i.second << RESET_LINE;
	}
}
void Server::fillRequestBody(std::vector<connecData*>::iterator	it)
{
	//get body
	size_t	begin;
	size_t	size;
	if ((*it)->request.headers.end() != (*it)->request.headers.find("Content-Length")) // or 
	{
		begin		= (*it)->request.raw.find("\r\n\r\n") + 4;
		size		= stoi((*it)->request.headers.at("Content-Length"));
		std::string	body = (*it)->request.raw.substr(begin, size);			//still have to do (what if big body .. idea is to char* to data(+size of headers))
		(*it)->request.body = body;
	}
	// else if ((*it)->request.headers.end() != (*it)->request.headers.find("Content-Length"))

	// print body
	// cout << PURPLE << (*it)->request.body << RESET_LINE;
}
