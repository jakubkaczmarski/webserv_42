#define SERVER_HPP
#ifdef SERVER_HPP
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
//Socket init 
class Server
{
    public:
        Server(std::string name);
        Server();
        // int listen(int port);
        void run(int port);
        void process_request(int socket_num);
        //
    private:
        int port;
        std::string host_name;
        int sock_fd;
        int newsock_fd;
        int pid;
        socklen_t clilen;
        struct sockaddr_in serv_addr;
        struct sockaddr_in cli_address;
};


#endif