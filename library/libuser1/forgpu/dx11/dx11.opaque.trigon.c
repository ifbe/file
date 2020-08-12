#include "libuser.h"
#define acc 32
#define circleacc 32
#define sphereaccx 16
#define sphereaccy (sphereaccx|0x1)
void carvetrigonindex_triangle_v4n4x4(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 v0,vec3 v1,vec3 v2);
void carvetrigonindex_rect_v4n4x4(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_circle_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_cone_v4n4x4(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vt);
void carvetrigonindex_prism4_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_cask_v4n4x4(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_sphere_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_tokamak_v4n4x4( float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_tetrahedron_v4n4x4( float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_octahedron_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_dodecahedron_v4n4x4(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_icosahedron_v4n4x4( float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);




char dx11opaquetrigon_vert[] =
"cbuffer VSConstantBuffer : register(b0){\n"
	"matrix matmvp;\n"
"};\n"
"struct VSin{\n"
	"float3 v : PA;\n"
	"float3 n : PB;\n"
	"float4 c : PC;\n"
"};\n"
"struct VSout{\n"
	"float4 where : SV_POSITION;\n"
	"float4 color : COLOR;\n"
"};\n"
"VSout main(VSin input){\n"
	"VSout output;\n"
	"output.where = mul(float4(input.v, 1.0), matmvp);\n"
	"output.color = input.c;\n"
	"return output;\n"
"}\n";
char dx11opaquetrigon_frag[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




static int opaque3d_fill(struct mysrc* src)
{
	if(0 == src->vs){
		src->vs = dx11opaquetrigon_vert;
		src->fs = dx11opaquetrigon_frag;
		src->shader_enq = 1;
	}

	struct vertex* vtx = src->vtx;
	vtx->opaque = 1;
	vtx->geometry = 3;

	if(0 == vtx->ibuf){
		vtx->ibuf_len = 0x100000;
		vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
		if(0 == vtx->ibuf)return -2;

		vtx->ibuf_w = 2*3;
		vtx->ibuf_h = 0;	//(src->ibuf_len) / (src->ibuf_w);
		vtx->ibuf_fmt = 0x222;
		src->ibuf_enq = 1;
	}

	if(0 == vtx->vbuf){
		vtx->vbuf_len = 4*4*3*0x10000;
		vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
		if(0 == vtx->vbuf)return -1;

		vtx->vbuf_w = 4*4*3;
		vtx->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		vtx->vbuf_fmt = vbuffmt_444;
		src->vbuf_enq = 1;
	}

	return 0;
}
int dx11opaquetrigon_vars(struct entity* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	if(0 == win)return -1;
	if(0 == win->dxfull_opaque)return -2;

	struct dx11data* p = win->dxfull_opaque[opaquetrigon3d];
	if(0 == p){
		p = win->dxfull_opaque[opaquetrigon3d] = memorycreate(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ilen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = opaque3d_fill(&p->src);
		if(ret < 0)return -3;
	}

	vlen = vtx->vbuf_h;
	ilen = vtx->ibuf_h;
	*vbuf = (vtx->vbuf) + (48*vlen);
	*ibuf = (vtx->ibuf) + (6*ilen);
	vtx->vbuf_h += vcnt;
	vtx->ibuf_h += icnt;

	return vlen;
}




void dx11opaque_triangle(struct entity* win, u32 rgba,
	vec3 v0, vec3 v1, vec3 v2)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, 3, 1);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*3;j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_triangle_v4n4x4(vbuf,vlen, ibuf,0, v0,v1,v2);
}




void dx11opaque_rect(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, 4, 2);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*4;j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_rect_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void dx11opaque_circle(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, circleacc+1, circleacc);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(circleacc+1);j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_circle_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void dx11opaque_prism3()
{
}
void dx11opaque_prism4(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, 24, 12);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*24;j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_prism4_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void dx11opaque_prism5()
{
}
void dx11opaque_prism6()
{
}




void dx11opaque_cone(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vt)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, acc + 1, acc);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(acc+1);j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_cone_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vt);
}
void dx11opaque_cask(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, acc * 2, acc * 2);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(acc*2);j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_cask_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void dx11opaque_cylinder(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vec3 tc;
	vec3 tf;
	dx11opaque_cask(win, rgb, vc, vr, vf, vu);

	tc[0] = vc[0]+vu[0];
	tc[1] = vc[1]+vu[1];
	tc[2] = vc[2]+vu[2];
	dx11opaque_circle(win, rgb, tc, vr, vf);

	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	tf[0] = -vf[0];
	tf[1] = -vf[1];
	tf[2] = -vf[2];
	dx11opaque_circle(win, rgb, tc, vr, tf);
}
void dx11opaque_sphere(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, sphereaccx*sphereaccy+2, sphereaccx*sphereaccy*2);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(sphereaccx*sphereaccy+2);j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_sphere_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void dx11opaque_tokamak(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, acc*acc*2, acc*acc);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(sphereaccx*sphereaccy*2);j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_tokamak_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




void dx11opaque_tetrahedron()		//4
{
}
void dx11opaque_octahedron()		//8
{
}
void dx11opaque_dodecahedron(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)		//12
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, 20, 36);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*20;j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_dodecahedron_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void dx11opaque_icosahedron(struct entity* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)		//20
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11opaquetrigon_vars(win, 0, &vbuf, &ibuf, 12, 20);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*12;j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_icosahedron_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
