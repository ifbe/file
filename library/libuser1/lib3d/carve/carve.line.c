#include "actor.h"
#define acc 24
#define linei 0x22
#define linev 0x23
#define PI 3.1415926535897932384626433832795028841971693993151
struct texandobj
{
	u32 obj;
	u32 len;
	void* buf;
};
struct eachone
{
	u32 program;
	u32 vao;
	u32 vbo;
	u32 ibo;
	u32 tex0;
	u32 tex1;
	float light0vertex[3];
	float light0color[3];
	float light1vertex[3];
	float light1color[3];
	float modmat[4][4];
};




void carveline(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf = (mod[linev].buf) + (24*vlen);
	mod[linei].len += 1;
	mod[linev].len += 2;

	vbuf[ 0] = x1;
	vbuf[ 1] = y1;
	vbuf[ 2] = z1;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = x2;
	vbuf[ 7] = y2;
	vbuf[ 8] = z2;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
}
void carveline_bezier(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float xc, float yc, float zc)
{
	int j;
	float t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += acc;
	mod[linev].len += acc + 1;

	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*x1 + 2*t*(1.0-t)*xc + t*t*x2;
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*y1 + 2*t*(1.0-t)*yc + t*t*y2;
		vbuf[6*j+2] = (1.0-t)*(1.0-t)*z1 + 2*t*(1.0-t)*zc + t*t*z2;

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		if(j >= acc)break;
		ibuf[2*j+0] = vlen + j;
		ibuf[2*j+1] = vlen + j+1;
	}
}
void carveline_special(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz,
	float sa, float da)
{
	int j;
	float s,t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += acc;
	mod[linev].len += acc + 1;

	for(j=0;j<=acc;j++)
	{
		s = j * da / acc;
		t = j * rx / acc;

		vbuf[6*j + 0] = t * cosine(sa+s);
		vbuf[6*j + 1] = t * sine(sa+s);
		vbuf[6*j + 2] = uz * j / acc;

		vbuf[6*j + 3] = rr;
		vbuf[6*j + 4] = gg;
		vbuf[6*j + 5] = bb;

		if(j >= acc)break;
		ibuf[2*j + 0] = vlen + j;
		ibuf[2*j + 1] = vlen + j+1;
	}
}




