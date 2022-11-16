#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> split(std::string &str, const char sep)
{
	std::vector<std::string>	outputArray;
	std::stringstream			streamString(str);
	std::string					val;

	while(std::getline(streamString, val, sep))
		outputArray.push_back(val);
	return outputArray;
}

int main()
{
    std::string str = "GET /nothing HTTP/1.1";
    const char sep = ' ';
    std::vector<std::string> v = split(str, sep);

	// for (auto var : v)
	// {
	// 	cout << var << endl;
	// }
    return 0;
}