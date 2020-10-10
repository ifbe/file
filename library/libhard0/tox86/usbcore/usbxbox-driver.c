#include "libhard.h"
#include "usb.h"
void eventwrite(u64,u64,u64,u64);
void DEVICE_REQUEST_SET_CONFIGURATION(void* req, u16 conf);
void INTERFACE_REQUEST_GET_REPORT_DESC(void* req, u16 intf, u16 typeindex, u16 len);
void INTERFACE_REQUEST_SET_IDLE(struct UsbRequest* req, u16 intf, u16 val);
int xhci_giveorderwaitevent(void* hc,int id, u32,u32, void* sendbuf,int sendlen, void* recvbuf, int recvlen);
struct xbox360report{
	u8 msgtype;	//0: 0x00
	u8 msglen;	//1: 0x14

	u8 dpad_u:1;	//2
	u8 dpad_d:1;
	u8 dpad_l:1;
	u8 dpad_r:1;
	u8  start:1;
	u8   back:1;
	u8     l3:1;
	u8     r3:1;

	u8     lb:1;	//3
	u8     rb:1;
	u8   xbox:1;
	u8   rsvd:1;
	u8 kpad_a:1;
	u8 kpad_b:1;
	u8 kpad_x:1;
	u8 kpad_y:1;

	u8 lt;	//4
	u8 rt;	//5

	short lx;	//[6,7]
	short ly;	//[8,9]
	short rx;	//[10,11]
	short ry;	//[12,13]
}__attribute__((packed));
struct xbox360command_rumble{
	u8 cmdtype;	//0: 0x00
	u8 cmdlen;	//1: 0x08
	u8 rsvd2;
	u8 rumble_left;
	u8 rumble_right;
	u8 rsvd5;
	u8 rsvd6;
	u8 rsvd7;
}__attribute__((packed));
struct xbox360command_led{
	u8 cmdtype;	//0: 0x00
	u8 cmdlen;	//1: 0x03
	u8 led;
}__attribute__((packed));




struct xboxonereport{
	u8 sequence;	//0
	u8 unknown;		//1

	u8   sync:1;	//2
	u8   what:1;
	u8   menu:1;
	u8  share:1;
	u8 kpad_a:1;
	u8 kpad_x:1;
	u8 kpad_b:1;
	u8 kpad_y:1;

	u8 dpad_u:1;	//3
	u8 dpad_d:1;
	u8 dpad_l:1;
	u8 dpad_r:1;
	u8     l2:1;
	u8     r2:1;
	u8     l3:1;
	u8     r3:1;

	u16 lt;		//[4,5]
	u16 rt;		//[6,7]

	u16 lx;		//[8.9]
	u16 ly;		//[a,b]
	u16 rx;		//[c,d]
	u16 ry;		//[e,f]
}__attribute__((packed));
struct xboxonecommand{
	u8 what;
}__attribute__((packed));




