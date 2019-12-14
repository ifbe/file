#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void drawarrorkey2d(void*, u32, int x0, int y0, int x1, int y1, u8*, int);
void carvearrorkey(void*, u32, vec3 vc, vec3 vr, vec3 vf, u8*, int);




static int vjoy_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static int vjoy_modify(struct entity* win)
{
	return 0;
}
static int vjoy_delete(struct entity* win)
{
	return 0;
}
static int vjoy_create(struct entity* win, u8* str)
{
	return 0;
}




void vjoy_draw_pixel(struct entity* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,m,n;
	int w = win->width;
	int h = win->height;
	//if(win->vjoyw < 0)return;

	//drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

	ch[0] = 'l';
	ch[1] = 'r';
	ch[2] = 'n';
	ch[3] = 'f';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '-';
	drawarrorkey2d(win, 0xff00ff, 0, h*13/16, h*3/16, h, ch, 1);

	ch[0] = 'x';
	ch[1] = 'b';
	ch[2] = 'a';
	ch[3] = 'y';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '+';
	drawarrorkey2d(win, 0xff00ff, w-h*3/16, h*13/16, w, h, ch, -1);
}
void vjoy_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	u8 ch[8];
	int x,y,j;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	int w = wnd->fbwidth * area->fs.vq[0];
	int h = wnd->fbheight * area->fs.vq[1];

	ch[0] = 'l';
	ch[1] = 'r';
	ch[2] = 'n';
	ch[3] = 'f';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '-';
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j];
		tc[j] = vc[j] - vr[j]+tr[j];
	}
	carvearrorkey(wnd, 0xff00ff, tc, tr, tf, ch,-1);

	ch[0] = 'x';
	ch[1] = 'b';
	ch[2] = 'a';
	ch[3] = 'y';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '+';
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j];
		tc[j] = vc[j] + vr[j]-tr[j];
	}
	carvearrorkey(wnd, 0xff00ff, tc, tr, tf, ch, 1);
}
void vjoy_draw_html(struct entity* win, struct style* sty)
{
}
void vjoy_draw_tui(struct entity* win, struct style* sty)
{
}
void vjoy_draw_cli(struct entity* win, struct style* sty)
{
}
void vjoy_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)vjoy_draw_cli(win, sty);
	else if(fmt == _tui_)vjoy_draw_tui(win, sty);
	else if(fmt == _html_)vjoy_draw_html(win, sty);
	else vjoy_draw_pixel(win, sty);
}
int vjoy_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	short tmp[4];
	int x,y,w,h,ret;
    //say("vjoy_joystick_write\n");
	//if(win->vjoyw <= 0)return 0;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	w = win->width;
	h = win->height;
	if('p' == (ev->what&0xff))
	{
		y = (h-y)*16/h;
		if(x*2 < w)
		{
			x = x*16/h;
			if((0==x)&&(1==y))ret = joyl_left;
			else if((2==x)&&(1==y))ret = joyl_right;
			else if((1==x)&&(0==y))ret = joyl_down;
			else if((1==x)&&(2==y))ret = joyl_up;
			else if((0==x)&&(3==y))ret = joyl_trigger;
			else if((2==x)&&(3==y))ret = joyl_bumper;
			else if((1==x)&&(1==y))ret = joyl_thumb;
			else if((3==x)&&(2==y))ret = joyl_select;
			else ret = 0;
			if(ret)
			{
				if(hex32('p','-',0,0) != ev->what)return 1;

				tmp[0] = tmp[1] = tmp[2] = 0;
				tmp[3] = ret;
				eventwrite(*(u64*)tmp, joy_left, (u64)win, 0);
				return 1;
			}
		}
		else
		{
			x = w-x;
			x = x*16/h;
			if((2==x)&&(y==1))ret = joyr_left;
			else if((0==x)&&(1==y))ret = joyr_right;
			else if((1==x)&&(y==0))ret = joyr_down;
			else if((1==x)&&(y==2))ret = joyr_up;
			else if((2==x)&&(y==3))ret = joyr_trigger;
			else if((0==x)&&(y==3))ret = joyr_bumper;
			else if((1==x)&&(y==1))ret = joyr_thumb;
			else if((3==x)&&(y==2))ret = joyr_start;
			else ret = 0;

			if(ret)
			{
				if(hex32('p','-',0,0) != ev->what)return 1;

				tmp[0] = tmp[1] = tmp[2] = 0;
				tmp[3] = ret;
				eventwrite(*(u64*)tmp, joy_right, (u64)win, 0);
				return 1;
			}
		}
	}
	return 0;
}




static void vjoy_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
    int ret;
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	struct entity* act;struct style* slot;
	struct entity* wrd;struct style* geom;

	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;
	ret = vjoy_search(cam, 0, &stack[rsp+0], &stack[rsp+1]);
    if(ret > 0){
	    act = stack[rsp+0]->pchip;slot = stack[rsp+0]->pfoot;
    	wrd = stack[rsp+1]->pchip;geom = stack[rsp+1]->pfoot;
        vjoy_draw_vbo(act, slot, wrd,geom, wnd,area);
        return;
    }
    else{
        struct fstyle fs;
        fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
        fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
        fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
        gl41data_before(wnd);
        vjoy_draw_vbo(cam, 0, 0,(void*)&fs, wnd,area);
        gl41data_after(wnd);

        gl41data_tmpcam(wnd);
    }
}
static int vjoy_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return 0;//vjoy_event(act, pin, win, sty, ev, 0);
}
static int vjoy_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int vjoy_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void vjoy_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('v', 'j', 'o', 'y');

	p->oncreate = (void*)vjoy_create;
	p->ondelete = (void*)vjoy_delete;
	p->onsearch = (void*)vjoy_search;
	p->onmodify = (void*)vjoy_modify;

	p->onstart = (void*)vjoy_start;
	p->onstop  = (void*)vjoy_stop;
	p->onread  = (void*)vjoy_read;
	p->onwrite = (void*)vjoy_write;
}
