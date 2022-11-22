// #include <iostream>
// #include <sstream>
// #include <vector>
// #include <string>

#include <iostream>
#include <sstream>
#include <vector>

#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>

#include <unistd.h>

typedef struct t_request
{
	std::string								method;
	std::string								URI;
	std::string								httpVers;
	std::map<std::string, std::string>		headers;
}	s_request;

using std::cout;
using std::cerr;
using std::endl;
using std::strerror;
using std::string;
using std::stringstream;


std::vector<std::string> split(std::string &str, const char sep, const int &flag=0)
{
	std::vector<std::string>	outputArray;
	std::stringstream			streamString(str);
	std::string					val;
	std::string					temp;
	int 						i;
	
	i = 0;
	while(std::getline(streamString, val, sep))
	{
		if (flag == 1 && i == 1)
		{
			int start = outputArray[0].length() + 2;
			outputArray.push_back(str.substr(start, str.length() - start));
			break ;
		}
		if(val.length() > 0)
			outputArray.push_back(val);
		i++;
	}
	return outputArray;
}

// int main()
// {
//     std::string strData = "Host: localhost:60000\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:105.0) Gecko/20100101 Firefox/105.0\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\nAccept-Language: en-GB,en;q=0.5\nAccept-Encoding: gzip, deflate, br\nDNT: 1\nConnection: keep-alive\nUpgrade-Insecure-Requests: 1\nSec-Fetch-Dest: document\nSec-Fetch-Mode: navigate\nSec-Fetch-Site: cross-site\n\n";
//     const char separator = '\n';
//     s_request TEST;
// 	std::vector<std::string> v = split(strData, separator);
// 	std::vector<std::string> key_value;

// 	TEST.method = v[0];
// 	TEST.URI = v[1];
// 	TEST.httpVers = v[2];
// 	for (int i = 0; i < v.size() - 1; i ++)
// 	{
// 		// cout << i << " " << v[i] << endl << endl;


// 		key_value = split(v[i], ':', 1);

// 		cout << i << " Key: " << key_value[0] << "\nValue: " << key_value[1] << endl;

// 	}
// 	// cout << TEST.method << TEST.URI << TEST.httpVers << endl;
//     return 0;
// }