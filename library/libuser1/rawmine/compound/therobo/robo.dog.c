#include "libuser.h"
#define _int_ hex32('i','n','t',0)




void robodog_calc(vec3 joint, vec3 vector, vec3 shaft, float rotate)
{
	quaternion_operation(vector, shaft, rotate);
	vector[0] += joint[0];
	vector[1] += joint[1];
	vector[2] += joint[2];
}
void gl41solid_jointandpole(struct entity* ctx, u32 rgb, vec3 joint, vec3 target, vec3 shaft)
{
	int j;
	vec3 tc,tr,tf,tu;
	gl41line(ctx, rgb, joint, target);

	for(j=0;j<3;j++)tr[j] = target[j] - joint[j];
	vec3_setlen(tr, 100);

	vec3_cross(tf, tr, shaft);
	vec3_setlen(tf, 100);

	for(j=0;j<3;j++)tu[j] = shaft[j];
	vec3_setlen(tu, 10);

	gl41solid_cylinder(ctx, rgb, joint, tr,tf,tu);


	for(j=0;j<3;j++){
		tc[j] = (target[j]+joint[j])/2;
		tr[j] = (target[j]-joint[j])/2;
	}
	vec3_setlen(tf, 50);
	vec3_setlen(tu, 50);
	gl41solid_prism4(ctx, rgb, tc,tr,tf,tu);
}
void robodog_leg(struct entity* ctx, float* f, int lr, vec3 joint, vec3 vector, vec3 shaft)
{
	int j;
	vec3 tc;

	//1:
	robodog_calc(joint, vector, shaft, lr*f[0]);
	gl41solid_jointandpole(ctx, 0x800000, joint, vector, shaft);

	//2: 
	for(j=0;j<3;j++){
		tc[j] = shaft[j];
		shaft[j] = (vector[j] - joint[j])*lr;
		joint[j] = vector[j];
	}
	vec3_cross(vector, shaft, tc);
	vec3_setlen(vector, 500);
	robodog_calc(joint, vector, shaft, f[1]);
	gl41solid_jointandpole(ctx, 0x008000, joint, vector, shaft);

	//3: shaft unchanged
	for(j=0;j<3;j++){
		tc[j] = joint[j];
		joint[j] = vector[j];
		vector[j] = tc[j] - vector[j];
	}
	robodog_calc(joint, vector, shaft, f[2]);
	gl41solid_jointandpole(ctx, 0x000080, joint, vector, shaft);
}




static void robodog_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	float* f;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_prism4(ctx, 0x404040, vc, vr, vf, vt);
	for(j=0;j<3;j++)tf[j] = vc[j]+vf[j];
	gl41line_arrow(ctx, 0xffffff, vc, tf, vt);

	//body
	for(j=0;j<3;j++){
		tt[j] = vt[j]/8;
		tr[j] = vr[j]*0.5;
		tc[j] = vc[j] +vt[j]-tt[j];
	}
	gl41solid_prism4(ctx, 0x202020, tc, tr, vf, tt);

	f = act->buf0;
	if(0 == f)return;

	//left, near
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] -vr[j]*0.5 -vf[j] +vt[j]*0.875;
		tr[j] = -vr[j]/4;
	}
	robodog_leg(ctx, &f[0], 1, tc,tr,tf);

	//right, near
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] +vr[j]*0.5 -vf[j] +vt[j]*0.875;
		tr[j] = vr[j]/4;
	}
	robodog_leg(ctx, &f[3],-1, tc,tr,tf);

	//left, front
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] -vr[j]*0.5 +vf[j] +vt[j]*0.875;
		tr[j] = -vr[j]/4;
	}
	robodog_leg(ctx, &f[6], 1, tc,tr,tf);

	//right, front
	for(j=0;j<3;j++){
		tf[j] = vf[j];
		tc[j] = vc[j] +vr[j]*0.5 +vf[j] +vt[j]*0.875;
		tr[j] = vr[j]/4;
	}
	robodog_leg(ctx, &f[9],-1, tc,tr,tf);
}
static void robodog_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




void robodog_write_float(struct entity* act, float* src, int len)
{
	int j;
	float* dst = act->buf0;
	for(j=0;j<12;j++)dst[j] = src[j];
}
void robodog_write_int(struct entity* act, int* src, int len)
{
	int j;
	float* dst = act->buf0;
	for(j=0;j<12;j++)dst[j] = src[j]*PI/50.0;
}




//-4: wnd, area
//-3: cam, 0
//-2: cam, 0
//-1: world, geom of cam
static void robodog_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//scene -> robodog
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)robodog_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}
static void robodog_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* act;
	say("robodog_write\n");

	act = self->pchip;
	if(_int_ == self->flag)robodog_write_int(act,buf,len);
	else robodog_write_float(act,buf,len);
}
static void robodog_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void robodog_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void robodog_modify(struct entity* act)
{
}
static void robodog_search(struct entity* act)
{
}
static void robodog_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void robodog_create(struct entity* act, u8* url, int argc, u8** argv)
{
	float* f;
	if(0 == act)return;

	f = act->buf0 = memorycreate(0x1000, 0);
	f[ 0] = 0.0;
	f[ 1] = PI/12;
	f[ 2] = PI*0.7;
	f[ 3] = 0.0;
	f[ 4] = PI/12;
	f[ 5] = PI*0.7;
	f[ 6] = 0.0;
	f[ 7] = PI/12;
	f[ 8] = PI*0.7;
	f[ 9] = 0.0;
	f[10] = PI/12;
	f[11] = PI*0.7;
}




void robodog_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 'o', 'b', 'o', 'd', 'o', 'g', 0);

	p->oncreate = (void*)robodog_create;
	p->ondelete = (void*)robodog_delete;
	p->onsearch = (void*)robodog_search;
	p->onmodify = (void*)robodog_modify;

	p->onlinkup = (void*)robodog_linkup;
	p->ondiscon = (void*)robodog_discon;
	p->onread  = (void*)robodog_read;
	p->onwrite = (void*)robodog_write;
}
