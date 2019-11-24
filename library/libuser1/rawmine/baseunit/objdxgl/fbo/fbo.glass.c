#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void entitycreatefromfile(struct entity* act, char* name);



/*
char* glass_glsl2d_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"void main()\n"
"{\n"
	"uvw = texuvw;\n"
	"gl_Position = vec4(vertex, 1.0);\n"
"}\n";
*/
char* glass_glsl_v =
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

char* glass_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"void main()\n"
"{\n"
	"mediump vec3 c = texture(tex0, uvw).rgb;\n"
	"FragColor = vec4(c, 0.5);\n"
"}\n";




void glasscamera(
	struct entity* leaf, struct style* lf,
	struct entity* twig, struct style* tf,
	struct entity* root, struct style* rf)
{/*
	float x,y,z,t;
	vec3 p,q;
	struct relation* rel;
	struct entity* fbo;
	struct glsrc* src = (void*)(lf->foot[0]);
	struct gldst* dst = (void*)(tf->foot[0]);

	rel = leaf->orel0;
	if(0 == rel)return;

	fbo = (void*)(rel->dstchip);
	if(0 == fbo)return;
	if(_fbo_ != fbo->fmt)return;

	//say("tex_rgb=%x\n", fbo->tex_color);
	dst->tex[0] = fbo->tex_color;


	//mirror.n
	x = -tf->f.vf[0];
	y = -tf->f.vf[1];
	z = -tf->f.vf[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//op*cos(on,op): t = op * mirror.n
	t = (root->camera.vc[0] - tf->f.vc[0])*x
	  + (root->camera.vc[1] - tf->f.vc[1])*y
	  + (root->camera.vc[2] - tf->f.vc[2])*z;

	//dir*len: fbo.n = t*mirror.n + offset
	fbo->camera.vn[0] = x * t * 1.001;
	fbo->camera.vn[1] = y * t * 1.001;
	fbo->camera.vn[2] = z * t * 1.001;

	//foot of a perpendicular: fbo.q = p - t*mirror.n
	fbo->camera.vq[0] = root->camera.vc[0] - t*x;
	fbo->camera.vq[1] = root->camera.vc[1] - t*y;
	fbo->camera.vq[2] = root->camera.vc[2] - t*z;

	//reflected point: p' = p - 2*t*mirror.n
	fbo->camera.vc[0] = root->camera.vc[0] - 2*t*x;
	fbo->camera.vc[1] = root->camera.vc[1] - 2*t*y;
	fbo->camera.vc[2] = root->camera.vc[2] - 2*t*z;


	//r = -mirror.r
	x = -tf->f.vr[0];
	y = -tf->f.vr[1];
	z = -tf->f.vr[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//l.len = (l-q) * nr
	t = (tf->f.vc[0] + tf->f.vr[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vc[1] + tf->f.vr[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vc[2] + tf->f.vr[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vl[0] = x * t;
	fbo->camera.vl[1] = y * t;
	fbo->camera.vl[2] = z * t;

	//r.len = (r-q) * nr
	t = (tf->f.vc[0] - tf->f.vr[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vc[1] - tf->f.vr[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vc[2] - tf->f.vr[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vr[0] = x * t;
	fbo->camera.vr[1] = y * t;
	fbo->camera.vr[2] = z * t;


	//mirror.t
	x = tf->f.vt[0];
	y = tf->f.vt[1];
	z = tf->f.vt[2];
	t = squareroot(x*x + y*y + z*z);
	x /= t;
	y /= t;
	z /= t;

	//b.len =  = (b-q) * nt
	t = (tf->f.vc[0] - tf->f.vt[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vc[1] - tf->f.vt[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vc[2] - tf->f.vt[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vb[0] = x * t;
	fbo->camera.vb[1] = y * t;
	fbo->camera.vb[2] = z * t;

	//t.len = (u-q) * nt
	t = (tf->f.vt[0] + tf->f.vt[0] - fbo->camera.vq[0]) * x
	  + (tf->f.vt[1] + tf->f.vt[1] - fbo->camera.vq[1]) * y
	  + (tf->f.vt[2] + tf->f.vt[2] - fbo->camera.vq[2]) * z;
	fbo->camera.vt[0] = x * t;
	fbo->camera.vt[1] = y * t;
	fbo->camera.vt[2] = z * t;

	carvefrustum(root, &fbo->camera);
*/
/*
	say("%f,%f,%f\n",root->camera.vc[0], root->camera.vc[1], root->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vc[0], tmp->camera.vc[1], tmp->camera.vc[2]);
	say("%f,%f,%f\n",tmp->camera.vn[0], tmp->camera.vn[1], tmp->camera.vn[2]);
	say("%f,%f,%f\n",tmp->camera.vl[0], tmp->camera.vl[1], tmp->camera.vl[2]);
	say("%f,%f,%f\n",tmp->camera.vr[0], tmp->camera.vr[1], tmp->camera.vr[2]);
	say("%f,%f,%f\n",tmp->camera.vb[0], tmp->camera.vb[1], tmp->camera.vb[2]);
	say("%f,%f,%f\n",tmp->camera.vu[0], tmp->camera.vu[1], tmp->camera.vu[2]);
	say("\n");

	carveline_rect(root, 0xffffff, tf->vc, tf->vr, tf->vf);
	p[0] = tf->vc[0] - tf->vr[0] - tf->vf[0];
	p[1] = tf->vc[1] - tf->vr[1] - tf->vf[1];
	p[2] = tf->vc[2] - tf->vr[2] - tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
	p[0] = tf->vc[0] + tf->vr[0] - tf->vf[0];
	p[1] = tf->vc[1] + tf->vr[1] - tf->vf[1];
	p[2] = tf->vc[2] + tf->vr[2] - tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
	p[0] = tf->vc[0] - tf->vr[0] + tf->vf[0];
	p[1] = tf->vc[1] - tf->vr[1] + tf->vf[1];
	p[2] = tf->vc[2] - tf->vr[2] + tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
	p[0] = tf->vc[0] + tf->vr[0] + tf->vf[0];
	p[1] = tf->vc[1] + tf->vr[1] + tf->vf[1];
	p[2] = tf->vc[2] + tf->vr[2] + tf->vf[2];
	q[0] = 2*p[0] - tmp->camera.vc[0];
	q[1] = 2*p[1] - tmp->camera.vc[1];
	q[2] = 2*p[2] - tmp->camera.vc[2];
	carveline(root, 0xffffff, tmp->camera.vc, q);
*/
}




