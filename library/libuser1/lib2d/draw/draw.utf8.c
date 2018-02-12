#include<actor.h>
int utf2unicode(u8* src,u32* dst);




static void* utf8table = 0;
void initutf8(void* addr)
{
	utf8table = addr;
}




void drawunicode(struct arena* win, u32 rgb, int xx, int yy, u32 unicode)
{
	u16 temp;
	u16* points;
	int x,y,offset;
	int width = win->w;
	int height = win->h;
	u32* screen = (u32*)(win->buf);

	if(0 == utf8table)return;
	points = utf8table + 32*unicode;

	rgb |= 0xff000000;
	for(y=0;y<16;y++)
	{
		temp = points[y];
		for(x=0;x<16;x++)
		{
			offset = width*(yy+y) + xx+x;
			if(offset < 0)continue;
			if( (temp&0x8000) != 0 )screen[offset] = rgb;

			temp<<=1;
		}//x
	}//y
}
void drawutf8(struct arena* win, u32 rgb, int xx, int yy, u8* ch, int len)
{
	u32 unicode;
	utf2unicode(ch, &unicode);
	drawunicode(win, rgb, xx, yy, unicode);
}