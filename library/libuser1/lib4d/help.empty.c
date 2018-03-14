#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define _json_ hex32('j','s','o','n')
#define _xml_ hex32('x','m','l',0)
void* allocstyle();
void* allocpinid();
//
void* relation_read(u64);
void relation_write(void*, void*, u64, void*, void*, u64);
//
void draw8bit_rect(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1);
void carveascii_area(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float x0, float y0, float x1, float y1);




static struct actor* actor = 0;
int arenaactor(struct arena* win, struct actor* act)
{
	int min;
	int w = win->w;
	int h = win->h;
	int d = (w+h) / 2;
	struct style* sty;
	struct pinid* pin;

	sty = allocstyle();
	if(0 == sty)return 0;

	pin = allocpinid();
	if(0 == pin)return 0;

	sty->cx = w/2;
	sty->cy = h/2;
	sty->cz = 0.0;

	if(w<h)min = w/2;
	else min = h/2;
	sty->rx = min;
	sty->fy = min;
	sty->uz = min;

	relation_write(
		win, sty, _win_,
		act, pin, _act_
	);

	//win->onstart(win, sty);
	act->onstart(act, pin);
	return 0;
}




void login_read_pixel(struct arena* win)
{
	u32 c;
	int x,y,j;
	int w = win->w;
	int h = win->h;

	for(j=0;j<64;j++)
	{
		if(j == win->flag0)c = 0x80ff00ff;
		else c = 0x800000ff;

		x = j%4;
		y = j/4;
		drawicon_1(
			win, c,
			(x+2)*w/8, (y+8)*h/32,
			(x+3)*w/8, (y+9)*h/32,
			(u8*)&actor[j].name, 8
		);
	}
}
void login_read_8bit(struct arena* win)
{
	int x,y;
	int j,c;
	for(j=0;j<64;j++)
	{
		if(j == win->flag0)c = 0x80;
		else c = 0x42;

		x = j%4;
		y = j/4;
		draw8bit_rect(
			win, c,
			 81+x*40, 61+y*10,
			118+x*40, 68+y*10
		);
	}
}
void login_read_vbo(struct arena* win)
{
	u32 c;
	int j,k;
	float x,y;

	carveline(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		0.0, 0.0, 100.0
	);
	carveascii_area(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		100.0, 0.0, 0.0,
		0.0, 100.0, 0.0,
		0.0, 0.0, 1.0, 1.0
	);

	for(j=0;j<64;j++)
	{
		if(j == win->flag0)
		{
			k = 4.0;
			c = 0x00ff00;
		}
		else
		{
			k = 1.0;
			c = 0xffffff;
		}

		x = cosine(j/PI)/32.0;
		y = sine(j/PI)/32.0;
		carvestring(
			win, c,
			x*256, y*256, j+1,
			x*64, y*64, 0.0,
			-y*64, x*64, 0.0,
			(u8*)&actor[j].name, 8
		);
	}
}
void login_read_html(struct arena* win)
{
	u32 c;
	int j;
	int len = win->len;
	u8* buf = win->buf;

	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;

		if(j == win->flag0)c = 0xff0000;
		else c = 0xffffff;

		len += mysnprintf(
			buf+len, 0x1000,
			"<div style=\"float:left;width:25%%;color:#%06x;\">%.8s</div>",
			c, (u8*)&actor[j].name
		);
	}
	win->len = len;
}
void login_read_tui(struct arena* win)
{
	int j,k,x,y;
	int ww = ((win->w)/2)&0xfffc;
	int hh = (win->h)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);

	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;

		if(j == win->flag0)k=1;
		else k=2;

		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&actor[j].name, 8);
	}
}
void login_read_cli(struct arena* win)
{
}
void login_read(struct arena* win)
{
	if(win->fmt == _cli_)login_read_cli(win);
	else if(win->fmt == _tui_)login_read_tui(win);
	else if(win->fmt == _vbo_)login_read_vbo(win);
	else if(win->fmt == _html_)login_read_html(win);
	else if(win->fmt == _8bit_)login_read_8bit(win);
	else login_read_pixel(win);
}
void login_write(struct arena* win, struct event* ev)
{
	int x,y,j,flag0;
	y = (ev->what)&0xff;
	x = ((ev->what)>>8)&0xff;
	flag0 = win->flag0;

	if(y == 'p')
	{
		if(x == '@')
		{
			x = (ev->why)&0xffff;
			x = (x*8) / (win->w);
			y = ((ev->why)>>16)&0xffff;
			y = (y*32) / (win->h);

			if((x>=2)&&(x<6)&&(y>=8)&&(y<24))
			{
				win->flag0 = (y-8)*4 + (x-2);
			}
		}
		else if(x == '-')
		{
			if((win->flag0 >= 0) && (flag0 < 64))
			{
				arenaactor(win, &actor[flag0]);
				win->flag0 = 0;
			}
		}
	}
	else if(ev->what == _char_)
	{
		if((ev->why == 0xd)|(ev->why == 0xa))
		{
			arenaactor(win, &actor[flag0]);
			win->flag0 = 0;
		}
		else if(ev->why == 0x435b1b)
		{
			flag0 = (flag0+1)%64;
		}
		else if(ev->why == 0x445b1b)
		{
			flag0 = (flag0+31)%64;
		}
	}
	else if(ev->what == _kbd_)
	{
		if(ev->why == 0x4b)
		{
			flag0 = (flag0+63)%64;
		}
		else if(ev->why == 0x4d)
		{
			flag0 = (flag0+1)%64;
		}
	}
}
void login_create(void* addr)
{
	actor = addr + 0x100000;
}
void login_delete()
{
}
