#include "libuser.h"
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void nor_draw_pixel(
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
static void nor_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	//gl41line_rect(ctx, 0x404040, vc, vr, vf);

	//vcc
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j];
		tr[j] = vc[j] +vr[j] +vf[j];
	}
	gl41line(ctx, 0xff0000, tc, tr);

	//gnd
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j];
		tr[j] = vc[j] +vr[j] -vf[j];
	}
	gl41line(ctx, 0x0000ff, tc, tr);

	//-
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j];
		tr[j] = tc[j] +vf[j]/4;
	}
	gl41line(ctx, 0x0000ff, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 -vf[j];
		tr[j] = tc[j] +vf[j]/4;
	}
	gl41line(ctx, 0x0000ff, tc,tr);


	u8 pstatus[2];
	u8 nstatus[2];
	if(act->ix0){pstatus[0] = 0;nstatus[0] = 1;}
	else        {pstatus[0] = 1;nstatus[0] = 0;}
	if(act->iy0){pstatus[1] = 0;nstatus[1] = 1;}
	else        {pstatus[1] = 1;nstatus[1] = 0;}
	//say("%d,%d,%d,%d,%d,%d\n",act->ix0,act->iy0, pstatus[0],pstatus[1], nstatus[0],nstatus[1]);

	u32 xcolor = act->ix0 ? 0xff0000 : 0x0000ff;
	u32 ycolor = act->iy0 ? 0xff0000 : 0x0000ff;
	u32 ocolor = act->iz0 ? 0xff0000 : 0x0000ff;
	u32 pcolor[2] = {0xffffff, 0xffffff};
	u32 ncolor[2] = {0xffffff, 0xffffff};
	if(nstatus[0])ncolor[0] = 0x0000ff;
	if(nstatus[1])ncolor[1] = 0x0000ff;
	if(pstatus[0])pcolor[0] = 0xff0000;
	if(pstatus[0]&&pstatus[1])pcolor[1] = 0xff0000;

	//p1
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, xcolor, pcolor[0], tc,tr,tf,vt);

	//p2
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, ycolor, pcolor[1], tc,tr,tf,vt);

	//n1
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, xcolor, ncolor[0], tc,tr,tf,vt);

	//n2
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, ycolor, ncolor[1], tc,tr,tf,vt);

	//a
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j]*3/4;
		tr[j] = tc[j] +vr[j];
	}
	gl41line(ctx, xcolor, tc,tr);

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j]*1/4;
		tr[j] = tc[j] +vr[j];
	}
	gl41line(ctx, ycolor, tc,tr);

	//o
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]/2;
		tr[j] = vc[j] + vr[j];
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2;
		tr[j] = tc[j] -vf[j]/4;
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] -vf[j]/4;
	}
	gl41line(ctx, ocolor, tc,tr);
}
static void nor_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nor_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nor_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nor_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void nor_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		nor_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void nor_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	u8 tmp;
	say("@norgate_write:%x\n",buf[0]);

	if('a' == foot){
		if('0' == buf[0])ent->ix0 = 0;
		else if('1' == buf[0])ent->ix0 = 1;
		else return;
	}
	else if('b' == foot){
		if('0' == buf[0])ent->iy0 = 0;
		else if('1' == buf[0])ent->iy0 = 1;
		else return;
	}
	else return;

	ent->iz0 = !(ent->ix0 | ent->iy0);
	tmp = ent->iz0 + 0x30;
	give_data_into_peer(ent,'o', stack,sp, 0,0, &tmp,1);
}
static void nor_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void nor_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void nor_search(struct entity* act, u8* buf)
{
}
static void nor_modify(struct entity* act, u8* buf)
{
}
static void nor_delete(struct entity* act, u8* buf)
{
}
static void nor_create(struct entity* act, u8* buf)
{
	act->ix0 = getrandom()&1;
	act->iy0 = getrandom()&1;
	act->iz0 = !(act->ix0 | act->iy0);
}




void nor_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('n','o','r', 0);

	p->oncreate = (void*)nor_create;
	p->ondelete = (void*)nor_delete;
	p->onsearch = (void*)nor_search;
	p->onmodify = (void*)nor_modify;

	p->onlinkup = (void*)nor_linkup;
	p->ondiscon = (void*)nor_discon;
	p->ontaking = (void*)nor_taking;
	p->ongiving = (void*)nor_giving;
}