static void glass_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void glass_draw_vbo(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	struct glsrc* src = (void*)(pin->data[0]);
	float (*vbuf)[6] = (void*)(src->vbuf);
	//carvesolid_rect(win, 0xffffff, vc, vr, vf);

	vbuf[0][0] = vc[0] - vr[0] - vu[0];
	vbuf[0][1] = vc[1] - vr[1] - vu[1];
	vbuf[0][2] = vc[2] - vr[2] - vu[2];
	vbuf[0][3] = 1.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vu[0];
	vbuf[1][1] = vc[1] + vr[1] + vu[1];
	vbuf[1][2] = vc[2] + vr[2] + vu[2];
	vbuf[1][3] = 0.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vu[0];
	vbuf[2][1] = vc[1] - vr[1] + vu[1];
	vbuf[2][2] = vc[2] - vr[2] + vu[2];
	vbuf[2][3] = 1.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vu[0];
	vbuf[3][1] = vc[1] + vr[1] + vu[1];
	vbuf[3][2] = vc[2] + vr[2] + vu[2];
	vbuf[3][3] = 0.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vu[0];
	vbuf[4][1] = vc[1] - vr[1] - vu[1];
	vbuf[4][2] = vc[2] - vr[2] - vu[2];
	vbuf[4][3] = 1.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vu[0];
	vbuf[5][1] = vc[1] + vr[1] - vu[1];
	vbuf[5][2] = vc[2] + vr[2] - vu[2];
	vbuf[5][3] = 0.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	//glasscamera(act, pin, 0, sty, win, 0);
}
static void glass_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void glass_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)glass_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)glass_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)glass_draw_html(act, pin, win, sty);
	else if(fmt == _json_)glass_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)glass_draw_vbo(act, pin, win, sty);
	else glass_draw_pixel(act, pin, win, sty);
}




static void glass_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//glass_draw(act, pin, win, sty);
}
static void glass_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void glass_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void glass_start(struct halfrel* self, struct halfrel* peer)
{
	struct datapair* pair;
	struct glsrc* src;
	struct gldst* dst;

	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
/*
	//
	pair = alloc_winobj(win, 'o');
	src = &pair->src;
	dst = &pair->dst;
	pin->foot[0] = (u64)src;
	sty->foot[0] = (u64)dst;

	//
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = glass_glsl_v;
	src->fs = glass_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
	src->vbuf_enq = 0;

	//texture
	src->tex_name[0] = "tex0";
	src->tex_enq[0] = 0;
*/
}




static void glass_search(struct entity* act)
{
}
static void glass_modify(struct entity* act)
{
}
static void glass_delete(struct entity* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
	act->buf = 0;
}
static void glass_create(struct entity* act, void* str)
{
	void* win;
	if(0 == act)return;

	//win = entitycreate(_fbo_, 0);
	//if(win)relationcreate(win, 0, _win_, 0, act, 0, _act_, 0);
}




void glass_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'l', 'a', 's', 's', 0, 0, 0);

	p->oncreate = (void*)glass_create;
	p->ondelete = (void*)glass_delete;
	p->onsearch = (void*)glass_search;
	p->onmodify = (void*)glass_modify;

	p->onstart = (void*)glass_start;
	p->onstop  = (void*)glass_stop;
	p->onread  = (void*)glass_read;
	p->onwrite = (void*)glass_write;
}
