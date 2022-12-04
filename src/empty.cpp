#include "../includes/server.hpp"

#include <filesystem>
struct epoll_event	createEpollStruct(int fdSocket, uint32_t flags)
{
	struct	epoll_event	ev;

	ev.events = flags;
	ev.data.fd = fdSocket;

	return (ev);
}

std::vector<connecData*>::iterator		Server::findStructVectorIt( struct epoll_event ev)
{
	std::vector<connecData*>::iterator	it = connections.begin();
	std::vector<connecData*>::iterator	it_e = connections.end();

	for (; it != it_e; it++)
	{
		if(ev.data.fd == (*it)->socket)
			break ;
	}
	return (it);
}

void			Server::stopInvaldiRequest( struct epoll_event ev )
{
	cout << GREEN << __func__ << RESET_LINE;
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	// close((*it)->response.body_fd);		// fd to body of response
	delete (*it);
	connections.erase(it);
	epoll_ctl(epollFD, EPOLL_CTL_DEL, ev.data.fd, &ev);
	close(ev.data.fd);	
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
	try
	{
		if (ft_atoi((*it)->request.headers.at("Content-Length").c_str()) > servConfig.getClientMaxBody())
			return (false);
	}
	catch(const std::exception& e)
	{
	}
	return (true);
}

bool	Server::parseRequest( struct epoll_event ev )
{
	cout << RED << __func__ << RESET_LINE;
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	fillRequestStruct(it);
	return (validateRequest(ev));
}

void	Server::endRequest( struct epoll_event ev, std::vector<connecData*>::iterator it )
{
	cout << RED << __func__ << RESET_LINE;
	epoll_ctl(epollFD, EPOLL_CTL_DEL, ev.data.fd, &ev);
	if (parseRequest(ev) == false)
		return ;
	ev = createEpollStruct((*it)->socket, EPOLLOUT);
	epoll_ctl(epollFD, EPOLL_CTL_ADD, ev.data.fd, &ev);
	(*it)->finishedRequest = true;

	// cout << RED << "endRequest and this is the uri: " << (*(findStructVectorIt(ev)))->request.URI << RESET_LINE;
	// connections.push_back(ev);

}

std::string	Server::get_extension_from_request_get(std::vector<connecData*>::iterator it)
{
	cout << RED << __func__ << RESET_LINE;
	
	std::string extension;
	int i = (*it)->request.URI.length() - 1;
	while((*it)->request.URI[i] && (*it)->request.URI[i] != '.')
	{
		i--;
	}
	if((*it)->request.URI.compare("/") == 0)
	{
		extension = ".html";
	}
	else if(!(*it)->request.URI[i])
	{
		//Default extension if there is none 
		extension = ".txt";
	}
	else{
		extension.append((*it)->request.URI.substr(i, (*it)->request.URI.length() - i));
	}
	if(get_possible_type(extension, false).empty())
	{
		if((*it)->response.status_code != "404")
			(*it)->response.status_code = "409";
		extension = "text/plain";
		std::cout << "Error "<< std::endl;
	}else{
		if((*it)->response.status_code != "404")
			(*it)->response.status_code = "200";
		extension = get_possible_type(extension, false);
	}
	return extension;
}

std::string	Server::get_extension_from_request_post(std::vector<connecData*>::iterator it)
{
	cout << RED << __func__ << RESET_LINE;
	
	std::string extension;
	FILE	*file_stream;
	int i = (*it)->request.URI.length() - 1;
 	if((*it)->request.URI.compare(0,8, "/uploads") == 0)
	{
		if((*it)->request.URI.length() < 9)
		{
			std::cout << "Path not specified correctly " << std::endl;
			return extension;
		}else{
			file_stream = fopen(("." + (*it)->request.URI).c_str(), "wb");
			if(!file_stream)
			{
				std::cout << "Cannot create a file " << std::endl;
				return extension;
			}
		}
	}else{
		return extension;
	}

	int pos = (*it)->request.raw.find("Content-Length: ");
	int j;
	for( j = pos + 15; (*it)->request.raw[j] != '\n'; j++)
	{
	}
	(*it)->response.content_lenght_str =  (*it)->request.raw.substr(pos + 15, j - 15 - pos);
	(*it)->request.content_size = ft_atoi(((*it)->request.raw.substr(pos + 15, j - 15 - pos)).c_str());
	if((*it)->request.content_size  == 0)
	{
		(*it)->response.status_code = "204";
		//Nothing to send 
		return extension;
	}
	int body_pos = (*it)->request.raw.find("\r\n\r\n");
	if(body_pos == (*it)->request.raw.npos)
	{
		(*it)->response.status_code = "204";
		return extension;
	}
	(*it)->request.body = (*it)->request.raw.substr(body_pos + 4, (*it)->request.raw.length() - body_pos - 4);
	while((*it)->request.URI[i] && (*it)->request.URI[i] != '.')
	{
		i--;
	}
	extension.append((*it)->request.URI.substr(i, (*it)->request.URI.length() - i));
	if(get_possible_type(extension, false).empty())
	{
		if((*it)->response.status_code != "404")
			(*it)->response.status_code = "409";
		extension = "text/plain";
		std::cout << "Error "<< std::endl;
	}else{
		if((*it)->response.status_code != "404")
			(*it)->response.status_code = "201";
		extension = get_possible_type(extension, false);
	}

	(*it)->request.body_in_char = (char *)(*it)->request.body.c_str();
	(*it)->request.fd = fileno(file_stream);
	(*it)->response.status_code = "201";

	return extension;
}

