#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int ncmp(void*, void*, int);
int cmp(void*, void*);
int termwrite(void*, int);
void say(void*, ...);




static u8 knowncmd[] = {'e','x','i','t'};
static u8* inbuf = 0;
//
static int dx = 0;
static int dy = 0;
static int enq = 0;
static int deq = 0;
void initstdin(void* addr)
{
	inbuf = addr;
	dx = 0;
	dy = 0;
	enq = 0;
	deq = 0;
}
void freestdin()
{
}
void* getstdin()
{
	return inbuf;
}
int getcurin()
{
	return enq;
}




int autocomplete_subcmd(u8* buf, int len)
{
	return 0;
}
int autocomplete(u8* buf, int len)
{
	u8 tmp;
	if((len < 4)&&(0 == ncmp(buf, knowncmd, len)))
	{
		for(;len<4;len++)
		{
			tmp = knowncmd[len];
			say("%c", tmp);

			inbuf[enq] = tmp;
			enq++;
		}
		return 1;
	}

	say("\n(matching: %.*s*)\n", len, buf);
	say("	0101	haha	device	driver\n");
	say("	system	artery	arena	actor\n");
	return 0;
}




void input(u8* buf, int len)
{
	int j,k,t;

	for(j=0;j<0x1000;j++)
	{
		k = buf[j];
		if(k < 8)return;

		if((0x8 == k)|(0x7f == k))
		{
			while(1)
			{
				if(enq <= 0)break;
				if(enq <= deq)break;

				enq--;
				k = inbuf[enq];

				say("\b \b");
				inbuf[enq] = 0;

				if((k < 0x80)|(k > 0xc0))break;
			}
		}
		else if((0x1b == k)&&(0x5b == buf[j+1]))
		{
			k = buf[j+2];
			if((0x41 == k)|(0x42 == k))
			{
				if(0x41 == k)dy++;
				if(0x42 == k)
				{
					if(0 == dy)return;
					dy--;
				}

				//clear line
				while(1)
				{
					if(enq <= 0)break;
					if('\n' == inbuf[enq-1])break;
					enq--;
					inbuf[enq] = 0;
					say("\b \b");
				}
				if(0 == dy)return;

				//print history-dy
				t = 0;
				for(j=enq-1;j>=0;j--)
				{
					if((j>0)&&('\n' != inbuf[j]))continue;

					if(t == dy)
					{
						if(j)j++;
						say("%.*s", k-j, inbuf+j);
						for(t=0;t<k-j;t++)inbuf[enq+t] = inbuf[j+t];

						deq = enq;
						enq += k-j;
						return;
					}

					t++;
					k = j;
				}
			}
			return;
		}
		else if(0x9 == k)
		{
			t = autocomplete(inbuf+deq, enq-deq);
			if(0 == t)	//failed
			{
				termwrite("\n", 1);
				say("%.*s", enq-deq, inbuf+deq);
			}
		}
		else
		{
			if(0xd == k)k = 0xa;
			say("%c", k);

			if(enq > 0xf0000)enq = 0;
			inbuf[enq] = k;
			enq++;

			if(0xa == k)
			{
				termwrite(inbuf+deq, enq-deq);
				deq = enq;
			}
			dy = 0;
		}
	}
}
