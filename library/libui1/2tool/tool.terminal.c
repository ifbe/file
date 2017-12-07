#include "actor.h"
struct uartinfo
{
	u8* buf;
	int len;
	int enq;
	int deq;
};
struct uartterm
{
	u8* buf;
	u32 len;
	u32 fg;
	u32 bg;

	int width;
	int height;
	int vimw;
	int vimh;

	int left;
	int right;
	int top;
	int bottom;

	int curx;
	int cury;
};
int uart_list(void*);
int uart_choose(void*);
int uart_write(void*);
void drawterm(struct arena* win, struct uartterm* term, int x0, int y0, int x1, int y1);




static struct uartinfo* old;
static struct uartterm term;
static u8 listbuf[0x100];
static int listlen = 0;
static u8 charbuf[0x100];
static int charlen = 0;
static int status = 0;




int queue_1b_color(u8* p)
{
	//reset
	if(p == 0)
	{
		term.fg = 7;
		term.bg = 0;
		return 0;
	}

	//reset
	if(p[0] == '0')
	{
		term.fg = 7;
		term.bg = 0;
		return 0;
	}

	//heavy
	else if(p[0] == '1')
	{
		term.fg = 1;
		return 1;
	}

	//foreground
	if(p[0] == '3')
	{
		if((p[1] >= '0')&&(p[1] <= '7'))term.fg = p[1]-0x30;
		return 3;
	}

	//background
	else if(p[0] == '4')
	{
		if((p[1] >= '0')&&(p[1] <= '7'))term.bg = p[1]-0x30;
		return 4;
	}
	return -1;
}
static int queue_1b_parsergb(u8* p)
{
	int j,k;
	//printmemory(p, 12);

	//? ;
	if(p[1] == ';')
	{
		queue_1b_color(p+0);
		queue_1b_color(p+2);
	}

	//? ? ;
	if(p[2] == ';')
	{
		j = ncmp(p+1, "8;5;", 4);
		if(j != 0)
		{
			queue_1b_color(p+0);
			queue_1b_color(p+3);
		}

		//? 8 ; 5 ; a b c ;
		else
		{
			j = 5;
			k = 0;
			while(1)
			{
				if(p[j] == ';')break;
				if(p[j] == 'm')break;

				k = (k*10) + p[j] - 0x30;
				j++;
			}
			if(p[0] == '3')term.fg = k;
			if(p[0] == '4')term.bg = k;
			if(p[j] == ';')queue_1b_parsergb(p+j+1);
		}
	}

	return 0;
}
static int queue_1b_parsewh(u8* p)
{
	u8* q;
	int j, k, t;
	int u=0, v=0;
	for(k=0;k<4;k++)
	{
		if(p[k] == ';')break;
	}
	if(k>=4)return 0;

	for(j=0;j<k;j++)
	{
		u = (u*10) + p[j] - 0x30;
	}

	for(;k<8;k++)
	{
		if(p[k] == 'r')break;
	}
	if(k>=8)return 0;

	j++;
	for(;j<k;j++)
	{
		v = (v*10) + p[j] - 0x30;
	}

	say("region:%d,%d\n", u, v);
	term.vimw = u-1;
	term.vimh = v-1;
	return 1;
}
static int queue_1b_parsexy(u8* p)
{
	int j, k, x=0, y=0;
	for(k=0;k<4;k++)
	{
		if(p[k] == ';')break;
	}
	if(k>=4)return 0;

	for(j=0;j<k;j++)
	{
		y = (y*10) + p[j] - 0x30;
	}

	for(;k<8;k++)
	{
		if((p[k] == 'H')|(p[k] == 'f'))break;
	}
	if(k>=8)return 0;

	j++;
	for(;j<k;j++)
	{
		x = (x*10) + p[j] - 0x30;
	}

	//say("position:%d,%d,%d,%d\n",x-1,y-1,term.top, term.cury);
	term.curx = x-1;
	term.cury = (term.top)+y-1;
	return 1;
}
static int queue_1b(u8* p)
{
	int j,k;
	int x,y,w,h;
	u8* buf = term.buf;
	if(p[0] != 0x1b)return 0;
	if(p[1] != 0x5b)return 1;
	printmemory(p, 16);

	//1b 5b m
	if(p[2] == 'm')
	{
		queue_1b_color(0);
		return 3;
	}

	//1b 5b 41: cursor up
	if(p[2] == 'A')
	{
		if(term.top + 1 >= term.cury)term.cury -= 1;
		else term.cury = term.top;
		return 3;
	}

	//1b 5b 42: cursor down
	if(p[2] == 'B')
	{
		term.cury += 1;
		return 3;
	}

	//1b 5b 43: cursor forward
	if(p[2] == 'C')
	{
		if(term.curx < term.width-1)term.curx += 1;
		else term.curx = term.width-1;
		return 3;
	}

	//1b 5b 44: cursor backward
	if(p[2] == 'D')
	{
		if(term.curx <= 1)term.curx = 0;
		else term.curx -= 1;
		return 3;
	}

	//1b 5b H/f
	if((p[2] == 'H')|(p[2] == 'f'))
	{
		term.curx = 0;
		term.cury = term.top;
		return 3;
	}

	//1b 5b J: Clear screen from cursor down
	if(p[2] == 'J')
	{
		x = term.curx;
		y = term.cury;
		w = term.width;
		buf = (term.buf) + y*w*4;
		for(j=x*4;j<w*(term.height)*4;j++)buf[j] = 0;
		return 3;
	}

	//1b 5b K: Clear line from cursor right
	if(p[2] == 'K')
	{
		x = term.curx;
		y = term.cury;
		w = term.width;
		buf = (term.buf) + y*w*4;
		for(j=x*4;j<w*4;j++)buf[j] = 0;
		return 3;
	}

	//1b 5b L: Insert line
	if(p[2] == 'L')
	{
		x = term.curx;
		y = term.cury;
		w = term.width;
		h = term.vimh;
		j = term.top;
		k = (h-y+j+1)*w*4 - 1;
		buf = (term.buf) + y*w*4;
		for(j=k;j>=w*4;j--)buf[j] = buf[j-w*4];
		for(j=0;j<w*4;j++)buf[j] = 0;
		return 3;
	}

	//1b 5b M: Delete line
	if(p[2] == 'M')
	{
		x = term.curx;
		y = term.cury;
		w = term.width;
		h = term.vimh;
		j = term.top;
		k = (h-1-y+j)*w*4;
		buf = (term.buf) + y*w*4;
		for(j=0;j<k;j++)buf[j] = buf[j+(w*4)];
		return 3;
	}

	//1b 5b ? m
	if(p[3] == 'd')
	{
		j = p[2]-0x30;
		say("%dd\n",j);
		return 4;
	}

	//1b 5b ? m
	if(p[3] == 'm')
	{
		queue_1b_color(p+2);
		return 4;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 4;
	}

	//1b 5b ? 41: cursor up
	if(p[3] == 'A')
	{
		j = p[2]-0x30;
		if(term.top + j >= term.cury)term.cury -= j;
		else term.cury = term.top;
		return 4;
	}

	//1b 5b ? 42: cursor down
	else if(p[3] == 'B')
	{
		j = p[2]-0x30;
		term.cury += j;
		return 4;
	}

	//1b 5b ? 43: cursor forward
	if(p[3] == 'C')
	{
		j = p[2]-0x30;
		if(term.curx +j+1 >= term.width)term.curx = term.width-1;
		else term.curx += j;
		return 4;
	}

	//1b 5b ? 44: cursor backward
	if(p[3] == 'D')
	{
		j = p[2]-0x30;
		if(term.curx <= j)term.curx = 0;
		else term.curx -= j;
		return 4;
	}

	//1b 5b ? G: cursor column
	if(p[3] == 'G')
	{
		j = p[2]-0x30;
		term.curx = j-1;
		return 4;
	}

	//1b 5b ? J: Clear screen from cursor down
	if(p[3] == 'J')
	{
		if(p[2] == '0')
		{
			buf = term.buf;
			j = (term.cury)*(term.width) + term.curx;
			k = (term.top + term.height)*(term.width);
		}
		else if(p[2] == '1')
		{
			buf = term.buf;
			j = (term.top)*(term.width);
			k = (term.cury)*(term.width) + term.curx;
		}
		else 	//2,3
		{
			buf = term.buf + (term.top)*(term.width)*4;
			j = 0;
			k = (term.height)*(term.width);
		}
		j *= 4;
		k *= 4;
		for(;j<k;j++)buf[j] = 0;
		return 4;
	}

	//1b 5b ? ? 41: cursor up
	if(p[4] == 'A')
	{
		j = (p[2]-0x30)*10 + (p[3]-0x30);
		if(term.top + j >= term.cury)term.cury -= j;
		else term.cury = term.top;
		return 5;
	}

	//1b 5b ? ? 42: cursor down
	if(p[4] == 'B')
	{
		j = (p[2]-0x30)*10 + (p[3]-0x30);
		term.cury += j;
		return 5;
	}

	//1b 5b ? ? 43: cursor forward
	if(p[4] == 'C')
	{
		j = (p[2]-0x30)*10 + (p[3]-0x30);
		if(term.curx +j+1 >= term.width)term.curx = term.width-1;
		else term.curx += j;
		return 5;
	}

	//1b 5b ? ? 44: cursor backward
	if(p[4] == 'D')
	{
		j = (p[2]-0x30)*10 + (p[3]-0x30);
		if(term.curx <= j)term.curx = 0;
		else term.curx -= j;
		return 5;
	}

	//1b 5b ? ? G: cursor backward
	if(p[4] == 'G')
	{
		j = (p[2]-0x30)*10 + (p[3]-0x30);
		term.curx = j-1;
		return 5;
	}

	//1b 5b ? ? d
	if(p[4] == 'd')
	{
		j = (p[2]-0x30)*10 + (p[3]-0x30);
		say("%xd\n",j);
		return 5;
	}

	//1b 5b ? ? h
	if(p[4] == 'h')
	{
		return 5;
	}

	//1b 5b ? ? l
	if(p[4] == 'l')
	{
		return 5;
	}

	//1b 5b ? ? h
	if(p[4] == 'h')
	{
		return 5;
	}

	//1b 5b ? ? l
	if(p[4] == 'l')
	{
		return 5;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		queue_1b_color(p+2);
		return 5;
	}

	//1b 5b '?' ? ? h
	if( (p[2] == '?') && (p[5] == 'h') )
	{
		return 6;
	}

	//1b 5b '?' ? ? l
	if( (p[2] == '?') && (p[5] == 'l') )
	{
		return 6;
	}

	j=2;
	k=3;
	while(1)
	{
		if(p[j] == 'm')
		{
			queue_1b_parsergb(p+2);
			return j+1;
		}
		if(p[j] == 'r')
		{
			queue_1b_parsewh(p+2);
			return j+1;
		}
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			queue_1b_parsexy(p+2);
			return j+1;
		}

		if(p[j] == ';')k=3;
		else
		{
			k--;
			if(k < 0)break;
		}
		j++;
	}

	printmemory(p, 16);
	return 2;
}
static void queue_copy(u8* buf, int len)
{
	int j,k;
	int x,y;
	int w = term.width;
	int h = term.height;
	u8* dst = term.buf;
	u8* q;
	//printmemory(buf,len);

	for(j=0;j<len;j++)
	{
		if(buf[j] == 0)continue;
		if(buf[j] == 0x7)continue;

		if(buf[j] == 0x8)
		{
			x = term.curx;
			if(x > 0)term.curx = x-1;
		}
		else if(buf[j] == '\r')
		{
			term.curx = 0;
		}
		else if(buf[j] == '\n')
		{
			if(term.vimh <= 22)
			{
				k = (term.width)*4;
				q = (term.buf) + (term.top)*k;

				x = k*(term.vimw);
				y = k*(term.vimh);
				for(;x<y;x++)q[x] = q[x+k];
				for(;x<y+k;x++)q[x] = 0;
				//say("scrolling:%d\n",term.vimh);
			}
			else
			{
				term.cury++;
				if(w*(term.cury) >= 0xf0000)
				{
					term.cury = 0;
					term.top = 0;
				}
				if(term.top < term.cury - term.height + 1)
				{
					term.top = term.cury - term.height + 1;
				}
			}
		}
		else if(buf[j] == 0x1b)
		{
			if(buf[j+1] == '=')j++;
			else if(buf[j+1] == '>')j++;
			else if((buf[j+1] == '(')&&(buf[j+2] == 'B'))j+=2;
			else if(buf[j+1] == 0x5b)
			{
				k = queue_1b(buf+j);
				//say("k=%d\n",k);

				if(k>2)j += k - 1;
			}
		}
		else if(buf[j] >= 0x80)
		{
			if(buf[j] < 0xe0)k = 2;
			else if(buf[j] < 0xf0)k = 3;
			else if(buf[j] < 0xf8)k = 4;
			else if(buf[j] < 0xfc)k = 5;
			else if(buf[j] < 0xfe)k = 6;
			else k = 1;

			y = 4*(w*(term.cury) + term.curx);
			dst[y + 7] = term.bg;
			dst[y + 6] = term.fg;
			for(x=0;x<k;x++)dst[y+x] = buf[j+x];

			term.curx += 2;
			j += k-1;
		}
		else
		{
			y = 4*(w*(term.cury) + term.curx);
			dst[y + 3] = term.bg;
			dst[y + 2] = term.fg;
			dst[y + 0] = buf[j];

			term.curx++;
		}
	}
}
static void terminal_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u8* p;
	int enq,deq;
	int cx = win->w / 2;
	int cy = win->h / 2;
	int ww = 320;
	int hh = 8*(term.height);
	sty->cx = 0x8000;
	sty->cy = 0x8000;
	sty->wantw = 80*8*0x10000/(win->w);
	sty->wanth = (term.height)*16*0x10000/(win->h);

	drawhyaline_rect(win, 0x111111, cx-ww, cy-hh, cx+ww, cy+hh);

	if(status == 0)
	{
		if(listlen == 0)
		{
			listlen = uart_list(listbuf);
			if(listlen != 0)say("%.*s", listlen, listbuf);
		}

		drawtext(win, 0xffffff,
			cx-ww, cy-hh, cx+ww, cy+hh,
			listbuf, listlen
		);
		return;
	}

	if(old == 0)return;
	p = old->buf;
	enq = old->enq;
	deq = old->deq;
	old->deq = enq;
	if(enq != deq)
	{
		if(enq > deq)queue_copy(p+deq, enq-deq);
		else
		{
			queue_copy(p+deq, 0x100000-deq);
			queue_copy(p, enq);
		}
	}
	drawterm(win, &term, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void terminal_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void terminal_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x, y, w, h, enq,deq;
	u32* p;
	u32* q;
	u8* buf;
	if((status == 0)&&(charlen == 0))
	{
		if(listlen == 0)
		{
			listlen = uart_list(listbuf);
		}
		gentui_text(win, 7, 0, 0, listbuf, listlen);
		return;
	}

	if(old == 0)return;
	buf = old->buf;
	enq = old->enq;
	deq = old->deq;
	old->deq = enq;

	if(enq != deq)
	{
		if(enq > deq)queue_copy(buf+deq, enq-deq);
		else
		{
			queue_copy(buf+deq, 0x100000-deq);
			queue_copy(buf, enq);
		}
	}

	w = win->w;
	if(w > term.width)w = term.width;
	h = win->h;
	if(h > term.height)h = term.height;

	p = (void*)(win->buf);
	q = (void*)(term.buf);
	for(y=0;y<h;y++)
	{
		for(x=0;x<w;x++)
		{
			p[(win->w)*y + x] = q[(term.width)*(term.top+y) + x];
		}
	}
}
static void terminal_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	u8* p;
	int enq, deq;
	if((status == 0)&&(charlen == 0))
	{
		if(listlen == 0)
		{
			//say("terminal(%x,%x,%x)\n",win,act,sty);
			listlen = uart_list(listbuf);
		}
		say("%.*s", listlen, listbuf);
		return;
	}

	if(old == 0)return;
	p = old->buf;
	enq = old->enq;
	deq = old->deq;
	old->deq = enq;
	if(enq == deq)return;

	if(enq > deq)say("%.*s", enq-deq, p+deq);
	else say("%.*s%.*s", 0x100000-deq, p+deq, enq, p);
}
static void terminal_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)terminal_read_cli(win, act, sty);
	else if(fmt == __tui__)terminal_read_tui(win, act, sty);
	else if(fmt == __vbo__)terminal_read_vbo(win, act, sty);
	else if(fmt == __html__)terminal_read_html(win, act, sty);
	else terminal_read_pixel(win, act, sty);
}




