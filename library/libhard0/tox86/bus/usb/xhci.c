#include "libhard.h"
u32 in32(u16 port);
void out32(u16 port, u32 data);




void xhci_mmioinit(u8* addr)
{
	say("xhci@mmio:%p{\n", addr);
	printmmio(addr, 0x20);
	say("}\n");
}
void xhci_portinit(u32 addr)
{
	u64 temp,high;
	say("xhci@port:%x{\n",addr);

	out32(0xcf8, addr+0x4);
	temp=in32(0xcfc) | (1<<10) | (1<<2);	//bus master=1
	out32(0xcf8, addr+0x4);
	out32(0xcfc, temp);

	out32(0xcf8, addr+0x4);
	temp = in32(0xcfc);
	say("sts,cmd=%x\n", temp);

	out32(0xcf8, addr+0x10);
	temp = in32(0xcfc);
	say("bar0=%x\n", temp);			//lo
	out32(0xcf8, addr+0x14);
	high = in32(0xcfc);
	say("bar1=%x\n", high);			//hi
	out32(0xcf8, addr+0x18);
	say("bar2=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x1c);
	say("bar3=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x20);
	say("bar4=%x\n", in32(0xcfc));
	out32(0xcf8, addr+0x24);
	say("bar5=%x\n", in32(0xcfc));

	say("}\n");


	//xhci mmio
	u8* mmio = (u8*)((temp&0xfffffff0) | (high<<32));
	xhci_mmioinit(mmio);
}