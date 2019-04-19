#include "libuser.h"
void* allocactor();
void* allocstyle();
int ui2d_cwrite(struct arena* win, struct style* sty, struct actor* sc, struct pinid* sf, int flag);




void corner_vbo_drag_lefttop(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	float x, float y)
{
	vec3 tc,tr,tf;

	tc[0] = x;
	tc[1] = y;
	tc[2] = -0.99;
	tr[0] = 1.0+tc[0];
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = tc[1]-1.0;
	tf[2] = 0.0;
	carveline2d_rect(win, 0xffffff, tc, tr, tf);
	carvesolid2d_circle(win, 0xffffff, tc, tr, tf);
}
void corner_vbo_drag_righttop(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	float x, float y)
{
	vec3 tc,tr,tf;

	tc[0] = x;
	tc[1] = y;
	tc[2] = -0.99;

	tr[0] = -1.0;
	tr[1] = -1.0;
	tr[2] = -0.99;
	tf[0] = 1.0;
	tf[1] = -1.0;
	tf[2] = -0.99;
	carvesolid2d_triangle(win, 0x0000ff, tc, tr, tf);

	tf[0] = -1.0;
	tf[1] = 1.0;
	tf[2] = -0.99;
	carvesolid2d_triangle(win, 0x00ffff, tc, tr, tf);

	tr[0] = 1.0;
	tr[1] = 1.0;
	tr[2] = -0.99;
	carvesolid2d_triangle(win, 0xff0000, tc, tr, tf);

	tf[0] = 1.0;
	tf[1] = -1.0;
	tf[2] = -0.99;
	carvesolid2d_triangle(win, 0xffff00, tc, tr, tf);
}
void corner_vbo_drag_leftbot(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	float x, float y)
{
	vec3 tc, tr, tf;

	tc[0] = x;
	tc[1] = 0.0;
	tc[2] = -0.9;
	tr[0] = 1.0+tc[0];
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0;
	tf[2] = 0.0;
	carvesolid2d_rect(win, 0x404040, tc, tr, tf);

	tc[0] = 0.0;
	tc[1] = y;
	tr[0] = 1.0;
	tr[1] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0+tc[1];
	carvesolid2d_rect(win, 0x404040, tc, tr, tf);
}
void corner_vbo_drag_rightbot(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	float x0, float y0, float xn, float yn)
{
	vec3 tc, tr, tf;

	tr[0] = x0;
	tr[1] = -1.0;
	tr[2] = -0.9;
	tf[0] = 1.0;
	tf[1] = y0;
	tf[2] = -0.9;
	tc[0] = xn;
	tc[1] = yn;
	tc[2] = -0.9;
	carvesolid2d_triangle(win, 0x808080, tc, tr, tf);
	tc[0] = 1.0;
	tc[1] = -1.0;
	carvesolid2d_triangle(win, 0x000000, tc, tr, tf);
}
void corner_vbo_drag(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j;
	float w,h,c;
	float x0,y0,xn,yn;

	w = win->width;
	h = win->height;
	if(w<h)c = w / 32;
	else c = h / 32;

	if(win->input[10].z0)j = 10;
	else if(win->input[0].z0)j = 0;
	else return;

	x0 = win->input[j].x0;
	y0 = win->input[j].y0;
	xn = win->input[j].xn;
	yn = win->input[j].yn;

	if(y0 < c){
		if(x0 < c){
			corner_vbo_drag_lefttop(
				act, pin, win, sty,
				xn/w - 1.0, 1.0 - yn/h
			);
		}
		else if(x0+c > w){
			corner_vbo_drag_righttop(
				act, pin, win, sty,
				2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
	else if(y0+c > h){
		if(x0 < c){
			corner_vbo_drag_leftbot(
				act, pin, win, sty,
				xn/w - 1.0, -yn/h
			);
		}
		else if(x0+c > w){
			x0 = ((h-yn)*(h-yn)/(xn-w) + (w+xn)) / 2.0;
			x0 = 2*x0/w - 1.0;
			y0 = ((xn-w)*(xn-w)/(yn-h) + (yn+h)) / 2.0;
			y0 = 1.0 - 2*y0/h;
			corner_vbo_drag_rightbot(
				act, pin, win, sty,
				x0, y0, 2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
}




void corner_vbo_circle(
	struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, int t)
{
	vec3 tr, tf;
	if(1 == t){
		carvesolid2d_circle(win, rgb, vc, vr, vf);
		return;
	}

	tr[0] = vr[0]*t;
	tr[1] = vr[1]*t;
	tr[2] = vr[2]*t;
	tf[0] = vf[0]*t;
	tf[1] = vf[1]*t;
	tf[2] = vf[2]*t;
	carvesolid2d_circle(win, rgb, vc, tr, tf);
}
void corver_vbo_hover(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j;
	float w,h,c;
	float x0,y0,xn,yn;
	vec3 vc;
	vec3 vr;
	vec3 vf;

	w = win->width;
	h = win->height;
	if(w<h)c = w / 32;
	else c = h / 32;

	x0 = win->input[10].x0;
	y0 = win->input[10].y0;
	xn = win->input[10].xn;
	yn = win->input[10].yn;

	vr[0] = 2*(float)c / (float)w;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 2*(float)c / (float)h;
	vf[2] = 0.0;

	vc[0] = 1.0;
	vc[1] = -1.0;
	vc[2] = -0.99;
	if((xn+c > w)&&(yn+c > h))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0x0000ff, vc, vr, vf, j);

	vc[0] = -1.0;
	if((xn < c)&&(yn+c > h))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0xff0000, vc, vr, vf, j);

	vc[1] = 1.0;
	if((xn < c)&&(yn < c))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0x00ffff, vc, vr, vf, j);

	vc[0] = 1.0;
	if((xn+c > w)&&(yn < c))j = 4;
	else j = 1;
	corner_vbo_circle(win, 0xffff00, vc, vr, vf, j);
}




void corner_vbo_popup(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int j;
	vec3 tc,tr,tf;
	struct relation* rel;
	struct style* st;

	rel = act->orel0;
	if(0 == rel)return;

	st = (void*)(rel->srcfoot);
	if(0 == st)return;
	if('#' == st->uc[3])return;

	tr[0] = 0.25;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.5;
	tf[2] = 0.0;
	tc[0] = -0.75;
	tc[1] = -0.5;
	tc[2] = -0.9;
	carveopaque2d_rect(win, 0xffffff, tc, tr, tf);

	tr[0] = 0.24;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0/18;
	tf[2] = 0.0;
	tc[2] = -0.9;
	for(j=0;j<8;j++){
		tc[0] = -0.75;
		tc[1] = (-1-j*2)/16.0;
		carveopaque2d_rect(win, 0x000080, tc, tr, tf);
		carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"haha", 0);
	}
}




void corner_read_vbo(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	corver_vbo_hover(act, pin, win, sty);

	corner_vbo_drag(act, pin, win, sty);

	corner_vbo_popup(act, pin, win, sty);
}
void corner_read_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int x,y,m,n,c;
	int w = win->width;
	int h = win->height;
	if(w<h)c = w>>6;
	else c = h>>6;

	x = win->input[10].x0;
	y = win->input[10].y0;
	m = win->input[10].xn;
	n = win->input[10].yn;

	if((m   < c)&&(n+c > h))drawsolid_circle(win, 0x0000ff, 0, h, c*4);
	else drawsolid_circle(win, 0x0000ff, 0, h, c);
	if((m+c > w)&&(n+c > h))drawsolid_circle(win, 0xff0000, w, h, c*4);
	else drawsolid_circle(win, 0xff0000, w, h, c);
	if((m   < c)&&(n   < c))drawsolid_circle(win, 0x00ffff, 0, 0, c*4);
	else drawsolid_circle(win, 0x00ffff, 0, 0, c);
	if((m+c > w)&&(n   < c))drawsolid_circle(win, 0xffff00, w, 0, c*4);
	else drawsolid_circle(win, 0xffff00, w, 0, c);

	if(win->input[0].z0)
	{
		m = win->input[0].x0;
		n = win->input[0].y0;
		x = win->input[0].xn;
		y = win->input[0].yn;
	}
	else if(win->input[10].z0)
	{
		m = win->input[10].x0;
		n = win->input[10].y0;
		x = win->input[10].xn;
		y = win->input[10].yn;
	}
	else return;

	if(n < h)
	{
		if(m < c)
		{
		}
		else if(m+c > w)
		{
		}
	}
	else if(n+c > h)
	{
		if(m < c)
		{
			drawsolid_rect(win, 0x404040, 0, 0, x, h);
			drawsolid_rect(win, 0x404040, 0, y, w, h);
		}
		else if(m+c > w)
		{
			m = ((h-y)*(h-y)/(x-w) + (w+x)) / 2;
			n = ((x-w)*(x-w)/(y-h) + (y+h)) / 2;
			drawsolid_triangle(win, 0x808080, m, h, w, n, x, y);
			drawsolid_triangle(win, 0x000000, m, h, w, n, w, h);
		}
	}
}
static int corner_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	switch(win->fmt)
	{
		case _vbo_:corner_read_vbo(act, pin, win, sty);break;
		default:corner_read_pixel(act, pin, win, sty);
	}
	return 0;
}




