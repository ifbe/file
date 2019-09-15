#include "libuser.h"
#define WIDTH 32
#define HEIGHT 32
int snake_generate(void* buf, int w, int h);
int snake_left(void* buf, int w, int h);
int snake_right(void* buf, int w, int h);
int snake_up(void* buf, int w, int h);
int snake_down(void* buf, int w, int h);




struct snake
{
	u8 x;
	u8 y;
	u16 next;
};
static struct snake buf[WIDTH*HEIGHT];




void snake_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int j;
	int t1, t2, t3, t4;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//body
	j = 1;
	while(1)
	{
		//say("[%x]=%x,%x,%x\n", j, buf[j].x, buf[j].y, buf[j].next);
		t1 = cx-ww + buf[j].x * 2 * ww / WIDTH;
		t2 = cy-hh + buf[j].y * 2 * hh / HEIGHT;
		t3 = cx-ww + (buf[j].x+1) * 2 * ww / WIDTH;
		t4 = cy-hh + (buf[j].y+1) * 2 * hh / HEIGHT;
		drawsolid_rect(win, 0xffffff, t1+1, t2+1, t3-1, t4-1);

		if(buf[j].next <= 1)break;
		j = buf[j].next;
	}

	//food
	t1 = cx-ww + buf[0].x * 2 * ww / WIDTH;
	t2 = cy-hh + buf[0].y * 2 * hh / HEIGHT;
	t3 = cx-ww + (buf[0].x+1) * 2 * ww / WIDTH;
	t4 = cy-hh + (buf[0].y+1) * 2 * hh / HEIGHT;
	drawsolid_rect(win, 0x00ff00, t1+1, t2+1, t3-1, t4-1);
}
void snake_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
void snake_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
void snake_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;
/*
	//<head>
	htmlprintf(win, 1,
		".snakebg{WIDTH:50%%;HEIGHT:50%%;float:left;background-color:#000;text-align:center;}\n"
		".snakefg{WIDTH:14%%;HEIGHT:14%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"snakebg\">\n");
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"snakefg\">%d</div>\n",
				data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
*/
}
void snake_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int j,t;
	int stride = win->stride;
	char* p = (char*)(win->buf);

	t = buf[0].x + buf[0].y*stride;
	p[t<<2] = '@';
}
void snake_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	say("snake(%x,%x,%x)\n",win,act,sty);
}
void snake_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)snake_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)snake_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)snake_draw_html(act, pin, win, sty);
	else if(fmt == _json_)snake_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)snake_draw_vbo(act, pin, win, sty);
	else snake_draw_pixel(act, pin, win, sty);
}
void snake_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(_char_ == type)
	{
		if(key == 'w')snake_up(buf, WIDTH, HEIGHT);
		else if(key == 's')snake_down(buf, WIDTH, HEIGHT);
		else if(key == 'a')snake_left(buf, WIDTH, HEIGHT);
		else if(key == 'd')snake_right(buf, WIDTH, HEIGHT);
	}
	if(_kbd_ == type)
	{
		if(key == 0x48)snake_up(buf, WIDTH, HEIGHT);
		else if(key == 0x4b)snake_left(buf, WIDTH, HEIGHT);
		else if(key == 0x4d)snake_right(buf, WIDTH, HEIGHT);
		else if(key == 0x50)snake_down(buf, WIDTH, HEIGHT);
	}
}




static void snake_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//snake_draw(act, pin, win, sty);
}
static void snake_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//snake_event(act, pin, win, sty, ev, 0);
}
static void snake_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void snake_start(struct halfrel* self, struct halfrel* peer)
{
}




static void snake_search(struct actor* act)
{
}
static void snake_modify(struct actor* act)
{
}
static void snake_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void snake_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buf;
	if(_copy_ == act->type)act->buf = memorycreate(WIDTH*HEIGHT*4, 0);

	snake_generate(buf, WIDTH, HEIGHT);
}




void snake_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'n', 'a', 'k', 'e', 0, 0, 0);

	p->oncreate = (void*)snake_create;
	p->ondelete = (void*)snake_delete;
	p->onsearch = (void*)snake_search;
	p->onmodify = (void*)snake_modify;

	p->onstart = (void*)snake_start;
	p->onstop  = (void*)snake_stop;
	p->onread  = (void*)snake_read;
	p->onwrite = (void*)snake_write;
}
