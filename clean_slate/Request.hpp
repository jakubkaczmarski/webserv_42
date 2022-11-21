#define REQUEST_HPP
#ifdef REQUEST_HPP
#include <sys/socket.h>
#include <fstream>
#include <string>
#include <iostream>
//Each request needs to be either of type GET | POST | DELETE
//PATH IT POINTS TO
//
enum req_type
{
    GET,
    POST,
    DELETE
};
class Request
{
    public:
        Request(std::string request, int pid);
        void execute_get_request();
    private:
        int type_;
        std::string path_;
        std::string request_;
        int send_pid;
};



#endif