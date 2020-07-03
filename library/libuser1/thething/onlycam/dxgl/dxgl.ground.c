#include "libuser.h"
//#include "unistd.h"
//#include "fcntl.h"
#define PIECE 16
#define OWNBUF buf0
int copypath(u8* path, u8* data);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


struct privdata{
	u8 dxvs[128];
	u8 dxps[128];
	u8 glvs[128];
	u8 glfs[128];
	u8 albedo[128];
	u8 normal[128];
	u8 matter[128];

	struct dx11data dx11;
	struct gl41data gl41;
};
static int loadshaderfromfile(char* buf, char* url)
{
	int ret = mysnprintf(buf, 99, "%s%s", GLSL_VERSION, GLSL_PRECISION);
	return openreadclose(url, 0, buf+ret, 0x10000-ret);
}
void ground_singlepiece(float (*vbuf)[6], float* vc,float* vr,float* vf)
{
	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;
}


static void ground_ctxfordx11(struct glsrc* src, char* tex0, char* tex1, char* vs, char* fs)
{
}
static void ground_ctxforgl41(struct glsrc* src, char* tex0, char* tex1, char* tex2, char* vs, char* fs)
{
	say("%s\n%s\n%s\n%s\n%s\n",tex0,tex1,tex2,vs,fs);

	//shader
	src->vs = memorycreate(0x10000, 0);
	loadshaderfromfile(src->vs, vs);
	src->fs = memorycreate(0x10000, 0);
	loadshaderfromfile(src->fs, fs);
	src->shader_enq = 42;

	//albedo
	src->tex[0].name = "tex0";
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], tex0);
	src->tex_enq[0] = 42;

	//normal
	src->tex[1].name = "tex1";
	src->tex[1].fmt = hex32('r','g','b','a');
	src->tex[1].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[1], tex1);
	src->tex_enq[1] = 42;

	//matter
	src->tex[2].name = "tex2";
	src->tex[2].fmt = hex32('r','g','b','a');
	src->tex[2].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[2], tex2);
	src->tex_enq[2] = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*3*2;	//sizeof(float) * float_per_attr * attr_per_trigon
	vtx->vbuf_h = 6*PIECE*PIECE;	//6point_per_block * blockx * blocky
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
}
static void ground_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41line_rect(wnd, 0xffffff, vc, vr, vf);

	struct privdata* own = act->OWNBUF;
	if(0 == own)return;
	struct glsrc* src = &own->gl41.src;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	int x,y,j;
	vec3 tc,tr,tf;
	for(j=0;j<3;j++){tr[j] = vr[j]/PIECE;tf[j] = vf[j]/PIECE;}
	for(y=0;y<PIECE;y++){
		for(x=0;x<PIECE;x++){
			for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(x+x-PIECE+1.0)/PIECE +vf[j]*(y+y-PIECE+1.0)/PIECE;
			ground_singlepiece(&vbuf[(y*PIECE+x)*6],tc,tr,tf);
		}
	}

	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', src, 1);
}




static void ground_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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
}
static void ground_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ground_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ground_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ground_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void ground_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		ground_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void ground_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void ground_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void ground_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void ground_search(struct entity* act)
{
}
static void ground_modify(struct entity* act)
{
}
static void ground_delete(struct entity* act)
{
}
static void ground_create(struct entity* act, void* str, int argc, u8** argv)
{
	int j;
	if(0 == act)return;

	struct privdata* own = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == own)return;

	//char* dxvs = 0;
	//char* dxfs = 0;
	char* glvs = 0;
	char* glfs = 0;
	char* albedo = 0;
	char* normal = 0;
	char* matter = 0;
	for(j=0;j<argc;j++){
		//say("%d:%.8s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "glvs:", 5)){
			copypath(own->glvs, argv[j]+5);
			glvs = (void*)own->glvs;
		}
		if(0 == ncmp(argv[j], "glfs:", 5)){
			copypath(own->glfs, argv[j]+5);
			glfs = (void*)own->glfs;
		}
		if(0 == ncmp(argv[j], "albedo:", 7)){
			copypath(own->albedo, argv[j]+7);
			albedo = (void*)own->albedo;
		}
		if(0 == ncmp(argv[j], "normal:", 7)){
			copypath(own->normal, argv[j]+7);
			normal = (void*)own->normal;
		}
		if(0 == ncmp(argv[j], "matter:", 7)){
			copypath(own->matter, argv[j]+7);
			matter = (void*)own->matter;
		}
	}
	//if(0 == dxvs)dxvs = "datafile/shader/ground/fv.glsl";
	//if(0 == dxfs)dxfs = "datafile/shader/ground/ff.glsl";
	if(0 == glvs)glvs = "datafile/shader/ground/fv.glsl";
	if(0 == glfs)glfs = "datafile/shader/ground/ff.glsl";
	if(0 == albedo)albedo = "datafile/jpg/wall.jpg";
	if(0 == normal)normal = "datafile/jpg/wallnormal.jpg";
	if(0 == matter)matter = "datafile/jpg/wallmatter.jpg";

	//ground_ctxforgl41(&own->gl41.src, albedo, normal, dxvs, dxfs);
	ground_ctxforgl41(&own->gl41.src, albedo, normal, matter, glvs, glfs);
}




void ground_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'r', 'o', 'u', 'n', 'd', 0, 0);

	p->oncreate = (void*)ground_create;
	p->ondelete = (void*)ground_delete;
	p->onsearch = (void*)ground_search;
	p->onmodify = (void*)ground_modify;

	p->onlinkup = (void*)ground_linkup;
	p->ondiscon = (void*)ground_discon;
	p->ontaking = (void*)ground_taking;
	p->ongiving = (void*)ground_giving;
}
