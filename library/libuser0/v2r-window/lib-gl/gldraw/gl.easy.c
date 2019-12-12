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
GLuint shaderprogram(void* v, void* f, void* g, void* tc, void* te, void* c);
GLuint uploadvertex(void* i, void* o);




static char vs[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";
static char fs[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";
static GLuint shader = 0;




static float vbuf[6*3] = {
	-0.5,-0.5, 0.0,		1.0, 0.0, 0.0,
	 0.5, 0.0, 0.0,		0.0, 1.0, 0.0,
	 0.0, 0.5, 0.0,		0.0, 0.0, 1.0
};
static GLuint vbo = 0;
static GLuint vao = 0;




void easywindow_renderself()
{
	//shader
	if(0 == shader)shader = shaderprogram(vs, fs, 0, 0, 0, 0);
	glUseProgram(shader);

	if(0 == vao)glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if(0 == vbo){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4*6*3, vbuf, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
		glEnableVertexAttribArray(1);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//draw
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
void easywindow_renderpeer(struct supply* win)
{
	struct relation* rel = win->orel0;
	if(0 == rel)return;

	struct entity* act = (void*)(rel->dstchip);
	if(0 == act)return;

	struct gl41data* pair = act->buf;
	if(0 == pair)return;

	struct glsrc* src = &pair->src;
	struct gldst* dst = &pair->dst;
	//say("src=%llx,dst=%llx\n", src, dst);

	//shader
	if(dst->shader_deq != src->shader_enq)
	{
		dst->shader = shaderprogram(src->vs, src->fs, 0, 0, 0, 0);
		dst->shader_deq = src->shader_enq;
	}
	glUseProgram(dst->shader);
	glUniformMatrix4fv(glGetUniformLocation(dst->shader, "cammvp"), 1, GL_FALSE, src->arg[0].data);

	//vao
	if(0 == dst->vao)glGenVertexArrays(1, &dst->vao);
	glBindVertexArray(dst->vao);

	//vbuf
	if(dst->vbo_deq != src->vbuf_enq)
	{
		glGenBuffers(1, &dst->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
		glBufferData(GL_ARRAY_BUFFER, src->vbuf_len, src->vbuf, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
		glEnableVertexAttribArray(1);

		dst->vbo_deq = src->vbuf_enq;
	}
	glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);

	//ibuf
	if(dst->ibo_deq != src->ibuf_enq)
	{
		glGenBuffers(1, &dst->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, src->ibuf_len, src->ibuf, GL_STATIC_DRAW);

		dst->ibo_deq = src->ibuf_enq;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);

	//draw
	glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_SHORT, (void*)0);
}




void easywindow_write(struct supply* win, struct event* ev)
{
	say("@easywindow_event\n");
}
void easywindow_read(struct supply* win)
{
	float w = win->fbwidth;
	float h = win->fbheight;

	//
	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);

	//clear screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(0 == win->orel0)easywindow_renderself();
	else easywindow_renderpeer(win);
}
void easywindow_delete(struct supply* win)
{
}
void easywindow_create(struct supply* win)
{
	win->fmt = _easy_;
}
