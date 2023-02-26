#include "const-naming.h"
#include "data-bitnode.h"
#ifndef _BIGNODE_H
#define _BIGNODE_H

#ifdef __cplusplus
extern "C" {
#endif



//--------------------------------relation--------------------------------
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

	u32 nodetype;
	u32 foottype;

	u32 prev;
	u32 next;
};
typedef struct halfrel _syn;

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
	u32 dstnodetype;
	u32 dstfoottype;
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
	u32 srcnodetype;
	u32 srcfoottype;
	u32 samesrcprevdst;
	u32 samesrcnextdst;
};
typedef struct relation _rel;




//--------------------------------public--------------------------------
struct sock_info{
	u8 self[0x20];
	u8 peer[0x20];
	void* parent;
	void* child;
	u64 fd;
};

struct file_info{
	u8 name[0x78];
	u64 fd;
};

struct whdf_xyzw{
	//[00,0f]
	int width;
	int height;
	int depth;
	int fourth;

	//[10,1f]
	int fbwidth;	//=stride=pitch=byte_per_line
	int fbheight;
	int fbdepth;
	int fbfourth;

	//[20,2f]
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

	//[30,3f]
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
struct eight_four{
	float a0[4];
	float a1[4];
	float a2[4];
	float a3[4];
	float a4[4];
	float a5[4];
	float a6[4];
	float a7[4];
};




//--------------------------------private--------------------------------
//normal
struct listu32{
	u32 data0;
	u32 data1;
	u32 data2;
	u32 data3;
	u32 data4;
	u32 data5;
	u32 data6;
	u32 data7;
};
struct listu64{
	u64 data0;
	u64 data1;
	u64 data2;
	u64 data3;
	u64 data4;
	u64 data5;
	u64 data6;
	u64 data7;
};
struct listfloat{
	float f0;
	float f1;
	float f2;
	float f3;
	float f4;
	float f5;
	float f6;
	float f7;
};
struct listdouble{
	float d0;
	float d1;
	float d2;
	float d3;
	float d4;
	float d5;
	float d6;
	float d7;
};
struct listptr{
	void* buf0;
	void* buf1;
	void* buf2;
	void* buf3;
	void* buf4;
	void* buf5;
	void* buf6;
	void* buf7;
};
struct buflen{
	void* buf0;
	u64 len0;
	void* buf1;
	u64 len1;
	void* buf2;
	u64 len2;
	void* buf3;
	u64 len3;
};
//audio
struct pcmeasy{
	struct pcmdata* mic;
	struct pcmdata* wall;
	struct pcmdata* data;
	struct pcmdata* what;
};
struct pcmlist{
	struct pcmdata** mic;
	struct pcmdata** wall;
	struct pcmdata** data;
	struct pcmdata** what;
};
struct appleaudioqueue{
	void* aqbufref[16];	//+0x80
	void* zero10ms;

	void*  aqref;
	int    aqenq;
	int    aqdeq;
};
//text
struct tuitext{
	void* buf;
	void* conv;
};
//html
struct htmltext{
	void* ctx;
	void* buf;
};
//json
struct jsontext{
	void* buf;
	int len;
};
//2d
struct rgbanode{
	void* buf;
	void* depthbuf;
};
struct rgbasdl{
	void* buf;
	void* depthbuf;

	void* sdlwnd;
	void* sdlren;
	void* sdltex;
};
struct rgbaxlib{
	void* buf;
	void* depthbuf;

	u64   xlibfd;
	void* xlibgc;
	void* ximage;

	void* save_stack;
	int save_sp;
};
struct rgbawinapi{
	void* buf;
	void* depthbuf;

	void*   hwnd;
	void*    hdc;
	void*  binfo;

	void* finger;

