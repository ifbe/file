#define QWORD unsigned long long
#define DWORD unsigned int
void double2decimalstring(double,char*);




static DWORD* screen=0;
static int width=0;
static int height=0;




static const unsigned char asciitable[128*16]={
0x00,0x00,0x3E,0x63,0x63,0x63,0x6B,0x6B,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x00
0x00,0x00,0x0C,0x1C,0x3C,0x0C,0x0C,0x0C,
0x0C,0x0C,0x0C,0x3F,0x00,0x00,0x00,0x00,	//0x01
0x00,0x00,0x3E,0x63,0x03,0x06,0x0C,0x18,
0x30,0x61,0x63,0x7F,0x00,0x00,0x00,0x00,	//0x02
0x00,0x00,0x3E,0x63,0x03,0x03,0x1E,0x03,
0x03,0x03,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x03
0x00,0x00,0x06,0x0E,0x1E,0x36,0x66,0x66,
0x7F,0x06,0x06,0x0F,0x00,0x00,0x00,0x00,	//0x04
0x00,0x00,0x7F,0x60,0x60,0x60,0x7E,0x03,
0x03,0x63,0x73,0x3E,0x00,0x00,0x00,0x00,	//0x05
0x00,0x00,0x1C,0x30,0x60,0x60,0x7E,0x63,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x06
0x00,0x00,0x7F,0x63,0x03,0x06,0x06,0x0C,
0x0C,0x18,0x18,0x18,0x00,0x00,0x00,0x00,	//0x07
0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x63,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x08
0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x3F,
0x03,0x03,0x06,0x3C,0x00,0x00,0x00,0x00,	//0x09
0x00,0x00,0x08,0x1C,0x36,0x63,0x63,0x63,
0x7F,0x63,0x63,0x63,0x00,0x00,0x00,0x00,	//0x0a
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x33,
0x33,0x33,0x33,0x7E,0x00,0x00,0x00,0x00,	//0x0b
0x00,0x00,0x1E,0x33,0x61,0x60,0x60,0x60,
0x60,0x61,0x33,0x1E,0x00,0x00,0x00,0x00,	//0x0c
0x00,0x00,0x7C,0x36,0x33,0x33,0x33,0x33,
0x33,0x33,0x36,0x7C,0x00,0x00,0x00,0x00,	//0x0d
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,
0x30,0x31,0x33,0x7F,0x00,0x00,0x00,0x00,	//0x0e
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,
0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,	//0x0f
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x10
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x11
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x12
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x13
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x14
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x15
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x16
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x17
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x18
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x19
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x1a
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x1b
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x1c
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x1d
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x1e
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,	//0x1f
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x20
0x00,0x00,0x18,0x3C,0x3C,0x3C,0x18,0x18,
0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,	//0x21 !
0x00,0x63,0x63,0x63,0x22,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x22 "
0x00,0x00,0x00,0x36,0x36,0x7F,0x36,0x36,
0x36,0x7F,0x36,0x36,0x00,0x00,0x00,0x00,	//0x23 #
0x0C,0x0C,0x3E,0x63,0x61,0x60,0x3E,0x03,
0x03,0x43,0x63,0x3E,0x0C,0x0C,0x00,0x00,	//0x24 $
0x00,0x00,0x00,0x00,0x00,0x61,0x63,0x06,
0x0C,0x18,0x33,0x63,0x00,0x00,0x00,0x00,	//0x25 %
0x00,0x00,0x00,0x1C,0x36,0x36,0x1C,0x3B,
0x6E,0x66,0x66,0x3B,0x00,0x00,0x00,0x00,	//0x26 &
0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x27 '
0x00,0x00,0x0C,0x18,0x18,0x30,0x30,0x30,
0x30,0x18,0x18,0x0C,0x00,0x00,0x00,0x00,	//0x28 (
0x00,0x00,0x18,0x0C,0x0C,0x06,0x06,0x06,
0x06,0x0C,0x0C,0x18,0x00,0x00,0x00,0x00,	//0x29 )
0x00,0x00,0x00,0x00,0x42,0x66,0x3C,0xFF,
0x3C,0x66,0x42,0x00,0x00,0x00,0x00,0x00,	//0x2a *
0x00,0x00,0x00,0x00,0x18,0x18,0x18,0xFF,
0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,	//0x2b +
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00,	//0x2c ,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x2d -
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,	//0x2e .
0x00,0x00,0x01,0x03,0x07,0x0E,0x1C,0x38,
0x70,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,	//0x2f /
0x00,0x00,0x3E,0x63,0x63,0x63,0x6B,0x6B,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x30 0
0x00,0x00,0x0C,0x1C,0x3C,0x0C,0x0C,0x0C,
0x0C,0x0C,0x0C,0x3F,0x00,0x00,0x00,0x00,	//0x31 1
0x00,0x00,0x3E,0x63,0x03,0x06,0x0C,0x18,
0x30,0x61,0x63,0x7F,0x00,0x00,0x00,0x00,	//0x32 2
0x00,0x00,0x3E,0x63,0x03,0x03,0x1E,0x03,
0x03,0x03,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x33 3
0x00,0x00,0x06,0x0E,0x1E,0x36,0x66,0x66,
0x7F,0x06,0x06,0x0F,0x00,0x00,0x00,0x00,	//0x34 4
0x00,0x00,0x7F,0x60,0x60,0x60,0x7E,0x03,
0x03,0x63,0x73,0x3E,0x00,0x00,0x00,0x00,	//0x35 5
0x00,0x00,0x1C,0x30,0x60,0x60,0x7E,0x63,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x36 6
0x00,0x00,0x7F,0x63,0x03,0x06,0x06,0x0C,
0x0C,0x18,0x18,0x18,0x00,0x00,0x00,0x00,	//0x37 7
0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x63,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x38 8
0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x3F,
0x03,0x03,0x06,0x3C,0x00,0x00,0x00,0x00,	//0x39 9
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,
0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,	//0x3a :
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,
0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00,	//0x3b //
0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,
0x30,0x18,0x0C,0x06,0x00,0x00,0x00,0x00,	//0x3c <
0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,
0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,	//0x3d =
0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,
0x0C,0x18,0x30,0x60,0x00,0x00,0x00,0x00,	//0x3e >
0x00,0x00,0x3E,0x63,0x63,0x06,0x0C,0x0C,
0x0C,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,	//0x3f ?
0x00,0x00,0x3E,0x63,0x63,0x6F,0x6B,0x6B,
0x6E,0x60,0x60,0x3E,0x00,0x00,0x00,0x00,	//0x40 @
0x00,0x00,0x08,0x1C,0x36,0x63,0x63,0x63,
0x7F,0x63,0x63,0x63,0x00,0x00,0x00,0x00,	//0x41 A
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x33,
0x33,0x33,0x33,0x7E,0x00,0x00,0x00,0x00,	//0x42 B
0x00,0x00,0x1E,0x33,0x61,0x60,0x60,0x60,
0x60,0x61,0x33,0x1E,0x00,0x00,0x00,0x00,	//0x43 C
0x00,0x00,0x7C,0x36,0x33,0x33,0x33,0x33,
0x33,0x33,0x36,0x7C,0x00,0x00,0x00,0x00,	//0x44 D
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,
0x30,0x31,0x33,0x7F,0x00,0x00,0x00,0x00,	//0x45 E
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,
0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,	//0x46 F
0x00,0x00,0x1E,0x33,0x61,0x60,0x60,0x6F,
0x63,0x63,0x37,0x1D,0x00,0x00,0x00,0x00,	//0x47 G
0x00,0x00,0x63,0x63,0x63,0x63,0x7F,0x63,
0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,	//0x48 H
0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,	//0x49 I
0x00,0x00,0x0F,0x06,0x06,0x06,0x06,0x06,
0x06,0x66,0x66,0x3C,0x00,0x00,0x00,0x00,	//0x4a J
0x00,0x00,0x73,0x33,0x36,0x36,0x3C,0x36,
0x36,0x33,0x33,0x73,0x00,0x00,0x00,0x00,	//0x4b K
0x00,0x00,0x78,0x30,0x30,0x30,0x30,0x30,
0x30,0x31,0x33,0x7F,0x00,0x00,0x00,0x00,	//0x4c L
0x00,0x00,0x63,0x77,0x7F,0x6B,0x63,0x63,
0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,	//0x4d M
0x00,0x00,0x63,0x63,0x73,0x7B,0x7F,0x6F,
0x67,0x63,0x63,0x63,0x00,0x00,0x00,0x00,	//0x4e N
0x00,0x00,0x1C,0x36,0x63,0x63,0x63,0x63,
0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x00,	//0x4f O
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x30,
0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,	//0x50 P
0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,
0x63,0x6B,0x6F,0x3E,0x06,0x07,0x00,0x00,	//0x51 Q
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x36,
0x36,0x33,0x33,0x73,0x00,0x00,0x00,0x00,	//0x52 R
0x00,0x00,0x3E,0x63,0x63,0x30,0x1C,0x06,
0x03,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x53 S
0x00,0x00,0xFF,0xDB,0x99,0x18,0x18,0x18,
0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,	//0x54 T
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x55 U
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,
0x63,0x36,0x1C,0x08,0x00,0x00,0x00,0x00,	//0x56 V
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x6B,
0x6B,0x7F,0x36,0x36,0x00,0x00,0x00,0x00,	//0x57 W
0x00,0x00,0xC3,0xC3,0x66,0x3C,0x18,0x18,
0x3C,0x66,0xC3,0xC3,0x00,0x00,0x00,0x00,	//0x58 X
0x00,0x00,0xC3,0xC3,0xC3,0x66,0x3C,0x18,
0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,	//0x59 Y
0x00,0x00,0x7F,0x63,0x43,0x06,0x0C,0x18,
0x30,0x61,0x63,0x7F,0x00,0x00,0x00,0x00,	//0x5a Z
0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,
0x30,0x30,0x30,0x3C,0x00,0x00,0x00,0x00,	//0x5b [
0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,
0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00,	//0x5c backslash
0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,
0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00,0x00,	//0x5d ]
0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x5e ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,	//0x5f _
0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x60 `
0x00,0x00,0x00,0x00,0x00,0x3C,0x46,0x06,
0x3E,0x66,0x66,0x3B,0x00,0x00,0x00,0x00,	//0x61 a
0x00,0x00,0x70,0x30,0x30,0x3C,0x36,0x33,
0x33,0x33,0x33,0x6E,0x00,0x00,0x00,0x00,	//0x62 b
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x60,
0x60,0x60,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x63 c
0x00,0x00,0x0E,0x06,0x06,0x1E,0x36,0x66,
0x66,0x66,0x66,0x3B,0x00,0x00,0x00,0x00,	//0x64 d
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x63,
0x7E,0x60,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x65 e
0x00,0x00,0x1C,0x36,0x32,0x30,0x7C,0x30,
0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,	//0x66 f
0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,
0x66,0x66,0x3E,0x06,0x66,0x3C,0x00,0x00,	//0x67 g
0x00,0x00,0x70,0x30,0x30,0x36,0x3B,0x33,
0x33,0x33,0x33,0x73,0x00,0x00,0x00,0x00,	//0x68 h
0x00,0x00,0x0C,0x0C,0x00,0x1C,0x0C,0x0C,
0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00,	//0x69 i
0x00,0x00,0x06,0x06,0x00,0x0E,0x06,0x06,
0x06,0x06,0x06,0x66,0x66,0x3C,0x00,0x00,	//0x6a j
0x00,0x00,0x70,0x30,0x30,0x33,0x33,0x36,
0x3C,0x36,0x33,0x73,0x00,0x00,0x00,0x00,	//0x6b k
0x00,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x0C,
0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00,	//0x6c l
0x00,0x00,0x00,0x00,0x00,0x6E,0x7F,0x6B,
0x6B,0x6B,0x6B,0x6B,0x00,0x00,0x00,0x00,	//0x6d m
0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,
0x33,0x33,0x33,0x33,0x00,0x00,0x00,0x00,	//0x6e n
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x63,
0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x6f o
0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,
0x33,0x33,0x3E,0x30,0x30,0x78,0x00,0x00,	//0x70 p
0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,
0x66,0x66,0x3E,0x06,0x06,0x0F,0x00,0x00,	//0x71 q
0x00,0x00,0x00,0x00,0x00,0x6E,0x3B,0x33,
0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,	//0x72 r
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x38,
0x0E,0x03,0x63,0x3E,0x00,0x00,0x00,0x00,	//0x73 s
0x00,0x00,0x08,0x18,0x18,0x7E,0x18,0x18,
0x18,0x18,0x1B,0x0E,0x00,0x00,0x00,0x00,	//0x74 t
0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,
0x66,0x66,0x66,0x3B,0x00,0x00,0x00,0x00,	//0x75 u
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x36,
0x36,0x1C,0x1C,0x08,0x00,0x00,0x00,0x00,	//0x76 v
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,
0x6B,0x6B,0x7F,0x36,0x00,0x00,0x00,0x00,	//0x77 w
0x00,0x00,0x00,0x00,0x00,0x63,0x36,0x1C,
0x1C,0x1C,0x36,0x63,0x00,0x00,0x00,0x00,	//0x78 x
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,
0x63,0x63,0x3F,0x03,0x06,0x3C,0x00,0x00,	//0x79 y
0x00,0x00,0x00,0x00,0x00,0x7F,0x66,0x0C,
0x18,0x30,0x63,0x7F,0x00,0x00,0x00,0x00,	//0x7a z
0x00,0x00,0x0E,0x18,0x18,0x18,0x70,0x18,
0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x00,	//0x7b {
0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x00,
0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,	//0x7c |
0x00,0x00,0x70,0x18,0x18,0x18,0x0E,0x18,
0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00,	//0x7d }
0x00,0x00,0x3B,0x6E,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//0x7e ~
0x00,0x70,0xD8,0xD8,0x70,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00		//0x7f DEL
};




void printascii(DWORD x1y1z1,char ch,DWORD fgcolor,DWORD bgcolor)
{
	int x,y;
	int xxxx,yyyy;
	unsigned char temp;
	unsigned char* points=(unsigned char*)&asciitable;

	if(ch<0x20)ch=0x20;
	points+=ch<<4;
	xxxx=x1y1z1&0xffff;
	yyyy=(x1y1z1>>16)&0xffff;

	for(y=0;y<16;y++)
	{
		temp=points[0];
		points++;

		for(x=0;x<8;x++)
		{
			if( (temp&0x80) != 0 )
			{
				screen[ ( (yyyy+y) * width ) + (xxxx+x)] = fgcolor;
			}
			else
			{
				screen[ ( (yyyy+y) * width ) + (xxxx+x)] = bgcolor;
			}
			temp<<=1;
		}
	}
}
void colorascii(int xxxx,int yyyy,char ch,unsigned int color)
{
	int x,y;
	unsigned char temp;
	unsigned char* points=(unsigned char*)&asciitable;

	if(ch<0x20)ch=0x20;
	points+=ch<<4;
	xxxx<<=3;
	yyyy<<=4;

	for(y=0;y<16;y++)
	{
		temp=points[0];
		points++;

		for(x=0;x<8;x++)
		{
			if( (temp&0x80) != 0 )
			{
				screen[ ( (yyyy+y) * width ) + (xxxx+x)] = color;
			}
			temp<<=1;
		}
	}
}
void ascii(int x,int y,unsigned char ch)
{
	colorascii(x,y,ch,0);
}




void printstring(DWORD x1y1z1,char* p,DWORD fgcolor,DWORD bgcolor)
{
	int x=(x1y1z1 & 0xffff)/8;
	while(1)
	{
		if( *p == 0x0 )break;
		if(x>=0x80)break;

		printascii( x1y1z1+(x*8) , *p , fgcolor , bgcolor );
		x++;
		p++;
	}
}
void colorstring(int x,int y,char* p,unsigned int color)
{
	while(1)
	{
		if( *p == 0x0 )break;
		if(x>=0x80)break;

		colorascii(x,y,*p,color);
		p++;
		x++;
	}
}
void string(int x,int y,char* p)
{
	colorstring(x,y,p,0);
}




void colordecimal(int x,int y,int z,unsigned int color)
{
	char ch;
	int i;
	long long temp;

	if(z<0)
	{
		colorascii(x,y,'-',color);
		x++;
		z=-z;
	}

	i=0;
	temp=z;
	while(1)
	{
		if(temp<10)break;
		temp=temp/10;
		i++;
	}

	for(;i>=0;i--)
	{
		ch=(char)(z%10);
		if(ch<=9)ch+=0x30;
		else if(ch<=0xf)ch+=0x37;
		colorascii(x+i,y,ch,color);
		z=z/10;
	}
}
void decimal(int x,int y,long long z)
{
	colordecimal(x,y,z,0);
}




void colorhexadecimal(int x,int y,unsigned long long z,unsigned int color)
{
	char ch;
	int i=0;

	unsigned long long temp=z;
	while(1)
	{
		if(temp<0x10)break;
		temp=temp>>4;
		i++;
	}
	for(;i>=0;i--)
	{
		ch=(char)(z&0x0000000f);
		if(ch<=9)ch+=0x30;
		else if(ch<=0xf)ch+=0x37;
		colorascii(x+i,y,ch,color);
		z=z>>4;
	}
}
void hexadecimal(int x,int y,unsigned long long z)
{
	colorhexadecimal(x,y,z,0);
}
void hexadecimal1234(int x,int y,unsigned int z)
{
	char fullbyte,ch;
	int i;
	for(i=0;i<4;i++)
	{
		fullbyte=z&0xff;
		z=z>>8;
		//if(fullbyte != 0)
		//{
			//�߰��ֽ�
			ch=(fullbyte>>4)&0xf;
			ch+=0x30;
			if(ch>0x39)ch+=0x7;
			colorascii(x+2*i,y,ch,0);

			//�Ͱ��ֽ�
			ch=fullbyte&0xf;
			ch+=0x30;
			if(ch>0x39)ch+=0x7;
			colorascii(x+2*i+1,y,ch,0);
		//}
	}
}




void printdouble(int x,int y,double z)
{
	char mystring[100];
	double2decimalstring(z,mystring);
	colorstring(x,y,mystring,0xffffff);
}




void ascii_start(DWORD size,void* addr)
{
	//how
	width=size&0xffff;
	height=(size>>16)&0xffff;

	//where
	screen=addr;
}
void ascii_stop()
{
}
void ascii_init()
{
}
void ascii_kill()
{
}
