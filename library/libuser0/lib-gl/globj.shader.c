#include <stdio.h>
#include <stdlib.h>
#include "libuser.h"

#ifdef __ANDROID__
	#include <jni.h>
	#include <errno.h>
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES3/gl32.h>
	#include <GLES3/gl3ext.h>
	#include <android/log.h>
	#include <android_native_app_glue.h>
	char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 colour;\n"
		"in mediump vec2 texuvw;\n"
		"out mediump vec4 FragColor;\n"
		"uniform sampler2D tex0;\n"
		"void main(){\n"
			"FragColor = vec4(colour, 1.0)*texture(tex0, texuvw).aaaa;\n"
		"}\n"
	};
#else
	#include <GL/glew.h>
	char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 colour;\n"
		"in mediump vec2 texuvw;\n"
		"out mediump vec4 FragColor;\n"
		"uniform sampler2D tex0;\n"
		"void main(){\n"
			"FragColor = vec4(colour, 1.0)*texture(tex0, texuvw).rrrr;\n"
		"}\n"
	};
#endif

char font3dvert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"layout(location = 2)in mediump vec2 t;\n"
"out mediump vec3 colour;\n"
"out mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"texuvw = t;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char font2dvert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"layout(location = 2)in mediump vec2 t;\n"
"out mediump vec3 colour;\n"
"out mediump vec2 texuvw;\n"
"void main(){\n"
	"colour = c;\n"
	"texuvw = t;\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";




char easy2d_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";

char easy2d_frag[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";




char simplevert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char simplefrag[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";




char glsl2d_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"out mediump vec3 vertex;\n"
"out mediump vec3 normal;\n"
"void main(){\n"
	"vertex = v;\n"
	"colour = c;\n"
	"normal = vec3(0.0, 0.0, -1.0);\n"
	"gl_Position = vec4(vertex,1.0);\n"
"}\n";

char glsl2d_frag[] =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"in mediump vec3 normal;\n"
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"mediump vec3 sunpos = vec3(1.0, 1.0, -2.0);\n"
"mediump vec3 campos = vec3(0.0, 0.0, -1.0);\n"

"mediump vec3 LA = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LD = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LS = vec3(1.0, 1.0, 1.0);\n"
//"mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);\n"
//"mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);\n"
"mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);\n"

"vec3 blinnphong(){\n"
	"mediump vec3 KD = colour;\n"

	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(sunpos - vertex);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(campos - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 89.6);\n"
"}\n"
"void main(){\n"
	"mediump vec3 KA = colour;\n"
	"mediump vec3 c = LA*KA + blinnphong();\n"
	"c = vec3(clamp(c.x, 0.0, 1.0), clamp(c.y, 0.0, 1.0), clamp(c.z, 0.0, 1.0));\n"
	"FragColor = vec4(c, 1.0);\n"
"}\n";




char prettyvert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"layout(location = 2)in mediump vec3 n;\n"
"out mediump vec3 normal;\n"
"out mediump vec3 vertex;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"vertex = v;\n"
	"normal = n;\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(vertex,1.0);\n"
"}\n";

char prettyfrag[] =
GLSL_VERSION
"in mediump vec3 vertex;\n"
"in mediump vec3 normal;\n"
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"uniform mediump vec3 camxyz;\n"

"mediump vec3 sunxyz = vec3(1000000.0, 1000000.0, 1000000.0);\n"
"mediump vec3 dirsun0 = vec3(-1.0, 0.0, 0.0);\n"
"mediump vec3 dirsun1 = vec3(0.0, -1.0, 0.0);\n"

"mediump vec3 LA = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LD = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 LS = vec3(1.0, 1.0, 1.0);\n"
//"mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);\n"
//"mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);\n"
"mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);\n"

"vec3 blinnphong(){\n"
	"mediump vec3 KD = colour;\n"

	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(sunxyz - vertex);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 89.6);\n"
"}\n"
"vec3 sun0(){\n"
	"mediump vec3 KD = colour;\n"

	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun0);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 89.6);\n"
"}\n"
"vec3 sun1(){\n"
	"mediump vec3 KD = colour;\n"

	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun1);\n"
	"mediump float SN = dot(N, L);\n"
	"mediump vec3 ret = LD*KD*max(SN, 0.0);\n"
	"if(SN < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = max(dot(N, H), 0.0);\n"
	"return ret + LS*KS*pow(NH, 89.6);\n"
"}\n"
//"float shadow(){\n"
	//"if(uvw.z - texture(tex0, uvw.xy).r > 0.000001)return 0.1;\n"
	//"return 1.0;\n"
//"}\n"
"void main(){\n"
	"mediump vec3 KA = colour;\n"
	"mediump vec3 light = LA*KA;\n"
	"light += blinnphong() / 3.0;\n"
	"light += sun0() / 3.0;\n"
	"light += sun1() / 3.0;\n"
	"light = vec3(clamp(light.x, 0.0, 1.0), clamp(light.y, 0.0, 1.0), clamp(light.z, 0.0, 1.0));\n"
	"FragColor = vec4(light, 1.0);\n"
