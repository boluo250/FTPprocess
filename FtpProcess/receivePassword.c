#include "head.h"

int receivePassword(int new_fd,char *usr)
{
	char buf[20]={0};
	char usrname[11];
	char pwd[7];
	char *s;
	int flag;
	int i = 0;
	recv(new_fd,buf,sizeof(buf),0);
	s = buf;
	while(*s != ' ')
	{
		usrname[i++] = *s;
		s++;
	}
	usrname[i] = '\0';
	strcpy(usr,usrname);
	s++;
	i = 0;
	while(*s != '\0')
	{
		pwd[i++] = *s;
		s++;
	}
	pwd[i] = '\0';
//	printf("name=%s pwd=%s\n",usrname,pwd);
	int ret = confirmPassword(usrname,pwd);
	if(-1 == ret)
	{
		flag = -1;
		send(new_fd,&flag,sizeof(int),0);
		return -1;
	}
	else
	{
		flag = 1;
		send(new_fd,&flag,sizeof(int),0);
		return 1;
	}
}

