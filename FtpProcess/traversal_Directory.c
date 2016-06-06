#include "head.h"

void send_directory(int fd,char *curdir)
{
	DIR *dir;
	dir = opendir(curdir);
	char buf[1024] = {0};
	if(NULL == dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	struct stat statbuf;
	int ret;
	char c;
	while((p = readdir(dir)) != NULL)
	{
		ret = stat(p->d_name,&statbuf);
		if(-1 == ret)
		{
			perror("stat");
			return;
		}
		if(strcmp(p->d_name,".") && strcmp(p->d_name,".."))
		{
			if(S_ISDIR(statbuf.st_mode))
				c = 'd';
			else
				c = '_';
			sprintf(buf,"%s%c %s %ld\n",buf,c,p->d_name,statbuf.st_size);
		}
	}
//	printf("%s",buf);
	send(fd,buf,strlen(buf)-1,0);
	closedir(dir);
}

