#include "actor.h"
int uart_list();
int uart_choose(void*);
int uart_write(void*);




struct uartinfo
{
	u8* buf;
	int len;
	int enq;
	int deq;
};
static struct uartinfo* info;
int status = 0;
int len = 0;
char buf[32];




static void terminal_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u8* p;
	int j,k,enq,deq;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x20000;
	int h = (win->h) * (sty->wanth) / 0x20000;
	drawline_rect(win, 0xffffff,
		cx-w, cy-h, cx+w, cy+h
	);

	if((status == 0)&&(len == 0))
	{
		say("terminal(%x,%x,%x)\n",win,act,sty);
		uart_list();
	}
	if(info == 0)return;
	p = info->buf;
	enq = info->enq;
	deq = info->deq;
	info->deq = enq;
	if(enq == deq)return;

	if(enq > deq)
	{
		j = w/8;
		k = enq-deq;
		if(k>j)k=j;
		drawstring(win, 0xffffff,
			cx-w, cy-h, p+deq, k
		);
	}
	else
	{
		j = w/8;
		k = 0x100000-deq;
		if(k>j)k=j;
		drawstring(win, 0xffffff,
			cx-w, cy-h, p+deq, k
		);

		k = enq;
		if(k>j)k=j;
		drawstring(win, 0xffffff,
			cx-w, cy-h+16, p, k
		);
	}
}
static void terminal_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	char* p;
	int enq, deq;
	if((status == 0)&&(len == 0))
	{
		say("terminal(%x,%x,%x)\n",win,act,sty);
		uart_list();
	}

	if(info == 0)return;
	p = info->buf;
	enq = info->enq;
	deq = info->deq;
	info->deq = enq;
	if(enq == deq)return;

	if(enq > deq)
	{
		//printmemory(p+deq, enq-deq);
		say("%.*s", enq-deq, p+deq);
	}
	else
	{
		//printmemory(p+deq, 0x200-deq);
		//printmemory(p, enq);
		say("%.*s%.*s", 0x100000-deq, p+deq, enq, p);
	}
}
static void terminal_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == hex32('c','l','i',0))terminal_read_cli(win, act, sty);
	else if(fmt == hex32('t','u','i',0))terminal_read_tui(win, act, sty);
	else if(fmt == hex32('h','t','m','l'))terminal_read_html(win, act, sty);
	else if(fmt == hex32('v','b','o',0))terminal_read_vbo(win, act, sty);
	else terminal_read_pixel(win, act, sty);
}
static void terminal_write(struct event* ev)
{
	u8 ch;
	if(ev->what == __uart__)
	{
		info = (void*)(ev->why);
		return;
	}

	if(ev->what == __char__)
	{
		ch = (ev->why)&0xff;
		if(status != 0)
		{
			uart_write((void*)ev);
			return;
		}
		if(ch == 0x8)
		{
			if(len > 0)
			{
				say("\b \b");
				len--;
				buf[len] = 0;
			}
			return;
		}

		say("%c",ch);
		if(ch == 0xd)
		{
			if(len == 0)return;

			uart_choose(buf);
			status = 1;
			return;
		}
		if(len < 31)
		{
			buf[len] = ch;
			len++;
		}
	}
}




static void terminal_list()
{
}
static void terminal_change()
{
}
static void terminal_start()
{
}
static void terminal_stop()
{
}
void terminal_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('t', 'e', 'r', 'm');

	p->start = (void*)terminal_start;
	p->stop = (void*)terminal_stop;
	p->list = (void*)terminal_list;
	p->choose = (void*)terminal_change;
	p->read = (void*)terminal_read;
	p->write = (void*)terminal_write;
}
void terminal_delete()
{
}
