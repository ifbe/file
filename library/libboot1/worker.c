#include "libboot.h"
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
//
void initexiter(void*);
void exiter_create(struct worker*, u8*, int, u8**);
void exiter(void*);
//
void initpulser(void*);
void pulser_create(struct worker*, u8*, int, u8**);
void pulser(void*);
//
void initpoller(void*);
void poller_create(struct worker*, u8*, int, u8**);
void poller(void*);
//
void initrealer(void*);
void realer_create(struct worker*, u8*, int, u8**);
void realer(void*);
//
void initwaiter(void*);
void waiter_create(struct worker*, u8*, int, u8**);
void waiter(void*);
//
void compiler_create(struct worker*, u8*, int, u8**);
//
void initkernel(void*);
void kernel_create(struct worker*, u8*, int, u8**);
//
void myml_create(struct worker*, u8*, int, u8**);
//
void initmython(void*);
void mython_create(struct worker*, u8*, int, u8**);




static struct worker* wrk;
static int wrklen = 0;
void* allocworker()
{
	void* addr = &wrk[wrklen];
	wrklen += 1;
	return addr;
}




int workerread(_wrk* wrk,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	return 0;
}
int workerwrite(_wrk* wrk,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int workerdiscon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int workerlinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@workerlinkup\n");
	return 0;
}




int workerdelete(void* addr)
{
	struct worker* tmp;
	if(0 == addr)return 0;

	tmp = addr;
	say("workerdelete:%.8s\n", &tmp->type);

	return 0;
}
void* workercreate(u64 type, void* url, int argc, u8** argv)
{
	struct worker* tmp;

	//
	if(_compiler_ == type){
		//self @ 0
		tmp = allocworker();
		tmp->type = _compiler_;
		compiler_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_kernel_ == type){
		//self @ 0
		tmp = allocworker();
		tmp->type = _kernel_;
		kernel_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_myml_ == type){
		//self @ 0
		tmp = allocworker();
		tmp->type = _myml_;
		myml_create(tmp, url, argc, argv);
		return tmp;
	}
	if(_mython_ == type){
		//self @ 0
		tmp = allocworker();
		tmp->type = _mython_;
		mython_create(tmp, url, argc, argv);
		return tmp;
	}

	//
	if(_exiter_ == type)
	{
		tmp = allocworker();
		tmp->type = _exiter_;

		exiter_create(tmp, url, argc, argv);
		threadcreate(exiter, tmp);
		return tmp;
	}
	if(_pulser_ == type)
	{
		tmp = allocworker();
		tmp->type = _pulser_;

		pulser_create(tmp, url, argc, argv);
		threadcreate(pulser, tmp);
		return tmp;
	}
	if(_poller_ == type){
		tmp = allocworker();
		tmp->type = _poller_;

		poller_create(tmp, url, argc, argv);
		threadcreate(poller, tmp);
		return tmp;
	}
	if(_realer_ == type)
	{
		tmp = allocworker();
		tmp->type = _realer_;

		realer_create(tmp, url, argc, argv);
		threadcreate(realer, tmp);
		return tmp;
	}
	if(_waiter_ == type)
	{
		tmp = allocworker();
		tmp->type = _waiter_;

		waiter_create(tmp, url, argc, argv);
		threadcreate(waiter, tmp);
		return tmp;
	}

	return 0;
}
int workermodify(int argc, u8** argv)
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
		workercreate(name, argv[3], argc-3, &argv[3]);
	}
	return 0;
}
int workersearch(u8* buf, int len)
{
	int j,k=0;
	for(j=0;j<64;j++)
	{
		if(0 == wrk[j].type)continue;
		say("[%04x]: %.8s\n", j, &wrk[j].type);
		k++;
	}

	if(0 == k)say("empth worker\n");
	return 0;
}




void freeworker()
{
	say("[2,4):worker freeing\n");

	freestdev();
	freestdrel();

	say("[2,4):worker freeed\n");
}
void initworker(u8* addr)
{
	say("[2,4):worker initing\n");

	int j;
	wrk = (void*)(addr+0x000000);

#define max (0x100000/sizeof(struct worker))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)wrk[j].tier = _wrk_;

	initstdev( addr+0x100000);
	initstdrel(addr+0x180000);

	initkernel(addr - 0x200000);
	initmython(addr - 0x200000);

	initpoller(addr - 0x200000);
	initrealer(addr - 0x200000);

	say("[2,4):worker inited\n");
}
