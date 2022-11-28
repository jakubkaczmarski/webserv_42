#include "../includes/server.hpp"

// s_connecData	initConnectionStruct(int fd)
// {
// 	connecData	out;

// 	out.socket = fd;
// 	return (out);
// }

struct epoll_event	createEpollStruct(int fdSocket, uint32_t flags)
{
	struct	epoll_event	ev;

	ev.events = flags;
	ev.data.fd = fdSocket;

	return (ev);
}

std::vector<connecData*>::iterator		server::findStructVectorIt( struct epoll_event ev)
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

void	server::parseRequest( struct epoll_event ev )
{
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	fillRequestStruct(it);
}

void	server::endRequest( struct epoll_event ev, std::vector<connecData*>::iterator it )
{
	epoll_ctl(epollFD, EPOLL_CTL_DEL, ev.data.fd, &ev);
	parseRequest(ev);
	ev = createEpollStruct((*it)->socket, EPOLLOUT);
	epoll_ctl(epollFD, EPOLL_CTL_ADD, ev.data.fd, &ev);
	(*it)->finishedRequest = true;

	cout << RED << "endRequest and this is the uri: " << (*(findStructVectorIt(ev)))->request.URI << RESET_LINE;
	// connections.push_back(ev);

}

void	server::responseHeader( std::vector<connecData*>::iterator it )
{
	// parse and send header to client
	// open fd into the (*it)->response.body_fd for the body

	FILE	*file_stream;
	std::string def_path("./database/default_index.html");
	std::string fav_path("./database/favicon.ico");
	std::string err_path("./database/Error_404.png");
	std::cout << "Hereeeeeeeeeee" << std::endl;
	std::cout << (*it)->request.URI << std::endl;
	if((*it)->request.URI.compare("/") == 0)
	{
		//Root path for welcome page
		file_stream = fopen(def_path.c_str() , "rb");
	}else if((*it)->request.URI.compare("./favicon.ico") == 0)
	{
		//Favicon for now streamlined
		file_stream = fopen(fav_path.c_str(), "rb");
	}
	else{
		//If there is a different file user wants to open
		file_stream = fopen(("."+ (*it)->request.URI).c_str(), "rb");
	}
	if(file_stream == nullptr)
	{
		//For errors
		file_stream = fopen(err_path.c_str(), "rb");
		(*it)->request.URI = "/database/Error_404.png";

	}
	std::string binaryString;
	fseek(file_stream, 0, SEEK_END);
	std::string extension;
	int i = (*it)->request.URI.length() - 1;
	while((*it)->request.URI[i] && (*it)->request.URI[i] != '.')
	{
		i--;
	}
	if(!(*it)->request.URI[i])
	{
		//Default extension if there is none 
		extension = "text";
	}else{
		extension.append((*it)->request.URI.substr(i, (*it)->request.URI.length() - i));
	}
	int file_num = fileno(file_stream);
	std::stringstream conv;
	(*it)->response.content_lenght = ftell(file_stream);
	conv << (*it)->response.content_lenght; 
	
	(*it)->response.headers = "HTTP/1.1 200 OK\n";
	(*it)->response.headers.append("Content-Length: ");
	(*it)->response.headers.append(conv.str());
	(*it)->response.headers.append("\n");
	(*it)->response.headers.append("Content-Type: ");
	(*it)->response.headers.append(extension);
	(*it)->response.headers.append("\n\n");
	(*it)->response.body_fd = file_num;
	send((*it)->socket, (*it)->response.headers.c_str(), (*it)->response.headers.length(), 0);
	// (*it)->response.headers.append("Connection: Closed\n\n\n");
	std::cout << (*it)->response.headers << std::endl;
	std::cout << (*it)->response.body_fd << std::endl;
	rewind(file_stream);
	

	
}

void	server::endResponse( struct epoll_event ev )
{
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	// cout << PURPLE << (*it)->response.body_fd << RESET_LINE;
	close((*it)->response.body_fd);		// fd to body of response
	delete (*it);
	connections.erase(it);
	epoll_ctl(epollFD, EPOLL_CTL_DEL, ev.data.fd, &ev);
	close(ev.data.fd);					// fd for the response socket
}

void	server::confusedEpoll( struct epoll_event ev )
{
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);

	if ((*it)->finishedRequest && (*it)->finishedResponse)
		endResponse(ev);
	else if ((*it)->finishedRequest)
		endRequest(ev, it);
	cout << RED << "confusedEpoll i dont know what to do!" << RESET_LINE;	
}