static int xboxhid_ongive(struct item* usb,int xxx, struct item* xhci,int endp, void* sbuf,int slen, void* rbuf,int rlen)
{
	struct perusb* perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	struct xboxonereport* data = *(void**)sbuf;
	printmemory(data, 0x40);

	say("l=%x,r=%x,d=%x,u=%x\n",data->dpad_l,data->dpad_r,data->dpad_d,data->dpad_u);
	say("a=%x,b=%x,x=%x,y=%x\n",data->kpad_a,data->kpad_b,data->kpad_x,data->kpad_y);
	return 0;
}
int xboxhid_driver(struct item* usb,int xxx, struct item* xhci,int slot, struct descnode* intfnode, struct InterfaceDescriptor* intfdesc)
{
	int j,ret;
	struct UsbRequest req;
	struct perusb* perusb;
	//per device
	struct descnode* devnode;
	struct DeviceDescriptor* devdesc;
	struct descnode* confnode;
	struct ConfigurationDescriptor* confdesc;
	//per interface
	struct descnode* endpnode;
	struct EndpointDescriptor* endpdesc;


//------------------------basic information------------------------
	perusb = usb->priv_ptr;
	if(0 == perusb)return 0;

	if(0 == perusb->my.devnode)return -1;		//no devdesc?
	devnode = (void*)perusb + perusb->my.devnode;
	devdesc = (void*)perusb + devnode->real;

	if(0 == devnode->lchild)return -2;		//no confdesc?
	confnode = (void*)perusb + perusb->my.confnode;
	confdesc = (void*)perusb + confnode->real;

	say("[xbox]intf=%x,alt=%x,epcnt=%x\n",intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting, intfdesc->bNumEndpoints);
	if(0 == intfdesc->bNumEndpoints){
		say("[xbox]bNumEndpoints=0, trying next\n");
		return 0;
	}


//------------------------host side + my parse------------------------
	int outaddr = 2;
	int inaddr = 3;
	int pktlen = 0x40;

	j = intfnode->lchild;
	while(1){
		if(0 == j)break;
		endpnode = (void*)perusb + j;
		endpdesc = (void*)perusb + endpnode->real;

		switch(endpdesc->bDescriptorType){
		case 5:{
			if(3 != endpdesc->bmAttributes){
				say("[xbox]bmAttributes=%x, trying next\n", endpdesc->bmAttributes);
				return 0;
			}

			ret = (endpdesc->bEndpointAddress & 0xf) << 1;
			if(0 == (endpdesc->bEndpointAddress & 0x80)){
				outaddr = ret;
			}
			else{
				inaddr = ret + 1;
			}

			pktlen = endpdesc->wMaxPacketSize;
			say("[xboxhid]endpdesc: addr=%x, attr=%x, pktlen=%x, interval=%x\n",
				endpdesc->bEndpointAddress, endpdesc->bmAttributes,
				endpdesc->wMaxPacketSize, endpdesc->bInterval
			);

			if(1){
				//inform the xHC of the value of the Max Exit Latency parameter
				//ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_EvaluateContext, buf,8, 0,0);
				//if(ret < 0)return -9;

				//inform the xHC of the endpoint
				ret = xhci_giveorderwaitevent(xhci,slot, 'h',TRB_command_ConfigureEndpoint, endpdesc,sizeof(struct EndpointDescriptor), 0,0);
				if(ret < 0)return -9;
			}
			break;
		}//ep desc
		default:{
			say("[xboxhid]desctype=%x\n", endpdesc->bDescriptorType);
		}//report desc?
		}//switch

		j = endpnode->rfellow;
	}


//------------------------device side------------------------
	say("[xboxhid]set_config\n");
	DEVICE_REQUEST_SET_CONFIGURATION(&req, confdesc->bConfigurationValue);
	ret = xhci_giveorderwaitevent(xhci,slot, 'd',0, &req,8, 0,0);
	if(ret < 0)return -10;


//------------------------transfer ring------------------------
	say("[xboxhid]making trb\n");
	usb->ongiving = (void*)xboxhid_ongive;

	if(pktlen > 0x40)pktlen = 0x40;
	ret = xhci_giveorderwaitevent(xhci,slot|(inaddr<<8), 'd',0, perusb->freebuf,pktlen, usb,0);
	return 0;
}
int usbxbox_driver(struct item* usb, int xxx, struct item* xhci, int slot)
{
	struct perusb* perusb = usb->priv_ptr;

	struct descnode* devnode;
	struct DeviceDescriptor* devdesc;
	struct descnode* confnode;
	struct ConfigurationDescriptor* confdesc;
	struct descnode* intfnode;
	struct InterfaceDescriptor* intfdesc;

	if(0 == perusb->my.devnode)return -1;		//no devdesc?
	devnode = (void*)perusb + perusb->my.devnode;
	devdesc = (void*)perusb + devnode->real;

	if(0 == devnode->lchild)return -2;		//no confdesc?
	confnode = (void*)perusb + devnode->lchild;
	confdesc = (void*)perusb + confnode->real;
	perusb->my.confnode = (u8*)confnode - (u8*)perusb;

	if(0 == confnode->lchild)return -3;		//no intfdesc?
	intfnode = (void*)perusb + confnode->lchild;
	intfdesc = (void*)perusb + intfnode->real;

	//check version
	switch(devdesc->idProduct){
	case 0x0202:say("xbox\n");break;
	case 0x0285:
	case 0x0289:say("xbox-s\n");break;
	case 0x028e:say("xbox360\n");break;
	case 0x028f:say("xbox360-wireless\n");break;
	case 0x02d1:say("xboxone\n");break;
	case 0x02dd:say("xboxone-2015\n");break;
	case 0x02e3:say("xboxone-elite\n");break;
	case 0x02e6:say("xbox-dongle\n");break;
	case 0x02ea:say("xboxones\n");break;
	}

	//composite device, all interface
	while(1){
		say("[xbox]interface: node=%p,desc=%p, num=%x, alt=%x, c=%x,s=%x,p=%x\n",
			intfnode,intfdesc, intfdesc->bInterfaceNumber, intfdesc->bAlternateSetting,
			intfdesc->bInterfaceClass, intfdesc->bInterfaceSubClass, intfdesc->bInterfaceProtocol);

		xboxhid_driver(usb,xxx, xhci,slot, intfnode,intfdesc);

		if(0 == intfnode->rfellow)break;
		intfnode = (void*)perusb + intfnode->rfellow;
		intfdesc = (void*)perusb + intfnode->real;
	}
	return 0;
}
