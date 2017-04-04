#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230
#define tau PI*2
//libui1
void line(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void backgroundcolor(void*,
	u32);
//libsoft1
void fft(double* real, double* imag, int k);
void ifft(double* real, double* imag, int k);
double squareroot(double);
double cosine(double);
double sine(double);
double log2(double);
double lg(double);
double ln(double);
int sound_output(double*, double*, u16*);
int piano_freq(int);
//libsoft0
int startsound(int rate, int chan, void* buf, int max);
int stopsound();
u32 getrandom();
//
void printmemory(void*,int);
void say(void*,...);




struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;

	u8 data[0xe0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
//before
static int maxpower;
static u16* pcmin;
static u16* pcmout;

//after
static double* real;
static double* imag;
static double* power;
static double* phase;

//
static int area=0;




static void spectrum_read_pixel(struct window* win)
{
	double t,cc,ss;
	int x,y;
	int width = win->w;
	int height = win->h;

	backgroundcolor(win, 0);
	for(x=0;x<512;x++)
	{
		t = x * tau /512.0;
		cc = cosine(t) * 256;
		ss = -sine(t) * 256;
		line(win,
			256 + (int)(cc * (1.0 - 2*power[x])),
			256 + (int)(ss * (1.0 - 2*power[x])),
			256 + (int)cc,
			256 + (int)ss,
			0xffffff
		);
	}
/*
	for(x=0;x<1024;x++)
	{
		if(pcmin[x]>32768)continue;
		y = pcmin[x] *height /maxpower /4;
		line(win,
			x*width/1024, (height/4) - y,
			x*width/1024, (height/4) + y,
			0xffffff
		);
	}
	for(x=0;x<512;x++)
	{
		y = (int)(power[x]*height);
		line(win,
			x*width/512, height - y,
			x*width/512, height,
			0xffffff
		);
	}
*/
}
static void spectrum_read_html(struct window* win)
{
}
static void spectrum_read_text(struct window* win)
{
	int x,y;
	int w = win->w;
	int h = win->h;
	u8* p = (u8*)(win->buf);

	for(x=0;x<w*h*4;x++)p[x]=0;
	for(x=0;x<w;x++)
	{
		y = h - (int)(real[x] * (double)h / (double)maxpower);
		for(;y<h;y++)
		{
			p[((y*w + x)<<2) + 3] =  0x2;
		}
	}
}
static void spectrum_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		spectrum_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		spectrum_read_html(win);
	}

	//pixel
	else
	{
		spectrum_read_pixel(win);
	}
}




void spectrum_random()
{
	int j;
	for(j=0;j<1024;j++)
	{
		real[j] = (double)pcmin[j] / 65536.0;
		imag[j] = 0.0;
	}

	fft(real, imag, 10);
	for(j=0;j<1024;j++)
	{
		//say("%lf	%lf\n", real[j], imag[j]);
		power[j]=squareroot(real[j]*real[j] + imag[j]*imag[j]) / 1024;
	}
	//say("%lf,%lf,%lf,%lf\n",power[0],power[1],power[2],power[3]);
}
static void spectrum_write(struct event* ev)
{
	int j;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type==0x72616863)	//'char'
	{
		if( (key>=0x31) && (key<=0x39) )
		{
			key -= 0x31;
		}
		else if(key == '0')key = 9;
		else if(key == '-')key = 10;
		else if(key == '=')key = 11;
		else
		{
			if( (key>='a') && (key<='z') )
			{
				area = key - 'a';
			}
			return;
		}

		for(j=0;j<1024;j++)real[j]=imag[j]=0.0;

		j = piano_freq(area*12+key);
		say("%d->",j);
		j = (j*1024)/44100;
		say("%d\n",j);

		real[j]=real[1023-j]=65535;
		sound_output(real, imag, pcmout);
	}
	else if(type==0x2d6d)
	{
		spectrum_random();
	}
	else if(type=='s')
	{
		//printmemory(pcmin, 16);
		spectrum_random();
	}
}




static void spectrum_list()
{
}
static void spectrum_into()
{
}
void spectrum_start()
{
	int j;
	maxpower = 65536;

	//
	startsound(44100, 2, pcmin, 0x100000);
	spectrum_random();
}
void spectrum_stop()
{
}
void spectrum_create(void* uibuf,void* addr)
{
	struct player* p = addr;
	pcmin=(void*)(uibuf+0x200000);
	pcmout=(void*)(uibuf+0x280000);
	real=(double*)(uibuf+0x300000);
	imag=(double*)(uibuf+0x340000);
	power=(double*)(uibuf+0x380000);
	phase=(double*)(uibuf+0x3c0000);

	p->type = 0x6c6f6f74;
	p->name = 0x6d75727463657073;

	p->start = (u64)spectrum_start;
	p->stop = (u64)spectrum_stop;
	p->list = (u64)spectrum_list;
	p->choose = (u64)spectrum_into;
	p->read = (u64)spectrum_read;
	p->write = (u64)spectrum_write;
}
void spectrum_delete()
{
}
