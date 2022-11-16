#ifndef WERBSERV_HPP
#define WERBSERV_HPP

#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>

#include <unistd.h>


#include "colours.h"


using std::cout;
using std::cerr;
using std::endl;
using std::strerror;
using std::string;
using std::stringstream;

#define PORT_NBR 60000 
#define MAX_LINE 4096
#define SA struct sockaddr
#define SOCKET_BACKLOG 1024


std::vector<std::string> split(std::string &str, const char sep, const int &flag=0);
#endif