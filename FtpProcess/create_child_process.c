#include "head.h"

void create_child(pchild p)
{
	int i;
	int fds[2];
	int ret;
	pid_t pid;
	for(i = 0; i < NUMCLIENT; i++)
	{
		ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);//每次fork前都要建立
		if(-1 == ret)
		{
			perror("socketpair");
			exit(-1);
		}
		pid = fork();                
		if(pid == 0)
		{
			close(fds[1]);
			child_handle(fds[0]);     //子进程的socketpair一端
		}
		close(fds[0]);
		p[i].fds = fds[1];  //把父进程的写端保存,全双工可读可写
		p[i].pid = pid;    //父进程返回子进程的pid保存
		p[i].busy = 0;      
	}
}

