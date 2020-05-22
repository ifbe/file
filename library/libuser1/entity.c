#include "libuser.h"
void thing_free();
void thing_init(void*);
void world_free();
void world_init(void*);
void mind_free();
void mind_init(void*);
void rule_free();
void rule_init(void*);

//
int baby_create(void*, void*, int, u8**);
int baby_delete(void*, void*);
int baby_linkup(void*, void*);
int baby_discon(void*, void*);
int baby_read( void*,int, void*,int, void*,int, void*,int);
int baby_write(void*,int, void*,int, void*,int, void*,int);
int test_create(void*, void*, int, u8**);
int test_delete(void*, void*);
int test_linkup(void*, void*);
int test_discon(void*, void*);
int test_read( void*,int, void*,int, void*,int, void*,int);
int test_write(void*,int, void*,int, void*,int, void*,int);


int toycar_create(void*, void*, int, u8**);
int toycar_delete(void*, void*);
int toycar_linkup(void*, void*);
int toycar_discon(void*, void*);
int toycar_read( void*,int, void*,int, void*,int, void*,int);
int toycar_write(void*,int, void*,int, void*,int, void*,int);
int stepcar_create(void*, void*, int, u8**);
int stepcar_delete(void*, void*);
int stepcar_linkup(void*, void*);
int stepcar_discon(void*, void*);
int stepcar_read( void*,int, void*,int, void*,int, void*,int);
int stepcar_write(void*,int, void*,int, void*,int, void*,int);


//
int analog_create(void*, void*, int, u8**);
int analog_delete(void*, void*);
int analog_linkup(void*, void*);
int analog_discon(void*, void*);
int analog_write(void*,int, void*,int, void*,int, void*,int);
int analog_read( void*,int, void*,int, void*,int, void*,int);
int digital_create(void*, void*, int, u8**);
int digital_delete(void*, void*);
int digital_linkup(void*, void*);
int digital_discon(void*, void*);
int digital_write(void*,int, void*,int, void*,int, void*,int);
int digital_read( void*,int, void*,int, void*,int, void*,int);

//
int force_create(void*, void*, int, u8**);
int force_delete(void*, void*);
int force_linkup(void*, void*);
int force_discon(void*, void*);
int force_write(void*,int, void*,int, void*,int, void*,int);
int force_read( void*,int, void*,int, void*,int, void*,int);
int graveasy_create(void*, void*, int, u8**);
int graveasy_delete(void*, void*);
int graveasy_linkup(void*, void*);
int graveasy_discon(void*, void*);
int graveasy_write(void*,int, void*,int, void*,int, void*,int);
int graveasy_read( void*,int, void*,int, void*,int, void*,int);
int gravtest_create(void*, void*, int, u8**);
int gravtest_delete(void*, void*);
int gravtest_linkup(void*, void*);
int gravtest_discon(void*, void*);
int gravtest_write(void*,int, void*,int, void*,int, void*,int);
int gravtest_read( void*,int, void*,int, void*,int, void*,int);

//scene
int axis3d_create(void*, void*, int, u8**);
int axis3d_delete(void*, void*);
int axis3d_linkup(void*, void*);
int axis3d_discon(void*, void*);
int axis3d_write(void*,int, void*,int, void*,int, void*,int);
int axis3d_read( void*,int, void*,int, void*,int, void*,int);
int guide3d_create(void*, void*, int, u8**);
int guide3d_delete(void*, void*);
int guide3d_linkup(void*, void*);
int guide3d_discon(void*, void*);
int guide3d_write(void*,int, void*,int, void*,int, void*,int);
int guide3d_read( void*,int, void*,int, void*,int, void*,int);
int border2d_create(void*, void*, int, u8**);
int border2d_delete(void*, void*);
int border2d_linkup(void*, void*);
int border2d_discon(void*, void*);
int border2d_write(void*,int, void*,int, void*,int, void*,int);
int border2d_read( void*,int, void*,int, void*,int, void*,int);
int border3d_create(void*, void*, int, u8**);
int border3d_delete(void*, void*);
int border3d_linkup(void*, void*);
int border3d_discon(void*, void*);
int border3d_write(void*,int, void*,int, void*,int, void*,int);
int border3d_read( void*,int, void*,int, void*,int, void*,int);
int scene3d_create(void*, void*, int, u8**);
int scene3d_delete(void*, void*);
int scene3d_linkup(void*, void*);
int scene3d_discon(void*, void*);
int scene3d_write(void*,int, void*,int, void*,int, void*,int);
int scene3d_read( void*,int, void*,int, void*,int, void*,int);

