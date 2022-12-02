#ifndef CGI_HPP_42
#define CGI_HPP_42

#include "server.hpp"
// #include <fstream>
// #include "config.hpp"



class CGI
{
	public:
		std::map<string, string>	env;
		string						fileCGI;

		void setEnvironment(std::vector<connecData*>::iterator it, config servConfig);
		// void executeCGI();
		char **envToDoubleCharacterArray();
	
	// CGI(){};
};





#endif //CGI_HPP_42