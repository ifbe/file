#include "libuser.h"




static void finfet_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int y,j;
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	//body.base
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vu[j]/8;
		tu[j] = vu[j] / 8;
	}
	gl41solid_prism4(wnd, 0x404040, tc, vr, vf, tu);

	//d
	for(j=0;j<3;j++){
		tr[j] = vr[j]/3;
		tf[j] = vf[j] / 16;
		tu[j] = vu[j] / 4;
	}
	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]+tr[j] +y*vf[j]/2 +vu[j]/2;
		gl41solid_prism4(wnd, 0x404040, tc, tr, tf, tu);
	}

	//s
	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]-tr[j] +y*vf[j]/2 +vu[j]/2;
		gl41solid_prism4(wnd, 0x404040, tc, tr, tf, tu);
	}

	//magic
	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2 +vu[j]/2;
		gl41solid_prism4(wnd, 0x808080, tc, tr, tf, tu);
	}

	//oxide
	for(j=0;j<3;j++){
		tr[j] = vr[j] * 3 / 4;
		tf[j] = vf[j] * 3 / 32;
		tu[j] = vu[j] / 64;
	}
	for(y=-1;y<2;y++){
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2 +vu[j]*3/4+tu[j];
		gl41opaque_prism4(wnd, 0x800000ff, tc,tr,tf,tu);
	}
	for(j=0;j<3;j++){
		tr[j] = vr[j] * 3 / 4;
		tf[j] = vf[j] / 64;
		tu[j] = vu[j] / 4;
	}
	for(y=-1;y<2;y++){
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2-vf[j]/16-tf[j] +vu[j]*1/2;
		gl41opaque_prism4(wnd, 0x800000ff, tc,tr,tf,tu);
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2+vf[j]/16+tf[j] +vu[j]*1/2;
		gl41opaque_prism4(wnd, 0x800000ff, tc,tr,tf,tu);
	}

	//gate.upper
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vu[j]*7/8;
		tr[j] = vr[j]/2;
		tu[j] = vu[j]/8;
	}
	gl41opaque_prism4(wnd, 0x80ff0000, tc, tr, vf, tu);

	//gate.bottom
	for(j=0;j<3;j++){
		tf[j] = vf[j] * 3 / 16;
		tu[j] = vu[j] / 4;
	}
	for(y=-3;y<4;y+=2)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] + y*vf[j]/4 + vu[j]/2;
		gl41opaque_prism4(wnd, 0x80ff0000, tc, tr, tf, tu);
	}
}
static void finfet_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;
	win = peer->pchip;geom = peer->pfoot;
	act = self->pchip;slot = self->pfoot;
	if(stack && ('v' == len)){
		struct entity* wnd;struct style* area;
		struct entity* wrd;struct style* camg;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		finfet_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
}
static void finfet_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
}




static void finfet_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void finfet_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void finfet_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void finfet_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void finfet_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void finfet_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct supply* sup = peer->pchip;
	switch(sup->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != len)break;
		finfet_read_bywnd(self,peer, arg,idx, buf,len);break;
	}
	default:{
		finfet_read_bycam(self,peer, arg,idx, buf,len);break;
	}
	}
}
static void finfet_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void finfet_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void finfet_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void finfet_search(struct entity* act, u8* buf)
{
}
static void finfet_modify(struct entity* act, u8* buf)
{
}
static void finfet_delete(struct entity* act, u8* buf)
{
}
static void finfet_create(struct entity* act, u8* buf)
{
}




void finfet_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('f','i','n','f','e','t', 0, 0);

	p->oncreate = (void*)finfet_create;
	p->ondelete = (void*)finfet_delete;
	p->onsearch = (void*)finfet_search;
	p->onmodify = (void*)finfet_modify;

	p->onlinkup = (void*)finfet_linkup;
	p->ondiscon = (void*)finfet_discon;
	p->onread  = (void*)finfet_read;
	p->onwrite = (void*)finfet_write;
}
