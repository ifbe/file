#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libuser.h"

#ifdef __ANDROID__
	#include <jni.h>
	#include <errno.h>
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <android/log.h>
	#include <android_native_app_glue.h>
#else
	#include <GL/glew.h>
#endif




int fbodelete(struct actor* win)
{
	if(win->tex_color)glDeleteTextures(1, &win->tex_color);
	if(win->tex_depth)glDeleteTextures(1, &win->tex_depth);
	if(win->rbo)glDeleteRenderbuffers(1, &win->rbo);
	if(win->fbo)glDeleteFramebuffers(1, &win->fbo);
	return 0;
}
int fbocreate(struct actor* win, int arg)
{
	//frame buffer
	glGenFramebuffers(1, &win->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, win->fbo);

/*
	//render buffer?
	glGenRenderbuffers(1, &w->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, w->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, w->rbo);
*/

switch(arg){
case 'd':{
	//depth buffer
	glGenTextures(1, &win->tex_depth);
	glBindTexture(GL_TEXTURE_2D, win->tex_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, win->tex_depth, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, win->tex_depth, 0);
#endif
}//c
case 'c':{
	//color buffer
	glGenTextures(1, &win->tex_color);
	glBindTexture(GL_TEXTURE_2D, win->tex_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, win->tex_color, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, win->tex_color, 0);
#endif

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		say("error@framebuffer!!!\n");
	}
}//c
case 'g':{
	//geometry buffer
	glGenTextures(1, &win->tex_color);
	glBindTexture(GL_TEXTURE_2D, win->tex_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, win->tex_color, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, win->tex_color, 0);
#endif

	glGenTextures(1, &win->tex_depth);
	glBindTexture(GL_TEXTURE_2D, win->tex_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, win->tex_depth, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, win->tex_depth, 0);
#endif

	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		say("error@framebuffer!!!\n");
	}
}//g
}//switch
return 0;
}