void 	Server::handle_post( std::vector<connecData*>::iterator it, struct epoll_event ev)
{
	cout << RED << __func__ << RESET_LINE;
	std::string extension = get_extension_from_request_post(it);
	if(extension.empty())
		endResponse(ev);

	(*it)->response.content_type = extension;
	create_response_and_send(it);
	// if((*it)->request.URI.

}

// std::map<std::string, std::string> Server::get_cgi_env(std::vector<connecData*>::iterator it)
// {
	// std::map<std::string, std::string> env;
	// std::map<std::string, std::string>::iterator iter;
	// std::cout << "Cgi stuff" << std::endl;
	// int pid = fork();
	// if(!pid)
	// {
	// 	execve("../cgi-bin/put_photo_in_cat.py", NULL, NULL);
	// }else{
	// 	waitpid(pid, 0, 0 );
	// }

// }

void	Server::handle_delete(std::vector<connecData*>::iterator it, struct epoll_event	ev)
{
	cout << RED << __func__ << RESET_LINE;
	
	FILE	*file_stream;
	std::cout << (*it)->request.URI << std::endl;

	if((*it)->request.URI.compare(0, 8,"/uploads") == 0)
	{
		//Root path for welcome page
		std::string ret = ".";
		ret.append((*it)->request.URI);
		if(remove(ret.c_str()) != 0)
		{
			(*it)->response.status_code = "417";
			std::cout << "Cannot remove this file " << "." + (*it)->request.URI << std::endl;
		}
		return ;
	}else{
		std::cout << "Cannot delete from diffrent directiory than uploads" << std::endl;
		(*it)->response.status_code = "417";
		return ;
	}	if(file_stream == nullptr)
	{
		//For errors
		(*it)->response.status_code = "404";
		std::cout << "File not found " << std::endl;
	}
	// (*it)->response.content_type = extension;
	// (*it)->response.content_lenght_str = conv.str();
	create_response_and_send(it);
}

