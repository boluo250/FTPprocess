#include "head.h"

void mmaprecefile(int sfd)
{
	data d;
	int lenth;
	memset(&d,0,sizeof(d));
	recv(sfd,&d.len,4,0);
	recv(sfd,d.buf,d.len,0);
	recv(sfd,&lenth,4,0);
	int fd;
	fd = open(d.buf,O_RDWR|O_CREAT,0666);
	if(-1 == fd)
	{
		perror("open");
		return;
	}
	int ret = ftruncate(fd,lenth);
	if(-1 == fd)
	{
		perror("ftruncate");
		return;
	}
	char *p = mmap(NULL,lenth,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if((char*)-1 == p)
	{
		perror("mmap");
		return;
	}
	printf("lenth=%d\n",lenth);
	recv_n(sfd,p,lenth);
	printf("recv success\n");
	munmap(p,lenth);
	close(fd);
}


