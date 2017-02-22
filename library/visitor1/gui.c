#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//visitor0
void* eventread();
void eventwrite(u64 why, u64 what, u64 where, u64 when);
void* birth();
void death();
//libui1
int characterstart(void* buf, void* fmt, int w, int h);
int characterstop();
int characterwrite(void* p);
int characterread();
int characterlist(char*);
int charactercommand(char* p);
//libui0
void* displaystart(int);
int displaystop();
int displaylist();
int displaychoose();
int displayread();
int displaywrite();
//libsoft1
void motion_explain(void*);
void network_explain(void*);
void sound_explain(void*);
void vision_explain(void*);
//libhard
//libboot
void printmemory(char*,int);
void say(char*,...);




//other
static u64 time;
static int fps;

//visitor
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};

//libui
struct screen
{
	void* buf;
	u64 fmt;
	u64 w;
	u64 h;
};




int main(int argc, char* argv[])
{
	//before
	int ret;
	struct event* ev;
	struct screen* ui = birth() + 0x400000;
	say("ui@%llx\n", (u64)ui);

	//config
	displaystart(0);
	characterstart(ui[0].buf, &ui[0].fmt, ui[0].w, ui[0].h);
	for(ret=1;ret<argc;ret++)
	{
		charactercommand(argv[ret]);
	}

	//forever
	while(1)
	{
/*
		//1.screen
		for(ret=0;ret<max;ret++)
		{
			//skip whom doesn't want
			if(list.flag != waiting)continue;

			//show whom want it
			characterread(list + ret*0x20);
			displaywrite( list + ret*0x20);
		}
		fps++;
*/
		characterread();
		displaywrite();
		fps++;

again:
		//2.event
		ev = eventread();
		if(ev == 0)break;		//error
		if(ev->what == 0)break;		//exit
		if(ev->what == 0x656d6974)	//time
		{
			if(ev->when > time+1000000)
			{
				say("fps=%d\n",fps);
				time = ev->when;
				fps = 0;
			}
			goto again;
		}

		//3.pre change
		if(((ev->what)&0xff) == 'p')
		{
			//sensor rawdata -> my event
			motion_explain(ev);
		}
		else if(((ev->what)&0xff) == 'n')
		{
			//network rawdata -> my event
			network_explain(ev);
/*
			if(ev->what == 'w')	//vnc, rdp, ...
			{
				//user come, re-draw
				if(+)
				{
					windowstart(addr, "net", 512, 512);
					continue;
				}

				//user gone, re-draw
				else if(-)
				{
					displaystop();
					continue;
				}

				//user resize, set-flag
				else .flag = 1;
			}
*/
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


		//4.real change
		characterwrite(ev);
	}

	//after
	death();
	return 0;
}
