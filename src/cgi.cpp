// #include "../includes/server.hpp"


// void CGI::setEnvironment(std::vector<connecData*>::iterator it, config servConfig)
// {
// 		if ((*it)->request.method.compare("GET") == 0)
// 		{
// 			env["QUERY_STRING"] = split((*it)->request.URI, '?')[1]; //gets everything after the question mark
// 			env["REQUEST_METHOD"] = "GET";
// 		}
// 		if ((*it)->request.method.compare("POST") == 0)
// 		{
// 			// objectCGI.env["QUERY_STRING"] = 
// 			env["REQUEST_METHOD"] = "POST";
			
// 		}
// 		env["SCRIPT_NAME"] = split((*it)->request.URI, '?')[0]; //The virtual path (e.g., /cgi-bin/program.pl) of the script being executed.
// 		env["SERVER_NAME"] = servConfig.getServName();
// 		env["SERVER_PORT"] = servConfig.getPort();
// 		env["SERVER_PROTOCOL"] = HTTPVERSION;
// }

// char **CGI::envToDoubleCharacterArray()
// {
// 	int envSize = env.size();
// 	char **environment = (char **)calloc(envSize + 1, sizeof(char *));

// 	if (environment == NULL)
// 	{
// 		exit(78);
// 	}
// 	;
// 	std::map<string, string>::iterator mapIter= env.begin();

// 	for(int i = 0; i < envSize; i++)
// 	{
// 		environment[i] = strdup((mapIter->first + "=" + mapIter->second).c_str());
// 		// cout << GREEN << environment[i] << RESET_LINE;
// 		mapIter++;
// 	}
// 	// cout << YELLOW << __func__ << "DO we ge here?" << RESET_LINE;

// 	return (environment);
// 	// environment[envSize + 1] = NULL;//not needed when using calloc?
// }


// // class 