#include "../includes/webserv.hpp"

bool file_exists (const std::string& name)
{
  struct stat buffer;   
  return (stat(name.c_str(), &buffer) == 0); 
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	out;

	i = 0;
	sign = 1;
	out = 0;
	while (nptr[i] == 32 || (nptr[i] > 8 && nptr[i] < 14))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		sign = (nptr[i++] - 44) * -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
		out = (out * 10) + (nptr[i++] - '0');
	return (out * sign);
}
