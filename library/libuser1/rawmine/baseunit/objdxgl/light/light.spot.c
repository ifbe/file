#include "libuser.h"
void fixmatrix(void* m, struct fstyle* sty);
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 u_rgb;

	u8 data[0];
};




char* spotlit_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* spotlit_glsl_f =
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




static void spotlight_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void spotlight_modify(struct entity* act)
{
}
static void spotlight_delete(struct entity* act)
{
}
static void spotlight_create(struct entity* act, void* str)
{
	struct sunbuf* sun;
	struct glsrc* src;
	if(0 == act)return;

	sun = act->buf0 = memorycreate(0x1000, 0);
	if(0 == sun)return;

	//
	sun->u_rgb = 0xff0000;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	//
	src = (void*)(sun->data);
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = spotlit_glsl_v;
	src->fs = spotlit_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}




static void spotlight_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spotlight_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct sunbuf* sun;
	struct glsrc* src;
	float (*vbuf)[6];

	vec3 tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	carveline_rect(ctx, 0xffffff, vc, vr, vt);


	sun = act->buf0;
	if(0 == sun)return;
	src = (void*)(sun->data);
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
	if(0 == vbuf)return;

	tt[0] = - vf[0];
	tt[1] = - vf[1];
	tt[2] = - vf[2];
	carvesolid_cone(ctx, sun->u_rgb, vc, vr, tt);


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
static void spotlight_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spotlight_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spotlight_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spotlight_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spotlight_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)spotlight_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)spotlight_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)spotlight_draw_html(act, pin, win, sty);
	else if(fmt == _json_)spotlight_draw_json(act, pin, win, sty);
	else spotlight_draw_pixel(act, pin, win, sty);
}
void spotlight_light(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct sunbuf* sun;
	struct glsrc* own;
	struct glsrc* src;

	sun = act->buf0;
	if(0 == sun)return;
	own = (void*)(sun->data);
	if(0 == own)return;
	src = ctx->gl_light;
	if(0 == src)return;

	src->routine_name = "passtype";
	src->routine_detail = "spotlight";

	src->arg[0].fmt = 'm';
	src->arg[0].name = "sunmvp";
	src->arg[0].data = sun->mvp;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "sunrgb";
	src->arg[1].data = sun->rgb;

	src->arg[2].fmt = 'v';
	src->arg[2].name = "sunxyz";
	src->arg[2].data = geom->frus.vc;

	src->arg[3].fmt = 'v';
	src->arg[3].name = "sundir";
	src->arg[3].data = geom->frus.vf;

	src->tex[0].glfd = own->tex[0].glfd;
	src->tex[0].name = "suntex";
	src->tex[0].fmt = '!';
	src->tex[0].enq += 1;
}




void spotlight_frustum(struct fstyle* d, struct fstyle* s)
{
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	//d->vr[3] = 1.0;
	d->vl[0] = -x / n;
	d->vl[1] = -y / n;
	d->vl[2] = -z / n;
	//d->vl[3] = -1.0;


	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	//d->vt[3] = 1.0;
	d->vb[0] = -x / n;
	d->vb[1] = -y / n;
	d->vb[2] = -z / n;
	//d->vb[3] = -1.0;


	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vn[0] = x / n;
	d->vn[1] = y / n;
	d->vn[2] = z / n;
	//d->vn[3] = 1.0;
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	//d->vf[3] = 1e20;
}
static void spotlight_matrix(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct supply* fbo, struct style* area)
{
	struct sunbuf* sun;
	struct glsrc* src;

	sun = act->buf0;
	if(0 == sun)return;
	src = fbo->gl_camera;
	if(0 == src)return;

	spotlight_frustum(&geom->frus, &geom->fs);
	fixmatrix(sun->mvp, &geom->frus);
	mat4_transpose(sun->mvp);

	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = sun->mvp;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = &geom->frus.vc;
}




static void spotlight_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

//world -> spotlight
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

#define _fbo_ hex32('f','b','o',0)
	if(stack){
		act = self->pchip;part = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len){
			spotlight_light(act,part, win,geom, wnd,area);
			spotlight_draw_vbo(act,part, win,geom, wnd,area);
		}
		if('?' == len){
			struct relation* rel = act->orel0;
			if(0 == rel)return;

			struct supply* fbo = rel->pdstchip;
			if(0 == fbo)return;

			struct style* rect = rel->pdstfoot;
			if(0 == fbo)return;

			spotlight_matrix(act,part, win,geom, fbo,rect);
			relationread(act,_fbo_, stack,rsp, buf,len);

			struct sunbuf* sun = act->buf0;
			if(0 == sun)return;

			struct glsrc* own = (void*)(sun->data);
			if(0 == own)return;

			own->tex[0].glfd = fbo->tex0;
		}
	}
}
static void spotlight_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void spotlight_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void spotlight_start(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	struct sunbuf* sun = act->buf0;
	pin->data[0] = (u64)(sun->data);
	say("@spotlight_start:%llx, %llx\n", pin->data[0], pin->data[1]);
}




void spotlight_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','p','o','t','l', 'i', 't', 0);

	p->oncreate = (void*)spotlight_create;
	p->ondelete = (void*)spotlight_delete;
	p->onsearch = (void*)spotlight_search;
	p->onmodify = (void*)spotlight_modify;

	p->onstart = (void*)spotlight_start;
	p->onstop  = (void*)spotlight_stop;
	p->onread  = (void*)spotlight_read;
	p->onwrite = (void*)spotlight_write;
}
