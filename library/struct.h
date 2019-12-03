typedef float f32;
typedef double f64;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float vec1[1];
typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef float mat1[1][1];
typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];




struct str
{
	int len;
	u8 buf[0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};




struct halfrel
{
	union{
		u64 chip;
		void* pchip;
	};
	union{
		u64 foot;
		void* pfoot;
	};
	u32 type;
	u32 flag;
};
struct relation
{
	//[0x00,0x1f]
	union{
		u8 dst[0];
		u64 dstchip;
		void* pdstchip;
	};
	union{
		u64 dstfoot;
		void* pdstfoot;
	};
	u32 dsttype;
	u32 dstflag;
	u32 samedstprevsrc;
	u32 samedstnextsrc;

	//0x20,0x3f
	union{
		u8 src[0];
		u64 srcchip;
		void* psrcchip;
	};
	union{
		u64 srcfoot;
		void* psrcfoot;
	};
	u32 srctype;
	u32 srcflag;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
};




struct xyzw
{
	u16 x;
	u16 y;
	u16 z;
	u16 id;
};
struct xyzwpair
{
	//touchdown, pointdown, gamepadleft
	u16 x0;
	u16 y0;
	u16 z0;
	u16 w0;

	//touchmove, pointmove, gamepadright
	u16 xn;
	u16 yn;
	u16 zn;
	u16 wn;
};




struct glsrc
{
	//[00,24)shader
	void* vs;
	void* tc;
	void* te;
	void* gs;
	void* fs;
	u8 shader_enq;

	//
	char* routine_name;
	char* routine_detail;

	//[24,88)argument
	struct arg{
		char* name;
		void* data;
		u32 fmt;
	}arg[7];

	//[88,fc)texture
	struct tex{
		char* name;
		union{
			void* data;
			u32 glfd;
		};
		u32 w;
		u32 h;
		u32 fmt;
		u8 enq;
	}tex[4];

	//[c0,e7]vertex
	void* vbuf;
	u32 vbuf_fmt;
	u32 vbuf_w;
	u32 vbuf_h;
	u32 vbuf_len;
	u8 vbuf_enq;

	void* ibuf;
	u32 ibuf_fmt;
	u32 ibuf_w;
	u32 ibuf_h;
	u32 ibuf_len;
	u8 ibuf_enq;

	//
	int ifirst;
	int icount;

	//[e8,eb]
	u8 method;		//'v'=glDrawArrays, 'i'=glDrawElements
	u8 geometry;	//1=point, 2=line, *=trigon
	u8 opaque;		//0=solid, n=opaque
};
struct gldst
{
	//shader
	u32 shader;
	u8 shader_deq;

	//texture
	u32 tex[4];
	u8 tex_deq[4];

	//vertex
	u32 vbo;
	u8 vbo_deq;
	u32 ibo;
	u8 ibo_deq;

	//
	u32 vao;
};
struct datapair
{
	//[000,1bf]
	struct glsrc src;
	u8 ipadd[0x1c0 - sizeof(struct glsrc)];

	//[1c0,1ff]
	struct gldst dst;
	u8 opadd[0x40 - sizeof(struct gldst)];
};/*
struct glctx
{
	//[00,24)shader
	u32 shader;
	u8 shader_enq;

	//[88,fc)texture
	char* tex_name[4];
	u32 tex[4];
	u32 tex_w[4];
	u32 tex_h[4];
	u32 tex_fmt[4];
	u8 tex_deq[4];

	//[24,88)argument
	char* arg_name[8];
	void* arg_data[8];
	u32 arg_fmt[8];

	//[c0,e7]vertex
	u32 vbo;
	u32 vbuf_fmt;
	u32 vbuf_w;
	u32 vbuf_h;
	u32 vbuf_len;
	u8 vbuf_enq;

	u32 ibo;
	u32 ibuf_fmt;
	u32 ibuf_w;
	u32 ibuf_h;
	u32 ibuf_len;
	u8 ibuf_enq;

	//
	u32 vao;
	int ifirst;
	int icount;

	//[e8,eb]
	u8 method;		//'v'=glDrawArrays, 'i'=glDrawElements
	u8 geometry;	//1=point, 2=line, *=trigon
	u8 target;		//0=rtt, 1=background, 2=geometry, 3=alphatest, 4=transparent, 5=overlay
};*/




struct current{
	float value;
	float ampli;
	float phase;
	float freq;
};
struct circstat{
	//[00, 7f]
	struct current c;
};
struct kalman{
	u8 padd[0x80];
};
struct pidval{
	float kp;
	float ki;
	float kd;
	float kk;

	float tp;	//enow - e_1
	float ti;	//enow
	float td;	//(enow - e_1) - (e_1 - e_2)
	float tt;

	float e_3;
	float e_2;
	float e_1;
	float enow;