void	Server::handleGet(std::vector<connecData*>::iterator it)
{
	cout << RED << __func__ << RESET_LINE;
	
	FILE	*file_stream;
	FILE	*file_str_2;

	cout << YELLOW << __func__ << "URI: "  << (*it)->request.URI << RESET_LINE;

	if ((*it)->isCGI)
	{
		cout << YELLOW << __func__ << " IS CGI" << RESET_LINE;
		file_stream = fopen((*it)->fileCGI.c_str(), "rb");
		file_str_2 = fopen((*it)->fileCGI.c_str(), "rb");
	}
	else if ((*it)->request.URI.compare("/") == 0)
	{
		//Root path for welcome page
		file_stream = fopen(DEFAULT_PATH , "rb");
		file_str_2 = fopen(DEFAULT_PATH , "rb");
	}
	else if ((*it)->request.URI.compare("/favicon.ico") == 0)
	// else if ((*it)->request.URI.compare("./favicon.ico") == 0)
	{
		//Favicon for now streamlined
		file_stream = fopen(FAV_ICON_PATH, "rb");
		file_str_2 = fopen(FAV_ICON_PATH, "rb");

	}
	else if ((*it)->request.URI.compare(0, 46, DEFAULT_CGI_FILE_PATH) == 0)
	{
		file_stream = fopen((*it)->request.URI.c_str(), "rb");
		file_str_2 = fopen((*it)->request.URI.c_str(), "rb");
	}
	else
	{
		//If there is a different file user wants to open
		file_stream = fopen(("." + (*it)->request.URI).c_str(), "rb");
		file_str_2 = fopen(("." + (*it)->request.URI).c_str(), "rb");
	}
	if(file_stream == nullptr)
	{
		//For errors
		cout << GREEN << "No file could be opened. Opening 404 instead.\n\n\n" << RESET_LINE;
		(*it)->response.status_code = "404";
		file_stream = fopen(ERROR_404_PATH, "rb");
		file_str_2 = fopen(ERROR_404_PATH, "rb");
		(*it)->request.URI = "/database/Error_404.png";
	}

	fseek(file_stream, 0, SEEK_END);
	(*it)->response.content_lenght = ftell(file_stream);

	rewind(file_stream);
	std::string binaryString;
	// this line is creating problems with small files
	std::string extension = get_extension_from_request_get(it);
	std::stringstream conv;
	conv << (*it)->response.content_lenght;
	// std::cout << RED << "Extension: " << extension << std::endl;
	// std::cout << "Reponse Content-Lenght == " << (*it)->response.content_lenght << std::endl;
	//Those values are sent in the header as a response
	(*it)->response.content_type = extension;
	(*it)->response.content_lenght_str = conv.str();
	create_response_and_send(it);
	(*it)->response.body_fd = fileno(file_str_2);

	// std::cout << (*it)->response.headers << std::endl;
	rewind(file_stream);
}
void	Server::responseHeader( std::vector<connecData*>::iterator it ,struct epoll_event	ev)
{
	cout << RED << __func__ << RESET_LINE;
	
	// parse and send header to client
	// open fd into the (*it)->response.body_fd for the body
	if ((*it)->request.URI.compare(0, strlen(CGI_FOLDER_PATH), CGI_FOLDER_PATH) == 0) //CGI
	{
		//check if path to script is legit maybe?
		cout << RED << __func__ << ": isCGI!" << RESET_LINE;
		//mark as CGI
		(*it)->isCGI = true;

		//prepare environment
		objectCGI.setEnvironment(it, servConfig);

		//create file name
		(*it)->fileCGI = DEFAULT_CGI_FILE_PATH + std::to_string((*it)->socket) + "_fileCGI.html";
		
		//create file to write to
		FILE * fileCGI= fopen((*it)->fileCGI.c_str(), "w");
		if (!file_exists((*it)->fileCGI))
		{
			cout << RED << __func__ << (*it)->fileCGI.c_str() << ": could not create file" << endl;
			exit(77);
		}

		//get file descriptor for dup
		int fdFileCGI = fileno(fileCGI);

		//give new file URI to the request struct
		(*it)->request.URI = (*it)->fileCGI;
		//fork
		pid_t pid = fork();

		//CHILD
		if (pid == 0)
		{
			// change output stream to the CGI file;
			dup2(fdFileCGI, 1);

			//make data doble array
			char **env = objectCGI.envToDoubleCharacterArray();
			
			//create path to executable

			// std::string executable = "/workspaces/webserv_42" + objectCGI.env.at("SCRIPT_NAME");
			std::string executable = PATH_TO_SCRIPTS + objectCGI.env.at("SCRIPT_NAME");
			// cout << PURPLE << executable << RESET_LINE;
			write(2, executable.c_str(), executable.length());
			//execute
			execve((executable).c_str(), NULL, env);
			// execve("../cgi-bin/put_photo_in_cat.py", NULL, env);
			perror("\nExecve: ");
			free(env);
			exit(33);
		}
		// cout << "response header get if" << endl;
		wait(NULL);
		handleGet(it);
		// executeCGI();
	}
	else if((*it)->request.method.compare("DELETE") == 0)
	{
		handle_delete(it, ev);
		endResponse(ev);
	}
	else if((*it)->request.method.compare("POST") == 0)
	{
		handle_post(it, ev);
	}else if((*it)->request.method.compare("GET") == 0){
		// cout << "response header get if" << endl;
		handleGet(it);
	}
	
}

void	Server::	endResponse( struct epoll_event ev )
{
	cout << RED << __func__ << RESET_LINE;
	
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	// cout << PURPLE << (*it)->response.body_fd << RESET_LINE;
	close((*it)->response.body_fd);		// fd to body of response
	delete (*it);
	connections.erase(it);
	epoll_ctl(epollFD, EPOLL_CTL_DEL, ev.data.fd, &ev);
	close(ev.data.fd);					// fd for the response socket
}

void	Server::confusedEpoll( struct epoll_event ev )
{
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	if ((*it)->finishedRequest && (*it)->finishedResponse)
		endResponse(ev);
	else if ((*it)->finishedRequest)
		endRequest(ev, it);
	cout << RED << "confusedEpoll i dont know what to do!" << RESET_LINE;	
}

