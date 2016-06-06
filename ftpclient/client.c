#include "head.h"

void main()
{
	int sfd = tcp_connect();
	int epfd = epoll_create(1);
	struct epoll_event evs[2];
	memset(evs,0,sizeof(struct epoll_event));
	epfd_add(sfd,epfd);  
	epfd_add(0,epfd);                        //注册epoll
	char buf[1024];
	char filename[20] = {0};
	char *pname;
	int ret;
	int n;
	int i;
	loginServer(sfd);
	while(1)
	{
		memset(evs,0,sizeof(struct epoll_event));
		n = epoll_wait(epfd,evs,2,-1);
		if(n > 0)
		{
			for(i = 0; i < n; i++)
			{
				if(evs[i].data.fd == 0)
				{
					memset(buf,0,sizeof(buf));
					ret = read(0,buf,sizeof(buf));
					if(ret == 0)//ctrl+d
					{
						printf("bye\n");
						close(sfd);
						exit(0);
					}
					send(sfd,buf,strlen(buf)-1,0);
					if(strncmp("puts",buf,4) == 0)
					{
						memset(filename,0,sizeof(filename));
						strcpy(filename,buf+4);
						pname = filename;
						while(*pname == ' ') pname++;
						send_file(sfd,pname);
					}
				}
				if(evs[i].data.fd == sfd)
				{
					if(strncmp("gets",buf,4) == 0)
					{
						memset(buf,0,sizeof(buf));
						recv(sfd,buf,1,0);
						if(buf[0] == '1')
						{
							 recv_file(sfd);
						}
						else if(buf[0] == '2')
						{
							 mmaprecefile(sfd);
						}
						else
							printf("file not exist\n");
					}
					else
					{
						memset(buf,0,sizeof(buf));
						ret =recv(sfd,buf,sizeof(buf),0);
						if(ret == 0)
						{
							printf("bye sfd\n");
							close(sfd);
							exit(-1);
						}
						system("clear");
						printf("%s\n",buf);
					}
				}
			}
		}
	}
	close(sfd);
	return;
}
int tcp_connect()
{
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd)
	{
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORTID);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIPADRR);
	int ret = connect(sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(ret == -1)
	{
		perror("connect");
		close(sfd);
		exit(-1);
	}
	return sfd;
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