static void terminal_write(struct event* ev)
{
	u64 tmp;
	u64 why = ev->why;
	if(ev->what == __uart__)
	{
		old = (void*)(ev->why);
		return;
	}

	if(ev->what == __kbd__)
	{
		if(status != 0)
		{
			if(why == 0x1b)tmp = 0x1b;
			else if(why == 0x25)tmp = 0x445b1b;
			else if(why == 0x26)tmp = 0x415b1b;
			else if(why == 0x27)tmp = 0x435b1b;
			else if(why == 0x28)tmp = 0x425b1b;
			else return;

			uart_write((void*)&tmp);
		}
		return;
	}

	if(ev->what == __char__)
	{
		if(status != 0)
		{
			uart_write((void*)ev);
			return;
		}
		if(why == 0x8)
		{
			if(charlen > 0)
			{
				say("\b \b");
				charlen--;
				charbuf[charlen] = 0;
			}
			return;
		}

		say("%c",why);
		if((why == 0xd)|(why == 0xa))
		{
			if(charlen == 0)
			{
				*(u32*)charbuf = hex32('C','O','M','8');
				charlen = 4;
			}

			uart_choose(charbuf);
			status = 1;
			return;
		}
		if(charlen < 31)
		{
			charbuf[charlen] = why;
			charlen++;
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
	//new.enq = 0;
	//new.deq = 0;
	//new.len = 0x100000;
	//new.buf = startmemory(new.len);

	term.curx = 0;
	term.cury = 0;
	term.left = 0;
	term.right = 0;
	term.top = 0;
	term.bottom = 0;
	term.vimw = 128;
	term.vimh = 25;
	term.width = 128;
	term.height = 25;
	term.bg = 0;
	term.fg = 7;
	term.len = 0x100000;
	term.buf = startmemory(term.len);
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
