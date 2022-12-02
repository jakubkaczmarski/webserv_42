#include "includes/webserv.hpp"

int main(void)
{
	string gg = "sadasfdasdfG4";
	std::vector<string> v = split(gg, 'G');
	int b = 0;
	cout << GREEN << "Size: " << v.size() << endl;
	for (auto i : v)
	{
		cout << YELLOW << b++ << ": " << RED << i << RESET_LINE;
	}
	return 0;
}