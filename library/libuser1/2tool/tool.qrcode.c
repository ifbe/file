#include "actor.h"
int qrcode_generate(char* src,char* dst,int slen);




static int slen;
static char* databuf;




static void qrcode_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 color;
	int x,y;
	int x1,y1,x2,y2;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	for(y=0;y<slen;y++)
	{
		for(x=0;x<slen;x++)
		{
			if( databuf[(y*slen)+x] == 0 )color=0;
			else color=0xffffffff;

			x1 = cx + (x*ww*2/slen) - ww;
			y1 = cy + (y*hh*2/slen) - hh;
			x2 = cx + ((x+1)*ww*2/slen) - ww;
			y2 = cy + ((y+1)*hh*2/slen) - hh;
			drawsolid_rect(
				win, color,
				x1, y1, x2, y2
			);
		}
//say("\n");
	}
}
static void qrcode_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	u32 color;
	char* p = (char*)(win->buf);

	p += mysnprintf(p, 0x1000, "<div style=\"width:500px;height:500px;background:#fff\">");
	p += mysnprintf(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"width:%dpx;"
		"height:%dpx;"
		"}"
		"</style>",

		500/slen, 500/slen
	);
	for(y=0;y<slen;y++)
	{
		for(x=0;x<slen;x++)
		{
			if( databuf[(y*slen)+x] != 0 )continue;

			p += mysnprintf(
				p, 0x1000,
				"<div class=\"rect\" style=\""
				"left:%dpx;"
				"top:%dpx;"
				"background:#000;"
				"\"></div>",
				x*500/slen, y*500/slen
			);
		}
	}
	p += mysnprintf(p, 99, "</div>");
}
static void qrcode_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void qrcode_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);
	for(x=0;x<width*height*4;x++)p[x] = 0;

	for(y=0;y<100;y++)
	{
		if(y >= slen)break;
		if(y >= height)break;
		for(x=0;x<100;x++)
		{
			if(x >= slen)break;
			if(x >= width/2)break;
			if( databuf[(y*slen)+x] != 0 )continue;

			p[( (y*width+x*2)<<2 ) + 3] = 7;
			p[( (y*width+x*2)<<2 ) + 7] = 7;
		}
	}
}
static void qrcode_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("qrcode(%x,%x,%x)\n",win,act,sty);
}
static void qrcode_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)qrcode_read_cli(win, act, sty);
	else if(fmt == _tui_)qrcode_read_tui(win, act, sty);
	else if(fmt == _vbo_)qrcode_read_vbo(win, act, sty);
	else if(fmt == _html_)qrcode_read_html(win, act, sty);
	else qrcode_read_pixel(win, act, sty);
}
static void qrcode_write(struct event* ev)
{
}




static void qrcode_list()
{
}
static void qrcode_into()
{
}
static void qrcode_start()
{
	slen=49;
	qrcode_generate("haha",databuf,slen);
}
static void qrcode_stop()
{
}
void qrcode_create(void* base,void* addr)
{
	struct actor* p = addr;
	databuf = base+0x300000;

	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('q', 'r', 'c', 'o', 'd', 'e', 0, 0);

	p->start = (void*)qrcode_start;
	p->stop = (void*)qrcode_stop;
	p->list = (void*)qrcode_list;
	p->choose = (void*)qrcode_into;
	p->read = (void*)qrcode_read;
	p->write = (void*)qrcode_write;
}
void qrcode_delete()
{
}
