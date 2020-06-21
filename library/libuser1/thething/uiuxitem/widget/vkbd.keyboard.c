#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




static int vkbd_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static int vkbd_modify(struct entity* act)
{
	return 0;
}
static int vkbd_delete(struct entity* act)
{
	return 0;
}
static int vkbd_create(struct entity* act, u8* str)
{
	act->iw0 = 0;
	return 0;
}




void vkbd_draw_pixel(struct entity* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,x0,y0,x1,y1;
	int w = win->width;
	int h = win->height;
	//if(win->vkbdw < 0)return;

	//drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

	for(y=0;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			l = 2;
			c = x+(y<<4);
			//if(c == (win->vkbdz))rgb = 0xffff00ff;
			//else rgb = 0x20808080;

			//joystick area
			if((y>8)&&(y<15))
			{
				if((x>0)&&(x<7))continue;
				if((x>8)&&(x<15))continue;
			}

			if(0x0 == c)c = hex32('\\','0',0,0);
			else if(0x7 == c)c = hex32('\\','a',0,0);
			else if(0x8 == c)c = hex32('\\','b',0,0);
			else if(0x9 == c)c = hex32('\\','t',0,0);
			else if(0xa == c)c = hex32('\\','n',0,0);
			else if(0xd == c)c = hex32('\\','r',0,0);
			else if(0xf0 <= c)
			{
				if(0xfa <= c)
				{
					l = 3;
					c = ((c-0xfa)<<16) + hex32('f','1','0',0);
				}
				else
				{
					l = 2;
					c = ((c-0xf0)<<8) + hex32('f','0',0,0);
				}
			}
			else if(0x80 <= c)
			{
				l = 1;
				c = ' ';
			}
			else l = 1;

			x0 = (x*w/16)+1;
			y0 = h+1-((y+1)*h/32)+1;
			x1 = ((x+1)*w/16)-1;
			y1 = h-1-(y*h/32);
			drawhyaline_rect(win, 0x7fffffff, x0, y0, x1, y1);
			drawstring_fit(win, rgb, x0, y0, x1, y1, (u8*)&c, l);
		}
	}
}
void vkbd_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,j;
	int c,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41opaque_rect(wnd, 0x800000ff, vc, vr, vf);

	for(y=0;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			for(j=0;j<3;j++){
				tr[j] = vr[j]/17;
				tf[j] = vf[j]/8.5;
				tc[j] = vc[j] + vr[j]*(x-7.5)/8.0 + vf[j]*(y-3.5)/4.0;
			}
			rgb = 0x80808080;
			if((act->iw0)&&(x == act->ix0)&&(y == act->iy0))rgb = 0x80ff0000;
			gl41opaque_rect(wnd, rgb, tc, tr, tf);

			c = x+(y<<4);
			if((0==c)|(7==c)|(8==c)|(9==c)|(0xa==c)|(0xd==c))
			{
				if(0x0 == c)c = '0';
				else if(0x7 == c)c = 'a';
				else if(0x8 == c)c = 'b';
				else if(0x9 == c)c = 't';
				else if(0xa == c)c = 'n';
				else if(0xd == c)c = 'r';
			}

			carveascii_center(wnd, 0xffffff, tc, tr, tf, c);
		}
	}
}
void vkbd_draw_html(struct entity* win, struct style* sty)
{
}
void vkbd_draw_tui(struct entity* win, struct style* sty)
{
}
void vkbd_draw_cli(struct entity* win, struct style* sty)
{
}




static int vkbd_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	short tmp[4];
	int x,y,w,h,ret;
	//say("vkbd_keyboard_write\n");
	//if(win->vkbdw <= 0)return 0;

	w = win->width;
	h = win->height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
	{
		x = 16*x/w;
		y = 31 - 32*y/h;
		//say("x=%d,y=%d\n",x,y);
		eventwrite(x+(y*16), _char_, (u64)win, 0);
	}
	return 1;
}




static void vkbd_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	vkbd_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}
static void vkbd_write_bywnd(_ent* ent,int foot, _syn* stack,int sp, struct event* ev,int len)
{
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	if(_char_ == ev->what){
		give_data_into_peer(ent,_evto_, stack,sp, 0,0, ev,0x20);
		return;
	}
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		ent->ix0 = xyz[0] * 16;
		ent->iy0 = xyz[1] * 8;

		if(0x2b70 == ev->what)ent->iw0 = 1;
		if(0x2d70 == ev->what){
			ent->iw0 = 0;

			struct event tmp;
			int x = ent->ix0;
			int y = ent->iy0;
			tmp.why = x + y*16;
			tmp.what = _char_;
			give_data_into_peer(ent,_evto_, stack,sp, 0,0, &tmp,0x20);
		}
	}
}




static int vkbd_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		vkbd_read_bywnd(ent,slot, wnd,area);break;
	}
	}
	return 0;
}
static int vkbd_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd = stack[sp-2].pchip;
	switch(wnd->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		vkbd_write_bywnd(ent,foot, stack,sp, buf,len);break;
	}
	}
	return 0;
}
static int vkbd_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vkbd_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vkbd_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('v', 'k', 'b', 'd');

	p->oncreate = (void*)vkbd_create;
	p->ondelete = (void*)vkbd_delete;
	p->onsearch = (void*)vkbd_search;
	p->onmodify = (void*)vkbd_modify;

	p->onlinkup = (void*)vkbd_linkup;
	p->ondiscon = (void*)vkbd_discon;
	p->ontaking = (void*)vkbd_taking;
	p->ongiving = (void*)vkbd_giving;
}
