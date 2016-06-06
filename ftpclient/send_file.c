#include "head.h"

void send_file(int fdw,char *filename)
{
	data d;
	memset(&d,0,sizeof(d));
	d.len = strlen(filename)-1;    //客户端发送文件名字,要去掉最后一个\n
//	printf("len=%d\n",d.len);
	strncpy(d.buf,filename,d.len);     //把要打开的名字复制到d.buf
	int ret = send(fdw,&d,4+d.len,0);         //发送文件名字
	if(-1 == ret)
	{
		perror("send1");
		exit(-1);
	}
	int fd = open(d.buf,O_RDONLY);   //这里要用d.buf 因为filename后面有\n
	if(-1 == ret)
	{
		perror("open");
		exit(-1);
	}
	while(memset(&d,0,sizeof(d)),(d.len = read(fd,d.buf,sizeof(d.buf))) > 0)
	{
		send(fdw,&d.len,sizeof(int),0);
		send_n(fdw,d.buf,d.len);
	}
	ret = 0;
	send(fdw,&ret,sizeof(int),0);//发送文件结束标志
	printf("send file success\n");
	close(fd);
}
void send_n(int fdw,char *buf, int len) //
{
	int total = 0;
	int ret = 0;
	while(total < len)
	{
		ret = send(fdw,buf+total,len-total,0);
		if(-1 == ret)
		{
			perror("send2");
			exit(-1);
		}
		total = total + ret;
	}
}


