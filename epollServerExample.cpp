#include <stdio.h>     // for fprintf()
#include <unistd.h>    // for close(), read()
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h>    // for strncmp

#define MAX_EVENTS 5
#define READ_SIZE 10

int main(void)
{
	bool running = true;
	int eventCount;
	int i;
	size_t bytesRead;

	char readBuffer[READ_SIZE + 1];
	
	struct epoll_event	event;
	struct epoll_event	events[MAX_EVENTS];

	//fd for a new epoll instance
	int epollFD = epoll_create1(0);

	if (epollFD == - 1)
	{
		fprintf(stderr, " Failed to create epoll file descriptor\n");
		return 1;
	}

	event.events = EPOLLIN;
	event.data.fd = 0;

	if (epoll_ctl(epollFD, EPOLL_CTL_ADD, 0, &event))
	{
		fprintf(stderr, "Failed to add file descriptor to epoll\n");
		close(epollFD);
		return (2);
	}

	while(running)
	{
		printf("Polling for input...\n");
		eventCount = epoll_wait(epollFD, events, MAX_EVENTS, 300000);
		printf("%d ready events\n", eventCount);
		for(i = 0; i < eventCount; i++)
		{
			printf("Reading file descriptor '%d' -- ", events[i].data.fd);
			bytesRead = read(events[i].data.fd, readBuffer, READ_SIZE);
			printf("%zd bytes read.\n", bytesRead);
			readBuffer[bytesRead] = '\0';
			printf("Read '%s'\n", readBuffer);

			if (!strncmp(readBuffer, "stop\n", 5))
				running = false;
		}
	}





	if (close(epollFD))
	{
		fprintf(stderr, "Failed to close epoll file decriptor\n");
		return 3;
	}

	return (0);
}