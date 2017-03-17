#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//special guys
void menu_create(u8*,u8*);
void menu_delete();
void roster_create(u8*,u8*);
void roster_delete();
void virtkbd_create(u8*,u8*);
void virtkbd_delete();
//libs
void ascii_create(u8*,u8*);
void ascii_delete();
//game
void the2048_create(u8*,u8*);
void the2048_delete();
void ooxx_create(u8*,u8*);
void ooxx_delete();
void snake_create(u8*,u8*);
void snake_delete();
void sudoku_create(u8*,u8*);
void sudoku_delete();
void tetris_create(u8*,u8*);
void tetris_delete();
void weiqi_create(u8*,u8*);
void weiqi_delete();
void xiangqi_create(u8*,u8*);
void xiangqi_delete();
//test
void color_create(u8*,u8*);
void color_delete();
void doodle_create(u8*,u8*);
void doodle_delete();
void font_create(u8*,u8*);
void font_delete();
void pure_create(u8*,u8*);
void pure_delete();
//hack
void browse_create(u8*,u8*);
void browse_delete();
void console_create(u8*,u8*);
void console_delete();
void control_create(u8*,u8*);
void control_delete();
void hex_create(u8*,u8*);
void hex_delete();
void tree_create(u8*,u8*);
void tree_delete();
//tool
void camera_create(u8*,u8*);
void camera_delete();
void calculator_create(u8*,u8*);
void calculator_delete();
void circuit_create(u8*,u8*);
void circuit_delete();
void sketchpad_create(u8*,u8*);
void sketchpad_delete();
void spectrum_create(u8*,u8*);
void spectrum_delete();
void stl_create(u8*,u8*);
void stl_delete();
void qrcode_create(u8*,u8*);
void qrcode_delete();
//
int arterycommand(void*);
int arteryprompt();
int ncmp(void*,void*,int);
int cmp(void*,void*);
u32 getrandom();
//
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




struct working
{
	//[0,7]:种类
	u64 type;

	//[8,f]:名字
	u64 name;

	//[10,17]:开始
	int (*start)();
	char padding2[ 8 - sizeof(char*) ];

	//[18,1f]:结束
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[20,27]:观察
	int (*list)();
	char padding4[ 8 - sizeof(char*) ];

	//[28,2f]:调整
	int (*choose)();
	char padding5[ 8 - sizeof(char*) ];

	//[30,37]:输出
	int (*read)(void* config);
	char padding6[ 8 - sizeof(char*) ];

	//[38,3f]:输入
	int (*write)(void* event);
	char padding7[ 8 - sizeof(char*) ];

