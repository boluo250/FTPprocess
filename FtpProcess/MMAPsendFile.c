#include "head.h"

void mmapsendfile(int new_fd,char *fname)
{
	int fd = open(fname,O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return;
	}
	struct stat s;
	fstat(fd,&s);
	char *p,*q;
	p =mmap(NULL,s.st_size,PROT_READ,MAP_SHARED,fd,0);
	if((char*)-1 == p)
	{
		perror("mmap");
		return;
	}
	data d;
	d.len = strlen(fname);
	strcpy(d.buf,fname);
	send(new_fd,&d,4+d.len,0);
	send(new_fd,&s.st_size,sizeof(int),0);
	send_n(new_fd,p,s.st_size);
/*	p = q;
	int sumsize = s.st_size;
	data d;
	memset(&d,0,sizeof(d));
	int sumdbuf = sizeof(d.buf);
	printf("sumdbuf=%d sumsize=%d\n",sumdbuf,sumsize);
	while(1)
	{
		memset(&d,0,sizeof(d));
		if(sumsize < sumdbuf)
		{
			strncpy(d.buf,p,sumsize);
			d.len = sumsize;
			send(new_fd,&d.len,sizeof(int),0);
			send_n(new_fd,d.buf,d.len);
			break;
		}
		else
		{
			strncpy(d.buf,p,sumdbuf);
			d.len = sumdbuf;
			send(new_fd,&d.len,sizeof(int),0);
			send_n(new_fd,d.buf,d.len);
			p = p+sumdbuf;
			sumsize = sumsize-sumdbuf;
		}
	}
	*/
//	int ret = 0;
//	send(new_fd,&ret,4,0);
	munmap(p,s.st_size);
	close(fd);
}
