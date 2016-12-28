#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);
static u8 random_example[] = {
	//0000
	0x4f,0xcc,0x1e,0x94,0xb7,0xe0,0x12,0xae,
	0x88,0x49,0x61,0x3b,0x0a,0xc7,0x5f,0xed,
	//0010
	0x90,0x01,0xcd,0x7d,0xc2,0xa4,0x9d,0xfa,
	0x0c,0x2e,0xe0,0x93,0x56,0xa2,0x02,0x7f
};
static u8 cert[] = {
	//0000
	0x30,0x82,0x02,0x01,0x30,0x82,0x01,0x6a,
	0x02,0x09,0x00,0xc8,0x45,0x6b,0x18,0xf2,
	//0010
	0xc0,0xcd,0x61,0x30,0x0d,0x06,0x09,0x2a,
	0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x0b,
	//0020
	0x05,0x00,0x30,0x45,0x31,0x0b,0x30,0x09,
	0x06,0x03,0x55,0x04,0x06,0x13,0x02,0x41,
	//0030
	0x55,0x31,0x13,0x30,0x11,0x06,0x03,0x55,
	0x04,0x08,0x0c,0x0a,0x53,0x6f,0x6d,0x65,
	//0040
	0x2d,0x53,0x74,0x61,0x74,0x65,0x31,0x21,
	0x30,0x1f,0x06,0x03,0x55,0x04,0x0a,0x0c,
	//0050
	0x18,0x49,0x6e,0x74,0x65,0x72,0x6e,0x65,
	0x74,0x20,0x57,0x69,0x64,0x67,0x69,0x74,
	//0060
	0x73,0x20,0x50,0x74,0x79,0x20,0x4c,0x74,
	0x64,0x30,0x1e,0x17,0x0d,0x31,0x36,0x31,
	//0070
	0x32,0x30,0x36,0x31,0x31,0x35,0x31,0x32,
	0x31,0x5a,0x17,0x0d,0x31,0x37,0x31,0x32,
	//0080
	0x30,0x36,0x31,0x31,0x35,0x31,0x32,0x31,
	0x5a,0x30,0x45,0x31,0x0b,0x30,0x09,0x06,
	//0090
	0x03,0x55,0x04,0x06,0x13,0x02,0x41,0x55,
	0x31,0x13,0x30,0x11,0x06,0x03,0x55,0x04,
	//00a0
	0x08,0x0c,0x0a,0x53,0x6f,0x6d,0x65,0x2d,
	0x53,0x74,0x61,0x74,0x65,0x31,0x21,0x30,
	//00b0
	0x1f,0x06,0x03,0x55,0x04,0x0a,0x0c,0x18,
	0x49,0x6e,0x74,0x65,0x72,0x6e,0x65,0x74,
	//00c0
	0x20,0x57,0x69,0x64,0x67,0x69,0x74,0x73,
	0x20,0x50,0x74,0x79,0x20,0x4c,0x74,0x64,
	//00d0
	0x30,0x81,0x9f,0x30,0x0d,0x06,0x09,0x2a,
	0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01,
	//00e0
	0x05,0x00,0x03,0x81,0x8d,0x00,0x30,0x81,
	0x89,0x02,0x81,0x81,0x00,0xbd,0xe2,0x62,
	//00f0
	0x6a,0xee,0x52,0x8b,0xa6,0x9d,0x04,0xa6,
	0x1e,0x65,0x42,0x47,0x1b,0x74,0x62,0x99,
	//0100
	0x83,0xd6,0x29,0xba,0x81,0x82,0x99,0x2f,
	0xb0,0xed,0x60,0x70,0x11,0x8e,0x3d,0xe8,
	//0110
	0x4b,0x90,0x21,0xe2,0xb1,0xf5,0x3a,0x93,
	0xe9,0x9e,0x7a,0x03,0x35,0x17,0x9b,0x1a,
	//0120
	0x84,0x31,0x02,0x11,0xb4,0x58,0x24,0xa5,
	0xdd,0x1c,0xb8,0x6e,0x08,0x37,0x89,0xac,
	//0130
	0x26,0xc4,0x48,0xbb,0x65,0x08,0x01,0x82,
	0x6c,0x3c,0x65,0x30,0x89,0xbc,0xed,0x3e,
	//0140
	0x63,0xd8,0x27,0x2a,0xc3,0x98,0x5f,0xb1,
	0xe1,0x36,0x9c,0x73,0xc8,0xc2,0xb0,0x9a,
	//0150
	0x0f,0x3e,0x87,0x5b,0xa1,0xe1,0x46,0xcb,
	0x6b,0x74,0xc5,0x96,0xe0,0x32,0xbc,0x45,
	//0160
	0x66,0xab,0x13,0xde,0x06,0xe7,0x98,0xbb,
	0xef,0x5a,0x62,0xb4,0xcd,0x02,0x03,0x01,
	//0170
	0x00,0x01,0x30,0x0d,0x06,0x09,0x2a,0x86,
	0x48,0x86,0xf7,0x0d,0x01,0x01,0x0b,0x05,
	//0180
	0x00,0x03,0x81,0x81,0x00,0x77,0x64,0x1b,
	0x41,0xd9,0xe0,0x89,0x59,0x9f,0x77,0x45,
	//0190
	0xb5,0x0e,0xa3,0xcd,0x9e,0x30,0x48,0x44,
	0x27,0x3b,0xff,0x44,0x3d,0xe9,0x0c,0x99,
	//01a0
	0xd8,0xcb,0x92,0x1c,0x79,0x71,0xa4,0x0d,
	0x0c,0x50,0x68,0xfe,0x70,0x39,0x14,0x46,
	//01b0
	0x99,0x0f,0xbd,0x0e,0xec,0x41,0x45,0x9d,
	0xeb,0xd5,0xf3,0xe7,0x9d,0x32,0xac,0x28,
	//01c0
	0x41,0xc6,0xcf,0xd9,0x4f,0xd2,0xb7,0xf5,
	0xa7,0xcf,0x35,0x62,0x10,0x9c,0xd4,0xd2,
	//01d0
	0x75,0x7a,0x3d,0xf4,0xd1,0x09,0x24,0x80,
	0xe9,0xaf,0x58,0x71,0xc1,0x0f,0x27,0x61,
	//01e0
	0x30,0xf3,0xd8,0xe9,0x92,0x80,0xeb,0x8b,
	0xce,0x04,0x79,0x2b,0x56,0x62,0x86,0xc0,
	//01f0
	0x53,0x63,0x18,0x54,0xf1,0xaf,0x35,0x87,
	0x34,0xef,0xa4,0x36,0xed,0xf0,0xce,0x0d,
	//0200
	0x10,0xab,0x14,0x2f,0x63
};
static u8 serverkey[] = {
	//0000
	0x16,0x03,0x03,0x00,0xcd,0x0c,0x00,0x00,
	0xc9,0x03,0x00,0x17,0x41,0x04,0x8c,0x2a,
	//0010
	0x95,0xee,0x12,0x8b,0x11,0x97,0xdb,0x49,
	0xd3,0x42,0xf2,0xc8,0x48,0xb5,0xdd,0x2b,
	//0020
	0x5b,0x24,0x9a,0x6f,0x5b,0x64,0xcc,0x86,
	0x9e,0x06,0x44,0xbf,0x39,0xab,0xbd,0xae,
	//0030
	0xe5,0x15,0x5d,0xb3,0x0c,0x45,0x1b,0xae,
	0x4c,0xec,0xa4,0xed,0x6d,0xa9,0x66,0xd4,
	//0040
	0xc9,0xab,0x67,0x26,0x73,0x17,0xdc,0x7a,
	0x27,0xa5,0x46,0x0c,0x22,0x88,0x06,0x01,
	//0050
	0x00,0x80,0xb0,0x50,0x1e,0x4c,0x95,0xf7,
	0xf8,0xfd,0x35,0x56,0xb4,0x5a,0xaa,0x6c,
	//0060
	0x9a,0xf4,0x70,0x26,0x7e,0x4e,0x8c,0x44,
	0xcb,0x64,0xc2,0xa7,0x97,0x3f,0x33,0x6a,
	//0070
	0xf7,0xa5,0x5f,0x67,0xc2,0xf6,0xfb,0x83,
	0xc9,0x02,0x17,0x8e,0x0f,0xd7,0xf4,0x98,
	//0080
	0xdc,0xab,0x1b,0x0b,0x93,0x3b,0x41,0xcd,
	0xd1,0x5e,0x00,0x57,0x8a,0x2f,0x43,0x69,
	//0090
	0x51,0x5a,0x4f,0x20,0x0d,0xe7,0xbf,0x4b,
	0x58,0x71,0xe7,0x19,0x0b,0x0f,0x44,0x06,
	//00a0
	0x21,0x12,0xfd,0x28,0xcd,0xb0,0xd9,0x17,
	0xbf,0x14,0x9d,0x37,0xd7,0x14,0xb9,0xa7,
	//00b0
	0xa0,0x35,0x96,0x7c,0x07,0x1e,0xca,0x98,
	0x2c,0x4b,0x39,0xb3,0xae,0x0d,0x89,0x95,
	//00c0
	0x9a,0x4e,0x9e,0xc2,0xc1,0xcf,0x66,0x30,
	0x72,0x71,0x52,0x4e,0x9c,0x61,0xba,0x84,
	//00d0
	0x47,0x79
};




