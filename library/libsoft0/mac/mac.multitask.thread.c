#include <pthread.h>
#include "libsoft.h"




u64 threadcreate(void* func, void* args)
{
	unsigned long long id;
	pthread_create((void*)&id, NULL, func, args);
	return id;
}
void threaddelete(u64 h)
{
}
int tasksearch(void* buf, int len)
{
	say("@tasksearch\n");

	return 0;
}
int taskmodify(void* buf, int len)
{
	return 0;
}




void initthread()
{
}
void freethread()
{
}