//
int reality_create(void*, void*, int, u8**);
int reality_delete(void*);
int reality_linkup(void*, void*);
int reality_discon(void*, void*);
int reality_write(void*,int, void*,int, void*,int, void*,int);
int reality_read( void*,int, void*,int, void*,int, void*,int);
int virtual_create(void*, void*, int, u8**);
int virtual_delete(void*, void*);
int virtual_linkup(void*, void*);
int virtual_discon(void*, void*);
int virtual_write(void*,int, void*,int, void*,int, void*,int);
int virtual_read( void*,int, void*,int, void*,int, void*,int);
int htmlroot_create(void*, void*, int, u8**);
int htmlroot_delete(void*);
int htmlroot_linkup(void*, void*);
int htmlroot_discon(void*, void*);
int htmlroot_write(void*,int, void*,int, void*,int, void*,int);
int htmlroot_read( void*,int, void*,int, void*,int, void*,int);
int xamlroot_create(void*, void*, int, u8**);
int xamlroot_delete(void*, void*);
int xamlroot_linkup(void*, void*);
int xamlroot_discon(void*, void*);
int xamlroot_write(void*,int, void*,int, void*,int, void*,int);
int xamlroot_read( void*,int, void*,int, void*,int, void*,int);
int mmiospace_create(void*, void*, int, u8**);
int mmiospace_delete(void*);
int mmiospace_linkup(void*, void*);
int mmiospace_discon(void*, void*);
int mmiospace_write(void*,int, void*,int, void*,int, void*,int);
int mmiospace_read( void*,int, void*,int, void*,int, void*,int);
int portspace_create(void*, void*, int, u8**);
int portspace_delete(void*, void*);
int portspace_linkup(void*, void*);
int portspace_discon(void*, void*);
int portspace_write(void*,int, void*,int, void*,int, void*,int);
int portspace_read( void*,int, void*,int, void*,int, void*,int);

//gl41 helper
int gl41data_create(void*, void*, int, u8**);
int gl41data_delete(void*);
int gl41data_linkup(void*, void*);
int gl41data_discon(void*, void*);
int gl41data_read( void*,int, void*,int, void*,int, void*,int);
int gl41data_write(void*,int, void*,int, void*,int, void*,int);
int gl41coop_create(void*, void*, int, u8**);
int gl41coop_delete(void*);
int gl41coop_linkup(void*, void*);
int gl41coop_discon(void*, void*);
int gl41coop_read( void*,int, void*,int, void*,int, void*,int);
int gl41coop_write(void*,int, void*,int, void*,int, void*,int);

