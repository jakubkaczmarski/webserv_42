#ifndef REQUEST_HPP
#define REQUEST_HPP

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
        Request(std::string request);
    private:
        int type_;
        std::string path_;
        std::string request_;
};



#endif