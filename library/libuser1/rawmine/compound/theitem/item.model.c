#include "libuser.h"
int windowread(int type, void* buf);
int windowwrite(int type, void* buf);
void parsevertfromstl(struct glsrc* ctx, struct fstyle* sty, u8* buf, int len);




void sty_sty_mat(struct fstyle* src, struct fstyle* dst, mat4 mat)
{
	float x,y,z,max;
	float nr, nf, nu;
	mat4 tmp;

	//move: (0,0,0) -> dst
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = dst->vc[0];
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = dst->vc[1];
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = dst->vc[2];
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	//rotate: 1 -> dst

	//scale: 1 -> dst
	x = dst->vr[0];
	y = dst->vr[1];
	z = dst->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = dst->vf[0];
	y = dst->vf[1];
	z = dst->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = dst->vt[0];
	y = dst->vt[1];
	z = dst->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nr;
	if(max < nf)max = nf;
	if(max < nu)max = nu;

	tmp[0][0] = max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//scale: src -> 1
	x = src->vr[0];
	y = src->vr[1];
	z = src->vr[2];
	nr = squareroot(x*x+y*y+z*z);

	x = src->vf[0];
	y = src->vf[1];
	z = src->vf[2];
	nf = squareroot(x*x+y*y+z*z);

	x = src->vt[0];
	y = src->vt[1];
	z = src->vt[2];
	nu = squareroot(x*x+y*y+z*z);

	max = nr;
	if(max < nf)max = nf;
	if(max < nu)max = nu;

	tmp[0][0] = 1.0 / max;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = 0.0;
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0 / max;
	tmp[1][2] = 0.0;
	tmp[1][3] = 0.0;
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0 / max;
	tmp[2][3] = 0.0;
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	//rotate: src -> 1

	//move: src -> (0,0,0)
	tmp[0][0] = 1.0;
	tmp[0][1] = 0.0;
	tmp[0][2] = 0.0;
	tmp[0][3] = - src->vc[0];
	tmp[1][0] = 0.0;
	tmp[1][1] = 1.0;
	tmp[1][2] = 0.0;
	tmp[1][3] = - src->vc[1];
	tmp[2][0] = 0.0;
	tmp[2][1] = 0.0;
	tmp[2][2] = 1.0;
	tmp[2][3] = - src->vc[2];
	tmp[3][0] = 0.0;
	tmp[3][1] = 0.0;
	tmp[3][2] = 0.0;
	tmp[3][3] = 1.0;
	mat4_multiply(mat, tmp);

	mat4_transpose(mat);
}



static void model_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float* p;
	float f;
	float v[3][3];
	int j,ret;
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
/*
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
	if(0 == (act->buf))return;
	if(0 == (act->len))return;

	if((act->width) > (act->height))f = 2.0*ww/(act->width);
	else f = 2.0*hh/(act->height);

	ret = *(u32*)((act->buf)+80);
	ret = ret % ((0x800000-0x84)/50);
	for(j=0;j<ret;j++)
	{
		p = (void*)(act->buf) + 84 + j*50;

		v[0][0] = cx + (p[ 3]-(act->cx))*f;
		v[0][1] = cy + (p[ 4]-(act->cy))*f;
		v[1][0] = cx + (p[ 6]-(act->cx))*f;
		v[1][1] = cy + (p[ 7]-(act->cy))*f;
		v[2][0] = cx + (p[ 9]-(act->cx))*f;
		v[2][1] = cy + (p[10]-(act->cy))*f;

		drawline(win, 0xffffff, v[0][0], v[0][1], v[1][0], v[1][1]);
		drawline(win, 0xffffff, v[0][0], v[0][1], v[2][0], v[2][1]);
		drawline(win, 0xffffff, v[1][0], v[1][1], v[2][0], v[2][1]);
	}
*/
}/*
static void model_draw_vbo2d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	if(act->buf == 0)return;
	if(0 == sty)sty = defaultstyle_vbo2d();

	struct glsrc* src = (void*)(pin->foot[0]);
	sty_sty_mat(&act->target, &sty->f, (void*)src->arg[0].data);
}*/
static void model_draw_vbo3d(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* none)
{
	if(0 == act)return;
	if(act->buf == 0)return;

	struct glsrc* src = act->buf;
	sty_sty_mat(&part->fs, &geom->fs, (void*)src->arg[0].data);
}
static void model_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void model_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void model_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void model_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("model(%x,%x,%x)\n",win,act,sty);
}
static void model_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)model_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)model_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)model_draw_html(act, pin, win, sty);
	else if(fmt == _json_)model_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)model_draw_vbo2d(act, pin, win, sty);
		//else model_draw_vbo3d(act, pin, win, sty);
	}
	else model_draw_pixel(act, pin, win, sty);
}
static void model_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	int j,ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(0x2d70 == type)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	else if(_drag_ == type)
	{
		char buffer[0x1000];
		ret = windowread(type, buffer);
		say("%s", buffer);

		for(j=0;j<ret;j++)
		{
			if(buffer[j] < 0x20)
			{
				buffer[j] = 0;
				break;
			}
		}
		//entitycreatefromfile(act, buffer);
	}
}




