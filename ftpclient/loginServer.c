#include "head.h"

void loginServer(int sfd)
{
	char usrname[11];
	char *pwd;
	char buf[20]={0};
	int flag;
	printf("username:");
	scanf("%10s",usrname);
	pwd=getpass("password:");
	sprintf(buf,"%s %s",usrname,pwd);
//	printf("buf=%s\n",buf);
	send(sfd,buf,strlen(buf),0);
	recv(sfd,&flag,sizeof(int),0);
	if(flag == -1)
	{
		printf("error password or usrname\n");
		exit(-1);
	}
	else
	{
		printf("loginServer success\n");
		return;
	}
}
