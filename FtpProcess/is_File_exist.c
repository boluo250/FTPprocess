#include "head.h"

int is_File_exist(char *filename,char *pathname)
{
	DIR *dir;
	dir = opendir(pathname);
	char buf[1024] = {0};
	if(NULL == dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	int ret;
	while((p = readdir(dir)) != NULL)
	{
		if(strcmp(p->d_name,filename) == 0)
			return 0;
	}
	closedir(dir);
	return -1;
}
	
