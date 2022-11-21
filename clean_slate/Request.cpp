#include "Request.hpp"


Request::Request(std::string request, int pid)
{
    this->send_pid = pid;
    this->request_ = request;
    if(request.compare(0, 3 ,"GET") == 0)
    {
        std::cout << "This is a GET REQUEST " << std::endl;
        this->type_ = GET;
        execute_get_request();
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

void Request::execute_get_request()
{
    int i = 4;
    std::string file_name;
    while(request_[i] != ' ')
    {
        i++;
    }
    std::cout << i << std::endl;
    file_name.append(request_, 4, i - 4);
    std::cout << "Request for the file " <<  file_name << std::endl;
    // int i = 0;
    // while()
}