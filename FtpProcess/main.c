#include "head.h"

//使用多进程，实现多个客户端通信
int main()
{
	pchild p =(pchild)calloc(NUMCLIENT,sizeof(child));//动态分配内存，初始化为零
	create_child(p);
	int sfd = tcp_init();
	int epfd = epoll_create(1);
	struct epoll_event* evs = (struct epoll_event*)calloc(NUMCLIENT+1,sizeof(struct epoll_event));
	epfd_add(sfd,epfd);
	int i;
	for(i = 0; i < NUMCLIENT; i++)  //子进程注册epolll
	{
		epfd_add(p[i].fds,epfd);//socketpair的另一端注册,父进程获取相应子进程的状态
	}
	int ret;
	int new_fd;
	int j;
	int flag;
	while(1)
	{
		memset(evs,0,(NUMCLIENT+1)*sizeof(struct epoll_event));
		ret = epoll_wait(epfd,evs,NUMCLIENT+1,-1);
		if(ret > 0)
		{
			for(i = 0; i < ret; i++)
			{
				if(evs[i].data.fd == sfd)  //监听客户端请求
				{
					new_fd = tcp_accept(sfd); //连接客户端
					for(j = 0; j < NUMCLIENT; j++) //寻找空闲的子进程
					{
						if(p[j].busy == 0)
						 	break;
					}
					send_fd(p[j].fds,new_fd); //向子进程发送fd
					p[j].busy = 1;
				}
				for(j = 0; j <  NUMCLIENT; j++)//判断子进程是否空闲
				{
					if(evs[j].data.fd == p[j].fds) //当对应子进程fds可读时，子进程不忙碌
					{
						read(p[j].fds,&flag,sizeof(flag));
						p[j].busy =0;
					}
				}
			}
		}
	}
}
