#ifndef _STRUCT_H 
#define _STRUCT_H
typedef float f32;
typedef double f64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
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
typedef struct str verylong;
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
typedef struct event event;




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

	u32 prev;
	u32 next;
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




struct pcmdata
{
	int fmt;
	int chan;
	int rate;
	int count;
	short buf[0];
};




struct texture{
	char* name;
	union{
		void* data;
		u32 glfd;
	};
	u32 w;
	u32 h;
	u32 fmt;
};
struct vertex{
	void* vbuf;
	u32 vbuf_fmt;
	u32 vbuf_w;
	u32 vbuf_h;
	u32 vbuf_len;

	void* ibuf;
	u32 ibuf_fmt;
	u32 ibuf_w;
	u32 ibuf_h;
	u32 ibuf_len;

	//[e8,eb]
	u8 geometry;	//0=default, 1=point, 2=line, 3=trigon, 4=volume
	u8 fill;		//0=default, 1=point, 2=line, 3=trigon, 4=volume
	u8 opaque;		//0=solid, n=opaque
	u8 flag;		//4b align
};




struct dxsrc
{
	//renderto
	void* target;

	//shader
	char* vs;
	char* ps;

	//constant
	struct constant{
		mat4 mat;
		vec4 vec;
	}arg;

	//texture
	struct texture tex[7];

	//vertex
	struct vertex vtx[2];

	//enqueue
	u8 shader_enq;
	u8 tex_enq[7];
	u8 arg_enq;
	u8 vbuf_enq;
	u8 ibuf_enq;
};
struct dxdst
{
	//framebuffer
	void* fbo;

	//shader
	void* vsblob;
	void* psblob;
	void* vsprog;
	void* psprog;

	//texture
	void* texture[7];
	void* resource[7];
	void* sampler[7];

	//constant
	void* constant;

	//vertex
	void* vbuf;
	void* ibuf;
	void* layout;

	//dequeue
	u8 shader_deq;
	u8 tex_deq[7];
	u8 arg_deq;
	u8 vbo_deq;
	u8 ibo_deq;
};
struct dx11data
{
	//[000,2ff]
	struct dxsrc src;
	u8 ipadd[0x300 - sizeof(struct dxsrc)];

	//[300,3ff]
	struct dxdst dst;
	u8 opadd[0x100 - sizeof(struct dxdst)];
};




struct glsrc
{
	//renderto
	void* target;

	//shader
	void* vs;
	void* tc;
	void* te;
	void* gs;
	void* fs;
	char* routine_name;
	char* routine_detail;

	//constant
	struct arg{
		char* name;
		void* data;
		u32 fmt;
	}arg[8];

	//texture
	struct texture tex[8];

	//vertex
	struct vertex vtx[2];

	//enqueue
	u8 shader_enq;
	u8 tex_enq[8];
	u8 vbuf_enq;
	u8 ibuf_enq;
};
struct gldst
{
	//framebuffer
	u32 fbo;

	//shader
	u32 vs;
	u32 fs;
	u32 shader;

	//texture
	u32 tex[8];

	//vertex
	u32 vbo;
	u32 ibo;
	u32 vao;

	//uniform
	u32 ubo;

	//dequeue
	u8 shader_deq;
	u8 tex_deq[8];
	u8 vbo_deq;
	u8 ibo_deq;
};
struct gl41data
{
	//[000,2ff]
	struct glsrc src;
	u8 ipadd[0x300 - sizeof(struct glsrc)];

	//[300,3ff]
	struct gldst dst;
	u8 opadd[0x100 - sizeof(struct gldst)];
};




struct mtsrc
{
	//render target
	void* target;

	//shader
	char* source;

	//constant
	struct uniform{
		mat4 mat;
		vec4 vec;
	}arg;

	//texture
	struct texture tex[7];

	//vertex
	struct vertex vtx[2];

	//enqueue
	u8 shader_enq;
	u8 tex_enq[7];
	u8 arg_enq;
	u8 vbuf_enq;
	u8 ibuf_enq;
};
struct mtdst
{
	//framebuffer
	void* fbo;
	void* pipeline;
	void* depthstencil;
	void* blend;

	//shader
	void* shader;
	void* vert;
	void* frag;

	//texture
	void* texture[7];
	void* sampler[7];

	//constant
	void* uniform;