//event
int event3rd_create(void*, void*, int, u8**);
int event3rd_delete(void*);
int event3rd_linkup(void*, void*);
int event3rd_discon(void*, void*);
int event3rd_read( void*,int, void*,int, void*,int, void*,int);
int event3rd_write(void*,int, void*,int, void*,int, void*,int);
int eventrts_create(void*, void*, int, u8**);
int eventrts_delete(void*);
int eventrts_linkup(void*, void*);
int eventrts_discon(void*, void*);
int eventrts_read( void*,int, void*,int, void*,int, void*,int);
int eventrts_write(void*,int, void*,int, void*,int, void*,int);
int clickray_create(void*, void*, int, u8**);
int clickray_delete(void*);
int clickray_linkup(void*, void*);
int clickray_discon(void*, void*);
int clickray_read( void*,int, void*,int, void*,int, void*,int);
int clickray_write(void*,int, void*,int, void*,int, void*,int);
int touchobj_create(void*, void*, int, u8**);
int touchobj_delete(void*);
int touchobj_linkup(void*, void*);
int touchobj_discon(void*, void*);
int touchobj_read( void*,int, void*,int, void*,int, void*,int);
int touchobj_write(void*,int, void*,int, void*,int, void*,int);
int follow_create(void*, void*, int, u8**);
int follow_delete(void*);
int follow_linkup(void*, void*);
int follow_discon(void*, void*);
int follow_read( void*,int, void*,int, void*,int, void*,int);
int follow_write(void*,int, void*,int, void*,int, void*,int);
int lookat_create(void*, void*, int, u8**);
int lookat_delete(void*);
int lookat_linkup(void*, void*);
int lookat_discon(void*, void*);
int lookat_read( void*,int, void*,int, void*,int, void*,int);
int lookat_write(void*,int, void*,int, void*,int, void*,int);
int wander_create(void*, void*, int, u8**);
int wander_delete(void*);
int wander_linkup(void*, void*);
int wander_discon(void*, void*);
int wander_read( void*,int, void*,int, void*,int, void*,int);
int wander_write(void*,int, void*,int, void*,int, void*,int);




static struct entity* entity = 0;
static int actlen = 0;
static struct style* style = 0;
static int stylen = 0;
void* allocentity()
{
	int j,max;
	struct entity* act;

	max = 0x100000 / sizeof(struct entity);
	for(j=0;j<max;j++){
		act = &entity[j];
		if(0 == act->fmt)return act;
	}
	return 0;
}
void* allocstyle()
{
#define maxlen (sizeof(struct style))
	int j;
	u8* buf;

	buf = (void*)style + stylen;
	for(j=0;j<maxlen;j++)buf[j] = 0;

	stylen += maxlen;
	return buf;
}
void* entitycreate_clone(u64 fmt, u8* arg, int argc, u8** argv)
{
	int j;
	u8* src;
	u8* dst;
	struct entity* ent;
	struct entity* tmp = 0x100000 + (void*)entity;

	for(j=1;j<256;j++){
		tmp = &tmp[-1];		//prev
		if(fmt == tmp->fmt)goto found;
	}
	return 0;

found:
	ent = allocentity();
	dst = (void*)ent;
	for(j=0;j<sizeof(struct entity);j++)dst[j] = 0;

	ent->tier = tmp->tier;
	ent->type = tmp->type;
	ent->fmt  = tmp->fmt;
	ent->vfmt = tmp->vfmt;

	ent->oncreate = tmp->oncreate;
	ent->ondelete = tmp->ondelete;
	ent->onsearch = tmp->onsearch;
	ent->onmodify = tmp->onmodify;
	ent->onlinkup = tmp->onlinkup;
	ent->ondiscon = tmp->ondiscon;
	ent->onread   = tmp->onread;
	ent->onwrite  = tmp->onwrite;

	ent->oncreate(ent, arg, argc, argv);
	return ent;
}



/*
void entityinput_touch(struct supply* win, struct event* ev)
{
	int x,y,z,btn;
	if('p' != (ev->what & 0xff))return;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	z = ((ev->why)>>32)&0xffff;
	btn = ((ev->why)>>48)&0xffff;

	if('l' == btn)btn = 10;
	else if('r' == btn)btn = 11;
	else if(10 < btn)return;

	if(hex32('p','@',0,0) == ev->what)
	{
		win->touch[btn].xn = x;
		win->touch[btn].yn = y;
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		win->touch[btn].x0 = win->touch[btn].xn = x;
		win->touch[btn].y0 = win->touch[btn].yn = y;
		win->touch[btn].z0 = 1;
	}
	else if(hex32('p','-',0,0) == ev->what)
	{
		win->touch[btn].z0 = 0;
	}
}*/