	float o_3;
	float o_2;
	float o_1;
	float onow;		//onow = o_1 + kp*tp + ki*ti + kd*kd

	u8 padd[0x40];
};




struct fstyle{
	vec4 vl;	//[00,0f]: left
	vec4 vr;	//[10,1f]: right
	vec4 vn;	//[20,2f]: near
	vec4 vf;	//[30,3f]: far
	vec4 vb;	//[40,4f]: bottom
	vec4 vt;	//[50,5f]: upper
	vec4 vq;	//[60,6f]: info
	vec4 vc;	//[70,7f]: center
};
struct istyle{
	int ul[4];
	int ur[4];
	int un[4];
	int uf[4];
	int ub[4];
	int ut[4];
	int uq[4];
	int uc[4];
};
struct fmotion{
	vec4 j_angular;		//q
	vec4 j_displace;	//v		jerk
	vec4 a_angular;		//q
	vec4 a_displace;	//v		accel
	vec4 v_angular;		//q
	vec4 v_displace;	//v		velocity
	vec4 x_angular;		//q
	vec4 x_displace;	//v		displace
};
struct imotion{
	int ja[4];
	int jd[4];
	int aa[4];
	int ad[4];
	int va[4];
	int vd[4];
	int xa[4];
	int xd[4];
};
struct style
{
	//[00, 7f]: actual, css shape
	union{
		//struct fstyle actualshape;
		//struct fmotion actualmotion;
		struct fstyle f;
		struct fstyle fs;
		struct fstyle fshape;
		struct istyle i;
		struct istyle is;
		struct istyle ishape;
	};

	//[80, ff]: actual, css shape
	union{
		//struct fstyle actualshape;
		//struct fmotion actualmotion;
		struct fstyle frus;
		struct fstyle frustum;
	};

	//[100, 17f]
	union{
		struct fmotion fm;
		struct fmotion fmotion;
		struct imotion im;
		struct imotion imotion;
	};

	//[180, 1ff]
	u64 data[8];
};




struct item
{
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	u64 tier;
	u64 type;
	u64 fmt;
	u64 name;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 addr0;
		void* buf0;
	};
	union{
		u64 addr1;
		void* buf1;
	};
	union{
		u64 addr2;
		void* buf2;
	};
	union{
		u64 addr3;
		void* buf3;
	};
};




struct pwrclk
{
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	u64 tier;
	u64 type;
	u64 hfmt;
	u64 vfmt;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 padd0;
		void* addr0;
	};
	union{
		u64 padd1;
		void* addr1;
	};
	union{
		u64 padd2;
		void* addr2;
	};
	union{
		u64 padd3;
		void* addr3;
		void* buf;
	};

	//[80, ff]
	u8 tmp[0x80];
};
struct worker
{
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	u64 tier;
	u64 type;
	u64 hfmt;
	u64 vfmt;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 padd0;
		void* addr0;
	};
	union{
		u64 padd1;
		void* addr1;
	};
	union{
		u64 padd2;
		void* addr2;
	};
	union{
		u64 padd3;
		void* addr3;
		void* buf;
	};

	//[80,ff]
	u8 tmp[0x80];
};




struct device
{
	//[00,1f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 stage1;
	u64 name;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 padd0;
		void* addr0;
	};
	union{
		u64 padd1;
		void* addr1;
	};
	union{
		u64 padd2;
		void* addr2;
	};
	union{
		u64 padd3;
		void* addr3;
		void* buf;
	};

	//[80,ff]
	u8 data[0x80];
};
struct driver
{
	//[00,1f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 stage1;
	u64 name;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: data
	union{
		u64 padd0;
		void* addr0;
	};
	union{
		u64 padd1;
		void* addr1;
	};
	union{
		u64 padd2;
		void* addr2;
	};
	union{
		u64 padd3;
		void* addr3;
	};

	//[80,ff]
	u8 data[0x80];
};




struct object
{
	//[00,1f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 fmt;
	u64 name;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 addr0;
		void* buf0;
	};
	union{
		u64 addr1;
		void* buf1;
	};
	union{
		u64 addr2;
		void* buf2;
	};
	union{
		u64 addr3;
		void* buf3;
	};

	//[0x80,0xbf]
	u8 self[0x20];
	u8 peer[0x20];

	//[0xc0,0xff]
	u8 data[0x40];
};
struct element
{
	//[00,1f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 stage1;
	u64 name;

	//[40,5f]: fd/handle
	union{
		u64 sz0;
		u64 selffd;
	};
	union{
		u64 sz1;
		void* selfobj;
	};
	union{
		u64 sz2;
		u64 tempfd;
	};
	union{
		u64 sz3;
		void* tempobj;
	};

