#include "libuser.h"
int actorinput_editor_camera(struct arena* win, struct event* ev);
int actorinput_editor_target(struct arena* win, struct event* ev);
static u8* str_left[8] = {
	"",		//0
	"target",		//1
	"chosen",		//2
	"",		//3
	"",		//4
	"",		//5
	""		//6
};
static u8* str_right[8] = {
	"",		//0
	"pos",		//1
	"fov",		//2
	"",		//3
	"",		//4
	"",		//5
	""		//6
};




void actoroutput_editor_vbo(struct arena* win, struct style* st)
{
	int j;
	vec3 vc, vr, vf;

	carveaxis(win);
	select_3d(win, 0x404040, &win->target, 0);

	//right
	vc[1] = 1.0 - 0.125;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[2] = 0.0;
	for(j=1;j<8;j++)
	{
		vc[0] = 31/32.0;
		vc[1] -= 0.25;
		vc[2] = -0.5;
		vr[0] = 1.0/32;
		vf[1] = 0.12;
		carvesolid2d_rect(win, 0x400000, vc, vr, vf);

		vc[0] = 30/32.0;
		vc[2] = -0.75;
		vr[0] = 1.0/64;
		vf[1] = 1.0/16;
		carve2d_string(win, 0xffffff, vc, vr, vf, str_right[j], 0);
	}

	//left
	vc[1] = 1.0 - 0.125;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[2] = 0.0;
	for(j=1;j<8;j++)
	{
		vc[0] = -31/32.0;
		vc[1] -= 0.25;
		vc[2] = -0.5;
		vr[0] = 1.0/32;
		vf[1] = 0.12;
		carvesolid2d_rect(win, 0x000040, vc, vr, vf);

		vc[0] = -1.0;
		vc[2] = -0.75;
		vr[0] = 1.0/64;
		vf[1] = 1.0/16;
		carve2d_string(win, 0xffffff, vc, vr, vf, str_left[j], 0);
	}

	//upper
	vc[0] = -1.0;
	vc[1] = 31/32.0;
	vr[0] = 0.12;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 1.0/32;
	vf[2] = 0.0;
	for(j=1;j<8;j++)
	{
		vc[0] += 0.375;
		vc[2] = -0.5;
		vr[0] = 0.12;
		vf[1] = 1.0/32;
		carvesolid2d_rect(win, 0x404000, vc, vr, vf);

		vc[0] -= 0.125;
		vc[2] = -0.75;
		vr[0] = 1.0/16;
		vf[1] = 1.0/64;
		carve2d_string(win, 0xffffff, vc, vr, vf, str_left[j], 0);
	}

	//bottom
	vc[0] = -1.0;
	vc[1] = -31/32.0;
	vr[0] = 0.12;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 1.0/32;
	vf[2] = 0.0;
	for(j=1;j<8;j++)
	{
		vc[0] += 0.375;
		vc[2] = -0.5;
		vr[0] = 0.12;
		vf[1] = 1.0/32;
		carvesolid2d_rect(win, 0x004040, vc, vr, vf);

		vc[0] -= 0.125;
		vc[2] = -0.75;
		vr[0] = 1.0/16;
		vf[1] = 1.0/64;
		carve2d_string(win, 0xffffff, vc, vr, vf, str_left[j], 0);
	}

	//selected
	if((0 == win->modex)|(7 == win->modex))
	{
		vr[0] = 1.0/32;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 0.125;
		vf[2] = 0.0;
		vc[0] = 1.0*31/32;
		vc[1] = (7.0 - win->modey*2) / 8.0;
		vc[2] = -0.9;
		if(0 == win->modex)vc[0] = -vc[0];
		carvesolid2d_rect(win, 0xffffff, vc, vr, vf);
	}
	else if((0 == win->modey)|(7 == win->modey))
	{
		vr[0] = 0.125;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 1.0/32;
		vf[2] = 0.0;
		vc[0] = (win->modex*2 - 7.0) / 8.0;
		vc[1] = 1.0*31/32;
		vc[2] = -0.9;
		if(7 == win->modey)vc[1] = -vc[1];
		carvesolid2d_rect(win, 0xffffff, vc, vr, vf);
	}
}
void actoroutput_editor_pixel(struct arena* win, struct style* st)
{
	int x, y;
	int w = win->width;
	int h = win->height;

	drawaxis(win);
	select_2d(win, 0x404040, &win->target, 0);

	//left
	drawsolid_rect(win, 0x000040, 0, h/8, w/32, h*7/8);
	for(y=1;y<8;y++)
	{
		drawline(win, 0xffffff, 0, y*h/8, w/32, y*h/8);
		drawstring(win, 0xffffff, 0, (2*y+1)*h/16, str_left[y], 0);
	}

	//right
	drawsolid_rect(win, 0x400000, w*31/32, h/8, w, h*7/8);
	for(y=1;y<8;y++)
	{
		drawline(win, 0xffffff, w*31/32, y*h/8, w, y*h/8);
		drawstring(win, 0xffffff, w-32, (2*y+1)*h/16, str_right[y], 0);
	}

	//upper
	drawsolid_rect(win, 0x404000, w/8, 0, w*7/8, h/32);
	for(x=1;x<8;x++)drawline(win, 0xffffff, x*w/8, 0, x*w/8, h/32);

	//bottom
	drawsolid_rect(win, 0x004040, w/8, h*31/32, w*7/8, h);
	for(x=1;x<8;x++)drawline(win, 0xffffff, x*w/8, h*31/32, x*w/8, h);

	//selected
	if((0 == win->modex)|(7 == win->modex))
	{
		if(0 == win->modex)x = 0;
		else x = w*31/32;
		y = (win->modey)*h/8;
		drawsolid_rect(win, 0xffffff, x, y, x+(w/32), y+(h/8));
	}
	else if((0 == win->modey)|(7 == win->modey))
	{
		x = (win->modex)*w/8;
		if(0 == win->modey)y = 0;
		else y = h*31/32;
		drawsolid_rect(win, 0xffffff, x, y, x+(w/8), y+(h/32));
	}
}
int actoroutput_editor(struct arena* win, struct style* stack)
{
	struct relation* orel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	u64 fmt = win->fmt;

	//world
	orel = win->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			sty = (void*)(orel->srcfoot);
			act->onread(win, sty, act, pin);

			if(_vbo_ == fmt)select_3d(win, 0xff00ff, sty, 0);
			else select_2d(win, 0xff00ff, sty, 0);
		}

		orel = samesrcnextdst(orel);
	}

    //panel
    if(_vbo_ == win->fmt)actoroutput_editor_vbo(win, stack);
    else actoroutput_editor_pixel(win, stack);
    return 0;
}




void actorinput_editor_fov(struct arena* win, struct event* ev)
{
	int id;
	if(0x2b70 == ev->what)
	{
		id = (ev->why)>>48;
		if('f' == id)win->nearstride *= 0.9;
		if('b' == id)win->nearstride *= 1.1;
	}
}
int actorinput_editor(struct arena* win, struct event* ev)
{
	int x,y,w,h;
	if(0x2d70 == ev->what)
	{
		x = (ev->why) & 0xffff;
		y = ((ev->why) >> 16) & 0xffff;
		w = win->width;
		h = win->height;
		if((x < w/32)|(x > w*31/32)|(y < h/32)|(y > h*31/32))
		{
			win->modex = x*8/w;
			win->modey = y*8/h;
			say("%d,%d\n", win->modex, win->modey);
		}
	}

	if(0 == win->modex)
	{
		actorinput_editor_target(win, ev);
	}
	if((7 == win->modex)&&(_vbo_ == win->fmt))
	{
		if(2 == win->modey)actorinput_editor_fov(win, ev);
		else actorinput_editor_camera(win, ev);
	}
    return 0;
}