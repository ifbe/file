#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char




//world
void birth();
void death();




//event
void eventwrite(int who);
void eventread(u64 what, u64 who, u64 where, u64 when);
#define input_char 0x72616863	//char
#define input_kbd 0x64626b	//kbd
	#define kbd_esc 0x1b
	#define kbd_left 0x25
	#define kbd_up 0x26
	#define kbd_right 0x27
	#define kbd_down 0x28
	#define kbd_f1 0x70
	#define kbd_f2 0x71
	#define kbd_f3 0x72
	#define kbd_f4 0x73
	#define kbd_f5 0x74
	#define kbd_f6 0x75
	#define kbd_f7 0x76
	#define kbd_f8 0x77

#define point_down 0x2b70	//p+
#define point_up 0x2d70		//p-
#define point_move 0x4070	//p@
	#define touch_0 0
	#define touch_1 1
	#define touch_2 2
	#define touch_3 3
	#define touch_4 4
	#define touch_5 5
	#define touch_6 6
	#define touch_7 7
	#define touch_8 8
	#define touch_9 9
	#define btn_left 'l'
	#define btn_right 'r'
	#define btn_middle 'm'
	#define btn_front 'f'
	#define btn_back 'b'
	#define btn_side1 '1'
	#define btn_side2 '2'
	#define btn_side3 '3'
	#define btn_side4 '4'
	#define btn_side5 '5'

#define net_in 0x2b6e		//n+
#define net_out 0x2d6e		//n-
#define net_data 0x406e		//n@

#define drag_file 0x656c6966	//file
#define drag_data 0x61746164	//data

#define screen_size 0x657a6973	//size




//libui1
#define uiguys 0
#define gamedata 0x100000

void backgroundcolor(u32 color);

void line(  int x1, int y1, int x2, int y2, u32 color);
void radial(int x1, int y1, int x2, int y2, u32 color);

void trianglebody( int x1, int y1, int x2, int y2, int x3,int y3, u32 body, u32 frame);
void triangleframe(int x1, int y1, int x2, int y2, int x3,int y3, u32 body, u32 frame);
void triangle(     int x1, int y1, int x2, int y2, int x3,int y3, u32 body, u32 frame);

void rectbody( int x1, int y1, int x2, int y2, u32 body);
void rectframe(int x1, int y1, int x2, int y2, u32 frame);
void rect(     int x1, int y1, int x2, int y2, u32 body, u32 frame);

void circlebody( int x, int y, int r, u32 body);
void circleframe(int x, int y, int r, u32 frame);
void circle(     int x, int y, int r, u32 body, u32 frame);

void printascii(      int x, int y, int size, char ch, u32 fg, u32 bg);
void printbyte(       int x, int y, int size, char ch, u32 fg, u32 bg);
void printstring(     int x, int y, int size, char* s, u32 fg, u32 bg);
void printdecimal(    int x, int y, int size, u64 dat, u32 fg, u32 bg);
void printhexadecimal(int x, int y, int size, u64 dat, u32 fg, u32 bg);
void printfloat(      int x, int y, int size, float dat, u32 fg, u32 bg);
void printdouble(     int x, int y, int size, double dat, u32 fg, u32 bg);

void defaultascii(      int x, int y, u8 ch);
void defaultstring(     int x, int y, u8* s);
void defaulthexadecimal(int x, int y, u64 hex);
void defaultdecimal(    int x, int y, long long dec);
void defaultdouble(     int x, int y, double data);




//libui0
void windowlist();
void windowchange();
void windowread();
void windowwrite();
void windowstart(char* addr, char* fmt, int w, int h);
void windowstop();
void windowcreate();
void windowdelete();




//libsoft1
#define softguys 0
#define fshome 0x100000
	#define memhome fshome+0
	#define nethome fshome+0xd0000
	#define syshome fshome+0xe0000
	#define wirehome fshome+0xf0000
#define dirhome 0x200000
#define datahome 0x300000

int utf2unicode(u8* src,u32* dst);
int unicode2utf(u32 src,u8* dst);

int buf2typename(u8* p,int max,u64* type,u8** name);
int buf2optval(u8* pp,int max,u8** type,u8** name);
int buf2suffix(u8* p,u8** suffix);
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2addrport(u8* pp,int max,u8* addr,int* port);
u8* buf2folder(u8* p);
u8* buf2filename(u8* p);

int decstr2double(u8* src,double* dst);
int hexstr2double(u8* src,double* dst);
int double2decstr(double src,u8* dst);
int double2hexstr(double src,u8* dst);

int data2decstr(u64 data,u8* str);
int data2hexstr(u64 data,u8* str);
int hexstr2data(u8* src,u64* dst);
int decstr2data(u8* src,u64* dst);

int bigint2decstr(u8* src,u8* dst,int count);
int bigint2hexstr(u8* src,u8* dst,int count);
int hexstr2bigint(u8* src, u8* dst);
int decstr2bigint(u8* src, u8* dst);

int cmp(u8*, u8*);
int ncmp(u8*, u8*, int);
int wildcard(u8* wild,u8* str);

int copy(u8* dst, u8* src);		//strcpy
int ncopy(u8* dst, u8* src, int len);	//strncpy
int movsb(u8* dst, u8* src, int len);

int findhead(u8* buf);
int findtail(u8* buf);
int findzero(u8* buf);
int finddata(u8* buf, u8 data);


int bigdup(u8* src, int sl, u8* dst, int dl);
int bigcmp(u8* src, int sl, u8* dst, int dl);
int bigshl(u8* buf, int len, int sh);
int bigshr(u8* buf, int len, int sh);
int bigadd(
	u8* abuf, int alen,
	u8* bbuf, int blen);
int bigadd_muled(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	int val);
int bigsub(
	u8* abuf, int alen,
	u8* bbuf, int blen);
int bigsub_muled(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	int val);
int bigmul(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* answer, int max);
int bigpow(
	u8* base, int bl,
	u8* exp, int el,
	u8* mod, int ml,
	u8* ans, int al,
	u8* t1, int l1);
int bigdiv(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* quotient, int max1,
	u8* remainder, int max2);




//libsoft0
int readfile(u8* file, u8* mem, u64 offset, u64 count);
int writefile(u8* file, u8* mem, u64 offset, u64 count);
u32 getrandom();
u64 gettime();




//libhard1
int initusb();
int initnet();




//libhard0
int initahci(u8* hba);
int initxhci(u8* hba);




//libboot1
void printmemory(char*,int);
void say(char*,...);




//libboot0
void power_off();
void power_on();