	void* save_stack;
	int save_sp;
};
//directx11
struct dx11easy{
	struct dx11data* camera;
	struct dx11data* light;
	struct dx11data* solid;
	struct dx11data* opaque;
};
struct dx11world{
	struct dx11data** camera;
	struct dx11data** light;
	struct dx11data** solid;
	struct dx11data** opaque;
};
struct dx11list{
	struct dx11world world[2];
	void* hwnd;
	void* spsave;
};
struct dx11cmdq{
	u64* data;
	u64* code;
};
//opengl
struct gl41easy{
	struct gl41data*  camera;
	struct gl41data*  light;
	struct gl41data*  solid;
	struct gl41data*  opaque;
};
struct gl41world{
	struct gl41data** camera;
	struct gl41data** light;
	struct gl41data** solid;
	struct gl41data** opaque;
};
struct gl41list{
	struct gl41world world[2];

	void* glwnd;
	u32 ctxage;

	u64   gltime;

	void* glevto;
	void* glsave;
};
struct gl41cmdq{
	u64* data;
	u64* code;
};
//metal
struct mt20easy{
	struct mt20data* camera;
	struct mt20data* light;
	struct mt20data* solid;
	struct mt20data* opaque;
};
struct mt20world{
	struct mt20data** camera;
	struct mt20data** light;
	struct mt20data** solid;
	struct mt20data** opaque;
};
struct mt20list{
	struct mt20world world[2];

	void* delegate;
	void* view;
};
struct mt20cmdq{
	u64* data;
	u64* code;
};




//--------------------------------object--------------------------------
struct item
{
	//[00,1f]: type
	u64 tier;
	u64 type;
	u64 hfmt;
	u64 vfmt;

	//[20,3f]: wire
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

	//[40,7f]: func
	union{
		int (*oncreate)(struct item* node, void* arg, int argc, u8** argv);
		char padding0[8];
	};
	union{
		int (*ondelete)(struct item* node);
		char padding1[8];
	};
	union{
		int (*onreader)(struct item* node,void* foot, void* arg,int cmd, void* buf,int len);
		char padding2[8];
	};
	union{
		int (*onwriter)(struct item* node,void* foot, void* arg,int cmd, void* buf,int len);
		char padding3[8];
	};
	union{
		int (*onattach)(void* self, void* peer);
		char padding4[8];
	};
	union{
		int (*ondetach)(void* self, void* peer);
		char padding5[8];
	};
	union{
		int (*take_xxxxxxxx)(u64 node,u64 foot, u64 stack,u64 sp, u64 arg,u32 cmd, u64 buf,u32 len);
		int (*take_pxpxpxpx)(struct item* node,u32 foot, struct halfrel* stack,u32 sp, void* arg,u32 cmd, void* buf,u32 len);
		int (*ontaking)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		int (*take)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		char padding6[8];
	};
	union{
		int (*give_xxxxxxxx)(u64 node,u64 foot, u64 stack,u64 sp, u64 arg,u32 cmd, u64 buf,u32 len);
		int (*give_pxpxpxpx)(struct item* node,u32 foot, struct halfrel* stack,u32 sp, void* arg,u32 cmd, void* buf,u32 len);
		int (*ongiving)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		int (*give)(struct item* node,void* foot, struct halfrel* stack,int sp, void* arg,int cmd, void* buf,int len);
		char padding7[8];
	};

	//[80,ff]: public
	union{
		u8 public_data[0x80];
		struct whdf_xyzw whdf;
		struct sock_info sockinfo;
		struct file_info fileinfo;
	};

	//[100,1ff]: private
	union{
		u8   priv_u8;
		u16   priv_u16;
		u32   priv_u32;
		u64   priv_u64;

		int   priv_fd;
		void* priv_ptr;
		u8    priv_256b[0x100];

		struct listu32 listu32;
		struct listu64 listu64;

		struct listfloat listf32;
		struct listdouble listf64;

		struct listptr listptr;
		struct str buflen;

		struct pcmeasy pcmeasy;
		struct appleaudioqueue appleaq;

		struct tuitext tuitext;
		struct htmltext html;
		struct jsontext json;

