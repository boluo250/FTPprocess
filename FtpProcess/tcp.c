#include "head.h"

void epfd_del(int fd,int epfd)
{
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = fd;
	int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		exit(-1);
	}
	close(fd);
}
 	
int tcp_accept(int sfd)
{
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	int addrlen = sizeof(clientaddr);
	int new_fd = accept(sfd,(struct sockaddr*)&clientaddr,&addrlen);
	if(-1 == new_fd)
	{
		perror("accept");
		return -1;
	}
	printf("new_fd= %d\n",new_fd);
	return new_fd;
}
void epfd_add(int fd,int epfd)
{
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = fd;
	int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		exit(-1);
	}
}
int  tcp_init()
{
 	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
	{
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in myaddr;
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(SERVERPORTID);
	myaddr.sin_addr.s_addr = inet_addr(SERVERIPADRR);
	int ret = bind(sfd,(struct sockaddr*)&myaddr,sizeof(myaddr));
	if(-1 == ret)
	{
		perror("bind");
		close(sfd);
		exit(-1);
	}
	ret = listen(sfd,NUMCLIENT);
	if(-1 == ret)
	{
		perror("listen");
		close(sfd);
		exit(-1);
	}
	printf("listen success!\n");
	return sfd;
}