int tls_read_client_hello(u8* buf, int len)
{
	int temp1, temp2;
	u8* p = buf;

	//head
	say("type=0x%x\n", p[0]);
	say("version=%02x%02x\n", p[1], p[2]);
	len = (p[3]<<8) + p[4];
	say("length=%x\n\n", len);
	p += 5;

	//body
	say("handshake=%x\n", p[0]);
	temp1 = (p[1]<<16) + (p[2]<<8) + p[3];
	say("length=%x\n", temp1);
	say("version=%02x%02x\n\n", p[4], p[5]);
	p += 6;

	//random
	say("random(len=0x20)\n");
	printmemory(p, 0x20);
	say("\n");
	p += 0x20;

	//sessionid
	temp1 = p[0];
	say("sessionid(len=%x)\n", temp1);
	printmemory(p+1, temp1);
	say("\n");
	p += 1 + temp1;

	//cipher
	temp1 = (p[0]<<8) + p[1];
	say("ciphersites(len=%x)\n", temp1);
	printmemory(p+2, temp1);
	say("\n");
	p += 2 + temp1;

	//compression
	temp1 = p[0];
	say("compression(len=%x)\n", temp1);
	printmemory(p+1, temp1);
	say("\n");
	p += 1 + temp1;

	//extension
	temp1 = (p[0]<<8) + p[1];
	say("extension(len=%x)\n", temp1);
	printmemory(p+2, temp1);
	p += 2;

	//
	while(1)
	{
		if(p-buf >= len)return 0;
		if(p-buf >= len+5)return 0;

		temp1 = (p[0]<<8) + p[1];
		temp2 = (p[2]<<8) + p[3];
		say("type=%04x, len=%x\n", temp1, temp2);

		p += 4 + temp2;
	}

	return len + 5;
}




