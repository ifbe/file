#include "libsoft.h"
//
void sha512sum(void* dst, void* src, int len);
void rsa2048(
	u8* dstbuf, int dstlen,
	u8* srcbuf, int srclen,
	u8* keybuf, int keylen,
	u8* modbuf, int modlen);
int pem2bin(  void* dest, void* mem, int off, int len);
//
int openreadclose(void* name, int off, void* mem, int len);
int openwriteclose(void* name, int off, void* mem, int len);
//letsencrypt
static u8 cert_first[0x1000];
static u8 cert_second[0x1000];
static u8 cert_private[256];
static u8 cert_modulus[256];
//testonly
static u8 fixed[19]= {
	0x30,0x51,0x30,0x0d,0x06,0x09,0x60,0x86,
	0x48,0x01,0x65,0x03,0x04,0x02,0x03,0x05,
	0x00,0x04,0x40
};
static u8 dh[] = {
	0x04,0x36,0xfb,0xdf,0xda,0x82,0xf5,0xc3,
	0x5e,0x4b,0x9f,0x90,0x24,0x64,0x13,0xa1,
	0x1c,0x2c,0xff,0x4c,0xbc,0xc2,0x74,0x40,
	0xc9,0x41,0xfb,0xc1,0x0a,0x14,0x63,0x23,
	0x17,0xb1,0xd8,0x15,0x62,0x25,0x26,0xc5,
	0x8f,0x05,0xba,0x34,0x19,0x52,0xb5,0x12,
	0x19,0x0b,0xb7,0x97,0x0d,0xe8,0xc7,0xc0,
	0x70,0x61,0x1f,0x49,0xb2,0x8d,0x22,0x8f,
	0x61
};
int tls_parse_cert(u8* buf, int len)
{
	printmemory(buf, len);
}




struct tlshead
{
	u8 msgtype;
	u8 version[2];
	u8 length[2];
};
int tls_read_head(u8* buf, int len)
{
	len = (buf[3]<<8)+buf[4];
	say("head{ctx=%x,ver=%02x%02x,len=%x}\n",
		buf[0],
		buf[1],buf[2],
		len
	);
	return len;
}