		struct rgbanode rgbanode;
		struct rgbasdl rgbasdl;
		struct rgbawinapi rgbawinapi;
		struct rgbaxlib rgbaxlib;

		struct gl41easy gl41easy;
		struct gl41list gl41list;
		struct gl41cmdq gl41cmdq;

		struct dx11list dx11list;

		struct mt20list mt20list;
	};
}__attribute__((packed));
typedef struct item _obj;




//--------------------------------function--------------------------------
void* entity_create(u64 type, void* addr, int argc, u8** argv);
int entity_delete(_obj*);
int entity_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int entity_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int entity_attach(struct halfrel* self, struct halfrel* peer);
int entity_detach(struct halfrel* self, struct halfrel* peer);
int entity_takeby(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int entity_giveby(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
void* supply_create(u64 type, void* addr, int argc, u8** argv);
int supply_delete(_obj*);
int supply_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int supply_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int supply_attach(struct halfrel* self, struct halfrel* peer);
int supply_detach(struct halfrel* self, struct halfrel* peer);
int supply_takeby(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int supply_giveby(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
void* artery_create(u64 type, void* addr, int argc, u8** argv);
int artery_delete(_obj*);
int artery_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int artery_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int artery_attach(struct halfrel* self, struct halfrel* peer);
int artery_detach(struct halfrel* self, struct halfrel* peer);
int artery_takeby(_obj* art,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int artery_giveby(_obj* art,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
void* system_create(u64 type, void* addr, int argc, u8** argv);
int system_delete(_obj*);
int system_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int system_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int system_attach(struct halfrel* self, struct halfrel* peer);
int system_detach(struct halfrel* self, struct halfrel* peer);
int system_takeby(_obj* obj,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int system_giveby(_obj* obj,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
void* driver_create(u64 type, void* addr, int argc, u8** argv);
int driver_delete(_obj*);
int driver_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int driver_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int driver_attach(struct halfrel* self, struct halfrel* peer);
int driver_detach(struct halfrel* self, struct halfrel* peer);
int driver_takeby(struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int driver_giveby(struct item* dri,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
void* device_create(u64 type, void* addr, int argc, u8** argv);
int device_delete(_obj*);
int device_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int device_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int device_attach(struct halfrel* self, struct halfrel* peer);
int device_detach(struct halfrel* self, struct halfrel* peer);
int device_takeby(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int device_giveby(struct item* dev,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
void* bootup_create(u64 type, void* addr, int argc, u8** argv);
int bootup_delete(_obj*);
int bootup_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int bootup_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int bootup_attach(struct halfrel* self, struct halfrel* peer);
int bootup_detach(struct halfrel* self, struct halfrel* peer);
int bootup_takeby(struct item* wrk,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int bootup_giveby(struct item* wrk,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
void* origin_create(u64 type, void* addr, int argc, u8** argv);
int origin_delete(_obj*);
int origin_reader(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int origin_writer(_obj* ent,void* foot, void* arg,int idx, void* buf,int len);
int origin_attach(struct halfrel* self, struct halfrel* peer);
int origin_detach(struct halfrel* self, struct halfrel* peer);
int origin_takeby(struct item* ori,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int origin_giveby(struct item* ori,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
//
int relationdetach(struct halfrel* self, struct halfrel* peer);
int relationattach(struct halfrel* self, struct halfrel* peer);
int relationsearch(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationmodify(void* item,u32 foot, struct halfrel** self,struct halfrel** peer);
int relationdelete(_rel* rel);
void* relationcreate(void*,void*,u32,u32,void*,void*,u32,u32);
//
int take_data_from_peer(void* chip,int ftype, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int give_data_into_peer(void* chip,int ftype, _syn* stack,int sp, void* arg,int idx, void* buf,int len);
int take_data_from_them(void* item,int ftype, _syn* stack,int sp, void* arg,int key, void* buf,int len);
int give_data_into_them(void* item,int ftype, _syn* stack,int sp, void* arg,int key, void* buf,int len);




#ifdef __cplusplus
}
#endif

#endif
