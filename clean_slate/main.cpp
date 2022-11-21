


//Okay the process
// 1. Program takes a config file as an argument
// This conifg file is pretty much 
// a setup so where should i look for files that are requested
// where to listen for request and what's the name of the server
// It also takes a path where we can find CGI binaries
// Our web browser is a client and it sends requests
// We need to parse the html files
// Which we will execute and they will send requests to our server (pretty much just printing)
// We have to send back the response
// thanks to this we should be able to see the response in our web browser :)

#include "Server.hpp"
#include "Client.hpp"
#include <fstream>
#include <utility>
#include <map>


//Cgi file parser
//
typedef struct s_server_config
{
    //int thread_count;
    //location
    int port;
    std::string name;
    // std::string root_path;
    // std::map<int, std::pair<std::string, std::string> >credentials;
    // std::string err_page_url;
    // std::string upload_dir;
    // bool autoindex; // on by default
    // std::string index;
} server_config;

int main(int argc, char **argv)
{
    //
    Server serv1;
    serv1.run(3225);
}
