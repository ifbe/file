#include "libuser.h"




void thridperson_fixcam(struct arena* win, float* v)
{
	float w,h;
	float x,y,z,n;

	//near = -v
	n = squareroot(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	win->camera.vn[0] = -v[0] / n;
	win->camera.vn[1] = -v[1] / n;
	win->camera.vn[2] = -v[2] / n;

	//right = cross(near, (0,0,1))
	x = -v[1]*1 + v[2]*0;
	y = -v[2]*0 + v[0]*1;
	z = -v[0]*0 + v[1]*0;
	n = 1.0 / squareroot(x*x + y*y + z*z);
	win->camera.vr[0] = x * n;
	win->camera.vr[1] = y * n;
	win->camera.vr[2] = z * n;
	win->camera.vl[0] =-x * n;
	win->camera.vl[1] =-y * n;
	win->camera.vl[2] =-z * n;

	//w,h
	w = win->width;
	h = win->height;

	//up = cross(right, near)
	x = win->camera.vr[1]*win->camera.vn[2] - win->camera.vr[2]*win->camera.vn[1];
	y = win->camera.vr[2]*win->camera.vn[0] - win->camera.vr[0]*win->camera.vn[2];
	z = win->camera.vr[0]*win->camera.vn[1] - win->camera.vr[1]*win->camera.vn[0];
	n = h / w / squareroot(x*x + y*y + z*z);
	win->camera.vu[0] = x * n;
	win->camera.vu[1] = y * n;
	win->camera.vu[2] = z * n;
	win->camera.vb[0] =-x * n;
	win->camera.vb[1] =-y * n;
	win->camera.vb[2] =-z * n;
}




static int thirdperson_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	vec3 tc,tr,tf;
	struct relation* rel;
	struct arena* tmpwin;
	struct style* tmpsty;

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_win_ == rel->dsttype)
		{
			tmpwin = (void*)(rel->dstchip);
			if(_ev3d_ == tmpwin->fmt)goto found;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;

found:
	rel = tmpwin->orel0;
	while(1)
	{
		if(0 == rel)return 0;

		if(_act_ == rel->dsttype)
		{
			tmpsty = (void*)(rel->srcfoot);

			//camera.xyz = target.top + act.vector
			win->camera.vc[0] = tmpsty->vc[0] + tmpsty->vu[0] + act->camera.vc[0];
			win->camera.vc[1] = tmpsty->vc[1] + tmpsty->vu[1] + act->camera.vc[1];
			win->camera.vc[2] = tmpsty->vc[2] + tmpsty->vu[2] + act->camera.vc[2];
			thridperson_fixcam(win, act->camera.vc);
			goto print;
		}

		rel = samesrcnextdst(rel);
	}
	return 0;

print:
	tr[0] = 0.025;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.025;
	tf[2] = 0.0;

	//camera
	tc[0] = -1.0;
	tc[1] = -0.05 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x0000ff, tc, tr, tf, (void*)"vl: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, win->camera.vl);

	tc[0] = -1.0;
	tc[1] = -0.1 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x0000ff, tc, tr, tf, (void*)"vr: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x0000ff, tc, tr, tf, win->camera.vr);

	tc[0] = -1.0;
	tc[1] = -0.15 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x00ff00, tc, tr, tf, (void*)"vb: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, win->camera.vb);

	tc[0] = -1.0;
	tc[1] = -0.2 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0x00ff00, tc, tr, tf, (void*)"vu: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0x00ff00, tc, tr, tf, win->camera.vu);

	tc[0] = -1.0;
	tc[1] = -0.25 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xff0000, tc, tr, tf, (void*)"vn: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, win->camera.vn);

	tc[0] = -1.0;
	tc[1] = -0.3 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xff0000, tc, tr, tf, (void*)"vf: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xff0000, tc, tr, tf, win->camera.vf);

	tc[0] = -1.0;
	tc[1] = -0.35 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vq: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vv);

	tc[0] = -1.0;
	tc[1] = -0.4 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vc: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vc);

	return 0;
}
static int thirdperson_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	//only hook mouse wheel
	int id;
	if(0 == act)return 0;

	if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)
		{
			act->camera.vc[0] *= 0.99;
			act->camera.vc[1] *= 0.99;
			act->camera.vc[2] *= 0.99;
			return 1;
		}
		if('b' == id)
		{
			act->camera.vc[0] *= 1.01;
			act->camera.vc[1] *= 1.01;
			act->camera.vc[2] *= 1.01;
			return 1;
		}
	}
	return 0;
}
static void thirdperson_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void thirdperson_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void thirdperson_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void thirdperson_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void thirdperson_delete(struct actor* act)
{
}
static void thirdperson_create(struct actor* act, void* str)
{
	if(0 == act)return;
	act->camera.vc[0] = 0.0;
	act->camera.vc[1] =-1000.0;
	act->camera.vc[2] = 1000.0;
}




void thirdperson_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('3', 'r', 'd', 'c', 'a', 'm', 0, 0);

	p->oncreate = (void*)thirdperson_create;
	p->ondelete = (void*)thirdperson_delete;
	p->onstart  = (void*)thirdperson_start;
	p->onstop   = (void*)thirdperson_stop;
	p->onget    = (void*)thirdperson_cread;
	p->onpost   = (void*)thirdperson_cwrite;
	p->onread   = (void*)thirdperson_sread;
	p->onwrite  = (void*)thirdperson_swrite;
}