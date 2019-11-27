#include "libuser.h"
#define _tar_ hex32('t','a','r',0)
void fixmatrix(float* m, struct fstyle* sty);
void freecam_shape2frustum(struct fstyle* d, struct fstyle* s);
void freecam_ratio(struct entity* wrd, struct style* geom, struct entity* wnd, struct style* area);




static void thirdperson_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	if(_tar_ == foot){
		while(1){
			if(0 == rel)return;
			if(_tar_ == rel->dstflag){
				self[0] = (void*)&rel->dstchip;
				peer[0] = (void*)&rel->srcchip;
				return;
			}
			rel = samedstnextsrc(rel);
		}
		return;
	}

	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void thirdperson_modify(struct entity* act)
{
}
static void thirdperson_delete(struct entity* act)
{
}
static void thirdperson_create(struct entity* act, void* str)
{
	if(0 == act)return;
	act->buf = memorycreate(64, 0);
}




static int thirdperson_draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrl, struct style* frus,
	struct entity* ctx, struct style* none)
{
	carvefrustum(ctx, &geom->frus);
	return 0;
}




static void thirdperson_fixcam(struct fstyle* obb, struct fstyle* tar, int dx, int dy, int roll)
{
	float a,c,s;
	float q[4];		//quaternion
	float t[4];		//target
	float v[4];		//tar to eye
	t[0] = tar->vc[0]+tar->vt[0];
	t[1] = tar->vc[1]+tar->vt[1];
	t[2] = tar->vc[2]+tar->vt[2];
	v[0] = obb->vc[0] - t[0];
	v[1] = obb->vc[1] - t[1];
	v[2] = obb->vc[2] - t[2];

	if(roll != 0){
		a = 1.0 + roll/20.0;
		v[0] *= a;
		v[1] *= a;
		v[2] *= a;
		obb->vc[0] = t[0]+v[0];
		obb->vc[1] = t[1]+v[1];
		obb->vc[2] = t[2]+v[2];
	}
	if(dy != 0){
		q[0] =-v[1];
		q[1] = v[0];
		q[2] = 0.0;
		quaternion_operation(v, q, -dy/100.0);
		obb->vc[0] = t[0]+v[0];
		obb->vc[1] = t[1]+v[1];
		obb->vc[2] = t[2]+v[2];
	}
	if(dx != 0){
		a = -dx/100.0;
		s = sine(a);
		c = cosine(a);
		obb->vc[0] = t[0] + v[0]*c - v[1]*s;
		obb->vc[1] = t[1] + v[0]*s + v[1]*c;
	}

	obb->vf[0] = t[0] - obb->vc[0];
	obb->vf[1] = t[1] - obb->vc[1];
	obb->vf[2] = t[2] - obb->vc[2];
	vec3_normalize(obb->vf);

	//a × b= [aybz-azby,azbx-axbz, axby-aybx]
	obb->vr[0] = obb->vf[1];
	obb->vr[1] =-obb->vf[0];
	obb->vr[2] = 0.0;

	//
	obb->vt[0] = obb->vr[1]*obb->vf[2] - obb->vr[2]*obb->vf[1];
	obb->vt[1] = obb->vr[2]*obb->vf[0] - obb->vr[0]*obb->vf[2];
	obb->vt[2] = obb->vr[0]*obb->vf[1] - obb->vr[1]*obb->vf[0];
}
static int thirdperson_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	int dx,dy,roll;
	short* t;
	struct entity* tmp;

	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;
	struct fstyle* tar;

	thirdperson_search(act, 0, &self, &peer);
	obb = peer->pfoot;
	//say("obb=%llx\n", obb);
	//printstyle(obb);

	thirdperson_search(act, _tar_, &self, &peer);
	tmp = peer->pchip;
	//say("111tmp=%llx,fmt=%.8s\n",tmp, &tmp->fmt);

	thirdperson_search(tmp, 0, &self, &peer);
	tar = peer->pfoot;
	//say("tar=%llx\n", tar);

	if(0x4070 == ev->what){
		t = (void*)ev;
		if(act->w0 != 0){
			thirdperson_fixcam(obb, tar, t[0] - act->xn, t[1] - act->yn, 0);
		}
		act->xn = t[0];
		act->yn = t[1];
	}
	if(0x2b70 == ev->what){
		t = (void*)ev;
		say("%c\n",t[3]);
		switch(t[3]){
			case 'l':
			case 'r':{
				act->w0 = 1;
				act->x0 = act->xn = t[0];
				act->y0 = act->yn = t[1];
				break;
			}
			case 'f':{
				thirdperson_fixcam(obb, tar, 0, 0, -1);
				break;
			}
			case 'b':{
				thirdperson_fixcam(obb, tar, 0, 0, 1);
				break;
			}
		}
	}
	if(0x2d70 == ev->what){
		act->w0 = 0;
	}

	return 0;
}




static void thirdperson_matrix(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wnd, struct style* area)
{
	void* mat = act->buf;
	struct fstyle* frus = &geom->frus;

	fixmatrix(mat, frus);
	mat4_transpose(mat);
	//printmat4(m);

	struct glsrc* src = wnd->gl_camera;
	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = mat;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = frus->vc;
}




static void thirdperson_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//world -> this
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		if('v' == len){
			freecam_ratio(win, geom, wnd, area);
			freecam_shape2frustum(&geom->fshape, &geom->frustum);
			thirdperson_draw(act,part, win,geom, wrd,camg, wnd,area);
		}
		if('m' == len){
			thirdperson_matrix(act,part, win,geom, wnd,area);
		}
	}
}
static int thirdperson_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* wld;struct style* geom;
	struct entity* act;struct style* part;
	struct event* ev;

	wld = peer->pchip;geom = peer->pfoot;
	act = self->pchip;part = self->pfoot;
	ev = (void*)buf;

	thirdperson_event(act, part, wld, geom, ev, 0);
	return 0;
}
static void thirdperson_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void thirdperson_start(struct halfrel* self, struct halfrel* peer)
{
}




void thirdperson_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('3', 'r', 'd', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)thirdperson_create;
	p->ondelete = (void*)thirdperson_delete;
	p->onsearch = (void*)thirdperson_search;
	p->onmodify = (void*)thirdperson_modify;

	p->onstart = (void*)thirdperson_start;
	p->onstop  = (void*)thirdperson_stop;
	p->onread  = (void*)thirdperson_read;
	p->onwrite = (void*)thirdperson_write;
}