"}\n";




char opaque2d_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec4 c;\n"
"out mediump vec4 colour;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = vec4(v, 1.0);\n"
"}\n";

char opaque2d_frag[] =
GLSL_VERSION
"in mediump vec4 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = colour;\n"
"}\n";




char opaquevert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 n;\n"
"layout(location = 2)in mediump vec4 c;\n"
"out mediump vec4 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

char opaquefrag[] =
GLSL_VERSION
"in mediump vec4 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = colour;\n"
"}\n";




GLuint compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	if(!shader)return 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (GL_TRUE == compileStatus)return shader;

	GLint infoLogLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength)
	{
		char* infoLog = (char*)malloc(infoLogLength);
		if (infoLog)
		{
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
			say("Could not compile shader %d:\n%s", type, infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
	}
	return 0;
}
GLuint shaderprogram(void* v, void* f, void* g, void* tc, void* te, void* c)
{
	GLuint vShader = compileShader(GL_VERTEX_SHADER, v);
	if(!vShader){
		say("fail@compileShader: %s\n", v);
		return 0;
	}

	GLuint fShader = compileShader(GL_FRAGMENT_SHADER, f);
	if(!fShader){
		say("fail@compileShader: %s\n", f);
		return 0;
	}

	GLuint gShader = 0;
	if(g){
		gShader = compileShader(GL_GEOMETRY_SHADER, g);
		if(!gShader)say("fail@compileShader: %s\n", g);
	}
/*
	GLuint tcShader = 0;
	if(tc){
		compileShader(GL_TESS_CONTROL_SHADER, tc);
		if(!tcShader)say("fail@compileShader: %s\n", tc);
	}

	GLuint teShader = 0;
	if(te){
		compileShader(GL_TESS_EVALUATION_SHADER, te);
		if(!teShader)say("fail@compileShader: %s\n", te);
	}

	GLuint cShader = 0;
	if(c){
		compileShader(GL_COMPUTE_SHADER, c);
		if(!cShader)say("fail@compileShader: %s\n", c);
	}
*/

	//3.glsl program
	GLuint prog = glCreateProgram();
	if(0 == prog)
	{
		printf("ERROR : create program failed");
		exit(1);
	}

	glAttachShader(prog, vShader);
	glAttachShader(prog, fShader);
	if(gShader)glAttachShader(prog, gShader);
	//if(tcShader)glAttachShader(prog, tcShader);
	//if(teShader)glAttachShader(prog, teShader);
	//if( cShader)glAttachShader(prog,  cShader);
	glLinkProgram(prog);

	GLint linkStatus;
	glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
	if(GL_TRUE == linkStatus)return prog;

	printf("ERROR : link shader program failed");
	GLint logLen;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
	if(logLen > 0)
	{
		char *log = (char*)malloc(logLen);
		GLsizei written;
		glGetProgramInfoLog(prog, logLen, &written, log);
		printf("Program log :%s\n", log);
	}

	glDeleteProgram(prog);
	return 0;
}
void initshader(struct arena* win)  
{
	int j;
	GLuint tmp;
	struct datapair* mod = win->mod;

	//1.check version
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor: %s\n", vendor);
	printf("GL Renderer: %s\n", renderer);
	printf("GL Version (string): %s\n", version);
	printf("GLSL Version: %s\n", glslVersion);
	printf("GL Version (integer): %x.%x\n", major, minor);




	tmp = shaderprogram(font3dvert, fontfrag, 0, 0, 0, 0);
	mod[font3d0].dst.shader = tmp;
	mod[font3d1].dst.shader = tmp;
	mod[font3d2].dst.shader = tmp;
	mod[font3d3].dst.shader = tmp;

	tmp = shaderprogram(font2dvert, fontfrag, 0, 0, 0, 0);
	mod[font2d0].dst.shader = tmp;
	mod[font2d1].dst.shader = tmp;
	mod[font2d2].dst.shader = tmp;
	mod[font2d3].dst.shader = tmp;




	tmp = shaderprogram(simplevert, simplefrag, 0, 0, 0, 0);
	mod[point3d].dst.shader = tmp;
	mod[line3d].dst.shader = tmp;

	tmp = shaderprogram(prettyvert, prettyfrag, 0, 0, 0, 0);
	mod[trigon3d].dst.shader = tmp;

	tmp = shaderprogram(opaquevert, opaquefrag, 0, 0, 0, 0);
	mod[opaque3d].dst.shader = tmp;




	tmp = shaderprogram(easy2d_vert, easy2d_frag, 0, 0, 0, 0);
	mod[point2d].dst.shader = tmp;
	mod[line2d].dst.shader = tmp;

	tmp = shaderprogram(glsl2d_vert, glsl2d_frag, 0, 0, 0, 0);
	mod[trigon2d].dst.shader = tmp;

	tmp = shaderprogram(opaque2d_vert, opaque2d_frag, 0, 0, 0, 0);
	mod[opaque2d].dst.shader = tmp;

	for(j=0;j<16;j++)
	{
		mod[j].src.tex_name[0] = "tex0";
	}
}
