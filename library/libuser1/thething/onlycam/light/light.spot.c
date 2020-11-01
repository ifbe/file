#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void matproj_transpose(void* m, struct fstyle* sty);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);


#define FBOBUF buf0
#define LITBUF buf1
#define CTXBUF buf2
#define OWNBUF buf3
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 u_rgb;
	u32 glfd;
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
"uniform sampler2D shadowmap;\n"
"in mediump vec2 uvw;\n"
"layout(location = 0)out mediump vec4 FragColor;\n"
"void main(){\n"
	//"FragColor = vec4(texture(tex0, uvw).rgb, 1.0);\n"
	"mediump float n = 1.0;"
	"mediump float f = 10000.0;"
	"mediump float d = texture(shadowmap, uvw).r;"
	"mediump float c = (2.0 * n) / (f + n - d * (f - n));"
	"FragColor = vec4(c, c, c, 1.0);\n"
"}\n";




static void spotlight_forfbo_update(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct supply* fbo, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	struct gl41data* data = act->FBOBUF;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = sun->mvp;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = &geom->frus.vc;

	fbo->glfull_camera[0] = act->FBOBUF;
}
static void dirlight_forfbo_prepare(struct mysrc* src)
{
}




static void spotlight_frustum(struct fstyle* d, struct fstyle* s)
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
static void spotlight_forwnd_light_update(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	spotlight_frustum(&geom->frus, &geom->fs);
	matproj_transpose(sun->mvp, &geom->frus);

	struct gl41data* data = act->LITBUF;
	if(0 == data)return;

	data->src.tex[0].glfd = sun->glfd;
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;


	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "sunmvp";
	data->dst.arg[0].data = sun->mvp;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "sunrgb";
	data->dst.arg[1].data = sun->rgb;

	data->dst.arg[2].fmt = 'v';
	data->dst.arg[2].name = "sunxyz";
	data->dst.arg[2].data = geom->frus.vc;

	data->dst.arg[3].fmt = 'v';
	data->dst.arg[3].name = "sundir";
	data->dst.arg[3].data = geom->frus.vf;

	data->dst.texname[0] = "shadowmap";


	ctx->glfull_light[0] = act->LITBUF;
}
static void dirlight_forwnd_light_prepare(struct gl41data* data)
{
	data->dst.routine_name = "passtype";
	data->dst.routine_detail = "spotlight";
}




static void spotlight_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vt);


	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	struct mysrc* src = act->CTXBUF;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	vec3 tt;
	tt[0] = - vf[0];
	tt[1] = - vf[1];
	tt[2] = - vf[2];
	gl41solid_cone(ctx, sun->u_rgb, vc, vr, tt);


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
	gl41data_insert(ctx, 's', act->CTXBUF, 1);
}
static void spotlight_forwnd_vertex_update(struct entity* act, struct style* slot, struct supply* fbo, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	sun->glfd = fbo->tex0;
}
static void dirlight_forwnd_vertex_prepare(struct mysrc* src)
{
	//shader
	src->vs = spotlit_glsl_v;
	src->fs = spotlit_glsl_f;
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
}




static void spotlight_read_bycam(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	if('v' == key){
		spotlight_forwnd_light_update(ent,slot, wor,geom, wnd,area);
		spotlight_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
	if('?' == key){
		//search for myown fbo
		int ret;
		struct halfrel* rel[2];
		ret = relationsearch(ent, _fbo_, &rel[0], &rel[1]);
		if(ret <= 0)return;

		//update matrix for fbo
		struct supply* fbo = rel[1]->pchip;
		struct style* rect = rel[1]->pfoot;
		spotlight_forfbo_update(ent,slot, wor,geom, fbo,rect);

		//wnd.data -> fbo.texture
		give_data_into_peer(ent,_fbo_, stack,sp, 0,0, 0,0);

		//fbo.texture -> my.data -> wnd.data
		spotlight_forwnd_vertex_update(ent,slot, fbo,rect);
	}
}
static void spotlight_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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




static void spotlight_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	spotlight_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void spotlight_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void spotlight_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void spotlight_linkup(struct halfrel* self, struct halfrel* peer)
{
}




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
		if(_virtual_ == world->type){
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
	if(0 == act)return;

	sun = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == sun)return;
	sun->u_rgb = 0xff0000;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	//
	act->FBOBUF = memorycreate(0x400, 0);
	dirlight_forfbo_prepare(act->FBOBUF);

	act->LITBUF = memorycreate(0x400, 0);
	dirlight_forwnd_light_prepare(act->LITBUF);

	act->CTXBUF = memorycreate(0x400, 0);
	dirlight_forwnd_vertex_prepare(act->CTXBUF);
}




void spotlight_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','p','o','t','l', 'i', 't', 0);

	p->oncreate = (void*)spotlight_create;
	p->ondelete = (void*)spotlight_delete;
	p->onsearch = (void*)spotlight_search;
	p->onmodify = (void*)spotlight_modify;

	p->onlinkup = (void*)spotlight_linkup;
	p->ondiscon = (void*)spotlight_discon;
	p->ontaking = (void*)spotlight_taking;
	p->ongiving = (void*)spotlight_giving;
}
