#include "Request.hpp"

static void	populatearr(char *arr, const char *str, int *counter)
{
	int	index;

	index = 0;
	while (str[index] != '\0')
	{
		arr[*counter] = str[index];
		*counter = *counter + 1;
		index++;
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		counter;
	int		index;
	char	*arr;

	if (!s1 || !s2)
		return (NULL);
	counter = 0;
	index = 0;
	while (s1[index++] != '\0')
		counter++;
	index = 0;
	while (s2[index++] != '\0')
		counter++;
	arr = (char *)malloc(sizeof(char) * (counter + 1));
	if (!arr)
		return (NULL);
	counter = 0;
	populatearr(arr, s1, &counter);
	populatearr(arr, s2, &counter);
	arr[counter] = '\0';
	return (arr);

}

Request::Request(std::string request, int pid)
{
    this->send_pid = pid;
    this->request_ = request;
    if(request.compare(0, 3 ,"GET") == 0)
    {
        std::cout << "This is a GET REQUEST " << std::endl;
        this->type_ = GET;
        execute_get_request();
    }else if(request.compare(0, 4, "POST") == 0)
    {
        std::cout << "This is a POST REQUEST" << std::endl;
        this->type_ = POST;
    }else if(request.compare(0, 6, "DELETE") == 0)
    {
        std::cout << "This is a DELETE request" << std::endl;
        this->type_ = DELETE;
    }else{
        std::cerr << "This is an unknown request ";
        exit(1);
    }
}

void Request::execute_get_request()
{
    
        char *reply = strdup( 
    "HTTP/1.1 200 OK\n"
    "Content-Type: text/html\n"
    "Content-Length: 100000\n"
    "Accept-Ranges: bytes\n"
    "Connection: close\n"
    "\n");
    int i = 4;
    std::string file_name;
    char *file_stuff = strdup("");
    while(request_[i] != ' ')
    {
        i++;
    }
    std::cout << i << std::endl;
    file_name.append(request_, 4, i - 4);
    if(file_name.length() == 0)
    {
        std::cerr << "Request is incorrect";
        exit(1);
    }
    FILE *fp = fopen(&((const char *)file_name.c_str())[1], "r");
    if (!fp)
    {
        std::cerr << "File doesn't exists";
    }
    fseek(fp, 0L, SEEK_END);
    long res = ftell(fp);
    std::cout << "Request for the file " <<  file_name << " " << res << std::endl;

    std::ifstream infile(&file_name[1]);
    for( std::string line; getline( infile, line ); )
    {
    //    std::cout << line << std::endl;
        file_stuff = ft_strjoin(file_stuff, line.c_str());
        file_stuff = ft_strjoin(file_stuff, "\n");
    }
    reply = ft_strjoin(reply, file_stuff);

    send(send_pid, reply, strlen(reply), 0);
    // int i = 0;
    // while()
}