#include "libuser.h"
#define font2d 4
int utf2unicode(u8* src,u32* dst);
int ascii2d_vars(struct arena* win, int id, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct datapair* mod = win->mod;
	struct glsrc* src = &mod[id].src;
	int vlen = src->vbuf_h;
	int ilen = src->ibuf_h;

	*vbuf = (src->vbuf) + (36*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




//ascii is 8x16, but in 16x16, so move right 1/2
void carve2d_ascii(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8 dat)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = ascii2d_vars(win, font2d, &vbuf, &ibuf, 4, 2);

	vbuf[ 0] = vc[0]+vr[0]/2-vr[0]-vf[0];
	vbuf[ 1] = vc[1]+vr[1]/2-vr[1]-vf[1];
	vbuf[ 2] = vc[2]+vr[2]/2-vr[2]-vf[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (dat+0.0)/128.0;
	vbuf[ 7] = 1.0/128.0;

	vbuf[ 9] = vc[0]+vr[0]/2+vr[0]-vf[0];
	vbuf[10] = vc[1]+vr[1]/2+vr[1]-vf[1];
	vbuf[11] = vc[2]+vr[2]/2+vr[2]-vf[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = (dat+1.0)/128.0;
	vbuf[16] = 1.0/128.0;

	vbuf[18] = vc[0]+vr[0]/2-vr[0]+vf[0];
	vbuf[19] = vc[1]+vr[1]/2-vr[1]+vf[1];
	vbuf[20] = vc[2]+vr[2]/2-vr[2]+vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (dat+0.0)/128.0;
	vbuf[25] = 0.0;

	vbuf[27] = vc[0]+vr[0]/2+vr[0]+vf[0];
	vbuf[28] = vc[1]+vr[1]/2+vr[1]+vf[1];
	vbuf[29] = vc[2]+vr[2]/2+vr[2]+vf[2];
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = (dat+1.0)/128.0;
	vbuf[34] = 0.0;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+3;
	ibuf[3] = vlen+0;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}
void carve2d_unicode(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 unicode)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vvv = (unicode&0xffff)/0x4000;
	int vlen = ascii2d_vars(win, font2d+vvv, &vbuf, &ibuf, 4, 2);

	unicode = unicode&0x3fff;
	vbuf[ 0] = vc[0]-vr[0]-vf[0];
	vbuf[ 1] = vc[1]-vr[1]-vf[1];
	vbuf[ 2] = vc[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;
	vbuf[ 6] = (unicode&0x7f)/128.0;
	vbuf[ 7] = ((unicode>>7)+1)/128.0;

	vbuf[ 9] = vc[0]+vr[0]-vf[0];
	vbuf[10] = vc[1]+vr[1]-vf[1];
	vbuf[11] = vc[2];
	vbuf[12] = rr;
	vbuf[13] = gg;
	vbuf[14] = bb;
	vbuf[15] = ((unicode&0x7f)+1)/128.0;
	vbuf[16] = ((unicode>>7)+1)/128.0;

	vbuf[18] = vc[0]-vr[0]+vf[0];
	vbuf[19] = vc[1]-vr[1]+vf[1];
	vbuf[20] = vc[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;
	vbuf[24] = (unicode&0x7f)/128.0;
	vbuf[25] = (unicode>>7)/128.0;

	vbuf[27] = vc[0]+vr[0]+vf[0];
	vbuf[28] = vc[1]+vr[1]+vf[1];
	vbuf[29] = vc[2];
	vbuf[30] = rr;
	vbuf[31] = gg;
	vbuf[32] = bb;
	vbuf[33] = ((unicode&0x7f)+1)/128.0;
	vbuf[34] = (unicode>>7)/128.0;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+3;
	ibuf[3] = vlen+0;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}




void carve2d_utf8(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	u32 unicode;
	utf2unicode(buf, &unicode);
	carve2d_unicode(win, rgb, vc, vr, vf, unicode);
}
void carve2d_decimal(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 val)
{
	int j,len;
	u8 str[8];
	vec3 tc;

	for(len=0;len<8;len++)
	{
		if(0 == val)break;
		str[len] = 0x30 + (val%10);
		val = val/10;
	}
	if(len == 0)
	{
		len = 1;
		str[0] = '0';
	}

	for(j=0;j<len;j++)
	{
		tc[0] = vc[0] + vr[0]*(j-len/2)*2;
		tc[1] = vc[1] + vr[1]*(j-len/2)*2;
		tc[2] = vc[2];
		carve2d_ascii(win, rgb, tc, vr, vf, str[len-1-j]);
	}
}
void carve2d_hexadecimal(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u32 val)
{
	int j,len;
	u8 str[8];
	vec3 tc;

	for(len=0;len<8;len++)
	{
		if(0 == val)break;
		str[len] = 0x30 + (val&0xf);
		if(str[len] > 0x39)str[len] += 7;
		val = val>>4;
	}
	if(len == 0)
	{
		len = 1;
		str[0] = '0';
	}

	for(j=0;j<len;j++)
	{
		tc[0] = vc[0] + vr[0]*(2*(j-len/2)+1);
		tc[1] = vc[1] + vr[1]*(2*(j-len/2)+1);
		tc[2] = vc[2];
		carve2d_ascii(win, rgb, tc, vr, vf, str[len-1-j]);
	}
}
void carve2d_string(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	int j;
	vec3 tc;
	vec3 tr;

	if(0 == buf)return;
	if(0 == len){while(buf[len] > 0x20)len++;}
	else
	{
		for(j=0;j<len;j++)
		{
			if(buf[j] < 0x20){len = j;break;}
		}
	}
	if(len == 0)return;

	for(j=0;j<len;j++)
	{
		tc[0] = vc[0] + (vr[0]/2)*(j+1);
		tc[1] = vc[1] + (vr[1]/2)*(j+1);
		tc[2] = vc[2] + (vr[2]/2)*(j+1);
		tr[0] = vr[0]/2;
		tr[1] = vr[1]/2;
		tr[2] = vr[2]/2;
		carve2d_ascii(win, rgb, tc, tr, vf, buf[j]);
	}
}
void carvestring2d_center(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, u8* buf, int len)
{
	int j;
	vec3 tc;
	vec3 tr;

	if(0 == buf)return;
	if(0 == len){while(buf[len] >= 0x20)len++;}
	else
	{
		for(j=0;j<len;j++)
		{
			if(buf[j] < 0x20){len = j;break;}
		}
	}
	if(len == 0)return;

	for(j=0;j<len;j++)
	{
		tc[0] = vc[0] + (vr[0]/4)*(2*j-len+1);
		tc[1] = vc[1] + (vr[1]/4)*(2*j-len+1);
		tc[2] = vc[2] + (vr[2]/4)*(2*j-len+1);
		tr[0] = vr[0]/2;
		tr[1] = vr[1]/2;
		tr[2] = vr[2]/2;
		carve2d_ascii(win, rgb, tc, tr, vf, buf[j]);
	}
}
void carve2d_double(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, double data)
{
	u8 mystr[100];
	double2decstr(data, mystr);
	carve2d_string(win, rgb, vc, vr, vf, mystr, 0);
}




void carvetext2d(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf,
	u8* buf, int len)
{
}
void carvetext2d_reverse(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf,
	u8* buf, int len)
{
	int j,k;
	float f;
	vec3 tc;
	vec3 tr;
	vec3 tf;
	if(0 == buf)return;
	if(0 == len)return;

	f = 16.0 / (float)(win->fbheight);
	tf[0] = vf[0] * f;
	tf[1] = vf[1] * f;
	tf[2] = vf[2] * f;

	tc[0] = vc[0] - vr[0] - vf[0] + tf[0];
	tc[1] = vc[1] - vr[1] - vf[1] + tf[1];
	tc[2] = vc[2] - vr[2] - vf[2] + tf[2];

	k = len;
	for(j=len-1;j>=0;j--)
	{
		if(tc[1] >= 1.0)break;

		f = 32.0 / (float)(win->fbwidth);
		tr[0] = vr[0] * f;
		tr[1] = vr[1] * f;
		tr[2] = vr[2] * f;

		if(0 == j)
		{
			carve2d_string(win, rgb, tc, tr, tf, buf, k);
			break;
		}

		if('\n' == buf[j])
		{
			carve2d_string(win, rgb, tc, tr, tf, buf+j+1, k-j-1);
			tc[0] += 2*tf[0];
			tc[1] += 2*tf[1];
			tc[2] += 2*tf[2];

			k = j;
		}
	}
}
