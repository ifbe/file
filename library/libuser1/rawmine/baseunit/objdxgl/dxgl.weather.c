#include "libuser.h"




char* weather_glsl_v =
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec2 texuvw;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec2 uvw;\n"
	"void main()\n"
	"{\n"
		"uvw = texuvw;\n"
		"gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n";
char* weather_glsl_f =
	GLSL_VERSION
	"uniform sampler2D tex0;\n"
	"in mediump vec2 uvw;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"mediump float k = uvw.x*uvw.x+uvw.y*uvw.y;\n"
		"FragColor = vec4(vec3(1.0,1.0,1.0)*k, 1.0);\n"
		//"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
		//"FragColor = vec4(uvw, 1.0, 1.0);\n"
	"}\n";




static void weather_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void weather_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y,z;
	vec3 tc;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	for(z=-16;z<=16;z++)
	{
		for(y=-16;y<=16;y++)
		{
			for(x=-16;x<=16;x++)
			{
				tc[0] = vc[0] + vr[0]*x/16 + vf[0]*y/16 + vu[0]*z/16;
				tc[1] = vc[1] + vr[1]*x/16 + vf[1]*y/16 + vu[1]*z/16;
				tc[2] = vc[2] + vr[2]*x/16 + vf[2]*y/16 + vu[2]*z/16;
				carvepoint(win, 0xffffff, tc);
			}
		}
	}
}
static void weather_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void weather_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void weather_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void weather_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("weather(%x,%x,%x)\n",win,act,sty);
}
static void weather_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)weather_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)weather_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)weather_draw_html(act, pin, win, sty);
	else if(fmt == _json_)weather_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)weather_draw_vbo2d(act, pin, win, sty);
		//else weather_draw_vbo3d(act, pin, win, sty);
	}
	else weather_draw_pixel(act, pin, win, sty);
}




static void weather_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//weather_draw(act, pin, win, sty);
}
static void weather_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void weather_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void weather_start(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
/*
	//alloc
	pair = alloc_winobj(win, 's');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//
	src->geometry = 3;
	src->method = 'v';

	//shader
	src->vs = weather_glsl_v;
	src->fs = weather_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 0;
*/
}




static void weather_search(struct entity* act)
{
}
static void weather_modify(struct entity* act)
{
}
static void weather_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void weather_create(struct entity* act)
{
	if(0 == act)return;
	act->buf = memorycreate(256, 0);
}




void weather_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('w', 'e', 'a', 't', 'h', 'e', 'r', 0);

	p->oncreate = (void*)weather_create;
	p->ondelete = (void*)weather_delete;
	p->onsearch = (void*)weather_search;
	p->onmodify = (void*)weather_modify;

	p->onstart = (void*)weather_start;
	p->onstop  = (void*)weather_stop;
	p->onread  = (void*)weather_read;
	p->onwrite = (void*)weather_write;
}