void	Server::doResponseStuff( struct epoll_event ev )
{
	// cout << RED << __func__ << RESET_LINE;
	
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);
	char								sendBuffer[MAX_LINE];
	int									sendReturn;
	int									readReturn;
	
	if ((*it)->request.fd != 0) // process request body
	{
		// std::cout << (*it)->request.body << std::endl;
		// sendReturn = write((*it)->request.fd, (*it)->request.body_in_char, (*it)->request.content_size);

		if((*it)->request.content_size - MAX_LINE <= 0)
		{

			(*it)->request.content_size -= write((*it)->request.fd, (*it)->request.body.c_str() + (*it)->request.already_sent, (*it)->request.content_size);
			close((*it)->request.fd);
			endResponse(ev);
		}
		sendReturn = write((*it)->request.fd, (*it)->request.body.c_str() + (*it)->request.already_sent, MAX_LINE);
		(*it)->request.already_sent += sendReturn;
		(*it)->request.content_size -= sendReturn;
	
	}
	else
	{
		memset(sendBuffer, 0, MAX_LINE);
		// cout << "this is do response stuff else  with fd = " << (*it)->response.body_fd << endl;
		sendReturn = read((*it)->response.body_fd, sendBuffer, MAX_LINE);

		// cout << "sendreturn = " << sendReturn << endl <<  sendBuffer << endl << "end of buffer print" <<  endl;

		failTest(sendReturn = send((*it)->socket, sendBuffer, sendReturn, 0), "Sending fractional Response body");
		if(sendReturn < MAX_LINE)
		{
			if ((*it)->isCGI)
			{
				// cout << PURPLE << "NEED TO REMOVE THE FILE NOW" << RESET_LINE;
				remove((*it)->fileCGI.c_str());
			}
			endResponse(ev);
		}
	}
}

void	Server::doRequestStuff( struct epoll_event ev )
{
	cout << RED << __func__ << RESET_LINE;

	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);
	char								recBuffer[MAX_LINE];
	int									recReturn;


	memset(recBuffer, 0, MAX_LINE);	
	// fullRequest.clear();
	failTest(recReturn = recv(ev.data.fd , recBuffer, MAX_LINE, 0), "recReturn in do requeststuff");
	(*it)->request.raw.append(recBuffer, recReturn);
	// std::cout << "doing some request stuff" << endl;
	if (recReturn < MAX_LINE) 
	{
		// std::cout << "Raw Souce " << endl << (*it)->request.raw;
		
		// std::cout << "Body " << (*it)->request.body << " End of body"<<std::endl;
		endRequest(ev, it);
		responseHeader(it, ev);
	}
	// done reading close event and open new writing event
}

void	Server::acceptConnection( int epollFD )
{
	cout << RED << __func__ << RESET_LINE;
	
	connecData			*tmp = new connecData();
	struct epoll_event	ev;

	tmp->socket = accept(serverSocket, (SA *) NULL, NULL);
	failTest(tmp->socket, "accept new Socket");
	tmp->finishedResponse = false;
	tmp->finishedRequest = false;
	connections.push_back(tmp);
	ev = createEpollStruct(tmp->socket, EPOLLIN );
	epoll_ctl(epollFD, EPOLL_CTL_ADD, ev.data.fd, &ev);
}


void		Server::requestLoop( void )
{
	cout << RED << __func__ << RESET_LINE;
	
	struct epoll_event	ev;
	struct epoll_event	events[MAX_EVENTS];

	epollFD = epoll_create1(0);
	int					readyFDs;
	failTest(epollFD, "epoll_create1");

	ev = createEpollStruct(serverSocket, EPOLLIN);

	epoll_ctl(epollFD, EPOLL_CTL_ADD, ev.data.fd, &ev);

	// cout << "epollFD: " << epollFD << endl << "serverSocket: " << serverSocket << endl;
	fillInPossibleTypes();
	while(1)
	{
		// cout << "calling epoll_wait" << endl;
		readyFDs = epoll_wait(epollFD, events, MAX_EVENTS, -1);
		failTest(readyFDs, "epoll_wait");

		// cout << "epoll_wait call successful" << endl;
		for (int idx = 0; idx < readyFDs; idx++)
		{
			if (events[idx].data.fd == serverSocket)				//accepting a connection
			{
				// cout << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 1" << endl;
				acceptConnection(epollFD);
			}
			else if (events[idx].events & (EPOLLRDHUP | EPOLLHUP))	// check for end of connection
			{
				// cout << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 2" << endl;
				// closeAndRemoveFromEpoll(events[idx]);
				endResponse(events[idx]);
			}
			else if (events[idx].events & ( EPOLLOUT ))				// check for  write() fd
			{
				// cout << PURPLE << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 3" << endl;
				doResponseStuff(events[idx]);

			}
			else if (events[idx].events & ( EPOLLIN ))				// check for read() fd
			{
				// cout << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 4" << endl;
				doRequestStuff(events[idx]);
				// closeAndRemoveFromEpoll(events[idx], epollFD);  //testing purposes
			}
			else
			{
				// beConfused()
				cout << RED << "Why did we get here. FD: " << events[idx].data.fd << RESET_LINE;
			}
			// cout << "currConnections: " << connections.size() << endl;
			for (size_t i = 0; i < connections.size(); i++)
			{
				// cout << "ind: " << i << " socket: " << connections[i]->socket << endl;
			}
		}
	}
}