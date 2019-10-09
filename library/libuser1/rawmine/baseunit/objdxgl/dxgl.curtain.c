#include "libuser.h"




char* curtain_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* curtain_glsl_f =
GLSL_VERSION
"uniform sampler2D suntex;\n"
"in mediump vec2 uvw;\n"
"layout(location = 0)out mediump vec4 FragColor;\n"
"void main(){\n"
	//"FragColor = vec4(texture(tex0, uvw).rgb, 1.0);\n"
	"mediump float n = 1.0;"
	"mediump float f = 10000.0;"
	"mediump float d = texture(suntex, uvw).r;"
	"mediump float c = (2.0 * n) / (f + n - d * (f - n));"
	"FragColor = vec4(c, c, c, 1.0);\n"
"}\n";




static void curtain_search(struct actor* act)
{
}
static void curtain_modify(struct actor* act)
{
}
static void curtain_delete(struct actor* act)
{
}
static void curtain_create(struct actor* act, void* str)
{
	struct glsrc* src = act->buf0 = memorycreate(0x1000, 0);
	if(0 == src)return;

	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = curtain_glsl_v;
	src->fs = curtain_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}




static void curtain_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void curtain_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	struct glsrc* src;
	float (*vbuf)[6];

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;

	src = act->buf0;
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
	if(0 == vbuf)return;

	//depth fbo (for debug)
	vbuf[0][0] = vc[0] - vr[0] - vt[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vt[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vt[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vt[0] - vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vt[1] - vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vt[2] - vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vt[0] - vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vt[1] - vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vt[2] - vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vt[0] - vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vt[1] - vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vt[2] - vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vt[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vt[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vt[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vt[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vt[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vt[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
}
static void curtain_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void curtain_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void curtain_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void curtain_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void curtain_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)curtain_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)curtain_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)curtain_draw_html(act, pin, win, sty);
	else if(fmt == _json_)curtain_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)curtain_draw_vbo(act, pin, win, sty);
	else curtain_draw_pixel(act, pin, win, sty);
}




static void curtain_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;

	if(ctx){
		switch(ctx->type){
			case _gl41data_:curtain_draw_vbo(act, pin, ctx, sty);break;
		}
	}
}
static void curtain_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void curtain_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void curtain_start(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	struct sunbuf* sun = act->buf0;
	pin->data[0] = (u64)(act->buf0);
}




void curtain_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'u', 'r', 't', 'a', 'i', 'n', 0);

	p->oncreate = (void*)curtain_create;
	p->ondelete = (void*)curtain_delete;
	p->onsearch = (void*)curtain_search;
	p->onmodify = (void*)curtain_modify;

	p->onstart = (void*)curtain_start;
	p->onstop  = (void*)curtain_stop;
	p->onread  = (void*)curtain_read;
	p->onwrite = (void*)curtain_write;
}
