#include "libuser.h"
#define level 4
void rubikscube_generate(void*, int);
void rubikscube_solve(void*, int);




//green, blue, red, orange, yellow, white
u32 rubikcolor[6] = {0x00ff00,0x0000ff,0xff0000,0xfa8010,0xffff00,0xffffff};




//left, right, near, far, bottom, upper
int rubikscube_import(char* file, u8 (*buf)[4][4])
{
	int x,y;
	int j,t;
	u8 tmp[0x100];
	j = openreadclose(file, 0, tmp, 0x100);
	//printmemory(tmp, 0x100);
	if(j<=0)return 0;

	x = y = 0;
	for(j=0;j<0x100;j++)
	{
		if(0xd == tmp[j])continue;
		if(0xa == tmp[j]){
			x = 0;y += 1;
			if(y >= 6)break;
			continue;
		}
		if(x<16)
		{
			t = tmp[j];
			switch(t)
			{
				case 'g':t = 0x30;break;
				case 'b':t = 0x31;break;
				case 'r':t = 0x32;break;
				case 'o':t = 0x33;break;
				case 'y':t = 0x34;break;
				case 'w':t = 0x35;break;
			}
			if((t >= 0x30)&&(t <= 0x35))
			{
				buf[y][x/4][x%4] = t - 0x30;
				x++;
			}
		}
	}

	//printmemory(buf, 81);
	return 1;
}




static void rubikscube_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u32 bg;
	int x, y, cx, cy, ww, hh;
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
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	u8 (*buf)[4][4] = act->buf;
	if(0 == buf)return;

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			bg = rubikcolor[(buf[0][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//right
			bg = rubikcolor[(buf[1][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+4*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+4*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			bg = rubikcolor[(buf[2][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//far
			bg = rubikcolor[(buf[3][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+6*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+6*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			bg = rubikcolor[(buf[4][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+4*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+4*level)*hh/4/level
			);

			//upper
			bg = rubikcolor[(buf[5][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2)*hh/4/level
			);
		}
	}
}
static void rubikscube_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int x,y,rgb;
	vec3 f;
	vec3 tc, tr, tf, tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	u8 (*buf)[4][4] = act->buf;
	if(0 == buf)return;

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			f[0] = -1.0;
			f[1] = 1.0 - (2.0*x+1.0)/level;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = -vf[0] / (level+0.5);
			tr[1] = -vf[1] / (level+0.5);
			tr[2] = -vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[0][y][x])%6];
			carvesolid_rect(win, rgb, tc, tr, tf);

			//right
			f[0] = 1.0;
			f[1] = (2.0*x+1.0)/level - 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vf[0] / (level+0.5);
			tr[1] = vf[1] / (level+0.5);
			tr[2] = vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[1][y][x])%6];
			carvesolid_rect(win, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = -1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[2][y][x])%6];
			carvesolid_rect(win, rgb, tc, tr, tf);

			//far
			f[0] = 1.0 - (2.0*x+1.0)/level;
			f[1] = 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = -vr[0] / (level+0.5);
			tr[1] = -vr[1] / (level+0.5);
			tr[2] = -vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[3][y][x])%6];
			carvesolid_rect(win, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = (2.0*y+1.0)/level - 1.0;
			f[2] = -1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = -vf[0] / (level+0.5);
			tf[1] = -vf[1] / (level+0.5);
			tf[2] = -vf[2] / (level+0.5);
			rgb = rubikcolor[(buf[4][y][x])%6];
			carvesolid_rect(win, rgb, tc, tr, tf);

			//upper
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = (2.0*y+1.0)/level - 1.0;
			f[2] = 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vf[0] / (level+0.5);
			tf[1] = vf[1] / (level+0.5);
			tf[2] = vf[2] / (level+0.5);
			rgb = rubikcolor[(buf[5][y][x])%6];
			carvesolid_rect(win, rgb, tc, tr, tf);
		}
	}
}
static void rubikscube_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void rubikscube_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"rubik\" style=\"width:50%%;height:100px;float:left;background-color:#404040;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");
	win->len = len;
}
static void rubikscube_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void rubikscube_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	say("rubik(%x,%x,%x)\n",win,act,sty);
}
static void rubikscube_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rubikscube_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)rubikscube_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)rubikscube_draw_html(act, pin, win, sty);
	else if(fmt == _json_)rubikscube_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)rubikscube_draw_vbo(act, pin, win, sty);
	else rubikscube_draw_pixel(act, pin, win, sty);
}
static void rubikscube_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	if(ev->what == _kbd_)
	{
	}
	else if(ev->what == _char_)
	{
	}
}




static void rubikscube_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	rubikscube_draw(act, pin, win, sty);
}
static void rubikscube_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	rubikscube_event(act, pin, win, sty, ev, 0);
}
static void rubikscube_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void rubikscube_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void rubikscube_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void rubikscube_start(struct halfrel* self, struct halfrel* peer)
{
}
static void rubikscube_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)
	{
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void rubikscube_create(struct actor* act, void* str)
{
	int ret;
	void* buf;
	if(0 == act)return;
	if(act->buf)return;
//printmemory(str,4);

	//malloc
	buf = memorycreate(6 * level * level);
	if(0 == buf)return;

	//read
	ret = 0;
	if(str)ret = rubikscube_import(str, buf);
	if((0==str)|(ret<=0))rubikscube_generate(buf, level);

	//
	for(ret=0;ret<6;ret++)printmemory(buf + level*level*ret, level*level);
	act->buf = buf;
}




void rubikscube_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 'u', 'b', 'i', 'k', 0, 0, 0);

	p->oncreate = (void*)rubikscube_create;
	p->ondelete = (void*)rubikscube_delete;
	p->onstart  = (void*)rubikscube_start;
	p->onstop   = (void*)rubikscube_stop;
	p->oncread  = (void*)rubikscube_cread;
	p->oncwrite = (void*)rubikscube_cwrite;
	p->onsread  = (void*)rubikscube_sread;
	p->onswrite = (void*)rubikscube_swrite;
}
