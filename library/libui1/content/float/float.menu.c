#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void drawstring(
	void*, void* str, int size, 
	int x, int y, u32 fg, u32 bg);
void triangle(void*,
	int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	u32 bc, u32 fc);
void rect(void*,
	int x1, int y1,
	int x2, int y2,
	u32 bc, u32 fc);
//
int actorchoose(char* p);
//
int fmt(char*, int, char*, ...);
int say(char*,...);




struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;
	
	u8 data[0xc0];
};
struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u8 data[0xc0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
static char buffer[128];
static int bufp=0;




static void menu_read_text(struct window* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	char* p = (char*)(win->buf1);
	char* src;
	char* dst;

	for(y=height/4;y<height*3/4;y++)
	{
		for(x=width/4;x<width*3/4;x++)
		{
			dst = p + ((x + width*y)<<2);
			dst[0]=dst[1]=dst[2]=dst[3]=0;
		}
	}
	for(x=width/4;x<=width*3/4;x++)
	{
		p[(x + (height/4)*width)<<2] = '-';
		p[(x + (height*3/4)*width)<<2] = '-';
	}
	for(y=height/4;y<=height*3/4;y++)
	{
		p[(width/4 + y*width)<<2] = '|';
		p[(width*3/4 + y*width)<<2] = '|';
	}

	x = (height/4+1)*width + width/4 + 1;
	dst = p + (x<<2);
	src = "what do you want?";

	x=y=0;
	while(1)
	{
		if(src[x] == 0)break;

		dst[y]=src[x];
		x += 1;
		y += 4;
	}

	x = (height/4+2)*width + width/4 + 1;
	dst = p + (x<<2);
	src = buffer;

	x=y=0;
	while(1)
	{
		if(x > bufp)break;

		dst[y]=src[x];
		x += 1;
		y += 4;
	}
}
static void menu_read_pixel(struct window* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;

	//title
	rect(win,
		width/4, (height/4)&0xfffffff0,
		width*3/4, (height/4+16)&0xfffffff0,
		0x01234567, 0xfedcba98
	);
	rect(win,
		(width*3/4) - 16, (height/4)&0xfffffff0,
		width*3/4, ((height/4) + 16)&0xfffffff0,
		0xff0000, 0
	);

	//left, right
	triangle(win,
		width/16, height*3/8,
		width*3/16, height/4,
		width*3/16, height/2,
		0x888888, 0xffffff
	);
	triangle(win,
		width*15/16, height*3/8,
		width*13/16, height/4,
		width*13/16, height/2,
		0x888888, 0xffffff
	);

	//body
	rect(win,
		width/4, (height/4+16)&0xfffffff0,
		width*3/4, height/2,
		0, 0xffffffff
	);

	//string
	drawstring(
		win, "what do you want?", 1,
		width/4, height/4 + 16, 0xffffffff, 0
	);
	drawstring(
		win, buffer, 1,
		width/4, height/4 + 32,	0xffffffff, 0
	);
}
static void menu_read_html(struct window* win)
{
	char* p = (char*)(win->buf1);
	buffer[bufp] = 0;

	fmt(p,0x1000,
		"<div style=\""
		"position:absolute;"
		"z-index:100;"
		"left:25%%;"
		"top:25%%;"
		"width:50%%;"
		"height:50%%;"
		"border:4px solid #000;"
		"background:#444444;"
		"color:#000;"
		"text-align:center;"
		"\">"
		"what do you want<hr>%s"
		"</div>",

		buffer
	);
}
static void menu_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		menu_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		menu_read_html(win);
	}

	//pixel
	else
	{
		menu_read_pixel(win);
	}
}




//write,read,into,list
static void menu_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;
	say("%x,%x\n",type,key);

	//'xyz left'
	if(type==0x2d70)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if( (y > 0xc000) | (y < 0x4000) )return;

		//last
		else if(x < 0x4000)
		{
			actorchoose("-");
		}

		//next
		else if(x > 0xc000)
		{
			actorchoose("+");
		}

		//点击红色矩形，退出
		else if(x>0xc000-16)
		{
			if(y<0x4000+16)
			{
				//退出
				actorchoose("q");
				return;
			}
		}
	}//left

	//'char'
	else if(type==0x72616863)
	{
		if(key==0x8)		//backspace
		{
			if(bufp!=0)
			{
				bufp--;
				buffer[bufp]=0;
			}
		}
		else if( (key==0xa) | (key==0xd) )	//回车
		{
			//say("%s\n",buffer);
			actorchoose(buffer);

			//clear
			for(bufp=0;bufp<127;bufp++) buffer[bufp]=0;
			bufp=0;
		}
		else
		{
			if(bufp<0x80)
			{
				buffer[bufp]=key&0xff;
				bufp++;
			}
		}
	}//kbd
}




static void menu_choose()
{
}
static void menu_list()
{
}
static void menu_start()
{
}
static void menu_stop()
{
}
void menu_create(void* base, void* addr)
{
	struct player* p = addr;

	p->type = 0;
	p->name = 0x756e656d;
	p->start = (u64)menu_start;
	p->stop = (u64)menu_stop;
	p->list = (u64)menu_list;
	p->choose = (u64)menu_choose;
	p->read = (u64)menu_read;
	p->write = (u64)menu_write;
}
void menu_delete()
{
}
