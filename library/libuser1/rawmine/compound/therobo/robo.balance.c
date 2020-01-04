#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void balance_draw_pixel(
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
static void balance_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* area)
{
	int j;
    vec3 tc,tr,tf,tt;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	carveline_prism4(ctx, 0xffffff, vc, vr, vf, vt);

	//board
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vt[j]/2;
		tr[j] = vr[j]*6/8;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/16;
	}
	quaternion_operation(tf, vr, -act->fy0);
	quaternion_operation(tt, vr, -act->fy0);
	carvesolid_prism4(ctx, 0x808080, tc,tr,tf,tt);

	//left wheel
	for(j=0;j<3;j++){
		tf[j] = vf[j]/2;
		tt[j] = vt[j]/2;
		tr[j] = vr[j]/8;
		tc[j] = vc[j] -vr[j]+tr[j] +tt[j];
	}
	carvesolid_cylinder(ctx, 0x404040, tc, tf, tt, tr);

	//right wheel
	for(j=0;j<3;j++){
		tf[j] = vf[j]/2;
		tt[j] = vt[j]/2;
		tr[j] = vr[j]/8;
		tc[j] = vc[j] +vr[j]-tr[j] +tt[j];
	}
	carvesolid_cylinder(ctx, 0x404040, tc, tf, tt, tr);
}
static void balance_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void balance_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void balance_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void balance_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




void balance_write_euler(struct entity* act, float* f)
{
	act->fx0 = f[0];
	act->fy0 = f[1];
	act->fz0 = f[2];
}




//-4: wnd, area
//-3: cam, 0
//-2: cam, 0
//-1: world, geom of cam
static void balance_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//scene -> balance
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)balance_draw_vbo(act,part, scn,geom, wrd,camg, wnd,area);
	}
}
static void balance_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	balance_write_euler(act, buf);
}
static void balance_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void balance_start(struct halfrel* self, struct halfrel* peer)
{
}




static void balance_modify(struct entity* act)
{
}
static void balance_search(struct entity* act)
{
}
static void balance_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void balance_create(struct entity* act)
{
	if(0 == act)return;
	act->fy0 = PI/4;
}




void balance_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('b','a','l','a','n','c','e', 0);

	p->oncreate = (void*)balance_create;
	p->ondelete = (void*)balance_delete;
	p->onsearch = (void*)balance_search;
	p->onmodify = (void*)balance_modify;

	p->onstart = (void*)balance_start;
	p->onstop  = (void*)balance_stop;
	p->onread  = (void*)balance_read;
	p->onwrite = (void*)balance_write;
}
