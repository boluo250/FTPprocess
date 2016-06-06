#include "head.h"

void get_salt(char *salt,char *passwd)
{
	int i,j;
	//salt,i记录密码字符下标,j记录$出现次数
	for(i=0,j=0;passwd[i] && j != 3;++i)
	{
		if(passwd[i] == '$')
		++j;
	}
	strncpy(salt,passwd,i-1);
}
int confirmPassword(char *usrname,char *passwd)
{
	struct spwd *sp;
	char salt[512]={0};
	if((sp=getspnam(usrname)) == NULL)
		return -1;
	//得到salt,用得到的密码作参数
	get_salt(salt,sp->sp_pwdp);
//	printf("sp_pwdp=%s\n",sp->sp_pwdp);
//	printf("crtpy=%s\n",(char*)crypt(passwd,salt));
	//进行密码验证
	if(strcmp(sp->sp_pwdp,(char*)crypt(passwd,salt)) == 0)
		return 1;
	else
		return -1;
	return 0;
}
