#include "libuser.h"
#define _gpio_ hex32('g','p','i','o')
int boardstart(int, int);
int boardstop(int);
int boardread(int, int, void*, int);
int boardwrite(int, int, void*, int);




static int name[16] = {
	hex32('x','d', 0, 0),
	hex32('x','s', 0, 0),
	hex32('y','d', 0, 0),
	hex32('y','s', 0, 0),

	hex32('z','d', 0, 0),
	hex32('z','s', 0, 0),
	hex32('w','d', 0, 0),
	hex32('w','s', 0, 0),

	hex32('e','n', 0, 0)
};
static char table[16];




int stepcar_delete(struct arena* win)
{
	int j;
	for(j=0;j<9;j++)boardstop(table[j]);
	return 0;
}
int stepcar_create(struct arena* win)
{
	int j;
	for(j=0;j<9;j++)table[j] = boardstart(name[j], 'o');
	return 0;
}
int stepcar_stop(struct arena* win, struct style* sty)
{
	boardwrite(_gpio_, table[8], 0, 1);
	return 0;
}
int stepcar_start(struct arena* win, struct style* sty)
{
	boardwrite(_gpio_, table[8], 0, 0);
	return 0;
}
int stepcar_rootread(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
	return 0;
}
int stepcar_rootwrite(struct arena* win, struct style* sty, void* sc, void* sf, u8* buf, int len)
{
	int j,k;
	u8 v[4];
	switch(buf[0])
	{
		case 'w':v[0] = 0;v[1] = 1;v[2] = 1;v[3] = 0;break;
		case 's':v[0] = 1;v[1] = 0;v[2] = 0;v[3] = 1;break;
		case 'a':v[0] = 1;v[1] = 1;v[2] = 0;v[3] = 0;break;
		case 'd':v[0] = 0;v[1] = 0;v[2] = 1;v[3] = 1;break;
		case 'j':v[0] = 1;v[1] = 1;v[2] = 1;v[3] = 1;break;
		case 'k':v[0] = 0;v[1] = 0;v[2] = 0;v[3] = 0;break;
		case '1':{
			boardwrite(_gpio_, table[0], 0, 1);
			boardwrite(_gpio_, table[1], 0, 1);
			sleep_us(1000);
			boardwrite(_gpio_, table[1], 0, 0);
			return 0;
		}
		case '2':{
			boardwrite(_gpio_, table[2], 0, 1);
			boardwrite(_gpio_, table[3], 0, 1);
			sleep_us(1000);
			boardwrite(_gpio_, table[3], 0, 0);
			return 0;
		}
		case '3':{
			boardwrite(_gpio_, table[4], 0, 1);
			boardwrite(_gpio_, table[5], 0, 1);
			sleep_us(1000);
			boardwrite(_gpio_, table[5], 0, 0);
			return 0;
		}
		case '4':{
			boardwrite(_gpio_, table[6], 0, 1);
			boardwrite(_gpio_, table[7], 0, 1);
			sleep_us(1000);
			boardwrite(_gpio_, table[7], 0, 0);
			return 0;
		}
		case ' ':stepcar_start(win,0);return 0;
		default: stepcar_stop(win, 0);return 0;
	}

	//dir
	for(j=0;j<8;j+=2)boardwrite(_gpio_, table[j], 0, v[j/2]);

	//step
	for(k=0;k<800;k++)
	{
		for(j=1;j<8;j+=2)boardwrite(_gpio_, table[j], 0, 1);
		sleep_us(1000);
		for(j=1;j<8;j+=2)boardwrite(_gpio_, table[j], 0, 0);
		sleep_us(1000);
	}

	return 0;
}