int tls_read_server_hello(u8* buf, int len)
{
	return 0;
}
int tls_read_server_certificate(u8* buf, int len)
{
	return 0;
}
int tls_read_server_keyexch(u8* buf, int len)
{
	return 0;
}
int tls_read_server_done(u8* buf, int len)
{
	return 0;
}




int tls_read_client_keyexch(u8* buf, int len)
{
	return 0;
}
int tls_read_client_cipherspec(u8* buf, int len)
{
	return 0;
}
int tls_read_client_hellorequest(u8* buf, int len)
{
	return 0;
}




int tls_read_server_newsession(u8* buf, int len)
{
	return 0;
}
int tls_read_server_cipherspec(u8* buf, int len)
{
	return 0;
}
int tls_read_server_encrypthandshake(u8* buf, int len)
{
	return 0;
}




int tls_read_both_data(u8* buf, int len)
{
	//head
	say("type=%02x\n", buf[0]);
	say("version=%02x%02x\n", buf[1], buf[2]);
	len = (buf[3]<<8) + buf[4];
	say("length=%x\n",len);

	//data
	return len;
}
int tls_read(u64* p, u8* buf, u64 len)
{
	int ret;
	say("tls{\n");

	if(buf[0] == 0x17)
	{
		return tls_read_both_data(buf,len);
	}
	else if(buf[0] == 0x16)
	{
		if(buf[5] == 1)
		{
			ret = tls_read_client_hello(buf, len);
		}
		else if(buf[5] == 2)
		{
			ret = tls_read_server_hello(buf, len);
			ret += tls_read_server_certificate(buf+ret, len);
			ret += tls_read_server_keyexch(buf+ret, len);
			ret += tls_read_server_done(buf+ret, len);
		}
		else if(buf[5] == 12)
		{
			ret = tls_read_client_keyexch(buf, len);
			ret += tls_read_client_cipherspec(buf+ret, len);
			ret += tls_read_client_hellorequest(buf+ret, len);
		}
		else if(0)
		{
			ret = tls_read_server_newsession(buf, len);
			ret += tls_read_server_cipherspec(buf+ret, len);
			ret += tls_read_server_encrypthandshake(buf+ret, len);
		}
	}
	else
	{
		printmemory(buf, len);
	}

	say("}tls\n");
	return 0;
}




