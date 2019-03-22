#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"
int arenaevent(struct event* ev);
//
void initobject(void*);
void initshader(void*);
void inittexture(void*);
void initvertex(void*);
void callback_update(void*);
void callback_display(void*, void*);
static u8 uppercase[] = {
	' ', '!','\"', '#', '$', '%', '&','\"',		//20,27
	'(', ')', '*', '+', '<', '_', '>', '?',		//28,2f
	')', '!', '@', '#', '$', '%', '^', '&',		//30,37
	'*', '(', ':', ':', '<', '+', '>', '?',		//38,3f
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',		//40,47
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',		//48,4f
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',		//50,57
	'X', 'Y', 'Z', '{', '|', '}', '^', '_',		//58,5f
	'~', 'A', 'B', 'C', 'D', 'E', 'F', 'G',		//60,67
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',		//68,6f
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',		//70,77
	'X', 'Y', 'Z', '{', '|', '}', '~', ' ',		//78,7f
};




static void thread_joystick(struct arena* win)
{
	int j, k;
	int c1, c2;
	void* p;
	const float* f;
	const unsigned char* u;
	struct xyzwpair pair;
	struct event ev;
	ev.where = 0;
	ev.when = 0;
	while(1)
	{
		for(j=0;j<16;j++)
		{
			if(0 == glfwJoystickPresent(GLFW_JOYSTICK_1 + j))continue;

			f = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + j, &c1);
			if(0 == f)continue;
			if(0 == c1)continue;

			u = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + j, &c2);
			if(0 == u)continue;
			if(0 == c2)continue;

			//for(k=0;k<c1;k++)say("a%d:%f\n", k, f[k]);
			//for(k=0;k<c2;k++)say("b%d:%x\n", k, u[k]);
			pair.x0 = (int)( 32767*f[0]);
			pair.y0 = (int)(-32767*f[1]);
			pair.z0 = (int)(127*(1.0+f[4]));
			pair.w0 = 0;
			pair.xn = (int)( 32767*f[2]);
			pair.yn = (int)(-32767*f[3]);
			pair.zn = (int)(127*(1.0+f[5]));
			pair.wn = 0;
/*
			switch(u[10] | (u[11]<<1) | (u[12]<<2) | (u[13]<<3))
			{
				case  9:pair.w0 |= joyl_left;break;
				case  6:pair.w0 |= joyl_right;break;
				case 12:pair.w0 |= joyl_down;break;
				case  3:pair.w0 |= joyl_up;break;
				case  8:pair.w0 |= joyl_down|joyl_left;break;
				case  4:pair.w0 |= joyl_down|joyl_right;break;
				case  2:pair.w0 |= joyl_up|joyl_right;break;
				case  0:pair.w0 |= joyl_up|joyl_left;break;
			}
			if(u[0])pair.wn |= joyr_down;
			if(u[1])pair.wn |= joyr_right;
			if(u[2])pair.wn |= joyr_left;
			if(u[3])pair.wn |= joyr_up;
			if(u[4])pair.w0 |= joyl_bumper;
			if(u[5])pair.wn |= joyr_bumper;
			if(u[6])pair.w0 |= joyl_select;
			if(u[7])pair.wn |= joyr_start;
			if(u[8])pair.w0 |= joyl_stick;
			if(u[9])pair.wn |= joyr_stick;
			if(f[4]>0.0)pair.w0 |= joyl_trigger;
			if(f[5]>0.0)pair.wn |= joyr_trigger;
*/
			if(u[ 0])pair.wn |= joyr_left;
			if(u[ 1])pair.wn |= joyr_down;
			if(u[ 2])pair.wn |= joyr_right;
			if(u[ 3])pair.wn |= joyr_up;
			if(u[ 4])pair.w0 |= joyl_bumper;
			if(u[ 5])pair.wn |= joyr_bumper;
			if(u[ 6])pair.w0 |= joyl_trigger;
			if(u[ 7])pair.wn |= joyr_trigger;
			if(u[ 8])pair.w0 |= joyl_select;
			if(u[ 9])pair.wn |= joyr_start;
			if(u[10])pair.w0 |= joyl_stick;
			if(u[11])pair.wn |= joyr_stick;
			//if(u[12])pair.w0 |= joyl_bumper;
			//if(u[13])pair.wn |= joyr_bumper;
			if(u[14])pair.w0 |= joyl_up;
			if(u[15])pair.w0 |= joyl_right;
			if(u[16])pair.w0 |= joyl_down;
			if(u[17])pair.w0 |= joyl_left;

			//printmemory(&pair, 16);
			p = &pair;

			ev.why = *(u64*)(p+0);
			ev.what = joy_left;
			arenaevent(&ev);

			ev.why = *(u64*)(p+8);
			ev.what = joy_right;
			arenaevent(&ev);
		}
		sleep_us(10000);
	}
}
static void callback_keyboard(GLFWwindow* fw, int key, int scan, int action, int mods)
{
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);
    //printf("key=%x,scan=%x,action=%x,mods=%x\n", key, scan, action, mods);

	if(0 == action)return;
	if(0x118 == key)return;		//capslock
	if((key >= 0x154)&&(key <= 0x15b))return;	//ctrl,alt...

	if(0x100 == key)
	{
		e.why = 0x1b;
		e.what = _kbd_;
	}
	else if((key >= 0x106)&&(key <= 0x109))
	{
		if(key == 0x109)e.why = 0x48;		//up
		else if(key == 0x108)e.why = 0x50;	//down
		else if(key == 0x107)e.why = 0x4b;	//left
		else if(key == 0x106)e.why = 0x4d;	//right
		e.what = _kbd_;
	}
	else if((key >= 0x122)&&(key <= 0x12d))
	{
		e.why = key + 0xf1 - 0x122;
		e.what = _kbd_;
	}
	else if(0x101 == key)
	{
		e.why = 0xd;
		e.what = _char_;
	}
	else if(0x103 == key)
	{
		e.why = 0x8;
		e.what = _char_;
	}
	else if((key >= 'A')&&(key <= 'Z'))
	{
		if(0 == (mods&1))key += 0x20;
		e.why = key;
		e.what = _char_;
	}
	else
	{
		if(mods&1)key = uppercase[key-0x20];
		e.why = key;
		e.what = _char_;
	}

	e.where = (u64)win;
	arenaevent(&e);
	//eventwrite(why, what, where, 0);
}
static void callback_mouse(GLFWwindow* fw, int button, int action, int mods)
{
	u64 x,y,temp;
	struct event e;
	double xpos, ypos;
	struct arena* win = glfwGetWindowUserPointer(fw);

	glfwGetCursorPos(fw, &xpos, &ypos);
	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	if(1 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2b70;
		e.where = (u64)win;
		arenaevent(&e);
	}
	else if(0 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2d70;
		e.where = (u64)win;
		arenaevent(&e);
	}
}
static void callback_move(GLFWwindow* fw, double xpos, double ypos)
{
	u64 x,y,temp;
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);

	if(0 != glfwGetMouseButton(fw, 1))temp = 'r';
	else temp = 'l';

	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	e.why = x + (y<<16) + (temp<<48);
	e.what = 0x4070;
	e.where = (u64)win;
	arenaevent(&e);
}
static void callback_scroll(GLFWwindow* fw, double x, double y)
{
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);
	//printf("%llx: %f,%f\n", (u64)win, x, y);

	e.where = (u64)win;
	e.what = 0x2b70;
	if(y > 0.0)	//wheel_up
	{
		e.why = ((u64)'f')<<48;
		arenaevent(&e);
		//eventwrite(why, 0x2b70, where, 0);
	}
	else	//wheel_down
	{
		e.why = ((u64)'b')<<48;
		arenaevent(&e);
		//eventwrite(why, 0x2b70, where, 0);
	}
}
static void callback_drop(GLFWwindow* fw, int count, const char** paths)
{
	char dragdata[0x1000];
	int j,ret=0;
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);

	for(j=0;j<count;j++)
	{
		ret += snprintf(dragdata+ret, 0x1000-ret, "%s\n", paths[j]);
		//printf("%s\n", paths[j]);
	}

	e.why = (u64)dragdata;
	e.what = _drag_;
	e.where = (u64)win;
	arenaevent(&e);
}
static void callback_reshape(GLFWwindow* fw, int w, int h)
{
	struct arena* win = glfwGetWindowUserPointer(fw);
	win->fbwidth = win->fbstride = w;
	win->fbheight = h;

	glfwGetWindowSize(fw, &w, &h);
	win->width = win->stride = w;
	win->height = h;
}
static void callback_joystick(int id, int ev)
{
	say("joystick: %d,%d\n", id, ev);
}




