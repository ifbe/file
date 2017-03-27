#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




static u8 bitmap[1024];		//1m per u8




void* startmemory(int size)
{
	u64 j;
	for(j=0;j<1024;j++)
	{
		if(bitmap[j] == 0)
		{
			bitmap[j] = 1;
			return (void*)(j<<20);
		}
	}

	return 0;
}
int stopmemory(void* addr)
{
	u64 j = (u64)addr;
	j >>= 20;
	bitmap[j] = 0;
	return 0;
}
void creatememory()
{
	int j;
	for(j=0;j<32;j++)bitmap[j] = 1;
}
void deletememory()
{
}
