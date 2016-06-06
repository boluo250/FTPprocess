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
#include <shadow.h>
#include <errno.h>
#include <sys/mman.h>
#include<time.h>
#define SERVERPORTID 2122    
#define SERVERIPADRR "192.168.65.129"
#define NUMCLIENT 5
#define FILESIZE 104857600 
typedef struct {
	pid_t pid;
//	int new_fd;   //自行选择
	int fds;       //sockpair管道的另一端,与父进程通信用,sockedpair是全双工的所以每一端都可以读写
	int busy;      //设置和判断子进程是否忙碌
}child,*pchild;

typedef struct {           //发送数据的结构体
	int len;               //发送len+strlen(buf)
	char buf[1000];        // 结束发送len = 0
}data,*pdata;

int tcp_init();      //tcp初始化
void epfd_add(int fd,int epfd);  //增加fd到epoll
int tcp_accept(int sfd);         //连接到客户端
void epfd_del(int fd,int epfd);
int tcp_connect();              //客户端连接
void create_child(pchild);    //创建NUM个子进程
void child_handle(int );        //子进程传送
void ChildprocesswithClient(int ,int); //子进程处理客户端请求
void send_fd(int sockfd,int fd);//父进程传递描述符给子进程
void recv_fd(int sockfd,int *fd);//子进程接收fd，recvmsg处于阻塞模式sleep
void send_n(int,char*,int);   //循环发送 
void recv_n(int,char*,int);   //循环读取
void recv_file(int );  //服务器接收文件
void send_file(int,char*);            //文件传送协议（自定）
void send_directory(int ,char *);  //发送当前目录下的文件名字
int is_File_exist(char *,char *); //判断当前目录下文件是否存在
int receivePassword(int,char*);   //获取客户端账号密码
int confirmPassword(char*,char *);//验证客户端账号密码root -lcrypt
void get_salt(char *,char *);      //
void mmapsendfile(int new_fd,char *fname); //mmap映射发送文件
char * gettimenow();  //获取现在时间
#endif