void windowopen(struct arena* r, struct arena* w)
{
	int x,y,j;
	GLFWwindow* rw = 0;

	//1.glfw
	if(r)rw = r->win;
	GLFWwindow* fw = glfwCreateWindow(512, 512, "42", NULL, rw);
	if(0 == fw)
	{
		printf("error@glfwCreateWindow\n");
		return;
	}

	//2.setup
	glfwSetWindowUserPointer(fw, w);
	w->win = fw;
	w->map = 0;
	glfwGetFramebufferSize(fw, &x, &y);
	w->fbwidth = w->fbstride = x;
	w->fbheight = y;

	//3.callback
	glfwSetDropCallback(fw, callback_drop);
	glfwSetKeyCallback(fw, callback_keyboard);
	glfwSetScrollCallback(fw, callback_scroll);
	glfwSetCursorPosCallback(fw, callback_move);
	glfwSetMouseButtonCallback(fw, callback_mouse);
	glfwSetFramebufferSizeCallback(fw, callback_reshape);

	if(0 == r)
	{
		//2.glew
		glfwMakeContextCurrent(fw);
		glewExperimental = 1;
		if(glewInit() != GLEW_OK)
		{
			printf("error@glewInit\n");
			return;
		}

		//3.init
		initobject(w);
		initshader(w);
		inittexture(w);
		initvertex(w);
	}
	else	//_coop_
	{
		w->map = malloc(0x100000);
		memset(w->map, 0, 0x100000);
	}
}




