#include "libuser.h"
int parsefv(float* vec, int flen, u8* str, int slen);




struct joint{
	float x;
	float y;
	float z;
	int exist;

	float gradx;
	float grady;
	float gradz;
	int sure;
};
static int parsejoint_oneline(u8* buf, struct joint* jo)
{
	int j;
	if(buf[0] < '0')return 0;
	if(buf[0] > '1')return 0;
	jo->sure = buf[0] - '0';

	for(j=1;j<8;j++){
		if('-' == buf[j])goto found;
		if((buf[j] >= '0')&&(buf[j] <= '9'))goto found;
	}
	return 0;

found:
	parsefv(&jo->x, 3, buf+j, 16);
	jo->exist = 1;
	return 1;
}
static int parsejoint(struct joint* jo, u8* buf)
{
	int j,k;
	int tmp=0,ioff=0;
	for(j=0;j<0x8000;j++){
		if(buf[j] <= 0xa){
			if(j-k<4)break;
			k = parsejoint_oneline(buf+tmp, &jo[ioff]);
			if(k){
				jo[ioff].gradx = 0.0;
				jo[ioff].grady = 0.0;
				jo[ioff].gradz = 0.0;
				say("%d: %f,%f,%f\n", ioff, jo[ioff].x, jo[ioff].y, jo[ioff].z);
				ioff += 1;
			}
			if(buf[j] < 0xa)break;
			tmp = j+1;
		}
	}

	jo[ioff].exist = 0;
	return j;
}




static void force_decent_spring(struct entity* ent, struct joint* jo)
{
	int j;
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].gradx = 0.0;
		jo[j].grady = 0.0;
		jo[j].gradz = 0.0;
		relationread(ent, 'a'+j, 0, 'R', jo, j);
	}
	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].x -= jo[j].gradx;
		jo[j].y -= jo[j].grady;
		jo[j].z -= jo[j].gradz;
		say("@force_decent_spring: %f,%f,%f\n",jo[j].x, jo[j].y, jo[j].z);
	}
}
static void force_decent_stick(struct entity* ent, struct joint* jo)
{
	int j;
	for(j=1;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].gradx = 0.0;
		jo[j].grady = 0.0;
		jo[j].gradz = 0.0;
		relationread(ent, 'a'+j, 0, 'V', jo, j);
	}
	for(j=1;j<16;j++){
		if(0 == jo[j].exist)break;
		if(0 != jo[j].sure)continue;

		jo[j].x -= jo[j].gradx;
		jo[j].y -= jo[j].grady;
		jo[j].z -= jo[j].gradz;
		say("force_decent_stick:%f,%f,%f\n",jo[j].x, jo[j].y, jo[j].z);
	}
}




static void force_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_prism4(wnd, 0x404040, vc, vr, vf, vt);

	struct joint* jo = act->buf0;
	if(0 == jo[0].exist)return;

	force_decent_spring(act, jo);
	force_decent_stick(act, jo);

	for(j=0;j<3;j++){tr[j] = tf[j] = tu[j] = 0.0;}
	tr[0] = tf[1] = tu[2] = 10.0;

	for(j=0;j<16;j++){
		if(0 == jo[j].exist)break;
		gl41solid_sphere(wnd, 0x808080, &jo[j].x,tr,tf,tu);
	}
}
void force_read_board(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> force
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	act = self->pchip;slot = self->pfoot;
	win = peer->pchip;geom = peer->pfoot;
	wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
	wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
	force_draw_gl41(act,slot, win,geom, wnd,area);
}
int force_read_child(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* scene;
	struct relation* rel;
	//say("@force_read\n");

	scene = self->pchip;
	if(0 == scene)return 0;
	rel = scene->orel0;
	if(0 == rel)return 0;

	while(1){
		if(0 == rel)break;
		if(rel->srcfoot)entityread((void*)(rel->dst), (void*)(rel->src), stack, rsp, buf, len);
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int force_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	if(stack && ('v' == len)){
		force_read_child(self,peer, stack,rsp, buf,len);
		force_read_board(self,peer, stack,rsp, buf,len);
	}
	return 0;
}
int force_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int force_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int force_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@force_linkup: %.4s\n", &self->flag);
	return 0;
}




int force_search(struct entity* scene)
{
	return 0;
}
int force_modify(struct entity* scene)
{
	return 0;
}
int force_delete(struct entity* scene)
{
	return 0;
}
int force_create(struct entity* scene, void* arg, int argc, u8** argv)
{
	int ret;
	void* buf;
	say("@force_create\n");
	if(0 == arg)return 0;

	buf = scene->buf0 = memorycreate(0x10000, 0);
	if(0 == buf)return 0;

	ret = openreadclose(arg, 0, buf+0x8000, 0x8000);
	if(ret <= 0)return 0;

	parsejoint(buf, buf+0x8000);
	return 0;
}
