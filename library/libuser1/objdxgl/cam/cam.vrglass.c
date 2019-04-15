#include "libuser.h"




static int vrglass_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	float y;
	vec3 tc,tr,tf,tu;
	tr[0] = win->width/2;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tu[0] = 0.0;
	tu[1] = 0.0;
	tu[2] = win->height/2;

	tc[0] = 0.0;
	tc[1] = 0.0;
	tc[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1000.0;
	tf[2] = 0.0;
	carveline_prism4(win, 0xff0000, tc, tr, tf, tu);
	carvefrustum(win, &win->camera);

	for(y=-1000.0;y<1001.0;y+=500.0)
	{
		tc[0] = 0.0;
		tc[1] = y;
		tc[2] = 0.0;
		carveline_rect(win, 0xff0000, tc, tr, tu);
	}
/*
	//camera
	tr[0] = 0.025;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 0.025;
	tf[2] = 0.0;

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
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vq);

	tc[0] = -1.0;
	tc[1] = -0.4 + 0.025;
	tc[2] = 0.0;
	carve2d_string(win, 0xffffff, tc, tr, tf, (void*)"vc: ", 4);
	tc[0] += 0.05;
	carve2d_vec4(win, 0xffffff, tc, tr, tf, win->camera.vc);
*/
	return 0;
}
static int vrglass_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int id;
	int x0,y0,x1,y1;
	short* t;
	float* vc = act->camera.vc;

	if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)vc[0] -= 10;
		else if(0x4d == ev->why)vc[0] += 10;
		else if(0x50 == ev->why)vc[2] -= 10;
		else if(0x48 == ev->why)vc[2] += 10;
	}
	else if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)vc[1] += 10;
		if('b' == id)vc[1] -= 10;
	}
	else if(0x4070 == ev->what)
	{
		id = (ev->why)>>48;
		if('l' == id)id = 10;
		else if('r' == id)id = 11;
		else if(id > 10)return 0;
		if(0 == win->input[id].z0)return 0;

		x0 = win->input[id].xn;
		y0 = win->input[id].yn;
		x1 = (ev->why)&0xffff;
		y1 = ((ev->why)>>16)&0xffff;

		vc[0] += x1-x0;
		vc[2] -= y1-y0;
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (void*)ev;
		//printmemory(t, 16);
		//say("%d,%d\n", t[0], t[1]);
		if(t[3] & joyl_left)		//x-
		{
			vc[0] -= 10;
		}
		if(t[3] & joyl_right)		//x+
		{
			vc[0] += 10;
		}
		if(t[3] & joyl_down)		//y-
		{
			vc[2] -= 10;
		}
		if(t[3] & joyl_up)			//y+
		{
			vc[2] += 10;
		}
		if(t[3] & joyl_trigger)		//z-
		{
			vc[1] -= 10.0;
		}
		if(t[3] & joyl_bumper)		//z+
		{
			vc[1] += 10.0;
		}
		if(t[3] & joyl_stick)		//w-
		{
			vc[0] = 0.0;
			vc[1] = -2000.0;
			vc[2] = 0.0;
		}
		if(t[3] & joyl_select)		//w+
		{
		}

		x0 = t[0];
		y0 = t[1];
		if((x0 < -4096) | (x0 > 4096) | (y0 < -4096) | (y0 > 4096))
		{
			vc[0] += x0/1000.0;
			vc[2] += y0/1000.0;
		}
	}
	//say("%f,%f,%f\n",vc[0], vc[1], vc[2]);

	win->camera.vc[0] = vc[0];
	win->camera.vc[1] = vc[1];
	win->camera.vc[2] = vc[2];

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = -1000.0 - win->camera.vc[1];
	win->camera.vn[2] = 0.0;

	win->camera.vl[0] = -win->width/2 - win->camera.vc[0];
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = win->width/2 - win->camera.vc[0];
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] = 0.0;
	win->camera.vb[2] = -win->height/2 - win->camera.vc[2];

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 0.0;
	win->camera.vu[2] = win->height/2 - win->camera.vc[2];
	return 1;
}
static void vrglass_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void vrglass_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void vrglass_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void vrglass_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void vrglass_delete()
{
}
static void vrglass_create(struct actor* act, void* str)
{
	float* vc = act->camera.vc;
	float* vn = act->camera.vn;

	vc[0] = 0.0;
	vc[1] = -2000.0;
	vc[2] = 200.0;

	vn[0] = 0.0;
	vn[1] = -1000.0 - vc[1];
	vn[2] = 0.0;
}




void vrglass_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'r', 'g', 'l', 'a', 's', 's', 0);

	p->oncreate = (void*)vrglass_create;
	p->ondelete = (void*)vrglass_delete;
	p->onstart  = (void*)vrglass_start;
	p->onstop   = (void*)vrglass_stop;
	p->oncread  = (void*)vrglass_cread;
	p->oncwrite = (void*)vrglass_cwrite;
	p->onsread  = (void*)vrglass_sread;
	p->onswrite = (void*)vrglass_swrite;
}