	//[60,7f]: memory
	union{
		u64 addr0;
		void* buf0;
	};
	union{
		u64 addr1;
		void* buf1;
	};
	union{
		u64 len;
		u64 addr2;
		void* buf2;
	};
	union{
		u64 addr3;
		void* buf3;
	};

	//[80,ff]
	u8 data[0x80];
};




struct supply
{
	//[00,1f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 fmt;
	u64 vfmt;

	//[40,5f]: fd
	union{
		u32 fbo;
		u64 padd0;
		u64 xlibfd;	//xlib
		void* hwnd;	//winapi
		void* sdlwnd;	//sdl
		void* glwnd;
	};
	union{
		u32 rbo;
		u64 padd1;
		void* xlibgc;	//xlib
		void* hdc;	//winapi
		void* sdlren;	//sdl
	};
	union{
		u32 tex0;
		u64 padd2;
		u64 len;
		void* ximage;	//xlib
		void* binfo;	//winapi
		void* sdltex;	//sdl
	};
	union{
		u32 tex1;
		u64 padd3;
		u64 addr;
		void* buf;
	};

	//[60,7f]: memory
	union{
		u64 data0;
		void* buf0;
		void* gl_camera;
	};
	union{
		u64 data1;
		void* buf1;
		void* gl_light;
	};
	union{
		u64 data2;
		void* buf2;
		void* gl_solid;
	};
	union{
		u64 data3;
		void* buf3;
		void* gl_opaque;
	};

	//[80,bf]: func
	union{
		int (*oncreate)(void* entity, void* url, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* entity);
		char padding1[8];
	};
	union{
		int (*onstart)(void* self, void* peer);
		char padding2[8];
	};
	union{
		int (*onstop)(void* self, void* peer);
		char padding3[8];
	};
	union{
		int (*oncread)(void* self, void* peer, void* buf, int len);
		char padding4[8];
	};
	union{
		int (*oncwrite)(void* self, void* peer, void* buf, int len);
		char padding5[8];
	};
	union{
		int (*onsread)(void* self, void* peer, void* buf, int len);
		char padding6[8];
	};
	union{
		int (*onswrite)(void* self, void* peer, void* buf, int len);
		char padding7[8];
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int stride;

	//[d0,df]
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[e0,ef]
	int x0;
	int y0;
	int z0;
	int w0;

	//[f0,ff]
	int xn;
	int yn;
	int zn;
	int wn;
};
struct entity
{
	//[00,1f]: wire
	union{
		void* irel0;
		u64 ipad0;
	};
	union{
		void* ireln;
		u64 ipadn;
	};
	union{
		void* orel0;
		u64 opad0;
	};
	union{
		void* oreln;
		u64 opadn;
	};

	//[20,3f]: type
	u64 tier;
	u64 type;
	u64 fmt;
	u64 name0;

	//[40,5f]: fd
	union{
		u64 fd;
		void* win;
		void* ctx;
		int vlen;
	};
	union{
		u64 dc;
		u64 gc;
		void* er;
		int ilen;
	};
	union{
		u64 len;
		void* idx;
		int nlen;
	};
	union{
		u64 addr;
		void* buf;
		int wlen;
	};

	//[60,7f]: buf
	union{
		u64 data0;
		void* buf0;
		void* vbuf;		//vert buf
		void* gl_camera;
	};
	union{
		u64 data1;
		void* buf1;
		void* ibuf;		//indx buf
		void* gl_light;
	};
	union{
		u64 data2;
		void* buf2;
		void* nbuf;		//node list
		void* gl_solid;
	};
	union{
		u64 data3;
		void* buf3;
		void* wbuf;		//wire list
		void* gl_opaque;
	};

	//[80,bf]: func
	union{
		int (*oncreate)(void* entity, void* url, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(void* entity);
		char padding1[8];
	};
	union{
		int (*onsearch)(void* entity, int flag, struct halfrel** self, struct halfrel** peer);
		char padding2[8];
	};
	union{
		int (*onmodify)(void* entity, void* buf);
		char padding3[8];
	};
	union{
		int (*onstart)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*onstop)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*onread)(void* self, void* peer, void* arg, int idx, void* buf, int len);
		char padding6[8];
	};
	union{
		int (*onwrite)(void* self, void* peer, void* arg, int idx, void* buf, int len);
		char padding7[8];
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int stride;

	//[d0,df]
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbstride;

	//[e0,ef]
	union{
		int ix0;
		f32 fx0;
	};
	union{
		int iy0;
		f32 fy0;
	};
	union{
		int iz0;
		f32 fz0;
	};
	union{
		int iw0;
		f32 fw0;
	};

	//[f0,ff]
	union{
		int ixn;
		f32 fxn;
	};
	union{
		int iyn;
		f32 fyn;
	};
	union{
		int izn;
		f32 fzn;
	};
	union{
		int iwn;
		f32 fwn;
	};
};