static int corner_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int j,k,x,y;
	int w,h,id,ret;
	struct relation* rel;
	struct arena* tmp;
	struct style* st;
	u64 why = ev->why;
	u64 what = ev->what;

	if('p' == (what&0xff))
	{
		w = win->width;
		h = win->height;
		if(w<h)ret = w>>4;
		else ret = h>>4;

		id = (why>>48)&0xffff;
		if('l' == id)id = 10;
		if((0 != id)&&(10 != id))return 0;
		j = win->input[id].x0;
		k = win->input[id].y0;

		if(0x2d70 == what)
		{
			//open or close vkbd
			x = why&0xffff;
			y = (why>>16)&0xffff;

			if(y+ret > h)
			{
				if(x < ret)
				{
					rel = act->orel0;
					if(rel){
						st = (void*)(rel->srcfoot);
						if(st){
							j = st->uc[3];
							if('#' == j)j = 0;
							else j = '#';
							st->uc[3] = j;
						}
					}
					return 1;
				}
				if(x+ret > w)
				{
					return 1;
				}
			}
			if(y < ret)
			{
				if(x < ret)
				{
					rel = act->irel0;
					if(rel)
					{
						tmp = (void*)(rel->srcchip);
						if(tmp)
						{
							ui2d_cwrite(tmp, sty, act, pin, '#');
						}
					}
					return 1;
				}
				if(x+ret > w)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
static int corner_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int corner_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int corner_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int corner_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int corner_delete(struct arena* win)
{
	return 0;
}
static int corner_create(struct actor* act, u8* str)
{
	struct actor* ac = allocactor();
	struct style* sty = allocstyle();
	sty->uc[3] = '#';
	relationcreate(ac, 0, _act_, act, sty, _act_);
	return 0;
}




void corner_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'o', 'r', 'n', 'e', 'r', 0, 0);

	p->oncreate = (void*)corner_create;
	p->ondelete = (void*)corner_delete;
	p->onstart  = (void*)corner_start;
	p->onstop   = (void*)corner_stop;
	p->oncread  = (void*)corner_cread;
	p->oncwrite = (void*)corner_cwrite;
	p->onsread  = (void*)corner_sread;
	p->onswrite = (void*)corner_swrite;
}
