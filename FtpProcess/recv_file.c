#include "head.h"

void recv_file(int client_fd)  //服务器接收文件
{
	data d;
	memset(&d,0,sizeof(d));
	recv(client_fd,&d.len,sizeof(int),0);   //读取文件名字长度
	recv(client_fd,d.buf,d.len,0);//读取文件名
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
		recv(client_fd,&d.len,sizeof(int),0);  //读取每次传输长度
		if(d.len > 0)
		{
			recv_n(client_fd,d.buf,d.len);  //写入结构体
			write(fd,d.buf,d.len);//写入文件
		}
		else
			break;
	}
	close(fd);
}	
void recv_n(int client_fd,char *buf,int len)
{
	int ret;
	int total = 0;
	while(total < len)
	{
		ret = recv(client_fd,buf+total,len-total,0);//加上total是偏移total个位置
		total = total +ret;
	}
}