void carveline_triangle(
	struct arena* win, u32 rgb,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += 3;
	mod[linev].len += 3;

	vbuf[ 0] = x1;
	vbuf[ 1] = y1;
	vbuf[ 2] = z1;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = x2;
	vbuf[ 7] = y2;
	vbuf[ 8] = z2;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = x3;
	vbuf[13] = y3;
	vbuf[14] = z3;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen;
	ibuf[5] = vlen+2;
}
void carveline_rect(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += 4;
	mod[linev].len += 4;

	vbuf[ 0] = cx - rx - fx;
	vbuf[ 1] = cy - ry - fy;
	vbuf[ 2] = cz - rz - fz;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = cx + rx - fx;
	vbuf[ 7] = cy + ry - fy;
	vbuf[ 8] = cz + rz - fz;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = cx - rx + fx;
	vbuf[13] = cy - ry + fy;
	vbuf[14] = cz - rz + fz;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = cx + rx + fx;
	vbuf[19] = cy + ry + fy;
	vbuf[20] = cz + rz + fz;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+3;
	ibuf[4] = vlen+3;
	ibuf[5] = vlen+2;
	ibuf[6] = vlen+0;
	ibuf[7] = vlen+2;
}
void carveline_circle(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int j;
	float t;
	float q[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += acc;
	mod[linev].len += acc;

	for(j=0;j<acc;j++)
	{
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/acc;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		vbuf[6*j+0] = rx;
		vbuf[6*j+1] = ry;
		vbuf[6*j+2] = rz;
		quaternionrotate(&vbuf[6*j], q);

		vbuf[6*j+0] += cx;
		vbuf[6*j+1] += cy;
		vbuf[6*j+2] += cz;

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		ibuf[2*j+0] = vlen+j;
		ibuf[2*j+1] = vlen+(j+1)%acc;
	}
}




void carveline_pyramid3()
{
}
void carveline_pyramid4()
{
}
void carveline_pyramid5()
{
}
void carveline_pyramid6()
{
}
void carveline_cone(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j,k;
	float s,t;
	float q[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += acc * 3;
	mod[linev].len += acc + 2;

	for(j=0;j<acc;j++)
	{
		q[0] = ux;
		q[1] = uy;
		q[2] = uz;
		vectornormalize(q);

		t = j*PI/acc;
		q[0] *= sine(t);
		q[1] *= sine(t);
		q[2] *= sine(t);
		q[3] = cosine(t);

		a = j*6;

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, q);

		vbuf[a+0] = cx + r[0];
		vbuf[a+1] = cy + r[1];
		vbuf[a+2] = cz + r[2];

		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;

		//bottom
		ibuf[a+0] = vlen+acc;
		ibuf[a+1] = vlen+j;

		//upper
		ibuf[a+2] = vlen+acc+1;
		ibuf[a+3] = vlen+j;

		//curve
		ibuf[a+4] = vlen+j;
		ibuf[a+5] = vlen+(j+1)%acc;
	}

	a = acc*6;

	vbuf[a+0] = cx;
	vbuf[a+1] = cy;
	vbuf[a+2] = cz;
	vbuf[a+0] = rr;
	vbuf[a+1] = gg;
	vbuf[a+2] = bb;

	vbuf[a+3] = cx+ux;
	vbuf[a+4] = cy+uy;
	vbuf[a+5] = cz+uz;
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
}




void carveline_prism3()
{
}
void carveline_prism4(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += 12;
	mod[linev].len += 8;

	vbuf[ 0] = cx - rx - fx - ux;
	vbuf[ 1] = cy - ry - fy - uy;
	vbuf[ 2] = cz - rz - fz - uz;
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = cx + rx - fx - ux;
	vbuf[ 7] = cy + ry - fy - uy;
	vbuf[ 8] = cz + rz - fz - uz;
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = cx - rx + fx - ux;
	vbuf[13] = cy - ry + fy - uy;
	vbuf[14] = cz - rz + fz - uz;
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = cx + rx + fx - ux;
	vbuf[19] = cy + ry + fy - uy;
	vbuf[20] = cz + rz + fz - uz;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	vbuf[24] = cx - rx - fx + ux;
	vbuf[25] = cy - ry - fy + uy;
	vbuf[26] = cz - rz - fz + uz;
	vbuf[27] = rr;
	vbuf[28] = gg;
	vbuf[29] = bb;

	vbuf[30] = cx + rx - fx + ux;
	vbuf[31] = cy + ry - fy + uy;
	vbuf[32] = cz + rz - fz + uz;
	vbuf[33] = rr;
	vbuf[34] = gg;
	vbuf[35] = bb;

	vbuf[36] = cx - rx + fx + ux;
	vbuf[37] = cy - ry + fy + uy;
	vbuf[38] = cz - rz + fz + uz;
	vbuf[39] = rr;
	vbuf[40] = gg;
	vbuf[41] = bb;

	vbuf[42] = cx + rx + fx + ux;
	vbuf[43] = cy + ry + fy + uy;
	vbuf[44] = cz + rz + fz + uz;
	vbuf[45] = rr;
	vbuf[46] = gg;
	vbuf[47] = bb;

	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+0;
	ibuf[ 3] = vlen+2;
	ibuf[ 4] = vlen+3;
	ibuf[ 5] = vlen+1;
	ibuf[ 6] = vlen+3;
	ibuf[ 7] = vlen+2;

	ibuf[ 8] = vlen+0;
	ibuf[ 9] = vlen+4;
	ibuf[10] = vlen+1;
	ibuf[11] = vlen+5;
	ibuf[12] = vlen+2;
	ibuf[13] = vlen+6;
	ibuf[14] = vlen+3;
	ibuf[15] = vlen+7;

	ibuf[16] = vlen+4;
	ibuf[17] = vlen+5;
	ibuf[18] = vlen+4;
	ibuf[19] = vlen+6;
	ibuf[20] = vlen+7;
	ibuf[21] = vlen+5;
	ibuf[22] = vlen+7;
	ibuf[23] = vlen+6;
}
void carveline_prism5()
{
}
void carveline_prism6()
{
}
void carveline_cask()
{
}
void carveline_cylinder(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
	int a,b,j,k;
	float s,t;
	float v[4];
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += acc * 3;
	mod[linev].len += acc * 2;

	for(j=0;j<acc;j++)
	{
		v[0] = ux;
		v[1] = uy;
		v[2] = uz;
		vectornormalize(v);

		t = j*PI/acc;
		v[0] *= sine(t);
		v[1] *= sine(t);
		v[2] *= sine(t);
		v[3] = cosine(t);

		a = j*12;
		b = j*6;

		r[0] = rx;
		r[1] = ry;
		r[2] = rz;
		quaternionrotate(r, v);

		vbuf[a+ 0] = cx - ux + r[0];
		vbuf[a+ 1] = cy - uy + r[1];
		vbuf[a+ 2] = cz - uz + r[2];
		vbuf[a+ 3] = rr;
		vbuf[a+ 4] = gg;
		vbuf[a+ 5] = bb;

		vbuf[a+ 6] = cx + ux + r[0];
		vbuf[a+ 7] = cy + uy + r[1];
		vbuf[a+ 8] = cz + uz + r[2];
		vbuf[a+ 9] = rr;
		vbuf[a+10] = gg;
		vbuf[a+11] = bb;

		ibuf[a+0] = vlen + j*2;
		ibuf[a+1] = vlen + ((j+1)%acc)*2;

		ibuf[a+2] = vlen + 1 + j*2;
		ibuf[a+3] = vlen + 1 + ((j+1)%acc)*2;

		ibuf[a+4] = vlen + j*2;
		ibuf[a+5] = vlen + 1 + j*2;
	}
}




void carveline_tetrahedron()
{
}
void carveline_octahedron()
{
}
void carveline_dodecahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	int j;
	float a = 1.618;
	float b = 1.0/1.618;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += 30;
	mod[linev].len += 20;

	for(j=0;j<20*6;j+=6)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}

	//(+-1, +-1, +-1)
	vbuf[  0] = cx-rx-fx-ux;
	vbuf[  1] = cy-ry-fy-uy;
	vbuf[  2] = cz-rz-fz-uz;

	vbuf[  6] = cx+rx-fx-ux;
	vbuf[  7] = cy+ry-fy-uy;
	vbuf[  8] = cz+rz-fz-uz;

	vbuf[ 12] = cx-rx+fx-ux;
	vbuf[ 13] = cy-ry+fy-uy;
	vbuf[ 14] = cz-rz+fz-uz;

	vbuf[ 18] = cx+rx+fx-ux;
	vbuf[ 19] = cy+ry+fy-uy;
	vbuf[ 20] = cz+rz+fz-uz;

	vbuf[ 24] = cx-rx-fx+ux;
	vbuf[ 25] = cy-ry-fy+uy;
	vbuf[ 26] = cz-rz-fz+uz;

	vbuf[ 30] = cx+rx-fx+ux;
	vbuf[ 31] = cy+ry-fy+uy;
	vbuf[ 32] = cz+rz-fz+uz;

	vbuf[ 36] = cx-rx+fx+ux;
	vbuf[ 37] = cy-ry+fy+uy;
	vbuf[ 38] = cz-rz+fz+uz;

	vbuf[ 42] = cx+rx+fx+ux;
	vbuf[ 43] = cy+ry+fy+uy;
	vbuf[ 44] = cz+rz+fz+uz;

	//(0, +-a, +-b)
	vbuf[ 48] = cx - a*fx - b*ux;
	vbuf[ 49] = cy - a*fy - b*uy;
	vbuf[ 50] = cz - a*fz - b*uz;

	vbuf[ 54] = cx + a*fx - b*ux;
	vbuf[ 55] = cy + a*fy - b*uy;
	vbuf[ 56] = cz + a*fz - b*uz;

	vbuf[ 60] = cx - a*fx + b*ux;
	vbuf[ 61] = cy - a*fy + b*uy;
	vbuf[ 62] = cz - a*fz + b*uz;

	vbuf[ 66] = cx + a*fx + b*ux;
	vbuf[ 67] = cy + a*fy + b*uy;
	vbuf[ 68] = cz + a*fz + b*uz;

	//(+-b, 0, +-a)
	vbuf[ 72] = cx - b*rx - a*ux;
	vbuf[ 73] = cy - b*ry - a*uy;
	vbuf[ 74] = cz - b*rz - a*uz;

	vbuf[ 78] = cx + b*rx - a*ux;
	vbuf[ 79] = cy + b*ry - a*uy;
	vbuf[ 80] = cz + b*rz - a*uz;

	vbuf[ 84] = cx - b*rx + a*ux;
	vbuf[ 85] = cy - b*ry + a*uy;
	vbuf[ 86] = cz - b*rz + a*uz;

	vbuf[ 90] = cx + b*rx + a*ux;
	vbuf[ 91] = cy + b*ry + a*uy;
	vbuf[ 92] = cz + b*rz + a*uz;

	//(+-a, +-b, 0)
	vbuf[ 96] = cx - a*rx - b*fx;
	vbuf[ 97] = cy - a*ry - b*fy;
	vbuf[ 98] = cz - a*rz - b*fz;

	vbuf[102] = cx + a*rx - b*fx;
	vbuf[103] = cy + a*ry - b*fy;
	vbuf[104] = cz + a*rz - b*fz;

	vbuf[108] = cx - a*rx + b*fx;
	vbuf[109] = cy - a*ry + b*fy;
	vbuf[110] = cz - a*rz + b*fz;

	vbuf[114] = cx + a*rx + b*fx;
	vbuf[115] = cy + a*ry + b*fy;
	vbuf[116] = cz + a*rz + b*fz;

	//front
	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+8;
	ibuf[ 2] = vlen+1;
	ibuf[ 3] = vlen+8;
	ibuf[ 4] = vlen+8;
	ibuf[ 5] = vlen+10;
	ibuf[ 6] = vlen+4;
	ibuf[ 7] = vlen+10;
	ibuf[ 8] = vlen+5;
	ibuf[ 9] = vlen+10;

	//back
	ibuf[10] = vlen+2;
	ibuf[11] = vlen+9;
	ibuf[12] = vlen+3;
	ibuf[13] = vlen+9;
	ibuf[14] = vlen+9;
	ibuf[15] = vlen+11;
	ibuf[16] = vlen+6;
	ibuf[17] = vlen+11;
	ibuf[18] = vlen+7;
	ibuf[19] = vlen+11;

	//bottom
	ibuf[20] = vlen+0;
	ibuf[21] = vlen+12;
	ibuf[22] = vlen+2;
	ibuf[23] = vlen+12;
	ibuf[24] = vlen+12;
	ibuf[25] = vlen+13;
	ibuf[26] = vlen+1;
	ibuf[27] = vlen+13;
	ibuf[28] = vlen+3;
	ibuf[29] = vlen+13;

	//upper
	ibuf[30] = vlen+4;
	ibuf[31] = vlen+14;
	ibuf[32] = vlen+6;
	ibuf[33] = vlen+14;
	ibuf[34] = vlen+14;
	ibuf[35] = vlen+15;
	ibuf[36] = vlen+5;
	ibuf[37] = vlen+15;
	ibuf[38] = vlen+7;
	ibuf[39] = vlen+15;

	//left
	ibuf[40] = vlen+0;
	ibuf[41] = vlen+16;
	ibuf[42] = vlen+4;
	ibuf[43] = vlen+16;
	ibuf[44] = vlen+16;
	ibuf[45] = vlen+18;
	ibuf[46] = vlen+2;
	ibuf[47] = vlen+18;
	ibuf[48] = vlen+6;
	ibuf[49] = vlen+18;

	//right
	ibuf[50] = vlen+1;
	ibuf[51] = vlen+17;
	ibuf[52] = vlen+5;
	ibuf[53] = vlen+17;
	ibuf[54] = vlen+17;
	ibuf[55] = vlen+19;
	ibuf[56] = vlen+3;
	ibuf[57] = vlen+19;
	ibuf[58] = vlen+7;
	ibuf[59] = vlen+19;
}
void carveline_icosahedron(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float ux, float uy, float uz)
{
	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += 30;
	mod[linev].len += 12;

	for(j=0;j<12*6;j++)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}

	//(+-m, 0, +-n)
	vbuf[ 0] = cx - m;
	vbuf[ 1] = cy;
	vbuf[ 2] = cz - n;

	vbuf[ 6] = cx + m;
	vbuf[ 7] = cy;
	vbuf[ 8] = cz - n;

	vbuf[12] = cx - m;
	vbuf[13] = cy;
	vbuf[14] = cz + n;

	vbuf[18] = cx + m;
	vbuf[19] = cy;
	vbuf[20] = cz + n;

	//(0, +-n, +-m)
	vbuf[24] = cx;
	vbuf[25] = cy - n;
	vbuf[26] = cz - m;

	vbuf[30] = cx;
	vbuf[31] = cy + n;
	vbuf[32] = cz - m;

	vbuf[36] = cx;
	vbuf[37] = cy - n;
	vbuf[38] = cz + m;

	vbuf[42] = cx;
	vbuf[43] = cy + n;
	vbuf[44] = cz + m;

	//(+-n, +-m, 0)
	vbuf[48] = cx - n;
	vbuf[49] = cy - m;
	vbuf[50] = cz;

	vbuf[54] = cx + n;
	vbuf[55] = cy - m;
	vbuf[56] = cz;

	vbuf[60] = cx - n;
	vbuf[61] = cy + m;
	vbuf[62] = cz;

	vbuf[66] = cx + n;
	vbuf[67] = cy + m;
	vbuf[68] = cz;

	ibuf[ 0] = vlen+2;
	ibuf[ 1] = vlen+3;
	ibuf[ 2] = vlen+3;
	ibuf[ 3] = vlen+7;
	ibuf[ 4] = vlen+2;
	ibuf[ 5] = vlen+7;
	ibuf[ 6] = vlen+7;
	ibuf[ 7] = vlen+11;
	ibuf[ 8] = vlen+3;
	ibuf[ 9] = vlen+11;

	ibuf[10] = vlen+3;
	ibuf[11] = vlen+6;
	ibuf[12] = vlen+3;
	ibuf[13] = vlen+9;
	ibuf[14] = vlen+6;
	ibuf[15] = vlen+9;
	ibuf[16] = vlen+6;
	ibuf[17] = vlen+2;
	ibuf[18] = vlen+9;
	ibuf[19] = vlen+11;

	ibuf[20] = vlen+0;
	ibuf[21] = vlen+1;
	ibuf[22] = vlen+0;
	ibuf[23] = vlen+4;
	ibuf[24] = vlen+0;
	ibuf[25] = vlen+5;

	ibuf[26] = vlen+0;
	ibuf[27] = vlen+8;
	ibuf[28] = vlen+0;
	ibuf[29] = vlen+10;
	ibuf[30] = vlen+1;
	ibuf[31] = vlen+4;

	ibuf[32] = vlen+4;
	ibuf[33] = vlen+8;
	ibuf[34] = vlen+1;
	ibuf[35] = vlen+5;
	ibuf[36] = vlen+5;
	ibuf[37] = vlen+10;
	ibuf[38] = vlen+8;
	ibuf[39] = vlen+10;

	ibuf[40] = vlen+1;
	ibuf[41] = vlen+11;
	ibuf[42] = vlen+5;
	ibuf[43] = vlen+7;
	ibuf[44] = vlen+10;
	ibuf[45] = vlen+2;
	ibuf[46] = vlen+8;
	ibuf[47] = vlen+6;
	ibuf[48] = vlen+4;
	ibuf[49] = vlen+9;
	ibuf[50] = vlen+1;
	ibuf[51] = vlen+9;
	ibuf[52] = vlen+5;
	ibuf[53] = vlen+11;
	ibuf[54] = vlen+10;
	ibuf[55] = vlen+7;
	ibuf[56] = vlen+8;
	ibuf[57] = vlen+2;
	ibuf[58] = vlen+4;
	ibuf[59] = vlen+6;
}
void carveline_sphere(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float ux, float uy, float uz)
{
#define odd ((acc&0xfffe)+1)
	int a,b,j,k;
	float s,t;
	float v[4];
	float tempcx,tempcy,tempcz;
	float temprx,tempry,temprz;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->buf;
	int ilen = mod[linei].len;
	int vlen = mod[linev].len;
	u16* ibuf = (mod[linei].buf) + (4*ilen);
	float* vbuf  = (mod[linev].buf) + (24*vlen);
	mod[linei].len += odd*(odd+odd-2);
	mod[linev].len += odd*(odd-2)+2;

	for(k=0;k<(odd-2);k++)
	{
		s = (k+1-(odd/2))*PI/(odd-1);
		t = cosine(s);
		temprx = rx*t;
		tempry = ry*t;
		temprz = rz*t;

		t = sine(s);
		tempcx = cx + ux*t;
		tempcy = cy + uy*t;
		tempcz = cz + uz*t;

		for(j=0;j<odd;j++)
		{
			v[0] = ux;
			v[1] = uy;
			v[2] = uz;
			vectornormalize(v);

			t = j*PI/odd;
			v[0] *= sine(t);
			v[1] *= sine(t);
			v[2] *= sine(t);
			v[3] = cosine(t);

			a = (k*odd + j)*6;
			b = (k*odd + j)*2;

			vbuf[a+0] = temprx;
			vbuf[a+1] = tempry;
			vbuf[a+2] = temprz;
			quaternionrotate(&vbuf[a], v);

			vbuf[a+0] += tempcx;
			vbuf[a+1] += tempcy;
			vbuf[a+2] += tempcz;

			vbuf[a+3] = rr;
			vbuf[a+4] = gg;
			vbuf[a+5] = bb;

			ibuf[b+0] = vlen+(k*odd)+j;
			ibuf[b+1] = vlen+(k*odd)+(j+1)%odd;
		}
	}

	for(k=0;k<odd;k++)
	{
		a = odd*(odd-2)*2 + k*(odd-3)*2;
		for(j=0;j<(odd-3);j++)
		{
			ibuf[a + 2*j + 0] = vlen+k+(j*odd);
			ibuf[a + 2*j + 1] = vlen+k+(j*odd)+odd;
		}
	}

	a = odd*(odd-2)*6;
	vbuf[a+ 0] = cx-ux;
	vbuf[a+ 1] = cy-uy;
	vbuf[a+ 2] = cz-uz;
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;

	vbuf[a+ 6] = cx+ux;
	vbuf[a+ 7] = cy+uy;
	vbuf[a+ 8] = cz+uz;
	vbuf[a+ 9] = rr;
	vbuf[a+10] = gg;
	vbuf[a+11] = bb;

	b = odd*(odd-2)*2+odd*(odd-3)*2;
	for(j=0;j<odd;j++)
	{
		ibuf[b + 2*j +0] = vlen+odd*(odd-2);
		ibuf[b + 2*j +1] = vlen+j;

		ibuf[b + 2*j + odd*2 + 0] = vlen+odd*(odd-2)+1;
		ibuf[b + 2*j + odd*2 + 1] = vlen+odd*(odd-3)+j;
	}
}
