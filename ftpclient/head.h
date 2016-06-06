#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/mman.h>
#define SERVERPORTID 2122    
#define SERVERIPADRR "192.168.65.129"
#define NUMCLIENT 5
//#define FILENAME  "file.txt"  //自定义文件名字


typedef struct {           //发送数据的结构体
	int len;               //发送len+strlen(buf)
	char buf[1024];        // 结束发送len = 0
}data,*pdata;

void epfd_add(int fd,int epfd);  //增加fd到epoll
void epfd_del(int fd,int epfd);
int tcp_connect();              //客户端连接
void recv_n(int,char*,int);   //循环读取
void recv_file(int );  //客户端接收文件
void send_file(int ,char *);
void send_n(int ,char *, int); 
int is_File_exist(char *,char *); //判断当前目录下文件是否存在
void loginServer(int sfd);//登陆服务器验证
void mmaprecefile(int ); //mmap接收

#endif
