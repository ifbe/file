#include "libuser.h"
static u8 data[7][7];




static void pegged_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if(data[y][x] == 0)continue;

			drawline_rect(
				win, 0x808080,
				cx+(2*x-7)*ww/7,
				cy+(2*y-7)*hh/7,
				cx+(2*x-5)*ww/7,
				cy+(2*y-5)*hh/7
			);
			drawsolid_circle(
				win, 0xffffff,
				cx+(x-3)*ww*2/7,
				cy+(y-3)*hh*2/7,
				ww/14
			);
		}
	}
}
static void pegged_draw_vbo(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int x,y;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if(data[y][x] == 0)continue;

			f[0] = (2*x-6)/7.0;
			f[1] = (2*y-6)/7.0;
			f[2] = 0.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];

			tr[0] = vr[0] / 7;
			tr[1] = vr[1] / 7;
			tr[2] = vr[2] / 7;
			tf[0] = vf[0] / 7;
			tf[1] = vf[1] / 7;
			tf[2] = vf[2] / 7;
			carveline_rect(win, 0x808080, tc, tr, tf);

			tr[0] = vr[0] / 14;
			tr[1] = vr[1] / 14;
			tr[2] = vr[2] / 14;
			tf[0] = vf[0] / 14;
			tf[1] = vf[1] / 14;
			tf[2] = vf[2] / 14;
			tu[0] = vu[0] / 14;
			tu[1] = vu[1] / 14;
			tu[2] = vu[2] / 14;
			carvesolid_sphere(win, 0xffffff, tc, tr, tf, tu);
		}
	}
}
static void pegged_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void pegged_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	int x,y;

	//<head>
	htmlprintf(win, 1,
		".pegbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".pegfg{width:14%%;height:14%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"pegbg\">\n");
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"pegfg\">%d</div>\n",
				data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void pegged_draw_tui(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void pegged_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void pegged_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)pegged_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)pegged_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)pegged_draw_html(act, pin, win, sty);
	else if(fmt == _json_)pegged_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)pegged_draw_vbo(act, pin, win, sty);
	else pegged_draw_pixel(act, pin, win, sty);
}




static void pegged_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	pegged_draw(act, pin, win, sty);
}
static void pegged_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void pegged_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void pegged_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void pegged_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void pegged_start(struct halfrel* self, struct halfrel* peer)
{
	int x,y;
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if((x>=2)&&(x<=4))data[y][x] = 1;
			else if((y>=2)&&(y<=4))data[y][x] = 1;
			else data[y][x] = 0;
		}
	}
}
static void pegged_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void pegged_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(49);
}




void pegged_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'e', 'g', 'g', 'e', 'd', 0, 0);

	p->oncreate = (void*)pegged_create;
	p->ondelete = (void*)pegged_delete;
	p->onstart  = (void*)pegged_start;
	p->onstop   = (void*)pegged_stop;
	p->oncread  = (void*)pegged_cread;
	p->oncwrite = (void*)pegged_cwrite;
	p->onsread  = (void*)pegged_sread;
	p->onswrite = (void*)pegged_swrite;
}
