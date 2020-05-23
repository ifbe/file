#include "libuser.h"
#define _gpio_ hex32('g','p','i','o')
int boardstart(int, int);
int boardstop(int);
int boardread(int, int, void*, int);
int boardwrite(int, int, void*, int);




static int name[9] = {
	hex32('x','d', 0, 0),	//direction
	hex32('x','s', 0, 0),	//step
	hex32('y','d', 0, 0),
	hex32('y','s', 0, 0),

	hex32('z','d', 0, 0),
	hex32('z','s', 0, 0),
	hex32('w','d', 0, 0),
	hex32('w','s', 0, 0),

	hex32('e','n', 0, 0)	//enable
};
static char table[9];
static int actual[9];
static int expect[9];




static void stepcar_test(int a)
{
	int j;
	boardwrite(_gpio_, table[a], 0, 0);

	for(j=0;j<200*32;j++)
	{
		boardwrite(_gpio_, table[a+1], 0, 1);
		sleep_us(100);

		boardwrite(_gpio_, table[a+1], 0, 0);
		sleep_us(100);
	}
}
static void stepcar_update(int a, int b, int c, int d)
{
	int j,k;

	actual[0] = a;
	actual[2] = b;
	actual[4] = c;
	actual[6] = d;

	for(j=0;j<8;j+=2)boardwrite(_gpio_, table[j], 0, actual[j]);
	for(k=0;k<200*32;k++)
	{
		for(j=1;j<8;j+=2)boardwrite(_gpio_, table[j], 0, 1);
		sleep_us(100);

		for(j=1;j<8;j+=2)boardwrite(_gpio_, table[j], 0, 0);
		sleep_us(100);
	}
}
static void stepcar_status(int EN)
{
	actual[8] = EN;
	boardwrite(_gpio_, table[8], 0, EN);
}





int stepcar_taking(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int key, u8* buf,int len)
{
	int j;
	int ret = 0;

	for(j=0;j<9;j++){
		ret += mysnprintf(buf+ret, 256,
		"%.4s: pin=%d, val=%d<br>\n",
		&name[j], table[j], expect[j]
		);
	}
	return ret;
}
int stepcar_giving(struct entity* ent,int foot, struct halfrel* stack,int sp, void* arg,int key, u8* buf,int len)
{
	say("@stepcar_write\n");
	printmemory(buf, len);

	switch(buf[0])
	{
		case '1':stepcar_test(0);break;
		case '2':stepcar_test(2);break;
		case '3':stepcar_test(4);break;
		case '4':stepcar_test(6);break;
		case 'w':stepcar_update(0, 1, 0, 1);break;
		case 's':stepcar_update(1, 0, 1, 0);break;
		case 'a':stepcar_update(1, 1, 0, 0);break;
		case 'd':stepcar_update(0, 0, 1, 1);break;
		case 'j':stepcar_update(1, 1, 1, 1);break;
		case 'k':stepcar_update(0, 0, 0, 0);break;
		case ' ':stepcar_status(0);return 0;
		default: stepcar_status(1);return 0;
	}

	return 0;
}
void stepcar_discon(struct halfrel* self, struct halfrel* peer)
{
}
void stepcar_linkup(struct halfrel* self, struct halfrel* peer)
{
}
int stepcar_delete(struct supply* win)
{
	int j;
	for(j=0;j<9;j++)boardstop(table[j]);
	return 0;
}
int stepcar_create(struct supply* win)
{
	int j;
	for(j=0;j<9;j++)table[j] = boardstart(name[j], 'o');
	return 0;
}
