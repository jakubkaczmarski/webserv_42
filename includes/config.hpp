#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "webserv.hpp"

#define	confMapType	std::map<std::string, std::string>

class config
{
	private:
		confMapType		configMap;

		void			initDefaultConfig( void );
	public:
		config();
		config(char *confPath);
		~config();
		confMapType&	getConfigMap( void );
};

#endif