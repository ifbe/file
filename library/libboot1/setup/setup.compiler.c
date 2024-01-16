#include "libboot.h"
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);




int compiler_fromfile(u8* str, int len)
{
	u8 buf[0x2000];
	len = openreadclose(str, 0, buf, 0x2000);
	if(len <= 0)return 0;

	int j,k = 0;
	for(j=0;j<=len;j++){
		if((j==len)|('\n' == buf[j])){
			logtoall("%.*s\n", j-k, buf+k);
			k = j+1;
		}
	}
	return 1;
}
int compiler_create(struct item* wrk, void* arg, int argc, u8** argv)
{
	int j;
	if(0 == argv)return 0;
	else if(argc <= 1)return 0;

	for(j=1;j<argc;j++){
		logtoall("arg[%d]=%s\n", j, argv[j]);
		compiler_fromfile(argv[j], 0);
	}

	return 0;
}




void compiler_init()
{
}
void compiler_exit()
{
}
