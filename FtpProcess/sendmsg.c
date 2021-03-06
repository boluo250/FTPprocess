#include "head.h"

void send_fd(int sockfd,int fd)
{
	char buf[10] = "hello";
	char buf1[10] = "world";
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	iov[0].iov_base = buf;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf1;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg) = fd;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = sendmsg(sockfd,&msg,0);
	if(-1 == ret)
	{
		perror("sendmsg");
		return;
	}
}
void recv_fd(int sockfd,int *fd)
{
	char buf[10] = {0};
	char buf1[10] = {0};
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	iov[0].iov_base = buf;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf1;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;
	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = recvmsg(sockfd,&msg,0);
	if(-1 == ret)
	{
		perror("sendmsg");
		return;
	}
	*fd = *(int*)CMSG_DATA(cmsg);
}	
