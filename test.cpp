#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string>
#include <string.h>
int main(int argc, char **argv, char **env)
{
	// int pid = fork();
	// if(pid == 0)
	// {
	// 	int fd = open("out.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	// 	dup2(fd, 1);
	// 	dup2(fd, 2);
	// 	// close(fd);
	// 	char *arr[5];
	// 	arr[0] = strdup("/usr/local/bin/python3");
	// 	arr[1] =  strdup("test.py");
	// 	arr[2] = NULL;
	// 	execve(arr[0], arr, 0);
	// }else{
	// 	waitpid(pid, 0 , 0);
	// }
	std::string name = "Siemanko ZIomeczku";
    FILE *ptr = fopen("notes.txt", "rb");
	int pd = fileno(ptr);
	char buff[4000];
	memset(buff, 0, 4000);
	
    fclose(ptr);
	char *s = (char *)name.c_str();
}