	char data[0xc0];
};
struct window
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;

	char data[0xe0];
};
static struct working* worker;
static struct window* win;
static u32 now=0;		//不能有负数
static u32 menu=0;
static u32 rost=0;
static u32 vkbd=0;



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void charactercreate(u8* type, u8* addr)
{
	int i;
	u8* temp;
	if(type!=0)return;

	//where
	win = (struct window*)(addr+0);
	worker = (struct working*)(addr+0x100000);

	//......
	temp = addr + 0x100000;
	for(i=0;i<0x100000;i++)temp[i]=0;

	//menu.center
	menu_create(addr,temp);
	temp+=0x100;

	//menu.roster
	roster_create(addr,temp);
	temp+=0x100;

	//menu.virtkbd
	virtkbd_create(addr,temp);
	temp+=0x100;

	//game.2048
	the2048_create(addr,temp);
	temp+=0x100;

	//game.ooxx
	ooxx_create(addr,temp);
	temp+=0x100;

	//game.snake
	snake_create(addr,temp);
	temp+=0x100;

	//game.sudoku
	sudoku_create(addr,temp);
	temp+=0x100;

	//game.tetris
	tetris_create(addr,temp);
	temp+=0x100;

	//game.weiqi
	weiqi_create(addr,temp);
	temp+=0x100;

	//game.xiangqi
	xiangqi_create(addr,temp);
	temp+=0x100;

	//hack.browse
	browse_create(addr,temp);
	temp+=0x100;

	//hack.console
	console_create(addr,temp);
	temp+=0x100;

	//hack.hex
	hex_create(addr,temp);
	temp+=0x100;

	//hack.input
	control_create(addr,temp);
	temp+=0x100;

	//hack.tree
	tree_create(addr,temp);
	temp+=0x100;

	//test.color
	color_create(addr,temp);
	temp+=0x100;

	//test.font
	font_create(addr,temp);
	temp+=0x100;

	//test.doodle
	doodle_create(addr,temp);
	temp+=0x100;

	//test.pure
	pure_create(addr,temp);
	temp+=0x100;

	//tool.calculator
	calculator_create(addr,temp);
	temp+=0x100;

	//tool.camera
	camera_create(addr,temp);
	temp+=0x100;

	//tool.circuit
	circuit_create(addr,temp);
	temp+=0x100;

	//tool.qrcode
	qrcode_create(addr,temp);
	temp+=0x100;

	//tool.sketchpad
	sketchpad_create(addr,temp);
	temp+=0x100;

	//tool.stl
	stl_create(addr,temp);
	temp+=0x100;

	//tool.spectrum
	spectrum_create(addr,temp);
	temp+=0x100;

	for(now=0;now<100;now++)
	{
		if(worker[now].type != 0)break;
	}
	say("[c,f):createed character\n");
}
void characterdelete()
{
	say("[c,f):deleteing character\n");

	spectrum_delete();
	stl_delete();
	sketchpad_delete();
	qrcode_delete();
	circuit_delete();
	calculator_delete();
	camera_delete();

	tree_delete();
	hex_delete();
	control_delete();
	console_delete();
	browse_delete();

	color_delete();
	doodle_delete();
	font_delete();
	pure_delete();

	the2048_delete();
	ooxx_delete();
	snake_delete();
	sudoku_delete();
	tetris_delete();
	weiqi_delete();
	xiangqi_delete();

	menu_delete();
	roster_delete();
	virtkbd_delete();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterstart(int j)
{
	worker[now].start();
	return 0;
}
int characterstop()
{
	worker[now].stop();
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterlist(u8* p)
{
	//列出所有“人物”
	int j;
	int ret;
	if(p == 0)
	{
		for(j=0;j<256;j++)
		{
			if(worker[j].name != 0)
			{
				say("%d:%s\n",j,&worker[j].name);
			}
		}
		return 0;
	}

	else
	{
		//start searching
		for(j=0;j<10;j++)
		{
			if(worker[j].type != 0)break;	//skip menu|draw
		}
		for(;j<0x100;j++)
		{
			//all searched
			if(worker[j].name == 0)return 0;

			//lookat this
			//say("[%s][%s]\n",&worker[j].name, p);
			ret = ncmp(&worker[j].name, p, 8);
			if(ret == 0)return j;
		}
		return 0;
	}
}
int characterchoose(u8* p)
{
	return 0;
}




void characterwrite(u64* p)
{
	int x;
	if(win->fmt == 0)
	{
		if(p[1] == 0x64626b)arterycommand(p);
		return;
	}

	//size
	if(p[1] == 0x657a6973)
	{
		win->w = p[0] & 0xffff;
		win->h = (p[0] >> 16) & 0xffff;
		return;
	}//size

	//kbd
	else if(p[1] == 0x64626b)
	{
		//按下esc
		if(p[0] == 0x1b)
		{
			menu ^= 1;
			return;
		}
	}//kbd

	//virtkbd
	if(vkbd > 0)x = worker[2].write(p);

	//其余所有消息，谁在干活就交给谁
	if(menu > 0)
	{
		//center
		x = worker[0].write(p);
	}
	else if(rost > 0)
	{
		//roster
		x = worker[1].write(p);
	}
	else
	{
		//player
		worker[now].write(p);
	}
}
void characterread(struct window* win)
{
	if(win->fmt == 0)
	{
		arteryprompt();
		return;
	}

	//
	worker[now].read(win);
	if(menu > 0)worker[0].read(win);
	if(rost > 0)worker[1].read(win);
	if(vkbd > 0)worker[2].read(win);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




int charactercommand(u8* p)
{
	int j,k,ret;
	u64 temp;

	//exit!
	if( (p==0) | (cmp(p,"exit")==0) )
	{
		say("chatacter(%d) wants to die\n",now);
		eventwrite(0,0,0,0);
		return 0;
	}

	//next
	ret=cmp(p,"+");
	if(ret==0)
	{
		if(worker[now+1].name == 0)return 0;
		now++;
		goto found;
	}

	//last
	ret=cmp(p,"-");
	if(ret==0)
	{
		if(worker[now-1].type == 0)return 0;
		now--;
		goto found;
	}

	//random
	ret=cmp(p,"random");
	if(ret==0)
	{
		for(j=0;j<10;j++)
		{
			if(worker[j].type != 0)break;	//skip menu|draw
		}
		k=j;

		for(;k<0x100000/0x80;k++)
		{
			if(worker[k].name == 0)break;
		}

		now=( getrandom() % (k-j) ) + j;
		goto found;
	}

	//search
	ret = characterlist(p);
	if(ret != 0)
	{
		now = ret;
		goto found;
	}

notfound:
	ret = arterycommand(p);
	ret = arterycommand("\n");
	return 0;

found:
	//
	characterstart(0);
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