//1:	client >>>> server
int tls_write_client_hello(u8* buf, int len)
{
	u8* p = buf + 9;

	//version
	p[0] = p[1] = 0x3;
	p += 2;

	//random
	//*(u32*)(p+0) = getrandom();
	//*(u32*)(p+0x4) = getrandom();
	//*(u32*)(p+0x8) = getrandom();
	//*(u32*)(p+0xc) = getrandom();
	//*(u32*)(p+0x10) = getrandom();
	//*(u32*)(p+0x14) = getrandom();
	//*(u32*)(p+0x18) = getrandom();
	//*(u32*)(p+0x1c) = getrandom();
	p += 0x20;

	//sessionid length
	p[0] = 0;
	p++;

	//ciphersuites
	p[0] = 0;
	p[1] = 0x1e;
	p += 0x20;

	//compress
	p[0] = 1;
	p[1] = 0;
	p += 2;

	//extensions
	p[0] = 0;
	p[1] = 0x63;
	p += 2 + 0x63;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = (len>>8)&0xff;
	buf[4] = len&0xff;

	buf[5] = 1;
	buf[6] = ((len-4)>>16)&0xff;
	buf[7] = ((len-4)>>8)&0xff;
	buf[8] = (len-4)&0xff;
	return len;
}




//2:	client <<<< server
int tls_write_server_hello(u8* buf, int len)
{
	int j;
	u8* p = buf + 9;

	//version
	p[0] = p[1] = 0x3;
	p += 2;

	//random
	for(j=0;j<0x20;j++)p[j] = random_example[j];
	p += 0x20;

	//sessionid length
	p[0] = 0;
	p++;

	//ciphersuites
	p[0] = 0xc0;
	p[1] = 0x30;
	p += 2;

	//compress
	p[0] = 0;
	p += 1;

	//extensions
	p[0] = 0;
	p[1] = 0x1a;
	p += 2;

	//.ff01
	p[0] = 0xff;
	p[1] = 1;
	p[2] = 0;
	p[3] = 1;
	p[4] = 0;
	p += 5;

	//.000b
	p[0] = 0;
	p[1] = 0xb;
	p[2] = 0;
	p[3] = 4;
	p[4] = 3;
	p[5] = 0;
	p[6] = 1;
	p[7] = 2;
	p += 8;

	//.0023
	p[0] = 0;
	p[1] = 0x23;
	p[2] = 0;
	p[3] = 0;
	p += 4;

	//.0010
	p[0] = 0;
	p[1] = 0x10;
	p[2] = 0;
	p[3] = 5;
	p[4] = 0;
	p[5] = 3;
	p[6] = 2;
	p[7] = 0x68;
	p[8] = 0x32;
	p += 9;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 2;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
}
int tls_write_server_certificate(u8* buf, int len)
{
	int j;
	u8* p = buf + 5 + 4 + 3;

	//cert 1th
	for(j=0;j<0x205;j++)
	{
		p[3+j] = cert[j];
	}

	//
	len = 0x205;
	p[0] = 0;	//len(cert1)
	p[1] = (len>>8)&0xff;
	p[2] = len&0xff;
	p += 3 + len;

	//cert 2th
	//len = writecert(p+3);
	//p[0] = 0;	//len(cert2)
	//p[1] = (len>>8)&0xff;
	//p[2] = len&0xff;
	//p += 3 + len;

	//5
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	//4
	buf[5] = 11;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;

	//3
	buf[9] = ((len-12)>>16)&0xff;
	buf[10] = ((len-12)>>8)&0xff;
	buf[11] = (len-12)&0xff;

	//
	return len;
}
int tls_write_server_keyexch(u8* buf, int len)
{
	int j;
	for(j=0;j<0xd2;j++)
	{
		buf[j] = serverkey[j];
	}
	return 0xd2;
/*
	u8* p = buf + 9;

	//curve type,name
	p[0] = 3;
	p[1] = 0;
	p[2] = 0x17;
	p += 3;

	//pubkey
	p[0] = 0x41;
	p += 0x42;

	//hash algorithm
	p[0] = 6;
	p[1] = 1;
	p += 2;

	//signature length
	p[0] = 0;
	p[1] = 0x80;
	p += 0x82;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 12;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
*/
}
int tls_write_server_done(u8* buf, int len)
{
	//5+4byte
	buf[0] = 0x16;
	buf[1] = buf[2] = 0x3;
	buf[3] = 0;
	buf[4] = 4;

	//little endian
	*(u32*)(buf+5) = 0xe;
	return 9;
}




