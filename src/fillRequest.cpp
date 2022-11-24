#include "../includes/server.hpp"

void	server::fillRequestStruct(std::string &fullRequest)
{
	fillRequestLineItems(fullRequest);
	fillRequestHeaders(fullRequest);
	fillRequestBody(fullRequest);
}

void server::fillRequestLineItems(string &fullRequest)
{
	//get request line
	std::string	requestLine = fullRequest.substr(0, fullRequest.find('\n'));
	std::vector<string> requestLineV = split(requestLine, ' ');
	currRequest.method		= requestLineV[0];
	currRequest.URI			= requestLineV[1];
	currRequest.httpVers	= requestLineV[2];

	// print out request line
	// cout << YELLOW << "method = " << currRequest.method << endl;
	// cout << "URI = " << currRequest.URI << endl;
	// cout << "httpVers = " << currRequest.httpVers << RESET_LINE;
}


void	server::fillRequestHeaders(string &fullRequest)
{
	//get headers
	int begin	= fullRequest.find('\n') + 1;
	int size	= fullRequest.find("\r\n\r\n") - begin;
	std::string	headers = fullRequest.substr(begin, size);
	std::vector<string> headersVector = split(headers, '\n');
	std::vector<string> key_value;
	size_t vectorSize = headersVector.size();
	for (size_t i = 0; i < vectorSize; i++)
	{
		key_value = split(headersVector[i], ':', 1);
		currRequest.headers.insert(std::make_pair(key_value[0], key_value[1]));
	}
	
	// print headers to terminal
	for (auto i : currRequest.headers)
	{
		cout << RED << i.first << ": " << i.second << RESET_LINE;
	}
}
void server::fillRequestBody(std::string &fullRequest)
{
	//get body
	size_t	begin;
	size_t	size;
	if (currRequest.headers.end() != currRequest.headers.find("Content-Length")) // or 
	{
		begin		= fullRequest.find("\r\n\r\n") + 4;
		size		= stoi(currRequest.headers.at("Content-Length"));
		std::string	body = fullRequest.substr(begin, size);
		currRequest.body = body;
	}
	// else if (currRequest.headers.end() != currRequest.headers.find("Content-Length"))

	// print body
	// cout << PURPLE << currRequest.body << RESET_LINE;
}
