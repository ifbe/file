#include "libuser.h"




struct plannet
{
	float diameter;
	float distance;
	float period;
	u32 color;
};
static struct plannet data[9] = {
	{1384376.0,          0.0, 999999999.99999, 0xffff00},		//sun
	{   4880.0,   57910000.0,        87.96900, 0xffc200},		//mercury
	{  12103.6,  108200000.0,       224.70000, 0xffc200},		//venus
	{  12756.3,  149600000.0,       365.25636, 0x0000ff},		//earth
	{   6794.0,  227940000.0,       687.00000, 0xff0000},		//mars
	{ 142984.0,  778330000.0,      4332.00000, 0x8a36cf},		//jupiter
	{ 120536.0, 1429400000.0,     10775.00000, 0xf5deb3},		//saturn
	{  51118.0, 2870990000.0,     30681.50000, 0x87ceeb},		//uranus
	{  49532.0, 4504000000.0,     60194.00000, 0x191970}		//neptune
};




static void planet_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u32 c;
	int x,y,l,r,j;
	int cx, cy, ww, hh;
	u64 t = timeread_us() / 10000;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	for(j=0;j<9;j++)
	{
		c = data[j].color;
		if('r' == (win->hfmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);

		l = ww*data[j].distance/data[8].distance;
		r = ww*data[j].diameter/data[8].distance;
		if(j>0)r *= 1024;
		drawline_circle(win, 0x404040, cx, cy, l);

		x = cx + l*getcos(tau*t/data[j].period);
		y = cy + l*getsin(tau*t/data[j].period);
		drawsolid_circle(win, c, x, y, r);
	}
}
static void planet_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j,k;
	float l, r;
	float a, c, s;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	u64 t = timeread_ms() / 100;

	for(j=0;j<9;j++)
	{
		l = data[j].distance/data[8].distance;
		for(k=0;k<3;k++){
			tr[k] = vr[k]*l;
			tf[k] = vf[k]*l;
		}
		gl41line_circle(wnd, 0x404040, vc, tr, tf);

		r = data[j].diameter/data[8].distance/2;
		//if(j>0)r *= 1024;
		for(k=0;k<3;k++){
			tr[k] = vr[k]*r;
			tf[k] = vf[k]*r;
			tu[k] = vu[k]*r;
		}

		//real size
		a = tau*t/data[j].period;
		c = getcos(a);
		s = getsin(a);
		tc[0] = vc[0] + (vr[0]*c + vf[0]*s)*l;
		tc[1] = vc[1] + (vr[1]*c + vf[1]*s)*l;
		tc[2] = vc[2] + (vr[2]*c + vf[2]*s)*l;
		gl41solid_sphere(wnd, data[j].color, tc, tr, tf, tu);

		//size*1000, otherwise too small to see
		if(j==0)continue;
		for(k=0;k<3;k++){
			tr[k] = tr[k]*1000;
			tf[k] = tf[k]*1000;
			tu[k] = tu[k]*1000;
		}
		gl41line_sphere(wnd, data[j].color, tc, tr, tf, tu);
	}
}
static void planet_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void planet_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void planet_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void planet_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void planet_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	planet_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void planet_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		planet_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void planet_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void planet_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void planet_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void planet_reader(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
}
static void planet_writer(_obj* ent,void* foot, p64 arg,int key, void* buf,int len)
{
}
static void planet_delete(_obj* act)
{
	if(0 == act)return;
}
static void planet_create(_obj* act)
{
	if(0 == act)return;
}




void planet_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('p', 'l', 'a', 'n', 'e', 't', 0, 0);

	p->oncreate = (void*)planet_create;
	p->ondelete = (void*)planet_delete;
	p->onreader = (void*)planet_reader;
	p->onwriter = (void*)planet_writer;

	p->onattach = (void*)planet_attach;
	p->ondetach = (void*)planet_detach;
	p->ontaking = (void*)planet_taking;
	p->ongiving = (void*)planet_giving;
}
