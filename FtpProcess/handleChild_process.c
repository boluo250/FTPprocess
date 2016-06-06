#include "head.h"

void child_handle(int fdr)
{
	int new_fd;
	int flag = 1;
	char buf[128]={0};
	char filename[20]={0};
	char *pname;
	int ret;
	char usr[11] = {0};
	int usrfd;
	char *stime;
	time_t begintime;
	time_t endtime;
	char linktime[30];
	while(1)
	{
		recv_fd(fdr,&new_fd);//接收父进程发送的链接描述符new_fd,处于阻塞状态
		memset(usr,0,sizeof(usr));
		ret = receivePassword(new_fd,usr);
		//拼接用户记录日志
		sprintf(usr,"%s.log",usr);
		printf("usr=%s\n",usr);
		//已追加方式打开日志文件
		usrfd = open(usr,O_RDWR|O_CREAT|O_APPEND,0666);
		if(-1 == usrfd)
		{
			perror("open");
		}
		stime = gettimenow();
		write(usrfd,"begin:",6);
		write(usrfd,stime,strlen(stime));
	//	printf("stime = %s",stime);
		if(-1 == ret)
		{
			printf("error password or usrname\n");
		}
		else
		{
			time(&begintime);
			ChildprocesswithClient(new_fd,usrfd);
			time(&endtime);
		}
		sprintf(linktime,"\nlinktime:%lds",(endtime-begintime));
		write(usrfd,linktime,strlen(linktime));
		stime = gettimenow();
		write(usrfd,"\nend:",5);
		write(usrfd,stime,strlen(stime));
		close(new_fd);
		close(usrfd);
		write(fdr,&flag,sizeof(flag));//子进程向父进程发送空闲通知
	}
	return;
}
void ChildprocesswithClient(int new_fd,int usrfd)
{
	int flag = 1;
	char buf[128]={0};
	char handlebuf[20]={0};
	char filename[20]={0};
	char *pname;
	int ret;
	clock_t begintime;
	clock_t endtime;
	clock_t alltime=0;
	char operatetime[30];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		memset(filename,0,sizeof(filename));
		//sleep(3);
		ret = recv(new_fd,buf,sizeof(buf),0);
//		begintime = clock();
		sprintf(handlebuf,"%s  ",buf);
		write(usrfd,handlebuf,strlen(handlebuf));
		if(ret <= 0)
		{
			printf("new_fd=%d exit\n",new_fd);
			break;
		}
		else
		{
			if(strncmp("pwd",buf,3) == 0)   //获取当前路径
			{
				strcpy(buf,getcwd(NULL,0));
				//printf("buf=%s\n",buf);
				send(new_fd,buf,strlen(buf),0);
			}
			else if(strncmp("ls",buf,2) == 0) //获取当前目录下的文件信息
			{
				send_directory(new_fd,getcwd(NULL,0));				
			}
			else if(strncmp("remove",buf,6) == 0) //删除文件
			{
				strcpy(filename,buf+6);
				pname = filename;
				while(*pname == ' ') pname++;
				//printf("pname=%s\n",pname);
				if(unlink(pname)< 0)
				{
					send(new_fd,"remove failed",13,0);
				}
				else
				{
					send(new_fd,"remove success",14,0);
				}
			}
			else if(strncmp("gets",buf,4) == 0)
			{
				strcpy(filename,buf+4);
				pname = filename;
				while(*pname == ' ') pname++;
				if(is_File_exist(pname,getcwd(NULL,0)) == -1)
				{
					send(new_fd,"0",1,0);//0标志file不存在
				}
				else
				{
					struct stat s;
					stat(pname,&s);
					printf("size=%ld\n",s.st_size);
					if(s.st_size > FILESIZE)//文件大于100M用MMAP
					{
						send(new_fd,"2",1,0);
						mmapsendfile(new_fd,pname);
					}
					else
					{
						send(new_fd,"1",1,0);
						send_file(new_fd,pname);
					}
				}
			}
			else if(strncmp("puts",buf,4) == 0)
			{
				recv_file(new_fd);
			}
			else if(strncmp("cd",buf,2) == 0)
			{
				strcpy(filename,buf+2);
				pname = filename;
				while(*pname == ' ') pname++;
			//	printf("pname=%s\n",pname);
				DIR *dir = opendir(pname);
				if(NULL == dir)
				{
					perror("opendir");
				}
				chdir(pname);
				strcpy(buf,getcwd(NULL,0));
				//printf("buf=%s\n",buf);
				send(new_fd,buf,strlen(buf),0);
			}	
		}
//		begintime = clock();
//		alltime += endtime - begintime;
	}
//	printf("alltime:%f\n",(double)alltime/CLOCKS_PER_SEC);
 //   sprintf(operatetime,"\noperatetime:%fs",alltime);
//	printf("operatetime:%s\n",operatetime);
//	write(usrfd,operatetime,strlen(operatetime));
	return;
}
	
