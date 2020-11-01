#include "libuser.h"




static void jkff_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
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
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
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




static void jkff_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		jkff_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void jkff_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	p->ontaking = (void*)jkff_taking;
	p->ongiving = (void*)jkff_giving;
}