struct bothhello
{
	u8 msg;
	u8 len[3];
	u8 ver[2];
};
int tls_read_hello(u8* buf, int len)
{
	struct bothhello* p = (void*)buf;
	len = (buf[1]<<16) + (buf[2]<<8) + buf[3];
	say("hello{msg=%x, len=%x, ver=%02x%02x}\n",
		p->msg,
		len,
		p->ver[0], p->ver[1]
	);
	return len;
}
int tls_write_server_hello(struct element* ele, int fd, u8* buf, int len)
{
	int j;
	u8* p = buf + 9;

	//version
	p[0] = p[1] = 0x3;
	p += 2;

	//random
	*(u32*)(p+0) = getrandom();
	*(u32*)(p+4) = getrandom();
	*(u32*)(p+8) = getrandom();
	*(u32*)(p+12) = getrandom();
	*(u32*)(p+16) = getrandom();
	*(u32*)(p+20) = getrandom();
	*(u32*)(p+24) = getrandom();
	*(u32*)(p+28) = getrandom();
	for(j=0;j<0x20;j++)p[j] = ele[fd].data[j+0x20] = p[j];
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
	p[1] = 0x11;
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




struct servercert
{
	u8 msg;
	u8 len[3];
	u8 certlen[3];
};
int tls_read_cert(u8* buf, int len)
{
	int certlen;
	struct servercert* p = (void*)buf;

	len = (buf[1]<<16) + (buf[2]<<8) + buf[3];
	certlen = (buf[4]<<16) + (buf[5]<<8) + buf[6];
	say("certi{msg=%x, len=%x, certlen=%x}\n",
		p->msg,
		len,
		certlen
	);
	return len;
}
int tls_write_server_certificate(u8* buf, int len)
{
	int j,k;
	u8* p = buf + 5 + 4 + 3;

	//cert 1th
	k = (cert_first[1]<<8) + cert_first[2] + 3;
	for(j=0;j<k;j++)
	{
		p[j] = cert_first[j];
	}
	p += k;

	//cert 2th
	k = (cert_second[1]<<8) + cert_second[2] + 3;
	for(j=0;j<k;j++)
	{
		p[j] = cert_second[j];
	}
	p += k;

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




struct serverkeyexch
{
	u8 msg;
	u8 len[3];

	u8 curvetype;
	u8 namedcurve[2];
};
int tls_read_keyex(u8* buf, int len)
{
	struct serverkeyexch* p = (void*)buf;

	len = (buf[1]<<16) + (buf[2]<<8) + buf[3];
	say("keyex{msg=%x, len=%x}\n",
		p->msg,
		len
	);
	return len;
}
int tls_read_client_keyexch(u8* buf, int len)
{
	int j = (buf[3]<<8) + buf[4];
	say("clientkeyexch{\n");
	say("}clientkeyexch\n");
	return j+5;
}
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
int tls_write_server_keyexch(struct element* ele, int fd, u8* buf, int len)
{
	int j;
	u8* p = buf + 9;




	//curve type,name
	p[0] = 3;
	p[1] = 0;
	p[2] = 0x17;
	p += 3;

	//pubkey(p + g + Y)
	for(j=0;j<0x41;j++)
	{
		p[1+j] = dh[j];
	}
	p[0] = 0x41;
	p += 1 + 0x41;




	//hash algorithm
	p[0] = 6;
	p[1] = 1;
	p += 2;

	p[0] = 0x01;
	p[1] = 0x00;
	p += 2;
/*
	rsa2048(
		//[173byte]padding
		//0001fffffffffffffffff...ffffffff00

		//[19byte]fixed header
		3051300d 06096086 48016503 04020305 000440

		//[64byte]result value
		sha512(
			clientrandom,
			serverrandom,
			curvetype,namedcurve,pubkeylength,pubkeydata
		)
	)//with cert's private key
*/
	//put all @ [0,0x84]
	for(j=0;j<0x20;j++)p[0x00+j] = ele[fd].data[j];
	for(j=0;j<0x20;j++)p[0x20+j] = ele[fd].data[j+0x20];
	for(j=0;j<0x45;j++)p[0x40+j] = buf[9+j];
	say("c+s+p:\n");
	printmemory(p, 0x85);

	//dst@[0x10c0,0x10ff], src@[0,0x84]
	p[0x1000] = 0;
	p[0x1001] = 1;
	for(j=0x1002;j<0x10ac;j++)p[j] = 0xff;
	p[0x10ac] = 0;
	for(j=0;j<19;j++)p[0x10ad+j] = fixed[j];
	sha512sum(p+0x10c0, p, 0x20+0x20+3+1+0x41);

	say("flag+sha512:\n");
	printmemory(p+0x1000, 256);

	//dst@[0x100,0x1ff], src@[0x1000,0x10ff]
	rsa2048(
		p,	    256,
		p+0x1000,     256,
		cert_private, 256,
		cert_modulus, 256
	);
	say("rsa2048:\n");
	printmemory(p, 256);

	//
	p += 0x100;




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
}




struct serverdone
{
	u8 msg;
	u8 len[3];
};
int tls_read_sdone(u8* buf, int len)
{
	struct serverdone* p = (void*)buf;

	len = (buf[1]<<16) + (buf[2]<<8) + buf[3];
	say("sdone{msg=%x, len=%x}\n",
		p->msg,
		len
	);
	return len;
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




struct servercipher
{
	u8 msgtype;
	u8 version[2];
	u8 length[2];
};
int tls_read_client_cipherspec(u8* buf, int len)
{
	say("client cipherspec\n");
	return 6;
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
int tls_read_server_cipherspec(u8* buf, int len)
{
	return 0;
}
int tls_write_server_cipherspec(u8* buf, int len)
{
	buf[0] = 0x14;
	buf[1] = 3;
	buf[2] = 3;
	buf[3] = 0;
	buf[4] = 1;
	buf[5] = 1;
	return 6;
}




struct hellorequest
{
	u8 msgtype;
	u16 version;
	u8 length[2];
};
int tls_read_client_hellorequest(u8* buf, int len)
{
	int j = (buf[3]<<8) + buf[4];
	say("client hellorequest\n");
	return j+5;
}
int tls_write_client_hellorequest(u8* buf, int len)
{
	return 0;
}




struct newsession
{
	u8 msgtype;
	u16 version;
	u8 length[2];
};
int tls_write_server_newsession(u8* buf, int len)
{
	u8* p = buf + 9;

	//pubkey
	p[0] = 0;
	p[1] = 0;
	p[2] = 2;
	p[3] = 0x58;
	p[4] = 0;
	p[5] = 0xb0;
	p += 0xb0 + 6;

	//5+4byte
	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	buf[5] = 4;
	buf[6] = ((len-9)>>16)&0xff;
	buf[7] = ((len-9)>>8)&0xff;
	buf[8] = (len-9)&0xff;
	return len;
}
int tls_read_server_newsession(u8* buf, int len)
{
	return 0;
}




struct encrypthandshake
{
	u8 msgtype;
	u16 version;
	u8 length[2];
};
int tls_read_server_encrypthandshake(u8* buf, int len)
{
	return 0;
}
int tls_write_server_encrypthandshake(u8* buf, int len)
{
	int j;
	u8* p = buf + 5;

	for(j=0;j<0x28;j++)p[j] = j;
	p += 0x28;

	len = p - buf;
	buf[0] = 0x16;
	buf[1] = buf[2] = 3;
	buf[3] = ((len-5)>>8)&0xff;
	buf[4] = (len-5)&0xff;

	return len;
}




struct bothdata
{
	u8 msgtype;
	u16 version;
	u8 length[2];
};
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
int tls_write_both_data(u8* buf, int len)
{
	return 0;
}




int tls_read(struct element* ele, int fd, u8* buf, int len)
{
	int ret=0;
	int stage;

	if(buf[0] == 0x17)
	{
		ret = tls_read_both_data(buf,len);
		ele[fd].stage1 = 0xff;
	}
	else if(buf[0] == 0x16)
	{
		if(buf[5] == 1)
		{
			//ret = tls_read_client_hello(ele, fd, buf, len);
			ele[fd].stage1 = 1;
		}
		else if(buf[5] == 2)
		{
			//ret = tls_read_server_hello(buf, len);
			//ret += tls_read_server_certificate(buf+ret, len);
			//ret += tls_read_server_keyexch(buf+ret, len);
			//ret += tls_read_server_done(buf+ret, len);
			ele[fd].stage1 = 2;
		}
		else if(buf[5] == 16)
		{
			ret = tls_read_client_keyexch(buf, len);
			ret += tls_read_client_cipherspec(buf+ret, len);
			ret += tls_read_client_hellorequest(buf+ret, len);
			ele[fd].stage1 = 16;
		}
		else if(buf[5] == 4)
		{
			ret = tls_read_server_newsession(buf, len);
			ret += tls_read_server_cipherspec(buf+ret, len);
			ret += tls_read_server_encrypthandshake(buf+ret, len);
			ele[fd].stage1 = 4;
		}
	}
	else
	{
		printmemory(buf, len);
		ret = -1;
	}

	return ret;
}
int tls_write(struct element* ele, int fd, u8* buf, int len)
{
	int ret;
	int stage = ele[fd].stage1;

	if(stage == 0)
	{
		//ret = tls_write_client_hello(buf, len);
	}
	else if(stage == 1)
	{
		ret = tls_write_server_hello(ele, fd, buf, len);
		ret += tls_write_server_certificate(buf+ret, len);
		ret += tls_write_server_keyexch(ele, fd, buf+ret, len);
		ret += tls_write_server_done(buf+ret, len);
	}
	else if(stage == 2)
	{
		ret = tls_write_client_keyexch(buf, len);
		ret += tls_write_client_cipherspec(buf+ret, len);
		ret += tls_write_client_hellorequest(buf+ret, len);
	}
	else if(stage == 16)
	{
		ret = tls_write_server_newsession(buf, len);
		ret += tls_write_server_cipherspec(buf+ret, len);
		ret += tls_write_server_encrypthandshake(buf+ret, len);
		printmemory(buf,ret);
	}
	else if(stage == 0xff)
	{
		ret = tls_write_both_data(buf, len);
	}

	return ret;
}




void tls_start()
{
	int j,fl;
	u8 buf[0x2000];

	//cert1,2,3......
	fl = openreadclose("fullchain.pem", 0, buf, 0x2000);
	if(fl<=0){say("err@fullchain.pem:%d\n",fl);return;}

	j = pem2bin(cert_first+3, buf, 0, fl);
	if(j<=0){say("err@cert1:%d\n",j);return;}
	cert_first[0] = 0;
	cert_first[1] = (j>>8)&0xff;
	cert_first[2] = j&0xff;
	say("cert1:\n");
	printmemory(cert_first, j+3);

	j = pem2bin(cert_second+3, buf, 1, fl);
	if(j<=0){say("err@cert2:%d\n",j);return;}
	cert_second[0] = 0;
	cert_second[1] = (j>>8)&0xff;
	cert_second[2] = j&0xff;
	say("cert2:\n");
	printmemory(cert_second, j+3);

	//private and modulus
	j = openreadclose("privkey.pem", 0, buf, 0x2000);
	if(j<=0){say("err@privkey.pem:%d\n",j);return;}

	j = pem2bin(buf, buf, 0, j);
	if(j<=0){say("err@pvk:%d\n",j);return;}

	for(j=0;j<0x100;j++)cert_modulus[j] = buf[0xff-j+0x26];
	say("modulus:\n");
	printmemory(cert_modulus, 0x100);

	for(j=0;j<0x100;j++)cert_private[j] = buf[0xff-j+0x130];
	say("private:\n");
	printmemory(cert_private, 0x100);
}
void tls_stop()
{
}
void tls_create()
{
	
}
void tls_delete()
{
	
}




#define TLS 0x534c54
#define tls 0x736c74
int tls_client(struct element* ele, int fd, u8* buf, int len)
{
	//printmemory(buf, len);
	tls_read(ele, fd, buf, len);
	return tls;
}
int tls_server(struct element* ele, int fd, u8* buf, int len)
{
	//tls >>>> ascii
	len = tls_read(ele, fd, buf, len);
	if(len < 0)goto error;

	//bin >>>> tls
	len = tls_write(ele, fd, buf, len);
	if(len <= 0)goto error;

good:
	writesocket(fd, 0, buf, len);
	return TLS;
error:
	return 0;
}
int tls_check(struct element* ele, int fd, u8* buf, int len)
{
	if(buf[0] == 0x16)return TLS;
	return 0;
}




int tls_clientread_serverhello(u8* buf, int len)
{
	u8* t;
	int j,k;

	//hello
	say("serverhello{\n");
	len = tls_read_hello(buf, len);


	//random
	t = buf+6;
	say("random(20):\n");
	printmemory(t, 0x20);
	t += 0x20;


	//sessid
	j = t[0];
	t += 1;

	if(j)say("sessid(%x)\n", j);
	t += j;


	//cipher
	j = (t[0]<<8)+t[1];
	say("cipher=%04x\n", j);
	t += 2;


	//compress
	j = t[0];
	t += 1;

	if(j)say("compress(%x)\n", j);
	t += j;


	//extension
	j = (t[0]<<8) + t[1];
	t += 2;

	say("extension(len=%x)\n", j);
	printmemory(t, j);
	while(1)
	{
		if(t-buf >= len)break;
		if(t-buf >= len+5)break;

		j = (t[0]<<8) + t[1];
		k = (t[2]<<8) + t[3];
		say("type=%04x, len=%x\n", j, k);

		t += 4 + k;
	}


	say("}serverhello\n\n");
	return len;
}
int tls_clientread_servercertificate(u8* buf, int len)
{
	int j,total;
	u8* q;
	say("servercert{\n");
	len = tls_read_cert(buf, len);



	//total
	q = buf+4;
	total = (q[0]<<16)+(q[1]<<8)+q[2];
	say("total=%x\n", total);
	q += 3;


	//cert
	while(1)
	{
		if(total <= 0)break;

		j = (q[0]<<16)+(q[1]<<8)+q[2];
		q += 3;

		say("cert(%x):\n", j);
		tls_parse_cert(q, j);
		q += j;

		total -= 3+j;
	}


	//
	say("}servercert\n\n");
	return len;
}
int tls_clientread_serverkeyexch(u8* buf, int len)
{
	int j;
	u8* q;
	say("serverkeyexch{\n");
	len = tls_read_keyex(buf, len);


	//curve
	q = buf + 4;
	say("curvetype=%x,namecurve=%02x%02x\n",
		q[0],
		q[1],q[2]
	);
	q += 3;


	//pubkey
	j = q[0];
	q += 1;

	say("pubkey(%x)\n",j);
	printmemory(q, j);
	q += j;


	//signature algorithm
	say("sig alg: %02x%02x\n", q[0], q[1]);
	q += 2;


	//signature context
	j = (q[0]<<8)+q[1];
	q += 2;

	say("sig ctx(%02x):\n", j);
	printmemory(q, j);


	//
	say("}serverkeyexch\n\n");
	return len;
}
int tls_clientread_serverdone(u8* buf, int len)
{
	struct serverdone* p = (void*)buf;
	u8* q;

	say("serverdone{\n");
	len = tls_read_sdone(buf, len);

	//
	say("}serverdone\n\n");
	return len;
}
int tls_clientwrite_clienthello(u8* buf, int len, u8* dst, int cnt)
{
	u8* p = dst + 9;
	u16* q;
	u32* r;

	//version
	p[0] = p[1] = 0x3;
	p += 2;

	//random
	r = (u32*)p;
	r[0] = getrandom();
	r[1] = getrandom();
	r[2] = getrandom();
	r[3] = getrandom();
	r[4] = getrandom();
	r[5] = getrandom();
	r[6] = getrandom();
	r[7] = getrandom();
	p += 0x20;

	//sessionid length
	p[0] = 0;
	p++;

	//ciphersuites
	p[0] = 0;
	p[1] = 0x26;
	p += 2;

	q = (u16*)p;
	q[0] = 0x0a0a;
	q[1] = 0x0113;
	q[2] = 0x0213;
	q[3] = 0x0313;
	q[4] = 0x2bc0;
	q[5] = 0x2fc0;
	q[6] = 0x2cc0;
	q[7] = 0x30c0;
	q[8] = 0xa9cc;
	q[9] = 0xa8cc;
	q[10] = 0x14cc;
	q[11] = 0x13cc;
	q[12] = 0x13c0;
	q[13] = 0x14c0;
	q[14] = 0x9c00;
	q[15] = 0x9d00;
	q[16] = 0x2f00;
	q[17] = 0x3500;
	q[18] = 0x0a00;
	p += 0x26;

	//compress
	p[0] = 1;
	p[1] = 0;
	p += 2;

	//extensions
	p[0] = 0;
	p[1] = 0xa9;
	p += 2;

	r = (u32*)p;
	r[0] = 0x00007a7a;	//unknown 31354
	r[1] = 0x010001ff;	//renegotiation info
	p[8] = 0;
	p += 9;

	r = (u32*)p;
	r[0] = 0x00001700;	//extended master secret
	r[1] = 0x00002300;	//sessionticket tls
	p += 8;

	r = (u32*)p;
	r[0] = 0x14000d00;	//signature algorithms
	r[1] = 0x03041200;
	r[2] = 0x01040408;
	r[3] = 0x05080305;
	r[4] = 0x06080105;
	r[5] = 0x01020106;
	p += 24;

	r = (u32*)p;
	r[0] = 0x05000500;
	r[1] = 0x00000001;
	p[8] = 0;
	p += 9;

	r = (u32*)p;
	r[0] = 0x00001200;	//signed certificate timestamp
	r[1] = 0x0e001000;	//application layer protocol negotiation
	r[2] = 0x68020c00;
	r[3] = 0x74680832;
	r[4] = 0x312f7074;
	p[20] = 0x2e;
	p[21] = 0x31;
	p += 22;

	r = (u32*)p;
	r[0] = 0x00005075;	//channel id
	r[1] = 0x02000b00;
	p[8] = 1;
	p[9] = 0;
	p += 10;

	r = (u32*)p;
	r[0] = 0x2b002800;	//unknown 40
	r[1] = 0xaaaa2900;
	r[2] = 0x00000100;
	r[3] = 0xa920001d;
	r[4] = 0xb5d22cec;
	r[5] = 0xaeffd3a6;
	r[6] = 0x4c9c560e;
	r[7] = 0x31b91be7;
	r[8] = 0xb11f15c7;
	r[9] = 0x51a2a5f2;
	r[10] = 0xc12a37d9;
	r[11] = 0x004347e0;
	p += 0x2f;

	r = (u32*)p;
	r[0] = 0x02002d00;	//unknown 45
	p[4] = 1;
	p[5] = 1;
	p += 6;

	r = (u32*)p;
	r[0] = 0x0b002b00;	//unknown 43
	r[1] = 0x7f1a1a0a;
	r[2] = 0x03030312;
	r[3] = 0x00010302;
	p += 15;

	r = (u32*)p;
	r[0] = 0x0a000a00;
	r[1] = 0xaaaa0800;
	r[2] = 0x17001d00;
	r[3] = 0xfafa1800;
	p += 14;

	r = (u32*)p;
	r[0] = 0x0100fafa;
	p[4] = 0;
	p += 5;

	//5+4byte
	len = p - dst;
	dst[0] = 0x16;
	dst[1] = 0x3;
	dst[2] = 0x1;
	dst[3] = ((len-5)>>8)&0xff;
	dst[4] = (len-5)&0xff;

	dst[5] = 1;
	dst[6] = ((len-9)>>16)&0xff;
	dst[7] = ((len-9)>>8)&0xff;
	dst[8] = (len-9)&0xff;
	return len;
}
int tlsclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	if(0 == ele->stage1)
	{
		ret = 5 + tls_read_head(buf, len);
		tls_clientread_serverhello(buf+5, len-5);
		buf += ret;
		len -= ret;

		ret = 5 + tls_read_head(buf, len);
		tls_clientread_servercertificate(buf+5, len-5);
		buf += ret;
		len -= ret;

		ret = 5 + tls_read_head(buf, len);
		tls_clientread_serverkeyexch(buf+5, len-5);
		buf += ret;
		len -= ret;

		ret = 5 + tls_read_head(buf, len);
		tls_clientread_serverdone(buf+5, len-5);

		//ret = 
	}
	else printmemory(buf,len);

	ele->stage1 += 1;
	return 0;
}
int tlsclient_read()
{
	return 0;
}
int tlsclient_delete(struct element* ele)
{
	return 0;
}
int tlsclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	ret = tls_clientwrite_clienthello(url, 0, buf, len);

	ret = systemwrite(obj, 0, ele, 0, buf, ret);
	if(ret <= 0)return 0;

	ele->type = _tls_;
	ele->stage1 = 0;
	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}




int tls_serverread_clienthello(u8* buf, int len, u8* dst, int cnt)
{
	int j, k;
	struct bothhello* p = (void*)buf;
	u8* q;

	//hello
	say("clienthello{\n");
	len = tls_read_hello(buf, len);


	//random
	q = buf+6;

	say("random(len=20)\n");
	printmemory(q, 0x20);
	q += 0x20;


	//sessionid
	j = q[0];
	q += 1;

	say("sessionid(len=%x)\n", j);
	printmemory(q, j);
	q += j;


	//cipher
	j = (q[0]<<8) + q[1];
	q += 2;

	say("ciphersites(len=%x)\n", j);
	printmemory(q, j);
	q += j;


	//compression
	j = q[0];
	q += 1;

	say("compression(len=%x)\n", j);
	if(j)printmemory(q, j);
	q += j;


	//extension
	j = (q[0]<<8) + q[1];
	q += 2;

	say("extension(len=%x)\n", j);
	printmemory(q, j);
	while(1)
	{
		if(q-buf >= len)break;
		if(q-buf >= len+5)break;

		j = (q[0]<<8) + q[1];
		k = (q[2]<<8) + q[3];
		say("type=%04x, len=%x\n", j, k);

		q += 4 + k;
	}

	say("}clienthello\n");
	return len + 5;
}
int tlsserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	if(0 == ele->stage1)
	{
		tls_read_head(buf,len);
		tls_serverread_clienthello(buf+5, len-5, 0, 0);
	}
	else printmemory(buf,len);
	return 0;
}
int tlsserver_read()
{
	return 0;
}
int tlsserver_delete(struct element* ele)
{
	return 0;
}
int tlsserver_create(struct element* ele, u8* url, u8* buf, int len)
{
	return 0;
}




int tlsmaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int tlsmaster_read()
{
	return 0;
}
int tlsmaster_delete(struct element* ele)
{
	return 0;
}
int tlsmaster_create(struct element* ele, u8* url, u8* buf, int len)
{
	return 0;
}
