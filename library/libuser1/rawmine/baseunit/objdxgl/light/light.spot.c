#include "libuser.h"




static void spotlight_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spotlight_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tt;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	tt[0] = - vf[0];
	tt[1] = - vf[1];
	tt[2] = - vf[2];
	carvesolid_cone(win, 0xffff00, vc, vr, tt);
}
static void spotlight_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spotlight_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spotlight_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spotlight_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void spotlight_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)spotlight_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)spotlight_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)spotlight_draw_html(act, pin, win, sty);
	else if(fmt == _json_)spotlight_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)spotlight_draw_vbo(act, pin, win, sty);
	else spotlight_draw_pixel(act, pin, win, sty);
}




void spotlight_light(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	struct glsrc* src = win->gl_light;

	src->arg_fmt[0] = 'v';
	src->arg_name[0] = "sunxyz";
	src->arg_data[0] = sty->fs.vc;
}
static void spotlight_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;

	if(ctx){
		switch(ctx->type){
			case _gl41data_:{
				spotlight_light(act,pin,ctx,sty);
				spotlight_draw_vbo(act,pin,ctx,sty);
			}
		}
	}
}
static void spotlight_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void spotlight_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void spotlight_start(struct halfrel* self, struct halfrel* peer)
{
}




static void spotlight_search(struct actor* act)
{
}
static void spotlight_modify(struct actor* act)
{
}
static void spotlight_delete(struct actor* act)
{
}
static void spotlight_create(struct actor* act, void* str)
{
}




void spotlight_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','p','o','t','l', 'i', 't', 0);

	p->oncreate = (void*)spotlight_create;
	p->ondelete = (void*)spotlight_delete;
	p->onsearch = (void*)spotlight_search;
	p->onmodify = (void*)spotlight_modify;

	p->onstart = (void*)spotlight_start;
	p->onstop  = (void*)spotlight_stop;
	p->onread  = (void*)spotlight_read;
	p->onwrite = (void*)spotlight_write;
}
