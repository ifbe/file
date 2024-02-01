#include "libuser.h"
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);
void gl41data_addcam(_obj* wnd, struct gl41data* data);
void gl41data_addlit(_obj* wnd, struct gl41data* data);
void gl41data_insert(_obj* ctx, int type, struct gl41data* src, int cnt);


#define OWNBUF listptr.buf0
struct sunbuf{
	mat4 wvp;
	vec4 rgb;
	u32 u_rgb;
	struct gl41data cam;
	struct gl41data lit;
	struct gl41data ctx;
};




char* spotlit_glsl_v =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"out vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* spotlit_glsl_f =
GLSL_VERSION
GLSL_PRECISION
"uniform sampler2D shadowmap;\n"
"in vec2 uvw;\n"
"layout(location = 0)out vec4 FragColor;\n"
"void main(){\n"
	//"FragColor = vec4(texture(tex0, uvw).rgb, 1.0);\n"
	"float n = 1.0;"
	"float f = 10000.0;"
	"float d = texture(shadowmap, uvw).r;"
	"float c = (2.0 * n) / (f + n - d * (f - n));"
	"FragColor = vec4(c, c, c, 1.0);\n"
"}\n";




static void spotlight_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spotlight_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spotlight_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spotlight_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spotlight_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
static void spotlight_lit_update(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	struct gl41data* data = &sun->lit;
	if(0 == data)return;

	//common
	data->src.tex[0].glfd = sun->cam.dst.tex[0];
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;

	//gl41
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "sunmvp";
	data->dst.arg[0].data = sun->wvp;

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

	gl41data_addlit(wnd, data);
}
static void spotlight_lit_prepare(struct gl41data* data)
{
	data->dst.lighttype = _spotlight_;
}




static void spotlight_cam_update(
	_obj* act, struct style* part,
	_obj* wrd, struct style* geom,
	_obj* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;
	struct gl41data* data = &sun->cam;
	if(0 == data)return;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = sun->wvp;

	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = &geom->frus.vc;

	gl41data_addcam(wnd, data);
}
static void spotlight_cam_prepare(struct mysrc* src)
{
	src->tex[0].w = 1024;
	src->tex[0].h = 1024;
	src->tex[0].fmt = 0;
	src->tex[0].glfd = 0;

	src->type = 'd';
	src->target_enq = 42;
}




static void spotlight_mesh_update(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vt);


	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	vec3 tt;
	tt[0] = - vf[0];
	tt[1] = - vf[1];
	tt[2] = - vf[2];
	gl41solid_cone(ctx, sun->u_rgb, vc, vr, tt);


	//depth fbo (for debug)
	struct gl41data* mesh = &sun->ctx;
	if(0 == mesh)return;
	float (*vbuf)[6] = (void*)(mesh->src.vtx[0].vbuf);
	if(0 == vbuf)return;

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

	mesh->src.vbuf_enq += 1;
	gl41data_insert(ctx, 's', mesh, 1);
}
static void spotlight_mesh_prepare(struct mysrc* src)
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
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
}




static void spotlight_read_bywnd(_obj* ent,void* foot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void spotlight_read_byworld_bycam_bywnd(_obj* ent,void* foot, _syn* stack,int sp)
{
	if(0 == stack)return;

	_obj* wor;struct style* geom;
	_obj* dup;struct style* camg;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	struct sunbuf* sun = ent->OWNBUF;
	spotlight_frustum(&geom->frus, &geom->fs);

	switch(wnd->vfmt){
	case _gl41list_:
		world2clip_projznzp_transpose(sun->wvp, &geom->frus);

		spotlight_cam_update(ent,foot, wor,geom, wnd,area);
		spotlight_lit_update(ent,foot, wor,geom, wnd,area);
		spotlight_mesh_update(ent,foot, wor,geom, wnd,area);
		break;
	default:
		world2clip_projz0z1_transpose(sun->wvp, &geom->frus);
		break;
	}
}




static void spotlight_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
		spotlight_read_bywnd(ent,slot, caller,area);
		break;
	default:
		spotlight_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void spotlight_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void spotlight_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void spotlight_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void spotlight_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;
	struct fstyle* obb = 0;
	//logtoall("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

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
static void spotlight_modify(_obj* act)
{
}
static void spotlight_delete(_obj* act)
{
}
static void spotlight_create(_obj* act, void* str)
{
	if(0 == act)return;

	struct sunbuf* sun = act->OWNBUF = memoryalloc(0x10000, 0);
	if(0 == sun)return;

	sun->u_rgb = 0xff0000;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	spotlight_cam_prepare(&sun->cam.src);

	spotlight_lit_prepare(&sun->lit);

	spotlight_mesh_prepare(&sun->ctx.src);
}




void spotlight_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s','p','o','t','l', 'i', 't', 0);

	p->oncreate = (void*)spotlight_create;
	p->ondelete = (void*)spotlight_delete;
	p->onreader = (void*)spotlight_search;
	p->onwriter = (void*)spotlight_modify;

	p->onattach = (void*)spotlight_attach;
	p->ondetach = (void*)spotlight_detach;
	p->ontaking = (void*)spotlight_taking;
	p->ongiving = (void*)spotlight_giving;
}
