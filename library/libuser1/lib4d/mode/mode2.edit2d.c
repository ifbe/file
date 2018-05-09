#include "actor.h"
int relation_swap(void*, void*);




void select_2d(struct arena* win, struct style* sty)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];

	drawline_rect(win, 0xff00ff, cx-ww, cy-hh, cx+ww-1, cy+hh-1);
}
void select_3d(struct arena* win, struct style* sty)
{
	vec3 tc;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	tc[0] = vc[0] + vu[0];
	tc[1] = vc[1] + vu[1];
	tc[2] = vc[2] + vu[2];
	carveline_prism4(win, 0xff0000, tc, vr, vf, vu);
}
int actoroutput_edit(struct arena* win)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	rel = win->irel;
	while(1)
	{
		if(rel == 0)break;

		if(rel->srctype == _act_)
		{
			act = (void*)(rel->srcchip);
			sty = (void*)(rel->dstfoot);
			pin = (void*)(rel->srcfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			//say("%x\n", rel);

			act->onread(win, sty, act, pin);

			if(win->fmt == _vbo_)select_3d(win, sty);
			else select_2d(win, sty);
		}

		rel = samedstnextsrc(rel);
	}
	return 0;
}




int joystick2style_2d(struct style* sty, short* tmp)
{
	int x0,y0;
	float c,s;
	float tx,ty,tz;

	if(_dl_ == tmp[2])
	{
		//dpad left
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] -= tx;
		sty->vr[1] -= ty;
		sty->vr[2] -= tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if(_dr_ == tmp[2])
	{
		//dpad right
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] -= tx;
		sty->vr[1] -= ty;
		sty->vr[2] -= tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if(_df_ == tmp[2])
	{
		//dpad down
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if(_dn_ == tmp[2])
	{
		//dpad up
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if(_lt_ == tmp[2])
	{
		//left trigger
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] -= tx;
		sty->vu[1] -= ty;
		sty->vu[2] -= tz;

		sty->vc[0] += tx*2;
		sty->vc[1] += ty*2;
		sty->vc[2] += tz*2;
		return 0;
	}
	else if(_lb_ == tmp[2])
	{
		//left bumper
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] -= tx;
		sty->vu[1] -= ty;
		sty->vu[2] -= tz;
		return 0;
	}
	else if(_kx_ == tmp[2])
	{
		//key x
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] += tx;
		sty->vr[1] += ty;
		sty->vr[2] += tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if(_kb_ == tmp[2])
	{
		//key b
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] += tx;
		sty->vr[1] += ty;
		sty->vr[2] += tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if(_ky_ == tmp[2])
	{
		//key a
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if(_ka_ == tmp[2])
	{
		//key y
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if(_rt_ == tmp[2])
	{
		//right trigger
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] += tx;
		sty->vu[1] += ty;
		sty->vu[2] += tz;

		sty->vc[0] -= tx*2;
		sty->vc[1] -= ty*2;
		sty->vc[2] -= tz*2;
		return 0;
	}
	else if(_rb_ == tmp[2])
	{
		//right bumper
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] += tx;
		sty->vu[1] += ty;
		sty->vu[2] += tz;
		return 0;
	}

	x0 = tmp[0];
	if(x0 < -8192)x0 = -1;
	else if(x0 > 8192)x0 = 1;
	else x0 = 0;

	y0 = tmp[1];
	if(y0 < -8192)y0 = -1;
	else if(y0 > 8192)y0 = 1;
	else y0 = 0;

	if('l' == tmp[2])
	{
		sty->vc[0] += (sty->vr[0])*x0/16 - (sty->vf[0])*y0/16;
		sty->vc[1] += (sty->vr[1])*x0/16 - (sty->vf[1])*y0/16;
		sty->vc[2] += (sty->vr[2])*x0/16 - (sty->vf[2])*y0/16;
	}
	else if('r' == tmp[2])
	{
		if(x0 < 0)
		{
			c = cosine(0.05);
			s = sine(0.05);
		}
		else if(x0 > 0)
		{
			c = cosine(-0.05);
			s = sine(-0.05);
		}
		else return 0;

		tx = sty->vr[0];
		ty = sty->vr[1];
		sty->vr[0] = tx*c - ty*s;
		sty->vr[1] = tx*s + ty*c;

		tx = sty->vf[0];
		ty = sty->vf[1];
		sty->vf[0] = tx*c - ty*s;
		sty->vf[1] = tx*s + ty*c;
	}
	return 0;
}
int keyboard2style_2d(struct style* sty, u8 tmp)
{
	int x0,y0;
	float c,s;
	float tx,ty,tz;

	if('s' == tmp)
	{
		//left
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] -= tx;
		sty->vr[1] -= ty;
		sty->vr[2] -= tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if('f' == tmp)
	{
		//right
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] -= tx;
		sty->vr[1] -= ty;
		sty->vr[2] -= tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if('e' == tmp)
	{
		//up
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if('d' == tmp)
	{
		//down
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] -= tx;
		sty->vf[1] -= ty;
		sty->vf[2] -= tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if('w' == tmp)
	{
		//left trigger
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] -= tx;
		sty->vu[1] -= ty;
		sty->vu[2] -= tz;

		sty->vc[0] += tx*2;
		sty->vc[1] += ty*2;
		sty->vc[2] += tz*2;
		return 0;
	}
	else if('r' == tmp)
	{
		//left bumper
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] -= tx;
		sty->vu[1] -= ty;
		sty->vu[2] -= tz;
		return 0;
	}
	else if('j' == tmp)
	{
		//key x
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] += tx;
		sty->vr[1] += ty;
		sty->vr[2] += tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if('l' == tmp)
	{
		//key b
		tx = (sty->vr[0])/16;
		ty = (sty->vr[1])/16;
		tz = (sty->vr[2])/16;

		sty->vr[0] += tx;
		sty->vr[1] += ty;
		sty->vr[2] += tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if('i' == tmp)
	{
		//key a
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] -= tx;
		sty->vc[1] -= ty;
		sty->vc[2] -= tz;
		return 0;
	}
	else if('k' == tmp)
	{
		//key y
		tx = (sty->vf[0])/16;
		ty = (sty->vf[1])/16;
		tz = (sty->vf[2])/16;

		sty->vf[0] += tx;
		sty->vf[1] += ty;
		sty->vf[2] += tz;

		sty->vc[0] += tx;
		sty->vc[1] += ty;
		sty->vc[2] += tz;
		return 0;
	}
	else if('u' == tmp)
	{
		//right trigger
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] += tx;
		sty->vu[1] += ty;
		sty->vu[2] += tz;

		sty->vc[0] -= tx*2;
		sty->vc[1] -= ty*2;
		sty->vc[2] -= tz*2;
		return 0;
	}
	else if('o' == tmp)
	{
		//right bumper
		tx = (sty->vu[0])/16;
		ty = (sty->vu[1])/16;
		tz = (sty->vu[2])/16;

		sty->vu[0] += tx;
		sty->vu[1] += ty;
		sty->vu[2] += tz;
		return 0;
	}
	return 0;
}
int playwith2d(struct arena* win, struct event* ev)
{
	struct relation* reltop;
	struct relation* relwow;
	struct style* stytop;
	struct style* stywow;
	int absx, absy;
	int x = (ev->why)&0xffff;
	int y = ((ev->why)>>16)&0xffff;
	int z = ((ev->why)>>32)&0xffff;
	int btn = ((ev->why)>>48)&0xffff;

	reltop = win->irel;
	if(reltop == 0)return 1;
	while(1)
	{
		relwow = samedstnextsrc(reltop);
		if(relwow == 0)break;

		reltop = relwow;
	}
	stytop = (void*)(reltop->dstfoot);

	stywow = 0;
	relwow = reltop;
	while(1)
	{
		if(relwow == 0)break;
		stywow = (void*)(relwow->dstfoot);

		if(x > stywow->vc[0])absx = x - (stywow->vc[0]);
		else absx = (stywow->vc[0]) - x;
		if(y > stywow->vc[1])absy = y - (stywow->vc[1]);
		else absy = (stywow->vc[1]) - y;

		if((absx <= stywow->vr[0])&&(absy <= stywow->vf[1]))break;

		relwow = samedstprevsrc(relwow);
		stywow = 0;
	}
	//say("%x,%x,%x,%x\n",reltop,stytop,stytop,stywow);

	if(_char_ == ev->what)
	{
		if(8 == ev->why)relationdelete(reltop);
		else keyboard2style_2d(stytop, (ev->why)&0xff);
		return 0;
	}
	if(_joy_ == ev->what)
	{
		joystick2style_2d(stytop, (void*)ev);
		return 0;
	}
	if('f' == btn)
	{
		stytop->vr[0] = (stytop->vr[0])*17/16;
		stytop->vf[1] = (stytop->vf[1])*17/16;
		stytop->vu[2] = (stytop->vu[2])*17/16;
		return 0;
	}
	if('b' == btn)
	{
		stytop->vr[0] = (stytop->vr[0])*15/16;
		stytop->vf[1] = (stytop->vf[1])*15/16;
		stytop->vu[2] = (stytop->vu[2])*15/16;
		return 0;
	}
	if(hex32('p','+',0,0) == ev->what)
	{
		if(stywow != 0)relation_swap(reltop, relwow);
		return 0;
	}
	if(hex32('p','@',0,0) == ev->what)
	{
		if(btn > 10)btn = 10;
		if(0 != win->touchdown[btn].z)
		{
			stytop->vc[0] += x - (win->touchmove[btn].x);
			stytop->vc[1] += y - (win->touchmove[btn].y);
			//say("%x,%x\n", stytop->vc[0], stytop->vc[1]);
		}
		if(1 >= btn)
		{
			if(0==win->touchdown[0].z)return 0;
			if(0==win->touchdown[1].z)return 0;

			if(0 == btn)
			{
				x -= (win->touchmove[1].x);
				y -= (win->touchmove[1].y);
			}
			if(1 == btn)
			{
				x -= (win->touchmove[0].x);
				y -= (win->touchmove[0].y);
			}

			absx = (win->touchmove[0].x) - (win->touchmove[1].x);
			absy = (win->touchmove[0].y) - (win->touchmove[1].y);
			stytop->vr[0] = (stytop->vr[0]) * (x*x+y*y) / (absx*absx+absy*absy);
			stytop->vf[1] = (stytop->vf[1]) * (x*x+y*y) / (absx*absx+absy*absy);
			stytop->vu[2] = (stytop->vu[2]) * (x*x+y*y) / (absx*absx+absy*absy);
		}
	}
	return 1;
}