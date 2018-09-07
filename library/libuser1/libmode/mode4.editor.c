#include "libuser.h"
int actorinput_editor_camera(struct arena* win, struct event* ev);
int actorinput_player_target(struct arena* win, struct event* ev);




void actoroutput_panel3d(struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf)
{
	float j;
	vec3 ta;
	vec3 tb;
	carvesolid2d_rect(win, rgb, vc, vr, vf);

	for(j=-0.75;j<1.0;j+=0.25)
	{
		ta[0] = vc[0] + j*vr[0] - 0.8*vf[0];
		ta[1] = vc[1] + j*vr[1] - 0.8*vf[1];
		ta[2] = vc[2] + j*vr[2] - 0.8*vf[2];
		tb[0] = vc[0] + j*vr[0] + 0.8*vf[0];
		tb[1] = vc[1] + j*vr[1] + 0.8*vf[1];
		tb[2] = vc[2] + j*vr[2] + 0.8*vf[2];
		carveline2d(win, 0xffffff, ta, tb);
	}
}
void actoroutput_editor_vbo(struct arena* win, struct style* st)
{
	vec3 vc, vr, vf;
	carveaxis(win);

	//right
	vc[0] = 1.0-0.05;
	vc[1] = 0.0;
	vc[2] = -0.8;
	vr[0] = 0.05;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 0.75;
	vf[2] = 0.0;
	actoroutput_panel3d(win, 0x400000, vc, vf, vr);

	//left
	vc[0] = -vc[0];
	actoroutput_panel3d(win, 0x000040, vc, vf, vr);

	//upper
	vc[0] = 0.0;
	vc[1] = 1.0-0.05;
	vr[0] = 0.75;
	vf[1] = 0.05;
	actoroutput_panel3d(win, 0x404000, vc, vr, vf);

	//bottom
	vc[1] = -vc[1];
	actoroutput_panel3d(win, 0x004040, vc, vr, vf);
}




void actoroutput_panel_pixel(struct arena* win, u32 rgb, vec3 vc, vec3 vr, vec3 vf)
{
}
void actoroutput_editor_pixel(struct arena* win, struct style* st)
{
	int w = win->width;
	int h = win->height;
	drawaxis(win);

	//left
	drawsolid_rect(win, 0x000040, 0, h/8, w/20, h*7/8);

	//right
	drawsolid_rect(win, 0x400000, w*19/20, h/8, w, h*7/8);

	//upper
	drawsolid_rect(win, 0x404000, w/8, 0, w*7/8, h/20);

	//bottom
	drawsolid_rect(win, 0x004040, w/8, h*19/20, w*7/8, h);
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

			if(_vbo_ == fmt)select_3d(win, sty);
			else select_2d(win, sty);
		}

		orel = samesrcnextdst(orel);
	}

    //panel
    if(_vbo_ == win->fmt)actoroutput_editor_vbo(win, stack);
    else actoroutput_editor_pixel(win, stack);
    return 0;
}
int actorinput_editor(struct arena* win, struct event* ev)
{
    return 0;
}