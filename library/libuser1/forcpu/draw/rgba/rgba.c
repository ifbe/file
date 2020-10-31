#include "libuser.h"
void* allocsupply();
//
int preprocess(void*);
int postprocess(void*);
int entityinput_touch(struct supply* win, struct event* ev);




void pixel_clearcolor(struct supply* win)
{
	int x;
	int w = win->width;
	int h = win->height;
	int s = win->fbwidth>>2;
	int len = s*h;
	u32* buf = (u32*)(win->rgbabuf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
}
void pixel_cleardepth(struct supply* wnd)
{
	int x,y,w,h,stride;
	float* depth = wnd->depthbuf;
	if(0 == depth){depth = wnd->depthbuf = memorycreate(2048*2048*4, 0);}

	stride = wnd->fbwidth>>2;
	for(y=0;y<wnd->height;y++){
		for(x=0;x<wnd->width;x++)depth[y*stride+x] = 1000.0;
	}
}




int wndmgr_take(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//pixel_clearcolor(wnd);

	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			entity_take(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}
next:
		rel = samesrcnextdst(rel);
	}

	int x = wnd->ix0;
	int y = wnd->iy0;
	drawline((void*)wnd, 0xffff00, x-16, y, x+16, y);
	drawline((void*)wnd, 0xffff00, x, y-16, x, y+16);
	return 0;
}
int wndmgr_give(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@rgbanode_write:%p,%x\n", wnd,foot);
	//printmemory(buf,16);
	struct event* ev = buf;
	if(0x4070 == ev->what){
		short* pp = (short*)ev;
		wnd->ix0 += pp[0];
		if(wnd->ix0 < 0)wnd->ix0 = 0;
		if(wnd->ix0 >= wnd->width)wnd->ix0 = wnd->width-1;

		wnd->iy0 += pp[1];
		if(wnd->iy0 < 0)wnd->iy0 = 0;
		if(wnd->iy0 >= wnd->height)wnd->iy0 = wnd->height-1;
		return 0;
	}

	float max = -1.0;
	struct relation* rel = wnd->oreln;
	struct relation* the = rel;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			struct style* sty = (void*)(rel->srcfoot);
			if(sty){
				//say("%f\n", sty->fs.vc[3]);
				if(max < sty->fs.vc[3]){
					the = rel;
					max = sty->fs.vc[3];
				}
			}
		}

		rel = samesrcprevdst(rel);
	}

//say("max=%f,the=%p\n",max,the);
	if(the){
		stack[sp+0].pchip = the->psrcchip;
		stack[sp+0].pfoot = the->psrcfoot;
		stack[sp+0].flag = the->srcflag;
		stack[sp+1].pchip = the->pdstchip;
		stack[sp+1].pfoot = the->pdstfoot;
		stack[sp+1].flag = the->dstflag;
		entity_give(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
	}
	return 0;
}




int rgbanode_take(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//pixel_clearcolor(wnd);

	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			entity_take(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}
next:
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int rgbanode_give(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@rgbanode_write:%p,%x\n", wnd,foot);
	//printmemory(buf,16);

	float max = -1.0;
	struct relation* rel = wnd->oreln;
	struct relation* the = rel;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dsttype){
			struct style* sty = (void*)(rel->srcfoot);
			if(sty){
				//say("%f\n", sty->fs.vc[3]);
				if(max < sty->fs.vc[3]){
					the = rel;
					max = sty->fs.vc[3];
				}
			}
		}

		rel = samesrcprevdst(rel);
	}

//say("max=%f,the=%p\n",max,the);
	if(the){
		stack[sp+0].pchip = the->psrcchip;
		stack[sp+0].pfoot = the->psrcfoot;
		stack[sp+0].flag = the->srcflag;
		stack[sp+1].pchip = the->pdstchip;
		stack[sp+1].pfoot = the->pdstfoot;
		stack[sp+1].flag = the->dstflag;
		entity_give(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
	}
	return 0;
}
int rgbanode_discon(struct supply* win, struct style* sty)
{
	return 0;
}
int rgbanode_linkup(struct supply* twig, void* tf, struct supply* root, void* rf)
{
	return 0;
}





int rgbanode_search(struct supply* win)
{
	return 0;
}
int rgbanode_modify(struct supply* win)
{
	return 0;
}
int rgbanode_delete(struct supply* win)
{
	return 0;
}
int rgbanode_create(u64 type, void* addr)
{
	return 0;
}
