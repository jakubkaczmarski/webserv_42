#include "Request.hpp"


Request::Request(std::string request)
{
    if(request.compare(0, 3 ,"GET") == 0)
    {
        std::cout << "This is a GET REQUEST " << std::endl;
        this->type_ = GET;
    }else if(request.compare(0, 4, "POST") == 0)
    {
        std::cout << "This is a POST REQUEST" << std::endl;
        this->type_ = POST;
    }else if(request.compare(0, 6, "DELETE") == 0)
    {
        std::cout << "This is a DELETE request" << std::endl;
        this->type_ = DELETE;
    }else{
        std::cerr << "This is an unknown request ";
        exit(1);
    }
}