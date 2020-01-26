#include "libuser.h"
#define _tar_ hex32('t','a','r', 0)




void border2d_draw_gl41(struct entity* scene, struct entity* wnd)
{
	struct relation* rel;
	struct entity* ent;
	struct fstyle* sty;
	vec3 tc,tr,tf;
	int j;
	//say("@border2d_read: %.8s\n", &scene->fmt);

	rel = scene->orel0;
	while(1){
		if(0 == rel)break;
		ent = rel->pdstchip;
		sty = rel->psrcfoot;
		if(sty){
			//say("%llx,%llx\n",ent,sty);
			for(j=0;j<3;j++){
				tr[j] = sty->vr[j];
				tf[j] = sty->vf[j];
				tc[j] = sty->vc[j];
			}
			gl41line_rect(wnd, 0xffffff, tc,tr,tf);

			for(j=0;j<3;j++)tc[j] += tf[j];
			vec3_setlen(tf, 32);
			for(j=0;j<3;j++)tc[j] += tf[j];
			gl41solid_rect(wnd, 0x404040, tc,tr,tf);

			vec3_setlen(tr, 32);
			vec3_setlen(tf, 32);
			for(j=0;j<3;j++)tc[j] += sty->vt[j]/1000;
			carvestring_center(wnd, 0xffffff, tc,tr,tf, (void*)&ent->fmt, 8);
		}
		rel = samesrcnextdst(rel);
	}
}
int border2d_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	if(stack && ('v' == len)){
		struct entity* ent = self->pchip;
		if(0 == ent)return 0;

		struct halfrel* aa[2];
		int ret = relationsearch(ent, _tar_, &aa[0], &aa[1]);
		if(ret <= 0)return 0;

		struct entity* tar = aa[1]->pchip;
		if(0 == tar)return 0;

		struct entity* wnd = stack[rsp-4]->pchip;
		if(0 == wnd)return 0;

		border2d_draw_gl41(tar, wnd);
	}
	return 0;
}




int border2d_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return border2d_read_bycam(self,peer, stack,rsp, buf,len);
}
int border2d_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	return 0;
}
int border2d_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int border2d_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@border2d_linkup\n");
	return 0;
}




int border2d_search(struct entity* scene)
{
	return 0;
}
int border2d_modify(struct entity* scene)
{
	return 0;
}
int border2d_delete(struct entity* scene)
{
	return 0;
}
int border2d_create(struct entity* scene, void* str)
{
	say("@border2d_create\n");
	return 0;
}
