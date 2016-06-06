#include "head.h"

void send_file(int fdw,char *filename)
{
	data d;
	memset(&d,0,sizeof(d));
	d.len = strlen(filename);
	strcpy(d.buf,filename);
	int ret = send(fdw,&d,4+d.len,0);         //发送文件名
	if(-1 == ret)
	{
		perror("send1");
		exit(-1);
	}
	int fd = open(filename,O_RDONLY);
	if(-1 == ret)
	{
		perror("open");
		exit(-1);
	}
	//read读取成功返回读取的字节数,到结尾返回0,文件读取位随着读取的字节数偏移
	while(memset(&d,0,sizeof(d)),(d.len = read(fd,d.buf,sizeof(d.buf))) > 0)
	{
		send(fdw,&d.len,sizeof(int),0);
		send_n(fdw,d.buf,d.len);
	}
	ret = 0;
	send(fdw,&ret,sizeof(int),0);//发送文件结束标志
	close(fd);
}
void send_n(int fdw,char *buf, int len) //
{
	int total = 0;
	int ret = 0;
	while(total < len)
	{
		//send返回成功发送的字节数,只是发送到协议缓冲区里,不保障客户端接收成功
		ret = send(fdw,buf+total,len-total,0);
		//由于网络差异,缓冲区有数据,不能发送len长度的字节,需要循环发送 ,直到对方接收len长度字节为止
		if(-1 == ret)
		{
			perror("send2");
			exit(-1);
		}
		total = total + ret;//发送的总字节数
	}
}


