#define BYTE unsigned char
#define QWORD unsigned long long
int systemi2c_list(char* towhere);
int systemi2c_choose(BYTE bus,BYTE dev,BYTE reg);
int systemi2c_read(BYTE dev,BYTE reg,BYTE* buf,BYTE count);
int systemi2c_write(BYTE dev,BYTE reg,BYTE* buf,BYTE count);
int systemi2c_start(char* p);
int systemi2c_stop();




static void i2c_list()
{
	systemi2c_list(0);
}
static void i2c_switch()
{
}
static void i2c_read()
{
}
static void i2c_write()
{
}
static void i2c_start(QWORD type,char* p)
{
	systemi2c_start(p);
}
static void i2c_stop()
{
	systemi2c_stop();
}
void i2c_init(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x633269;		//id
	p[2]=(QWORD)i2c_start;
	p[3]=(QWORD)i2c_stop;
	p[4]=(QWORD)i2c_list;
	p[5]=(QWORD)i2c_switch;
	p[6]=(QWORD)i2c_read;
	p[7]=(QWORD)i2c_write;
}
void i2c_kill()
{
	i2c_stop();
}
