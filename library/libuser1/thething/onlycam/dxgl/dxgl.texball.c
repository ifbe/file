#include "libuser.h"
#define CTXBUF buf0
void carveplanet(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




char* texball_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* texball_glsl_t = 0;
char* texball_glsl_g = 0;
char* texball_glsl_f = 
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	//"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";




void texball_ctxforwnd(struct glsrc* src, char* str)
{
	//shader
	src->vs = texball_glsl_v;
	src->fs = texball_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], str);
	src->tex_enq[0] = 42;
	//say("w=%d,h=%d\n",src->tex[0].w, src->tex[0].h);

#define accx 64
#define accy 63
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*6;
	vtx->vbuf_h = accx*accy+(accx-1)*2;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
	src->vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accy*(accx-1)*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
	src->ibuf_enq = 0;
}
static void texball_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
{
	struct glsrc* src = act->CTXBUF;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	carveplanet(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	gl41data_insert(ctx, 's', src, 1);
}




static void texball_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{/*
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	xmax = act->width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf) + 4*y*(act->width);
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->fmt)&0xff))
		{
			for(x=0;x<xmax;x++)dst[x] = src[x];
		}
		else
		{
			for(x=0;x<xmax;x++)
			{
				tmp = src[x];
				dst[x] = 0xff000000 | (tmp&0xff00) | ((tmp>>16)&0xff) | ((tmp&0xff)<<16);
			}
		}
	}*/
}
static void texball_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texball_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texball_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texball_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("texball(%x,%x,%x)\n",win,act,sty);
}
static void texball_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
}




static void texball_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		texball_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void texball_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}




static void texball_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* sup = stack[sp-2].pchip;
	switch(sup->fmt){
	case _gl41fbog_:
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		texball_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	default:{
		texball_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
}
static void texball_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void texball_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void texball_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@texball_linkup\n");
}




static void texball_search(struct entity* act)
{
}
static void texball_modify(struct entity* act)
{
}
static void texball_delete(struct entity* act)
{
	if(0 == act)return;
}
static void texball_create(struct entity* act, void* str)
{
	void* ctx;
	if(0 == act)return;

	ctx = act->CTXBUF = memorycreate(0x200, 0);
	if(0 == ctx)return;

	if(0 == str)str = "datafile/jpg/texball-earth.jpg";
	texball_ctxforwnd(ctx, str);
}




void texball_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'x', 'b', 'a', 'l', 'l', 0);

	p->oncreate = (void*)texball_create;
	p->ondelete = (void*)texball_delete;
	p->onsearch = (void*)texball_search;
	p->onmodify = (void*)texball_modify;

	p->onlinkup = (void*)texball_linkup;
	p->ondiscon = (void*)texball_discon;
	p->ontaking = (void*)texball_taking;
	p->ongiving = (void*)texball_giving;
}
