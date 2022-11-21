#include "../includes/config.hpp"
#include <fstream> //for opening files

config::config()
{
	initDefaultConfig();
}

config::config(char *confPath)
{
	// std::string		confPathStr = confPath;

	std::ifstream	confStream;
	std::string		workingLine;


	confStream.open(confPath);
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
	// configMap[""] = "";
	// configMap[""] = "";
	// configMap[""] = "";
	// configMap[""] = "";
	// configMap[""] = "";
}

