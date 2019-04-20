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

void fixmatrix(float*, void*);
GLuint shaderprogram(void* v, void* f, void* g, void* tc, void* te, void* c);
GLuint uploadtexture(void* i, u32 t, void* buf, int fmt, int w, int h);
GLuint uploadvertex(void* i, void* o);




void display_eachpass(
	struct gldst* dst, struct glsrc* src,
	struct arena* win, float* cammvp)
{
	int j;
	u32 tmp;
	u32 fmt;
	u32 vbo;
	u32 vao;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	if(src->opaque)
	{
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	//0.shader
	glUseProgram(dst->shader);

	//1.argument
	glUniformMatrix4fv(glGetUniformLocation(dst->shader, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform3fv(glGetUniformLocation(dst->shader, "camxyz"  ), 1, win->camera.vc);
	//glUniformMatrix4fv(glGetUniformLocation(dst->shader, "sunmvp"), 1, GL_FALSE, cammvp);
	//glUniform3fv(glGetUniformLocation(dst->shader, "sunxyz"  ), 1, win->camera.vc);
	for(j=0;j<4;j++){
		if(0 == src->arg_name[j])continue;
		if(0 == src->arg_data[j])continue;

		tmp = glGetUniformLocation(dst->shader, src->arg_name[j]);
		switch(src->arg_fmt[j]){
			case 'm':{
				glUniformMatrix4fv(tmp, 1, GL_FALSE, src->arg_data[j]);
				break;
			}//mat4

			case 'f':{
				glUniform1fv(tmp, 1, src->arg_data[j]);
				break;
			}
		}//switch
	}//for

	//2.texture
	for(j=0;j<4;j++)
	{
		if(0 == dst->tex[j])continue;
		if(0 == src->tex_name[j])continue;

		glUniform1i(glGetUniformLocation(dst->shader, src->tex_name[j]), j);
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, dst->tex[j]);
	}

	//3.vertex
	fmt = src->vbuf_fmt;
	vbo = dst->vbo;
	vao = dst->vao;
	glBindVertexArray(vao);
//say("%d,%d,%d,%d,%d,%d\n", dst->shader, dst->tex[0], vao, vbo, src->ibuf_h, src->vbuf_h);

	if('i' == src->method)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
		if(1 == src->geometry)glDrawElements(GL_POINTS, src->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else if(2 == src->geometry)glDrawElements(GL_LINES, 2*src->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else glDrawElements(GL_TRIANGLES, 3*src->ibuf_h, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		if(1 == src->geometry)glDrawArrays(GL_POINTS, 0, src->vbuf_h);
		else if(2 == src->geometry)glDrawArrays(GL_LINES, 0, src->vbuf_h);
		else glDrawArrays(GL_TRIANGLES, 0, src->vbuf_h);
	}

	if(src->opaque)
	{
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}
static struct arena* saved;
void hostctx_render(struct arena* this)
{
	int j;
	struct arena* win;
	struct datapair* mod;
	GLfloat cammvp[4*4];

	if(_fbo_ == this->fmt)
	{
		win = saved;
		if(0 == win)return;

		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		glViewport(0, 0, 1024, 1024);
		fixmatrix(cammvp, this);
	}
	else
	{
		win = this;
		if(_vbo_ == this->fmt)saved = this;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, win->fbwidth, win->fbheight);
		fixmatrix(cammvp, win);
	}
	mat4_transpose((void*)cammvp);

	//prepare
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glPointSize(2.0 * win->fbwidth / win->width);
	//glLineWidth(8);

	//else
	mod = win->mod;
	for(j=16;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(&mod[j].dst, &mod[j].src, win, cammvp);
	}
	if(_fbo_ == this->fmt)
	{
		display_eachpass(&mod[trigon3d].dst, &mod[trigon3d].src, win, cammvp);
		return;
	}

	//geom
	for(j=8;j<16;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(&mod[j].dst, &mod[j].src, win, cammvp);
	}

	//font
	for(j=0;j<8;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(&mod[j].dst, &mod[j].src, win, cammvp);
	}
}




void update_eachpass(struct gldst* dst, struct glsrc* src)
{
	int j;
	u32 fd;
	int w,h,fmt;
	void* buf0;
	void* buf1;
	void* buf2;
	//say("%llx,%llx\n", dst, src);

	//0: shader
	if(dst->shader_deq != src->shader_enq[0])
	{
		//say("@1\n");
		buf0 = (void*)(src->vs);
		buf1 = (void*)(src->fs);
		buf2 = (void*)(src->gs);
		if((0 != buf0)&&(0 != buf1))
		{
			fd = shaderprogram(buf0, buf1, buf2, 0, 0, 0);

			dst->shader = fd;
			say("(%llx,%llx)->%x\n", buf0, buf1, fd);
		}

		dst->shader_deq = src->shader_enq[0];
	}

	//2: texture
	for(j=0;j<4;j++){
		if(0 == src->tex_enq[j])continue;

		buf0 = (void*)(src->tex_data[j]);
		if(0 != buf0)
		{
			fmt = src->tex_fmt[j];
			w = src->tex_w[j];
			h = src->tex_h[j];
			fd = uploadtexture(dst, dst->tex[j], buf0, fmt, w, h);

			dst->tex[j] = fd;
			//say("(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, fd);
		}

		dst->tex_deq[j] = src->tex_enq[j];
	}

	//3: vertex
	if(	(dst->vbo_deq != src->vbuf_enq) |
		(dst->ibo_deq != src->ibuf_enq) )
	{
		//say("@4\n");
		uploadvertex(dst, src);
		//say("(%x,%x,%x)\n", dst->vao, dst->vbo, dst->ibo);
		dst->vbo_deq = src->vbuf_enq;
		dst->ibo_deq = src->ibuf_enq;
	}
}
void hostctx_update(struct arena* win)
{
	int j;
	struct datapair* mod;

	//local
	mod = win->mod;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		update_eachpass(&mod[j].dst, &mod[j].src);
	}
}




void hostctx_create(struct arena* win)
{
	int j;
	u8* buf;
	struct datapair* mod;
	struct glsrc* src;

	buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->mod = mod = (void*)buf;


//--------------------font3d-------------------
	//[0000,3fff]
	src = &mod[font3d0].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[4000,7fff]
	src = &mod[font3d1].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[8000,bfff]
	src = &mod[font3d2].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[c000,ffff]
	src = &mod[font3d3].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


//--------------------font2d-------------------
	//[0000,3fff]
	src = &mod[font2d0].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[4000,7fff]
	src = &mod[font2d1].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[8000,bfff]
	src = &mod[font2d2].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//[c000,ffff]
	src = &mod[font2d3].src;

	src->vbuf = memorycreate(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


//--------------------3d-------------------
	//drawarray.point3d
	src = &mod[point3d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->method = 'v';
	src->geometry = 1;

	src->vbuf_enq = 1;


	//drawelement.line3d
	src = &mod[line3d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x22;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0x100000/4;

	src->method = 'i';
	src->geometry = 2;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//drawelement.trigon3d
	src = &mod[trigon3d].src;

	src->vbuf = memorycreate(0x1000000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x1000000/36;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//opaque3d
	src = &mod[opaque3d].src;

	src->vbuf = memorycreate(0x1000000);
	src->vbuf_fmt = vbuffmt_444;
	src->vbuf_w = 4*4*3;
	src->vbuf_h = 0x1000000/48;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


//----------------------2d--------------------
	//drawarray.point2d
	src = &mod[point2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->method = 'v';
	src->geometry = 1;

	src->vbuf_enq = 1;


	//drawelement.line2d
	src = &mod[line2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x22;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0x100000/4;

	src->method = 'i';
	src->geometry = 2;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//drawelement.trigon2d
	src = &mod[trigon2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;


	//opaque2d
	src = &mod[opaque2d].src;

	src->vbuf = memorycreate(0x100000);
	src->vbuf_fmt = vbuffmt_44;
	src->vbuf_w = 4*4*2;
	src->vbuf_h = 0x100000/32;

	src->ibuf = memorycreate(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;
	src->opaque = 1;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
}