#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define _S5_ hex32('_','S','5','_')
#define _APIC_ hex32('A','P','I','C')
#define _DSDT_ hex32('D','S','D','T')
#define _FACP_ hex32('F','A','C','P')
#define _HPET_ hex32('H','P','E','T')
#define _MADT_ hex32('M','A','D','T')
#define _MCFG_ hex32('M','C','F','G')
#define RSD_PTR_ hex64('R','S','D',' ','P','T','R',' ')
void printmemory(void*, int);
void say(void*, ...);




struct ACPIHEAD{
	u32 signature;	//[0,3]
	u32 len;		//[4,7]
	u8 revision;	//8
	u8 checksum;	//9
	u8 OEMID[6];	//[a,f]
	u8 OEMtableID[8];	//[10,17]
	u32 OEMrevision;	//[18,1b]
	u32 creatorID;		//[1c,1f]
	u32 creatorrev;		//[20,23]
}__attribute__((packed));
struct MCFG_CONFSPACE{
	u64 BaseAddr;
	u16 GroupNum;
	u8 BusNum_start;
	u8 BusNum_end;
	u32 Rsvd;
}__attribute__((packed));




static u16 g_port;
static u16 g_data;
void getportanddata(u16* p, u16* d)
{
	*p = g_port;
	*d = g_data;
}




void parse_S5_(void* p)
{
	u16 data;
	say("%.4s@%p\n", p, p);

	data = (*(u8*)(p+8))<<10;
	say("data=%p\n", data);

	g_data = data;
}
void acpi_DSDT(void* p)
{
	int j,cnt;
	say("%.4s@%p\n", p, p);

	cnt = *(u32*)(p+4);
	for(j=4;j<cnt;j++){
		if(_S5_ == *(u32*)(p+j)){
			parse_S5_(p+j);
			break;
		}
	}
}
void acpi_FACP(void* p)
{
	u16 port;
	u64 addr;

	port = *(u16*)(p+0x40);
	say("port=%p\n", port);

	addr = *(u32*)(p+0x28);
	acpi_DSDT((void*)addr);

	g_port = port;
}
void acpi_HPET(void* p)
{
}
void acpi_MADT(void* p)
{
}
void acpi_MCFG(void* p)
{
	int j;
	struct MCFG_CONFSPACE* c = p+0x2c;
	for(j=0;j<1;j++){
		say("%02x: basebase=%llx,group=%x,start=%x,end=%x\n", j,
		c[j].BaseAddr, c[j].GroupNum, c[j].BusNum_start, c[j].BusNum_end);
	}
}
void acpi_APIC(void* p)
{
}
void acpitable(void* p)
{
	say("%.4s@%p\n", p, p);
	switch(*(u32*)p){
	case _APIC_:acpi_APIC(p);break;
	case _DSDT_:acpi_DSDT(p);break;
	case _FACP_:acpi_FACP(p);break;
	case _HPET_:acpi_HPET(p);break;
	case _MADT_:acpi_MADT(p);break;
	case _MCFG_:acpi_MCFG(p);break;
	}
}
void acpirsdt(void* buf)
{
	int j;
	int len = *(u8*)(buf+4);
	say("rsdt,%x@%p\n", len, buf);

	u64 addr;
	for(j=0x24;j<len;j+=4){
		addr = *(u32*)(buf+j);
		acpitable((void*)addr);
	}
}
void acpixsdt(void* buf)
{
	int j;
	int len = *(u8*)(buf+4);
	say("xsdt,%x@%p\n", len, buf);

	u64 addr;
	for(j=0x24;j<len;j+=8){
		addr = *(u32*)(buf+j);
		acpitable((void*)addr);
	}
}
void initacpi(void* buf)
{
	if(0 == buf)return;

	int len = *(u8*)(buf+4);
	say("rsdptr,%x@%p\n", len, buf);

	if(0 == *(u8*)(buf+0xf)){
		void* rsdt = (void*)(u64)(*(u32*)(buf+0x10));
		acpirsdt(rsdt);
	}
	else{
		void* xsdt = (void*)(u64)(*(u32*)(buf+0x18));
		acpixsdt(xsdt);
	}
}