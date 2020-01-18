#include "libuser.h"




static void jkff_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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
}
static void jkff_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_prism4(ctx, 0xffffff, vc, vr, vf, vu);
}
static void jkff_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void jkff_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void jkff_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void jkff_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void jkff_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> jkff
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)jkff_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void jkff_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void jkff_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void jkff_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void jkff_search(struct entity* act, u8* buf)
{
}
static void jkff_modify(struct entity* act, u8* buf)
{
}
static void jkff_delete(struct entity* act, u8* buf)
{
}
static void jkff_create(struct entity* act, u8* buf)
{
}




void jkff_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('j','k','f','f');

	p->oncreate = (void*)jkff_create;
	p->ondelete = (void*)jkff_delete;
	p->onsearch = (void*)jkff_search;
	p->onmodify = (void*)jkff_modify;

	p->onlinkup = (void*)jkff_linkup;
	p->ondiscon = (void*)jkff_discon;
	p->onread  = (void*)jkff_read;
	p->onwrite = (void*)jkff_write;
}
