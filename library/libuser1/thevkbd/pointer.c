#include "libuser.h"




void pointer_read_cli(struct arena* win, struct style* sty)
{
}
void pointer_read_tui(struct arena* win, struct style* sty)
{
}
void pointer_read_html(struct arena* win, struct style* sty)
{
}
void pointer_read_json(struct arena* win, struct style* sty)
{
}
void pointer_read_vbo(struct arena* win, struct style* sty)
{
	int j;
	float x0,y0,x1,y1;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	for(j=0;j<12;j++)
	{
		if(0 == win->input[j].z0)continue;

		x0 = (float)(win->input[j].x0) / (float)(win->width);
		x0 = x0*2 - 1.0;
		y0 = (float)(win->input[j].y0) / (float)(win->height);
		y0 = 1.0 - y0*2;
		x1 = (float)(win->input[j].xn) / (float)(win->width);
		x1 = x1*2 - 1.0;
		y1 = (float)(win->input[j].yn) / (float)(win->height);
		y1 = 1.0 - y1*2;

		vc[0] = x0;
		vc[1] = y0;
		vc[2] = -0.99;
		vr[0] = x1;
		vr[1] = y1;
		vr[2] = -0.99;
		carveline2d_arrow(win, 0xff00ff, vc, vr);
/*
		vc[0] = (x0+x1)/2;
		vc[1] = (y0+y1)/2;
		vc[2] = -0.99;
		vr[0] = (x1-x0)/2;
		vr[1] = 0.0;
		vr[2] = -0.99;
		vf[0] = 0.0;
		vf[1] = (y1-y0)/2;
		vf[2] = -0.99;
		carveline2d_rect(win, 0x00ff00, vc, vr, vf);
*/
	}
}
void pointer_read_pixel(struct arena* win, struct style* sty)
{
	int j;
	int x0,y0,x1,y1;
	for(j=0;j<12;j++)
	{
		if(0 == win->input[j].z0)continue;

		x0 = win->input[j].x0;
		y0 = win->input[j].y0;
		x1 = win->input[j].xn;
		y1 = win->input[j].yn;
		drawline_arrow(win, 0xff00ff, x0, y0, x1, y1);
		drawline_rect(win, 0x00ff00, x0, y0, x1, y1);
	}
}
static int pointer_sread(struct arena* win, struct style* sty, struct arena* cc, struct style* cf)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)pointer_read_cli(win, sty);
	else if(fmt == _tui_)pointer_read_tui(win, sty);
	else if(fmt == _html_)pointer_read_html(win, sty);
	else if(fmt == _json_)pointer_read_json(win, sty);
	else if(fmt == _vbo_)pointer_read_vbo(win, sty);
	else pointer_read_pixel(win, sty);
	return 0;
}
static int pointer_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	return 0;
}
static int pointer_cread(struct arena* cc, struct style* cf, struct arena* win, struct style* sty)
{
	return 0;
}
static int pointer_cwrite(struct arena* win, struct style* stack, struct event* ev)
{
	return 0;
}
static int pointer_stop(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
static int pointer_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
static int pointer_delete(struct arena* win)
{
	return 0;
}
static int pointer_create(struct arena* win, u8* str)
{
	return 0;
}




void pointer_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'o', 'i', 'n', 't', 'e', 'r', 0);

	p->oncreate = (void*)pointer_create;
	p->ondelete = (void*)pointer_delete;
	p->onstart  = (void*)pointer_start;
	p->onstop   = (void*)pointer_stop;
	p->onget    = (void*)pointer_cread;
	p->onpost   = (void*)pointer_cwrite;
	p->onread   = (void*)pointer_sread;
	p->onwrite  = (void*)pointer_swrite;
}