// void	server::removeFromEpoll( struct epoll_event ev )
// {
// 	epoll_ctl(epollFD, EPOLL_CTL_DEL, ev.data.fd, &ev);

// }

// void	server::closeAndRemoveFromEpoll( struct epoll_event ev )
// {
// 	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);
// 	delete (*it);
// 	connections.erase(it);
// }

void	server::doResponseStuff( struct epoll_event ev )
{
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);
	char								sendBuffer[MAX_LINE];
	int									sendReturn;
	int									readReturn;

	sendReturn = read((*it)->response.body_fd, sendBuffer, MAX_LINE);

	failTest(sendReturn = send((*it)->socket, sendBuffer, sendReturn, 0), "Sending fractional Response body");
	
	if(sendReturn < MAX_LINE)
		endResponse(ev);
}

void	server::doRequestStuff( struct epoll_event ev )
{
	std::vector<connecData*>::iterator	it = findStructVectorIt(ev);
	char								recBuffer[MAX_LINE];
	int									recReturn;


	memset(recBuffer, 0, MAX_LINE);	
	// fullRequest.clear();
	failTest(recReturn = recv(ev.data.fd , recBuffer, MAX_LINE, 0), "recReturn in do requeststuff");
	(*it)->request.raw.append(recBuffer, recReturn);
	if (recReturn < MAX_LINE)
	{
		endRequest(ev, it);
		responseHeader(it);
	}
	// done reading close event and open new writing event
}




	// // if (it == connections.end())
	// if ((*it)->untouched)
	// {}	// read until start of body
	// else
	// {
	// }	// read the rest // if here we get to end of fd close epoll struct and open new one for writing


	// char buf[1000];
	// while(recv(ev.data.fd, buf, 999, 0))
	// {}



void	server::acceptConnection( int epollFD )
{
	connecData			*tmp = new connecData();
	struct epoll_event	ev;

	tmp->socket = accept(serverSocket, (SA *) NULL, NULL);
	failTest(tmp->socket, "accept new Socket");
	tmp->finishedResponse = false;
	tmp->finishedRequest = false;
	connections.push_back(tmp);
	// currConnections++;
	ev = createEpollStruct(tmp->socket, EPOLLIN );
	epoll_ctl(epollFD, EPOLL_CTL_ADD, ev.data.fd, &ev);
}


void		server::requestLoop( void )
{
	struct epoll_event	ev;
	struct epoll_event	events[MAX_EVENTS];

	epollFD = epoll_create1(0);
	int					readyFDs;
	failTest(epollFD, "epoll_create1");

	ev = createEpollStruct(serverSocket, EPOLLIN);

	epoll_ctl(epollFD, EPOLL_CTL_ADD, ev.data.fd, &ev);

	cout << "epollFD: " << epollFD << endl << "serverSocket: " << serverSocket << endl;

	while(1)
	{
		cout << "calling epoll_wait" << endl;
		readyFDs = epoll_wait(epollFD, events, MAX_EVENTS, -1);
		failTest(readyFDs, "epoll_wait");

		cout << "epoll_wait call successful" << endl;
		for (int idx = 0; idx < readyFDs; idx++)
		{
			if (events[idx].data.fd == serverSocket)				//accepting a connection
			{
				cout << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 1" << endl;
				acceptConnection(epollFD);
			}
			else if (events[idx].events & (EPOLLRDHUP | EPOLLHUP))	// check for end of connection
			{
				cout << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 2" << endl;
				// closeAndRemoveFromEpoll(events[idx]);
				endResponse(events[idx]);
			}
			else if (events[idx].events & ( EPOLLOUT ))				// check for  write() fd
			{
				cout << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 3" << endl;
				doResponseStuff(events[idx]);

			}
			else if (events[idx].events & ( EPOLLIN ))				// check for read() fd
			{
				cout << "IDX: " << idx << " socket: " << events[idx].data.fd << " case 4" << endl;

				doRequestStuff(events[idx]);
				// closeAndRemoveFromEpoll(events[idx], epollFD);  //testing purposes
			}
			else
			{
				// beConfused()
				cout << RED << "Why did we get here. FD: " << events[idx].data.fd << RESET_LINE;
			}
			cout << "currConnections: " << connections.size() << endl;
			for (size_t i = 0; i < connections.size(); i++)
			{
				cout << "ind: " << i << " socket: " << connections[i]->socket << endl;
			}
		}
	}

}