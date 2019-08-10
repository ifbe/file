#include "libuser.h"
void invmvp(vec3 v, struct style* sty);




void select_3d(struct actor* win, u32 rgb, struct fstyle* sty, u32 flag)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vt;
	carveline_prism4(win, rgb, vc, vr, vf, vu);
}
void carveaxis(struct actor* win)
{
	vec3 va, vb;
	va[0] = -10000.0;
	va[1] = 0.0;
	va[2] = 0.0;
	vb[0] = 10000.0;
	vb[1] = 0.0;
	vb[2] = 0.0;
	carveline(win, 0xff0000, va, vb);
	va[0] = 0.0;
	va[1] = -10000.0;
	va[2] = 0.0;
	vb[0] = 0.0;
	vb[1] = 10000.0;
	vb[2] = 0.0;
	carveline(win, 0x00ff00, va, vb);
	va[0] = 0.0;
	va[1] = 0.0;
	va[2] = -10000.0;
	vb[0] = 0.0;
	vb[1] = 0.0;
	vb[2] = 10000.0;
	carveline(win, 0x0000ff, va, vb);
}
void carvefrustum(struct actor* win, struct fstyle* sty)
{
	vec3 tc, tr, tf;
	float* vc = sty->vc;
	float* vn = sty->vn;
	float* vl = sty->vl;
	float* vr = sty->vr;
	float* vb = sty->vb;
	float* vu = sty->vt;

	tc[0] = vc[0] + vn[0]*0.99 + (vl[0] + vr[0])/2 + (vb[0] + vu[0])/2;
	tc[1] = vc[1] + vn[1]*0.99 + (vl[1] + vr[1])/2 + (vb[1] + vu[1])/2;
	tc[2] = vc[2] + vn[2]*0.99 + (vl[2] + vr[2])/2 + (vb[2] + vu[2])/2;
	tr[0] = (vr[0] - vl[0])/2;
	tr[1] = (vr[1] - vl[1])/2;
	tr[2] = (vr[2] - vl[2])/2;
	tf[0] = (vu[0] - vb[0])/2;
	tf[1] = (vu[1] - vb[1])/2;
	tf[2] = (vu[2] - vb[2])/2;
	carveline_rect(win, 0xff0000, tc, tr, tf);

	tc[0] = vc[0] + (vn[0] + (vl[0] + vr[0])/2 + (vb[0] + vu[0])/2)*1000;
	tc[1] = vc[1] + (vn[1] + (vl[1] + vr[1])/2 + (vb[1] + vu[1])/2)*1000;
	tc[2] = vc[2] + (vn[2] + (vl[2] + vr[2])/2 + (vb[2] + vu[2])/2)*1000;
	tr[0] = (vr[0] - vl[0])*500;
	tr[1] = (vr[1] - vl[1])*500;
	tr[2] = (vr[2] - vl[2])*500;
	tf[0] = (vu[0] - vb[0])*500;
	tf[1] = (vu[1] - vb[1])*500;
	tf[2] = (vu[2] - vb[2])*500;
	carveline_rect(win, 0xffffff, tc, tr, tf);

	tc[0] = vc[0] + (vn[0] + vl[0] + vb[0])*1000.0;
	tc[1] = vc[1] + (vn[1] + vl[1] + vb[1])*1000.0;
	tc[2] = vc[2] + (vn[2] + vl[2] + vb[2])*1000.0;
	carveline(win, 0xffffff, vc, tc);

	tc[0] = vc[0] + (vn[0] + vr[0] + vb[0])*1000.0;
	tc[1] = vc[1] + (vn[1] + vr[1] + vb[1])*1000.0;
	tc[2] = vc[2] + (vn[2] + vr[2] + vb[2])*1000.0;
	carveline(win, 0xffffff, vc, tc);

	tc[0] = vc[0] + (vn[0] + vl[0] + vu[0])*1000.0;
	tc[1] = vc[1] + (vn[1] + vl[1] + vu[1])*1000.0;
	tc[2] = vc[2] + (vn[2] + vl[2] + vu[2])*1000.0;
	carveline(win, 0xffffff, vc, tc);

	tc[0] = vc[0] + (vn[0] + vr[0] + vu[0])*1000.0;
	tc[1] = vc[1] + (vn[1] + vr[1] + vu[1])*1000.0;
	tc[2] = vc[2] + (vn[2] + vr[2] + vu[2])*1000.0;
	carveline(win, 0xffffff, vc, tc);
}/*
void carvecamera(struct actor* win, struct actor* w)
{
	vec3 va0,vb0,vc0,vd0;
	vec3 va1,vb1,vc1,vd1;
	vec3 va,vb,vv;
	float x = (float)(w->width);
	float y = (float)(w->height);

	//left,bot,near
	va0[0] = -1.0;
	va0[1] = -1.0;
	va0[2] = -1.0;

	//right,bot,near
	vb0[0] = 1.0;
	vb0[1] = -1.0;
	vb0[2] = -1.0;

	//left,top,near
	vc0[0] = -1.0;
	vc0[1] = 1.0;
	vc0[2] = -1.0;

	//right,top,near
	vd0[0] = 1.0;
	vd0[1] = 1.0;
	vd0[2] = -1.0;

	//left,bot,far
	va1[0] = -1.0;
	va1[1] = -1.0;
	va1[2] = 0.5;

	//right,bot,far
	vb1[0] = 1.0;
	vb1[1] = -1.0;
	vb1[2] = 0.5;

	//left,top,far
	vc1[0] = -1.0;
	vc1[1] = 1.0;
	vc1[2] = 0.5;

	//right,top,far
	vd1[0] = 1.0;
	vd1[1] = 1.0;
	vd1[2] = 0.5;

	//ray
	va[0] = (w->input[10].xn)*2/x - 1.0;
	va[1] = 1.0 - (w->input[10].yn)*2/y;
	va[2] = -1.0;
	vb[0] = (w->input[10].xn)*2/x - 1.0;
	vb[1] = 1.0 - (w->input[10].yn)*2/y;
	vb[2] = 0.5;

	invmvp(va0, &w->camera);
	invmvp(vb0, &w->camera);
	invmvp(vc0, &w->camera);
	invmvp(vd0, &w->camera);
	invmvp(va1, &w->camera);
	invmvp(vb1, &w->camera);
	invmvp(vc1, &w->camera);
	invmvp(vd1, &w->camera);
	invmvp(va,  &w->camera);
	invmvp(vb,  &w->camera);

	va1[0] = va0[0] + (va1[0]-va0[0])*10;
	va1[1] = va0[1] + (va1[1]-va0[1])*10;
	va1[2] = va0[2] + (va1[2]-va0[2])*10;
	vb1[0] = vb0[0] + (vb1[0]-vb0[0])*10;
	vb1[1] = vb0[1] + (vb1[1]-vb0[1])*10;
	vb1[2] = vb0[2] + (vb1[2]-vb0[2])*10;
	vc1[0] = vc0[0] + (vc1[0]-vc0[0])*10;
	vc1[1] = vc0[1] + (vc1[1]-vc0[1])*10;
	vc1[2] = vc0[2] + (vc1[2]-vc0[2])*10;
	vd1[0] = vd0[0] + (vd1[0]-vd0[0])*10;
	vd1[1] = vd0[1] + (vd1[1]-vd0[1])*10;
	vd1[2] = vd0[2] + (vd1[2]-vd0[2])*10;
	vv[0] = va[0] - va[2]*(vb[0]-va[0])/(vb[2]-va[2]);
	vv[1] = va[1] - va[2]*(vb[1]-va[1])/(vb[2]-va[2]);
	vv[2] = 0.0;

	//rect near
	carveline(win, 0xffffff, va0, vb0);
	carveline(win, 0xffffff, va0, vc0);
	carveline(win, 0xffffff, vb0, vd0);
	carveline(win, 0xffffff, vc0, vd0);

	//rect far
	carveline(win, 0xffffff, va1, vb1);
	carveline(win, 0xffffff, va1, vc1);
	carveline(win, 0xffffff, vb1, vd1);
	carveline(win, 0xffffff, vc1, vd1);

	//frame
	carveline(win, 0xffffff, va0, va1);
	carveline(win, 0xffffff, vb0, vb1);
	carveline(win, 0xffffff, vc0, vc1);
	carveline(win, 0xffffff, vd0, vd1);

	//ray
	carveline(win, 0xffffff, va, vv);
}*/