int entityread(_ent* act,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(act->type){
	case _follow_:return follow_read(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_read(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_read(act,foot, stack,sp, arg,key, buf,len);
	case _touchobj_:return touchobj_read(act,foot, stack,sp, arg,key, buf,len);
	case _clickray_:return clickray_read(act,foot, stack,sp, arg,key, buf,len);
	case _event3rd_:return event3rd_read(act,foot, stack,sp, arg,key, buf,len);
	case _eventrts_:return eventrts_read(act,foot, stack,sp, arg,key, buf,len);

	case _gl41data_:return gl41data_read(act,foot, stack,sp, arg,key, buf,len);
	case _gl41coop_:return gl41coop_read(act,foot, stack,sp, arg,key, buf,len);

	//case _field_:return field_read(act,foot, stack,sp, arg,key, buf,len);
	//case _fluid_:return fluid_read(act,foot, stack,sp, arg,key, buf,len);
	case _force_:return force_read(act,foot, stack,sp, arg,key, buf,len);
	case _graveasy_:return graveasy_read(act,foot, stack,sp, arg,key, buf,len);
	case _gravtest_:return gravtest_read(act,foot, stack,sp, arg,key, buf,len);

	case _analog_:return analog_read(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_read(act,foot, stack,sp, arg,key, buf,len);

	case _axis3d_:return axis3d_read(act,foot, stack,sp, arg,key, buf,len);
	case _guide3d_:return guide3d_read(act,foot, stack,sp, arg,key, buf,len);
	case _border2d_:return border2d_read(act,foot, stack,sp, arg,key, buf,len);
	case _border3d_:return border3d_read(act,foot, stack,sp, arg,key, buf,len);
	case _scene3d_:return scene3d_read(act,foot, stack,sp, arg,key, buf,len);

	case _reality_:return reality_read(act,foot, stack,sp, arg,key, buf,len);
	case _virtual_:return virtual_read(act,foot, stack,sp, arg,key, buf,len);
	case _htmlroot_:return htmlroot_read(act,foot, stack,sp, arg,key, buf,len);
	case _xamlroot_:return xamlroot_read(act,foot, stack,sp, arg,key, buf,len);
	case _mmio_:return mmiospace_read(act,foot, stack,sp, arg,key, buf,len);
	case _port_:return portspace_read(act,foot, stack,sp, arg,key, buf,len);

	case _step_:return stepcar_read(act,foot, stack,sp, arg,key, buf,len);
	case _bdc_:return toycar_read(act,foot, stack,sp, arg,key, buf,len);

	case _test_:return test_read(act,foot, stack,sp, arg,key, buf,len);
	case _baby_:return baby_read(act,foot, stack,sp, arg,key, buf,len);
	}

	if(0 == act->onread)return 0;
	return act->onread(act,foot, stack,sp, arg,key, buf,len);
}
int entitywrite(_ent* act,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(act->type){
	case _follow_:return follow_write(act,foot, stack,sp, arg,key, buf,len);
	case _lookat_:return lookat_write(act,foot, stack,sp, arg,key, buf,len);
	case _wander_:return wander_write(act,foot, stack,sp, arg,key, buf,len);
	case _touchobj_:return touchobj_write(act,foot, stack,sp, arg,key, buf,len);
	case _clickray_:return clickray_write(act,foot, stack,sp, arg,key, buf,len);
	case _event3rd_:return event3rd_write(act,foot, stack,sp, arg,key, buf,len);
	case _eventrts_:return eventrts_write(act,foot, stack,sp, arg,key, buf,len);

	case _gl41data_:return gl41data_write(act,foot, stack,sp, arg,key, buf,len);
	case _gl41coop_:return gl41coop_write(act,foot, stack,sp, arg,key, buf,len);

	//case _field_:return field_write(act,foot, stack,sp, arg,key, buf,len);
	//case _fluid_:return fluid_write(act,foot, stack,sp, arg,key, buf,len);
	case _force_:return force_write(act,foot, stack,sp, arg,key, buf,len);
	case _graveasy_:return graveasy_write(act,foot, stack,sp, arg,key, buf,len);
	case _gravtest_:return gravtest_write(act,foot, stack,sp, arg,key, buf,len);

	case _analog_:return analog_write(act,foot, stack,sp, arg,key, buf,len);
	case _digital_:return digital_write(act,foot, stack,sp, arg,key, buf,len);

	case _axis3d_:return axis3d_write(act,foot, stack,sp, arg,key, buf,len);
	case _guide3d_:return guide3d_write(act,foot, stack,sp, arg,key, buf,len);
	case _border2d_:return border2d_write(act,foot, stack,sp, arg,key, buf,len);
	case _border3d_:return border3d_write(act,foot, stack,sp, arg,key, buf,len);
	case _scene3d_:return scene3d_write(act,foot, stack,sp, arg,key, buf,len);

	case _reality_:return reality_write(act,foot, stack,sp, arg,key, buf,len);
	case _virtual_:return virtual_write(act,foot, stack,sp, arg,key, buf,len);
	case _htmlroot_:return htmlroot_write(act,foot, stack,sp, arg,key, buf,len);
	case _xamlroot_:return xamlroot_write(act,foot, stack,sp, arg,key, buf,len);
	case _mmio_:return mmiospace_write(act,foot, stack,sp, arg,key, buf,len);
	case _port_:return portspace_write(act,foot, stack,sp, arg,key, buf,len);

	case _step_:return stepcar_write(act,foot, stack,sp, arg,key, buf,len);
	case _bdc_:return toycar_write(act,foot, stack,sp, arg,key, buf,len);

	case _test_:return test_write(act,foot, stack,sp, arg,key, buf,len);
	case _baby_:return baby_write(act,foot, stack,sp, arg,key, buf,len);
	}

	if(0 == act->onwrite)return 0;
	return act->onwrite(act,foot, stack,sp, arg,key, buf,len);
}
int entitydiscon(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act;
	if(0 == self)return 0;
	act = self->pchip;
	if(0 == act)return 0;

	//say("@entity_discon\n");
	switch(act->type){
	case _follow_:return follow_discon(self, peer);
	case _lookat_:return lookat_discon(self, peer);
	case _wander_:return wander_discon(self, peer);
	case _touchobj_:return touchobj_discon(self, peer);
	case _clickray_:return clickray_discon(self, peer);
	case _event3rd_:return event3rd_discon(self, peer);
	case _eventrts_:return eventrts_discon(self, peer);

	case _gl41data_:return gl41data_discon(self, peer);
	case _gl41coop_:return gl41coop_discon(self, peer);

	//case _field_:return field_discon(self, peer);
	//case _fluid_:return fluid_discon(self, peer);
	case _force_:return force_discon(self, peer);
	case _graveasy_:return graveasy_discon(self, peer);
	case _gravtest_:return gravtest_discon(self, peer);

	case _analog_:return analog_discon(self, peer);
	case _digital_:return digital_discon(self, peer);

	case _axis3d_:return axis3d_discon(self, peer);
	case _guide3d_:return guide3d_discon(self, peer);
	case _border2d_:return border2d_discon(self, peer);
	case _border3d_:return border3d_discon(self, peer);
	case _scene3d_:return scene3d_discon(self, peer);

	case _reality_:return reality_discon(self, peer);
	case _virtual_:return virtual_discon(self, peer);
	case _htmlroot_:return htmlroot_discon(self, peer);
	case _xamlroot_:return xamlroot_discon(self, peer);
	case _mmio_:return mmiospace_discon(self, peer);
	case _port_:return portspace_discon(self, peer);

	case _step_:return stepcar_discon(self, peer);
	case _bdc_:return toycar_discon(self, peer);

	case _test_:return test_discon(self, peer);
	case _baby_:return baby_discon(self, peer);
	}

	if(0 == act->ondiscon)return 0;
	return act->ondiscon(self, peer);
}
int entitylinkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* act;
	if(0 == self)return 0;
	act = self->pchip;
	if(0 == act)return 0;

	//say("@entity_linkup\n");
	switch(act->type){
	case _follow_:return follow_linkup(self, peer);
	case _lookat_:return lookat_linkup(self, peer);
	case _wander_:return wander_linkup(self, peer);
	case _touchobj_:return touchobj_linkup(self, peer);
	case _clickray_:return clickray_linkup(self, peer);
	case _event3rd_:return event3rd_linkup(self, peer);
	case _eventrts_:return eventrts_linkup(self, peer);

	case _gl41data_:return gl41data_linkup(self, peer);
	case _gl41coop_:return gl41coop_linkup(self, peer);

	//case _field_:return field_linkup(self, peer);
	//case _fluid_:return fluid_linkup(self, peer);
	case _force_:return force_linkup(self, peer);
	case _graveasy_:return graveasy_linkup(self, peer);
	case _gravtest_:return gravtest_linkup(self, peer);

	case _analog_:return analog_linkup(self, peer);
	case _digital_:return digital_linkup(self, peer);

	case _axis3d_:return axis3d_linkup(self, peer);
	case _guide3d_:return guide3d_linkup(self, peer);
	case _border2d_:return border2d_linkup(self, peer);
	case _border3d_:return border3d_linkup(self, peer);
	case _scene3d_:return scene3d_linkup(self, peer);

	case _reality_:return reality_linkup(self, peer);
	case _virtual_:return virtual_linkup(self, peer);
	case _htmlroot_:return htmlroot_linkup(self, peer);
	case _xamlroot_:return xamlroot_linkup(self, peer);
	case _mmio_:return mmiospace_linkup(self, peer);
	case _port_:return portspace_linkup(self, peer);

	case _step_:return stepcar_linkup(self, peer);
	case _bdc_:return toycar_linkup(self, peer);

	case _test_:return test_linkup(self, peer);
	case _baby_:return baby_linkup(self, peer);
	}

	if(0 == act->onlinkup)return 0;
	return act->onlinkup(self, peer);
}




int entitydelete(void* p)
{
	if(0 == p)return 0;
	struct entity* act = p;

	switch(act->type){
		case _orig_:return 0;
		case _copy_:return 0;
		case _ORIG_:act->type = _orig_;
		case _COPY_:act->type = _copy_;
	}
	act->ondelete(act);

	return 0;
}
void* entitycreate(u64 type, void* buf, int argc, u8** argv)
{
	struct entity* act;
	//say("%llx,%llx\n", type, buf);

	switch(type){
//----------------world----------------
	case _reality_:
	{
		act = allocentity();
		act->fmt = act->type = _reality_;
		reality_create(act, buf, argc, argv);
		return act;
	}
	case _virtual_:
	{
		act = allocentity();
		act->fmt = act->type = _virtual_;
		virtual_create(act, buf, argc, argv);
		return act;
	}
	case _htmlroot_:
	{
		act = allocentity();
		act->fmt = act->type = _htmlroot_;
		htmlroot_create(act, buf, argc, argv);
		return act;
	}
	case _xamlroot_:
	{
		act = allocentity();
		act->fmt = act->type = _xamlroot_;
		xamlroot_create(act, buf, argc, argv);
		return act;
	}
	case _mmio_:
	{
		act = allocentity();
		act->fmt = act->type = _mmio_;
		mmiospace_create(act, buf, argc, argv);
		return act;
	}
	case _port_:
	{
		act = allocentity();
		act->fmt = act->type = _port_;
		portspace_create(act, buf, argc, argv);
		return act;
	}

	case _axis3d_:
	{
		act = allocentity();
		act->fmt = act->type = _axis3d_;
		axis3d_create(act, buf, argc, argv);
		return act;
	}
	case _guide3d_:
	{
		act = allocentity();
		act->fmt = act->type = _guide3d_;
		guide3d_create(act, buf, argc, argv);
		return act;
	}
	case _border2d_:
	{
		act = allocentity();
		act->fmt = act->type = _border2d_;
		border2d_create(act, buf, argc, argv);
		return act;
	}
	case _border3d_:
	{
		act = allocentity();
		act->fmt = act->type = _border3d_;
		border3d_create(act, buf, argc, argv);
		return act;
	}
	case _scene3d_:
	{
		act = allocentity();
		act->fmt = act->type = _scene3d_;
		scene3d_create(act, buf, argc, argv);
		return act;
	}

//----------------mind----------------
 	case _baby_:
	{
		act = allocentity();
		act->fmt = act->type = _baby_;
		baby_create(act, buf, argc, argv);
		return act;
	}
	case _test_:
	{
		act = allocentity();
		act->fmt = act->type = _test_;
		test_create(act, buf, argc, argv);
		return act;
	}

	//event
	case _follow_:
	{
		act = allocentity();
		act->fmt = act->type = _follow_;
		follow_create(act, buf, argc, argv);
		return act;
	}
	case _lookat_:
	{
		act = allocentity();
		act->fmt = act->type = _lookat_;
		lookat_create(act, buf, argc, argv);
		return act;
	}
	case _wander_:
	{
		act = allocentity();
		act->fmt = act->type = _wander_;
		wander_create(act, buf, argc, argv);
		return act;
	}
	case _touchobj_:
	{
		act = allocentity();
		act->fmt = act->type = _touchobj_;
		touchobj_create(act, buf, argc, argv);
		return act;
	}
	case _clickray_:
	{
		act = allocentity();
		act->fmt = act->type = _clickray_;
		clickray_create(act, buf, argc, argv);
		return act;
	}
	case _event3rd_:
	{
		act = allocentity();
		act->fmt = act->type = _event3rd_;
		event3rd_create(act, buf, argc, argv);
		return act;
	}
	case _eventrts_:
	{
		act = allocentity();
		act->fmt = act->type = _eventrts_;
		eventrts_create(act, buf, argc, argv);
		return act;
	}

//----------------rule----------------
	//circuit
	case _analog_:
	{
		act = allocentity();
		act->fmt = act->type = _analog_;
		analog_create(act, buf, argc, argv);
		return act;
	}
	case _digital_:
	{
		act = allocentity();
		act->fmt = act->type = _digital_;
		digital_create(act, buf, argc, argv);
		return act;
	}

	//physic
	case _force_:
	{
		act = allocentity();
		act->fmt = act->type = _force_;
		force_create(act, buf, argc, argv);
		return act;
	}
	case _graveasy_:
	{
		act = allocentity();
		act->fmt = act->type = _graveasy_;
		graveasy_create(act, buf, argc, argv);
		return act;
	}
	case _gravtest_:
	{
		act = allocentity();
		act->fmt = act->type = _gravtest_;
		gravtest_create(act, buf, argc, argv);
		return act;
	}

//----------------other----------------
	case _bdc_:
	{
		act = allocentity();
		act->fmt = act->type = _bdc_;
		toycar_create(act, buf, argc, argv);
		return act;
	}
	case _step_:
	{
		act = allocentity();
		act->fmt = act->type = _step_;
		stepcar_create(act, buf, argc, argv);
		return act;
	}

	case _gl41data_:
	{
		act = allocentity();
		act->fmt = act->type = _gl41data_;
		gl41data_create(act, buf, argc, argv);
		return act;
	}
	case _gl41coop_:
	{
		act = allocentity();
		act->fmt = act->type = _gl41coop_;
		gl41coop_create(act, buf, argc, argv);
		return act;
	}
	}//switch

	//
	return entitycreate_clone(type, buf, argc, argv);
}
void* entitymodify(int argc, u8** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		entitycreate(name, argv[3], argc-3, &argv[3]);
	}

	return 0;
}
void* entitysearch(u8* buf, int len)
{
	int j,k;
	u8* p;
	struct entity* act;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			act = &entity[j];
			if(0 == act->fmt)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->fmt, &act->fmt);
		}
		if(0 == j)say("empty entity\n");
	}
	else
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == entity[j].fmt)break;
			p = (void*)(&entity[j].fmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &entity[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




void freeentity()
{
	say("[e,f):freeing entity\n");
	mind_free();
	rule_free();
	thing_free();
	world_free();

	style = 0;
	entity = 0;
}
void initentity(u8* addr)
{
	int j;
	entity = (void*)(addr+0x000000);
	style = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct entity))
	for(j=0;j<0x200000;j++)addr[j] = 0;
	for(j=0;j<max;j++)entity[j].tier = _ent_;

	world_init(addr);
	thing_init(addr);
	rule_init(addr);
	mind_init(addr);
	say("[e,f):inited entity\n");
}