void windowread(struct arena* w)
{
	GLFWwindow* fw;

	//
	if(_fbo_ == w->fmt)
	{
		//say("@windowread fbo\n");
		callback_display(w, 0);
	}
	else
	{
		arena_rootread(w, 0, 0, 0, 0, 0);

		fw = w->win;
		glfwMakeContextCurrent(fw);
		callback_update(w);
		callback_display(w, 0);
		glfwSwapBuffers(fw);

		//cleanup events
		if(glfwWindowShouldClose(fw)){eventwrite(0,0,0,0);return;}
		glfwPollEvents();
	}

}
void windowwrite(struct arena* w)
{
}
void windowchange()
{
}
void windowlist()
{
}
void windowstop(struct arena* w)
{
}
void windowstart(struct arena* w)
{
}
void windowdelete(struct arena* w)
{
	if(_fbo_ == w->fmt)
	{
		glDeleteRenderbuffers(1, &w->rbo);
		glDeleteFramebuffers(1, &w->fbo);
	}
	else
	{
		glfwDestroyWindow(w->win);
	}
}
void windowcreate(struct arena* w)
{
	if(_fbo_ == w->fmt)
	{
		w->fbwidth = w->width = 1024;
		w->fbheight = w->height = 1024;
		w->fbdepth = w->depth = 1024;
		w->fbstride = w->stride = 1024;


		//frame buffer
		glGenFramebuffers(1, &w->fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, w->fbo);

/*
		//render buffer?
		glGenRenderbuffers(1, &w->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, w->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, w->rbo);
*/

		//depth buffer
		glGenTextures(1, &w->tex_depth);
		glBindTexture(GL_TEXTURE_2D, w->tex_depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, w->tex_depth, 0);


		//color buffer
		glGenTextures(1, &w->tex_color);
		glBindTexture(GL_TEXTURE_2D, w->tex_color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, w->tex_color, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			say("error@framebuffer!!!\n");
		}
	}
	else
	{
		w->width = 512;
		w->height = 512;
		w->depth = 512;
		w->stride = 512;

		w->fmt = _vbo_;
		if(_win_ == w->type)windowopen(0, w);
	}
}




void freewindow()
{
	glfwTerminate();
}
void initwindow()
{
	if(glfwInit() == 0)
	{
		printf("error@glfwInit\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetJoystickCallback(callback_joystick);
	threadcreate(thread_joystick, 0);
}
