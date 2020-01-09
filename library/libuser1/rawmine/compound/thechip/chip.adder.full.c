#include "libuser.h"
void carveline_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void carveline_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void fulladd_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y,j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	carveline_rect(ctx, 0x404040, vc, vr, vf);
/*
	//grid
	for(j=0;j<3;j++){
		tr[j] = vr[j]/8.1;
		tf[j] = vf[j]/8.1;
	}
	for(y=0;y<8;y++){
		for(x=0;x<8;x++){
			for(j=0;j<3;j++){
				tc[j] = vc[j] +vr[j]*(2*x-7)/8 +vf[j]*(2*y-7)/8;
			}
			carveline_rect(ctx, 0x404040, tc,tr,tf);
		}
	}
*/
	//ia
	u32 iacolor = act->ix0 ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*2/8;
		tr[j] = tc[j] +vr[j]*3/4;
	}
	carveline(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]*2/8;
		tr[j] = tc[j] -vf[j]*3/8;
	}
	carveline(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 -vf[j]*2/8;
		tr[j] = tc[j] -vf[j]*5/8;
	}
	carveline(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 -vf[j]*7/8;
		tr[j] = tc[j] +vr[j]*7/4;
	}
	carveline(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*7/8 -vf[j]*7/8;
		tr[j] = tc[j] +vf[j]*3/8;
	}
	carveline(ctx, iacolor, tc,tr);

	//ib
	u32 ibcolor = act->iy0 ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]/8;
		tr[j] = tc[j] +vr[j]*7/8;
	}
	carveline(ctx, ibcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/4 -vf[j]/8;
		tr[j] = tc[j] -vf[j]*4/8;
	}
	carveline(ctx, ibcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 -vf[j]*1/8;
		tr[j] = tc[j] -vf[j]*1/8;
	}
	carveline(ctx, ibcolor, tc,tr);

	//ic
	u32 iccolor = act->iz0 ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j]*6/8;
		tr[j] = tc[j] +vr[j]*6/8;
	}
	carveline(ctx, iccolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/4 +vf[j]*6/8;
		tr[j] = tc[j] -vf[j]*5/8;
	}
	carveline(ctx, iccolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/4 +vf[j]*1/8;
		tr[j] = tc[j] +vr[j]*9/8;
	}
	carveline(ctx, iccolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*7/8 +vf[j]*1/8;
		tr[j] = tc[j] -vf[j]*3/8;
	}
	carveline(ctx, iccolor, tc,tr);

	//oc
	u32 occolor = act->iyn ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j] -vf[j]*4/8;
		tr[j] = tc[j] -vr[j]/8;
	}
	carveline(ctx, occolor, tc,tr);

	//os
	u32 oscolor = act->ixn ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j] +vf[j]*4/8;
		tr[j] = tc[j] -vr[j]*5/8;
	}
	carveline(ctx, oscolor, tc,tr);

	//bot,0
	u32 m0p = (0 == act->iy0) ? iacolor : 0xffffff;
	u32 m0n = (1 == act->iy0) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	carveline_pmos(ctx, ibcolor, m0p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*5/8 -vf[j]*5/8;
	carveline_nmos(ctx, ibcolor, m0n, tc,tr,tf,vt);

	//mos0.o -> mos1.s
	u32 bot0to1 = 0xffffff;
	if(0xffffff != m0p)bot0to1 = m0p;
	if(0xffffff != m0n)bot0to1 = m0n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 -vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*4/8 -vf[j]*2/8;
	}
	carveline(ctx, bot0to1, tc,tr);

	//bot,1
	u32 m1p = (0 == act->ix0) ? ibcolor : 0xffffff;
	u32 m1n = (1 == act->ix0) ? bot0to1 : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	carveline_pmos(ctx, iacolor, m1p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*1/8 -vf[j]*5/8;
	carveline_nmos(ctx, iacolor, m1n, tc,tr,tf,vt);

	//mos1.o -> mos2.g
	u32 bot1to2 = 0xffffff;
	if(0xffffff != m1p)bot1to2 = m1p;
	if(0xffffff != m1n)bot1to2 = m1n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 -vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*3/8;
	}
	carveline(ctx, bot1to2, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*2/8 -vf[j]*3/8;
		tr[j] = tc[j] -vf[j]*2/8;
	}
	carveline(ctx, bot1to2, tc,tr);

	//bot,2
	u32 m2p = (0x0000ff == bot1to2) ? 0xff0000 : 0xffffff;
	u32 m2n = (0xff0000 == bot1to2) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*3/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	carveline_pmos(ctx, bot1to2, m2p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*3/8 -vf[j]*5/8;
	carveline_nmos(ctx, bot1to2, m2n, tc,tr,tf,vt);

	//mos2.o -> mos3.g
	u32 bot2to3 = 0xffffff;
	if(0xffffff != m2p)bot2to3 = m2p;
	if(0xffffff != m2n)bot2to3 = m2n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*3/8 -vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*3/8;
	}
	carveline(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*6/8 -vf[j]*3/8;
		tr[j] = tc[j] -vf[j]*2/8;
	}
	carveline(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] -vf[j]/2;
	}
	carveline(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] -vr[j]*5/4;
	}
	carveline(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/4;
		tr[j] = tc[j] +vf[j]*7/8;
	}
	carveline(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/4 +vf[j]*7/8;
		tr[j] = tc[j] +vr[j]*5/8;
	}
	carveline(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 +vf[j]*7/8;
		tr[j] = tc[j] -vf[j]*1/8;
	}
	carveline(ctx, bot2to3, tc,tr);

	//bot,3
	u32 m3p = (0x0000ff == bot2to3) ? iccolor : 0xffffff;
	u32 m3n = (0xff0000 == bot2to3) ? iacolor : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*7/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	carveline_pmos(ctx, bot2to3, m3p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*7/8 -vf[j]*5/8;
	carveline_nmos(ctx, bot2to3, m3n, tc,tr,tf,vt);

	//top,0
	u32 m4p = (0x0000ff == bot2to3) ? iccolor : 0xffffff;
	u32 m4n = (0xff0000 == bot2to3) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 +vf[j]*5/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	carveline_pmos(ctx, bot2to3, m4p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*5/8 +vf[j]*3/8;
	carveline_nmos(ctx, bot2to3, m4n, tc,tr,tf,vt);

	//top0.o -> top1.g
	u32 top0to1 = 0xffffff;
	if(0xffffff != m4p)top0to1 = m4p;
	if(0xffffff != m4n)top0to1 = m4n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 +vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*4/8 -vf[j]*2/8;
	}
	carveline(ctx, top0to1, tc,tr);

	//top,1
	u32 m5p = (0x0000ff == iccolor) ? bot2to3 : 0xffffff;
	u32 m5n = (0xff0000 == iccolor) ? top0to1 : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 +vf[j]*5/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	carveline_pmos(ctx, iccolor, m5p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*1/8 +vf[j]*3/8;
	carveline_nmos(ctx, iccolor, m5n, tc,tr,tf,vt);

	//top1.o -> top2.g
	u32 top1to2 = 0xffffff;
	if(0xffffff != m5p)top1to2 = m5p;
	if(0xffffff != m5n)top1to2 = m5n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 +vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*3/8;
	}
	carveline(ctx, top1to2, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*2/8 +vf[j]*3/8;
		tr[j] = tc[j] +vf[j]*2/8;
	}
	carveline(ctx, top1to2, tc,tr);

	//top,2
	u32 m6p = (0x0000ff == top1to2) ? 0xff0000 : 0xffffff;
	u32 m6n = (0xff0000 == top1to2) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*3/8 +vf[j]*5/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	carveline_pmos(ctx, top1to2, m6p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*3/8 +vf[j]*3/8;
	carveline_nmos(ctx, top1to2, m6n, tc,tr,tf,vt);

}




static void fulladd_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fulladd_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fulladd_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fulladd_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void fulladd_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void fulladd_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> fulladd
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)fulladd_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void fulladd_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	u8 x,y,z,tmp;
	struct entity* ent;
	//say("@fulladd_write:%x\n",buf[0]);

	ent = self->pchip;
	if(_src_ == self->flag){
		tmp = buf[0] - 0x30;
		if((tmp >= 0)&&(tmp <= 7)){
			x = ent->ix0 = (tmp>>0)&1;
			y = ent->iy0 = (tmp>>1)&1;
			z = ent->iz0 = (tmp>>2)&1;

			ent->ixn = (x+y+z)&1;
			ent->iyn = (x+y+z)>>1;
		}
	}
}
static void fulladd_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void fulladd_start(struct halfrel* self, struct halfrel* peer)
{
}




static void fulladd_search(struct entity* act, u8* buf)
{
}
static void fulladd_modify(struct entity* act, u8* buf)
{
}
static void fulladd_delete(struct entity* act, u8* buf)
{
}
static void fulladd_create(struct entity* act, u8* buf)
{
	int x = act->ix0 = getrandom()&1;
	int y = act->iy0 = getrandom()&1;
	int z = act->iz0 = getrandom()&1;

	act->ixn = (x+y+z)&1;
	act->iyn = (x+y+z)>>1;
}




void fulladd_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('f','u','l','l','a','d','d', 0);

	p->oncreate = (void*)fulladd_create;
	p->ondelete = (void*)fulladd_delete;
	p->onsearch = (void*)fulladd_search;
	p->onmodify = (void*)fulladd_modify;

	p->onstart = (void*)fulladd_start;
	p->onstop  = (void*)fulladd_stop;
	p->onread  = (void*)fulladd_read;
	p->onwrite = (void*)fulladd_write;
}