	//vertex
	void* vbuf;
	void* ibuf;

	//dequeue
	u8 shader_deq;
	u8 tex_deq[7];
	u8 arg_deq;
	u8 vbo_deq;
	u8 ibo_deq;
};
struct mt20data
{
	//[000,2ff]
	struct mtsrc src;
	u8 ipadd[0x300 - sizeof(struct mtsrc)];

	//[300,3ff]
	struct mtdst dst;
	u8 opadd[0x100 - sizeof(struct mtdst)];
};




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
	vec4 displace_j;	//jerk
	vec4 displace_a;	//accel
	vec4 displace_v;	//velocity
	vec4 displace_x;	//displace
	vec4 angular_j;
	vec4 angular_a;
	vec4 angular_v;
	vec4 angular_x;
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
	//[00, 7f]: bounding volume
	union{
		//float
		struct fstyle fs;
		struct fstyle fshape;
		struct fstyle fbound;
		//int
		struct istyle is;
		struct istyle ishape;
		struct istyle ibound;
	};

	//[80, ff]: observe frustum
	union{
		struct fstyle frus;
		struct fstyle frustum;
	};

	//[100, 17f]: current motion
	union{
		struct fmotion actual;
		struct fmotion fm;
		struct fmotion fmotion;
		struct imotion im;
		struct imotion imotion;
	};

	//[180, 1ff]: future motion
	union{
		struct fmotion desire;
	};

	//[200, 27f]
	vec4 force[8];

	//[280, 2ff]
	vec4 where[8];

	//[300, 3ff]
	u8 padd[0x100];
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




