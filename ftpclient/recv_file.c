#include "head.h"

void recv_file(int sfd)  //客户端接收文件
{
	data d;
	memset(&d,0,sizeof(d));
	recv(sfd,&d.len,sizeof(int),0);   //读取文件名字长度
	recv(sfd,d.buf,d.len,0);//读取文件名
	int fd;
	fd = open(d.buf,O_RDWR|O_CREAT,0666);
	if(-1 == fd)
	{
		perror("open");
		return;
	}
	while(1)
	{
		memset(&d,0,sizeof(d));
		recv(sfd,&d.len,sizeof(int),0);  //读取每次传输长度
		if(d.len > 0)
		{
			recv_n(sfd,d.buf,d.len);  //写入结构体
			write(fd,d.buf,d.len);//写入文件
		}
		else
			break;
	}
	close(fd);
	printf("file receive success\n");
}	

void recv_n(int new_fd,char *buf,int len)
{
	int ret;
	int total = 0;
	while(total < len)
	{
		ret = recv(new_fd,buf+total,len-total,0);//加上total是偏移total个位置
		total = total +ret;
	}
}
