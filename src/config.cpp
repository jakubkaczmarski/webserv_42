#include "../includes/config.hpp"
#include <fstream> //for opening files
#include <signal.h>
config::config()
{
	initDefaultConfig();
}

config::config(char *confPath)
{
<<<<<<< HEAD
	// std::string		confPathStr = confPath;
=======
// 	// std::string		confPathStr = confPath;
>>>>>>> e2ecf6cee60b98c08d0e9d5c74fe742229ec86e3

	std::ifstream	confStream;
	std::string		workingLine;


// 	confStream.open(confPath);
}

config::~config()
{

}

confMapType&	config::getConfigMap( void )
{
	return (configMap);
} 

void	config::initDefaultConfig( void )
{
	configMap[SERVNAME] = "defaultServerName";
	configMap[HOST] = "0.0.0.0";
	configMap[PORT] = "1999";
	configMap[CLIENTMAXBODY] = "1000000";
	configMap[ERROR404] = "/database/Error_404.png";
	configMap[METHODS] = "GET";
	configMap[ROOT] = "/workspaces/webserv_42";
	configMap[DIR] = "/";
	configMap[UPLOADDIR] = "/database/uploads";
	// configMap[""] = "";
	// configMap[""] = "";
	// configMap[""] = "";
	// configMap[""] = "";
	// configMap[""] = "";
}