struct origin
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
		u64 data0;
		void* addr0;
	};
	union{
		u64 data1;
		void* addr1;
	};
	union{
		u64 data2;
		void* addr2;
	};
	union{
		u64 data3;
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




struct sysobj
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
struct artery
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
		u64 data0;
		void* buf0;
	};
	union{
		u64 addr1;
		u64 data1;
		void* buf1;
	};
	union{
		u64 len;
		u64 addr2;
		u64 data2;
		void* buf2;
	};
	union{
		u64 addr3;
		u64 data3;
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
		u64 padd0;
		void*  aqref;	//audio.aq
		u64   xlibfd;	//2d.xlib
		void*   hwnd;	//2d.winapi
		void* sdlwnd;	//2d.sdl
		void*  glwnd;	//3d.glfw
		u32      fbo;	//3d.fbo
	};
	union{
		u64 padd1;
		void*  aqctx;	//audio.aq
		void* xlibgc;	//2d.xlib
		void*    hdc;	//2d.winapi
		void* sdlren;	//2d.sdl
		u64   gltime;	//3d.glfw
	};
	union{
		u64 padd2;
		int    aqenq;	//audio.aq
		void* ximage;	//2d.xlib
		void*  binfo;	//2d.winapi
		void* sdltex;	//2d.sdl
		void* glevto;	//3d.glfw
		u32      rbo;	//3d.depth/stencil
		u32      dep;	//3d.depth/stencil
	};
	union{
		u64 padd3;
		int    aqdeq;	//audio.aq
		void* spsave;	//2d.*
		void* glsave;	//3d.glfw
		u32     tex0;	//3d.color
		u32*     tex;	//3d.gbuffer
	};

	//[60,7f]: memory
	union{
		u64 data0;
		void* buf0;
		void* perwnd;
		struct pcmdata*  pcmeasy_mic;
		struct pcmdata** pcmfull_mic;
		struct dx11data*  dxeasy_camera;
		struct dx11data** dxfull_camera;
		struct gl41data*  gleasy_camera;
		struct gl41data** glfull_camera;
		struct mt20data*  mteasy_camera;
		struct mt20data** mtfull_camera;
	};
	union{
		u64 data1;
		void* buf1;
		struct pcmdata*  pcmeasy_wall;
		struct pcmdata** pcmfull_wall;
		struct dx11data*  dxeasy_light;
		struct dx11data** dxfull_light;
		struct gl41data*  gleasy_light;
		struct gl41data** glfull_light;
		struct mt20data*  mteasy_light;
		struct mt20data** mtfull_light;
	};
	union{
		u64 data2;
		void* buf2;
		void* rgbaalloc;
		void* depthbuf;
		void* htmlctx;	//html
		int jsonlen;	//json
		int textlen;	//text
		int rgbalen;	//rgba
		struct pcmdata*  pcmeasy_data;
		struct pcmdata** pcmfull_data;
		struct dx11data*  dxeasy_solid;
		struct dx11data** dxfull_solid;
		struct gl41data*  gleasy_solid;
		struct gl41data** glfull_solid;
		struct mt20data*  mteasy_solid;
		struct mt20data** mtfull_solid;
	};
	union{
		u64 data3;
		void* buf3;
		void* htmlbuf;	//html
		void* jsonbuf;	//json
		void* textbuf;	//text
		void* rgbabuf;	//rgba
		void* colorbuf;
		struct pcmdata*  pcmeasy_what;
		struct pcmdata** pcmfull_what;
		struct dx11data*  dxeasy_opaque;
		struct dx11data** dxfull_opaque;
		struct gl41data*  gleasy_opaque;
		struct gl41data** glfull_opaque;
		struct mt20data*  mteasy_opaque;
		struct mt20data** mtfull_opaque;
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
		int (*onlinkup)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondiscon)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*ontaking)(struct supply* sup,int foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*ongiving)(struct supply* sup,int foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding7[8];
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int fourth;

	//[d0,df]
	int fbwidth;	//=stride=pitch=byte_per_line
	int fbheight;
	int fbdepth;
	int fbfourth;

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
	u64 vfmt;

	//[40,5f]: fd
	union{
		u64 fd0;
		u64 fd;
		void* win;
		int vlen;
	};
	union{
		u64 fd1;
		u64 dc;
		u64 gc;
		void* er;
		int ilen;
	};
	union{
		u64 fd2;
		int nlen;
	};
	union{
		u64 fd3;
		int wlen;
	};

	//[60,7f]: buf
	union{
		u64 data0;
		float fdata0;
		double ddata0;

		void* buf0;
		struct pcmdata*  pcmeasy_mic;
		struct pcmdata** pcmfull_mic;
		struct dx11data*  dxeasy_camera;
		struct dx11data** dxfull_camera;
		struct gl41data*  gleasy_camera;
		struct gl41data** glfull_camera;
		struct mt20data*  mteasy_camera;
		struct mt20data** mtfull_camera;
	};
	union{
		u64 data1;
		float fdata1;
		double ddata1;

		void* buf1;
		struct pcmdata*  pcmeasy_wall;
		struct pcmdata** pcmfull_wall;
		struct dx11data*  dxeasy_light;
		struct dx11data** dxfull_light;
		struct gl41data*  gleasy_light;
		struct gl41data** glfull_light;
		struct mt20data*  mteasy_light;
		struct mt20data** mtfull_light;
	};
	union{
		u64 data2;
		float fdata2;
		double ddata2;

		int jsonlen;	//json
		int textlen;	//text
		int rgbalen;	//rgba

		void* buf2;
		void* htmlctx;	//html
		void* depthbuf;

		struct pcmdata*  pcmeasy_data;
		struct pcmdata** pcmfull_data;
		struct dx11data*  dxeasy_solid;
		struct dx11data** dxfull_solid;
		struct gl41data*  gleasy_solid;
		struct gl41data** glfull_solid;
		struct mt20data*  mteasy_solid;
		struct mt20data** mtfull_solid;
	};
	union{
		u64 data3;
		float fdata3;
		double ddata3;

		void* buf3;
		void* htmlbuf;	//html
		void* jsonbuf;	//json
		void* textbuf;	//text
		void* rgbabuf;	//rgba
		void* colorbuf;

		struct pcmdata*  pcmeasy_what;
		struct pcmdata** pcmfull_what;
		struct dx11data*  dxeasy_opaque;
		struct dx11data** dxfull_opaque;
		struct gl41data*  gleasy_opaque;
		struct gl41data** glfull_opaque;
		struct mt20data*  mteasy_opaque;
		struct mt20data** mtfull_opaque;
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
		int (*onlinkup)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondiscon)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*ontaking)(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*ongiving)(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int idx, void* buf,int len);
		char padding7[8];
	};

	//[c0,cf]
	int width;
	int height;
	int depth;
	int fourth;

	//[d0,df]
	int fbwidth;
	int fbheight;
	int fbdepth;
	int fbfourth;

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
#endif