//3:	client >>>> server
int tls_write_client_keyexch(u8* buf, int len)
{
	u8* p = buf + 9;

	//pubkey
	p[0] = 0x41;
	p += 0x42;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 12;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
}
int tls_write_client_cipherspec(u8* buf, int len)
{
	buf[0] = 0x14;
	buf[1] = buf[2] = 3;
	buf[3] = 0;
	buf[4] = 1;
	buf[5] = 1;
	return 6;
}
int tls_write_client_hellorequest(u8* buf, int len)
{
	return 0;
}




//4:	client <<<< server
int tls_write_server_newsession(u8* buf, int len)
{
	return 0;
}
int tls_write_server_cipherspec(u8* buf, int len)
{
	return 0;
}
int tls_write_server_encrypthandshake(u8* buf, int len)
{
	return 0;
}




//@:	client <--> server
int tls_write_both_data(u8* buf, int len)
{
	return 0;
}
int tls_write(u64* p, u8* buf, u64 len)
{
	int ret = 1;

	if(ret == 0)
	{
		ret = tls_write_client_hello(buf, len);
	}
	else if(ret == 1)
	{
		ret = tls_write_server_hello(buf, len);
		ret += tls_write_server_certificate(buf+ret, len);
		ret += tls_write_server_keyexch(buf+ret, len);
		ret += tls_write_server_done(buf+ret, len);
	}
	else if(ret == 2)
	{
		ret = tls_write_client_keyexch(buf, len);
		ret += tls_write_client_cipherspec(buf+ret, len);
		ret += tls_write_client_hellorequest(buf+ret, len);
	}
	else if(ret == 3)
	{
		ret = tls_write_server_newsession(buf, len);
		ret += tls_write_server_cipherspec(buf+ret, len);
		ret += tls_write_server_encrypthandshake(buf+ret, len);
	}

	return ret;
}
