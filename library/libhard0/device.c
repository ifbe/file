#include "libhard.h"
//hw
int inithardware();
int freehardware();
//i2c
int i2c_create(void*, int, int, u8**);
int i2c_delete(int);
int i2c_read(int fd, int addr, u8* buf, int len);
int i2c_write(int fd, int addr, u8* buf, int len);
//spi
int spi_create(void*, int, int, u8**);
int spi_delete(int);
int spi_read(int fd, int addr, u8* buf, int len);
int spi_write(int fd, int addr, u8* buf, int len);
//gpio
int gpio_create(void*,void*,int,void*);
int gpio_delete(void*);
int gpio_read(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);
int gpio_write(_obj* obj,void* foot, p64 arg,int cmd, u8* buf,int len);




static struct item* dev;
static int devlen = 0;
static void* aaa;
static int aaalen = 0;




#define maxdevlen (0x100000/sizeof(struct item))
void device_init(u8* addr)
{
	logtoall("[4,6):device initing\n");

	int j;
	for(j=0;j<0x200000;j++)addr[j]=0;

	dev = (void*)(addr+0x000000);
	devlen = maxdevlen-1;
	for(j=0;j<maxdevlen;j++)dev[j].tier = _dev_;

//#define maxaaalen
	aaa = (void*)(addr+0x100000);
	aaalen = 0;

	logtoall("[4,6):device inited\n");
}
void device_exit()
{
	logtoall("[4,6):device exiting\n");

	freehardware();

	logtoall("[4,6):device exited\n");
}




void device_recycle()
{
}
int device_obj2fd(_obj* obj)
{
	u8* a0 = (u8*)dev;
	u8* a1 = (u8*)obj;
	return (a1-a0)/sizeof(struct item);
}
void* device_fd2obj(int fd)
{
	return &dev[fd];
}
void* device_alloc()
{
	void* addr = &dev[devlen];
	devlen -= 1;
	return addr;
}
void* device_alloc_prep(u64 tier, u64 type, u64 hfmt, u64 vfmt)
{
	_obj* obj;
	switch(type){
	case _gpio_:
		obj = device_alloc();
		if(0 == obj)return 0;

		obj->type = _gpio_;
		return obj;
	default:
		return 0;
	}
}




void* device_create(u64 type, void* name, int argc, u8** argv)
{
	void* obj;
	if(0 == type){
		return device_alloc();
	}
	if(_uart_ == type){
		struct item* p = device_alloc();
		p->type = _uart_;
		p->hfmt = _uart_;
		return p;
	}
	if(_cpu_ == type){
		struct item* p = device_alloc();
		p->type = _cpu_;
		p->hfmt = _cpu_;
		return p;
	}
	if(_irq_ == type){
		struct item* p = device_alloc();
		p->type = _irq_;
		p->hfmt = _irq_;
		return p;
	}
	if(_tmr_ == type){
		struct item* p = device_alloc();
		p->type = _tmr_;
		p->hfmt = _tmr_;
		return p;
	}
	if(_pci_ == type){
		struct item* p = device_alloc();
		p->type = _bus_;
		p->hfmt = _pci_;
		return p;
	}
	if(_ahci_ == type){
		struct item* p = device_alloc();
		p->type = _ahci_;
		p->hfmt = _ahci_;
		return p;
	}
	if(_xhci_ == type){
		struct item* p = device_alloc();
		p->type = _xhci_;
		p->hfmt = _xhci_;
		return p;
	}
	if(_usb_ == type){
		struct item* p = device_alloc();
		p->type = _usb_;
		p->hfmt = _usb_;
		return p;
	}
	if(_mmc_ == type){
		struct item* p = device_alloc();
		p->type = _mmc_;
		p->hfmt = _mmc_;
		return p;
	}
	if(_eth_ == type){
		struct item* p = device_alloc();
		p->type = _eth_;
		p->hfmt = _eth_;
		return p;
	}
	if(_i2c_ == type){
		int fd = i2c_create(name, 0, argc, argv);
		if(fd <= 0)return 0;

		dev[fd].type = _i2c_;
		dev[fd].priv_fd = fd;

		return &dev[fd];
	}
	if(_spi_ == type){
		int fd = spi_create(name, 0, argc, argv);
		if(fd <= 0)return 0;

		dev[fd].type = _spi_;
		dev[fd].priv_fd = fd;
		return &dev[fd];
	}
	if(_gpio_ == type){
		obj = device_alloc_prep(0, _gpio_, 0, 0);
		gpio_create(obj, name, argc, argv);
		return obj;
	}
	return 0;
}
int device_delete(_obj* this)
{
	return 0;
}
int device_reader(struct item* dev,void* foot, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@device_reader\n");
	int fd = dev->priv_fd;
	switch(dev->type){
		case _gpio_:return gpio_read(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}
int device_writer(struct item* dev,void* foot, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@device_writer\n");
	int fd = dev->priv_fd;
	switch(dev->type){
		case _gpio_:return gpio_write(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}




int device_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@deviceattach\n");
	return 0;
}
int device_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@devicedetach\n");
	return 0;
}
int device_takeby(struct item* dev,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@deviceread\n");
	if(dev->ontaking){
		return dev->ontaking(dev,foot, stack,sp, arg,cmd, buf,len);
	}

	int fd = dev->priv_fd;
	switch(dev->type){
		case _i2c_:return i2c_read(fd, cmd, buf, len);break;
		case _spi_:return spi_read(fd, cmd, buf, len);break;
		case _gpio_:return gpio_read(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}
int device_giveby(struct item* dev,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	//logtoall("@device_giveby\n");
	u8 t[2];
	if(0 == buf){
		t[0] = len;
		buf = t;
		len = 1;
	}

	int fd = dev->priv_fd;
	switch(dev->type){
		case _i2c_:return i2c_write(fd, cmd, buf, len);break;
		case _spi_:return spi_write(fd, cmd, buf, len);break;
		case _gpio_:return gpio_write(dev,foot, arg,cmd, buf,len);break;
	}
	return 0;
}




int devicecommand_insert(u8* name, u8* arg)
{
	return 0;
}
int devicecommand_remove(u8* name)
{
	return 0;
}
int devicecommand_search(u8* name)
{
	int j;
	_obj* act;
	if(0 == name){
		for(j=0;j<maxdevlen;j++){
			act = &dev[j];
			if((0 == act->type)&&(0 == act->hfmt))continue;
			logtoall("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->hfmt, &act->hfmt);
		}
		if(0 == j)logtoall("empty device\n");
	}
	else{
		for(j=0;j<0x100;j++){
			if(0 == dev[j].hfmt)break;
			if(0 == cmp(&dev[j].hfmt, name))logtoall("name=%d,node=%p\n", name, &dev[j]);
			break;
		}
	}
	return 0;
}
int devicecommand_modify(int argc, u8** argv)
{
	return 0;
}
void* devicecommand(int argc, u8** argv)
{
	if(argc < 2){
		logtoall("device insert name arg\n");
		logtoall("device search name\n");
	}
	else if(0 == ncmp(argv[1], "insert", 6)){
		//device create name arg
		devicecommand_insert(argv[2], argv[3]);
	}
	else if(0 == ncmp(argv[1], "search", 6)){
		//device search <name>
		devicecommand_search((argc<3) ? 0 : argv[2]);
	}

	return 0;
}