static void model_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//world -> model
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		if('v' == len){
			model_draw_vbo3d(act,part, win,geom, wrd,camg, wnd,area);
		}
	}
}
static void model_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//model_event(act, pin, win, sty, ev, 0);
}
static void model_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void model_start(struct halfrel* self, struct halfrel* peer)
{
	struct glsrc* src;
	struct style* pin = (void*)(self->foot);
	struct entity* act = (void*)(self->chip);
	if(0 == act)return;
	if(0 == pin)return;

	//vertex
	src = act->buf;
	parsevertfromstl(src, &pin->fs, src->vbuf, src->vbuf_len);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*6;
	src->vbuf_enq = 42;

	pin->data[0] = (u64)(act->buf);
	say("@model_start:%llx, %llx\n", pin->data[0], pin->data[1]);
}




static void model_search(struct entity* act)
{
}
static void model_modify(struct entity* act)
{
}
static void model_delete(struct entity* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void model_create(struct entity* act, void* str)
{
	int j;
	float* tmp;
	struct glsrc* src;
	if(0 == act)return;

	src = act->buf = memorycreate(0x200, 0);
	if(0 == src)return;

	//
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/model/vert.glsl", 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/model/frag.glsl", 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//argument
	src->arg[0].fmt = 'm';
	src->arg[0].name = "objmat";
	src->arg[0].data = memorycreate(4*4*4, 0);

	src->arg[1].fmt = 'v';
	src->arg[1].name = "KA";
	tmp = src->arg[1].data = memorycreate(4*4, 0);
	tmp[0] = tmp[1] = tmp[2] = 0.231250;

	src->arg[2].fmt = 'v';
	src->arg[2].name = "KD";
	tmp = src->arg[2].data = memorycreate(4*4, 0);
	tmp[0] = tmp[1] = tmp[2] = 0.277500;

	src->arg[3].fmt = 'v';
	src->arg[3].name = "KS";
	tmp = src->arg[3].data = memorycreate(4*4, 0);
	tmp[0] = tmp[1] = tmp[2] = 0.773911;

	//vertex
	src->vbuf_len = 0x1000000;
	src->vbuf = memorycreate(src->vbuf_len, 0);
	if(0 == str)str = "datafile/stl/bunny-lowpoly.stl";
	j = openreadclose(str, 0, src->vbuf, src->vbuf_len);
}




void model_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'o', 'd', 'e', 'l', 0, 0, 0);

	p->oncreate = (void*)model_create;
	p->ondelete = (void*)model_delete;
	p->onsearch = (void*)model_search;
	p->onmodify = (void*)model_modify;

	p->onstart = (void*)model_start;
	p->onstop  = (void*)model_stop;
	p->onread  = (void*)model_read;
	p->onwrite = (void*)model_write;
}
