#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//libui1
int actorread();
int actorwrite(void* event);
//libui0
int arenaread();
int arenawrite();
//libsoft1
void motion_explain(void*);
void network_explain(void*);
void sound_explain(void*);
void vision_explain(void*);
//libsoft0
u64 gettime();
void sleep_us(int);
//libboot
void printmemory(void*, int);
void say(void*, ...);
//
void* birth();
void death();
void* eventread();
void eventwrite(u64,u64,u64,u64);




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
static u64 time;
static int fps;




int main(int argc, char* argv[])
{
	//before
	int ret;
	struct event temp;
	struct event* ev;

	birth();
	//say("@birth\n");

	//config
	for(ret=1;ret<argc;ret++)
	{
		temp.why = (u64)(argv[ret]);
		temp.what = 0x727473;
		actorwrite(&temp);
	}

	//forever
	while(1)
	{
		//1.show
		actorread();
		arenawrite();
		fps++;

again:
		//2.wait
		ev = eventread();
		if(ev == 0)
		{
			sleep_us(1000);
			goto again;
		}

		//exit or timer
		if(ev->what == 0)break;		//exit
		if(ev->what == 0x656d6974)	//time
		{
			if(ev->when > time+1000000)
			{
				//say("fps=%d\n",fps);
				time = ev->when;
				fps = 0;
			}
			goto again;
		}


		//3.pre process
		if(((ev->what)&0xff) == 'p')
		{
			//sensor rawdata -> my event
			motion_explain(ev);
		}
		else if(((ev->what)&0xff) == 'n')
		{
			//network rawdata -> my event
			network_explain(ev);
		}
		else if(((ev->what)&0xff) == 's')
		{
			//sound rawdata -> my event
			sound_explain(ev);
		}
		else if(((ev->what)&0xff) == 'v')
		{
			//video rawdata -> my event
			vision_explain(ev);
		}
		if(ev->what == 0)goto again;


		//4.real process
		actorwrite(ev);
	}

	//after
	//say("@death\n");
	death();
	return 0;
}
