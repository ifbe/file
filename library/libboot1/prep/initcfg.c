#include "libuser.h"
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
void* allocarena();
void* allocactor();
void* allocstyle();
void* allocpinid();
int actorstart(void*, void*);




struct chiplist{
	u64 tier;
	u64 type;

	u64 hash;
	void* addr;
};
struct footlist{
	u64 hash;
	void* addr;
};




int parsefmt(u8* buf, u8* str)
{
	int j,k;
	while(*str == 0x20)str += 1;

	for(j=0;j<8;j++){
		k = str[j];
		if((k >= '0') && (k <= '9')){
			buf[j] = k;
			continue;
		}
		if((k >= 'a') && (k <= 'z')){
			buf[j] = k;
			continue;
		}
		break;
	}
	for(;j<8;j++)buf[j] = 0;
	return 0;
}
void parserelation(u8* buf, int len,
	struct chiplist chip[], int clen, struct footlist foot[], int flen,
	struct halfrel* rel)
{
	//window, leftpart, win, vp
	int j,k;
	int c[4];
	u64 chiphash;
	u64 foothash;

	k = 0;
	for(j=0;j<len;j++){
		if(',' == buf[j]){
			c[k] = j+1;
			k++;
			if(k >= 4)return;
		}
	}
	if(k != 3){
		say("error@parserelation,k=%d\n",k);
		return;
	}

thechip:
	j = 0;
	while(0x20 == buf[j])j++;
	parsefmt((void*)&chiphash, buf+j);

	for(k=0;k<clen;k++){
		if(chiphash == chip[k].hash){
			rel->chip = (u64)chip[k].addr;
		}
	}

thefoot:
	j = c[0];
	while(0x20 == buf[j])j++;
	parsefmt((void*)&foothash, buf+j);

	for(k=0;k<flen;k++){
		if(foothash == foot[k].hash){
			rel->foot = (u64)foot[k].addr;
		}
	}

thetype:
	j = c[1];
	while(0x20 == buf[j])j++;
	parsefmt((void*)&rel->type, buf+j);

theflag:
	j = c[2];
	while(0x20 == buf[j])j++;
	parsefmt((void*)&rel->flag, buf+j);
}
int parsefloat(float* f, u8* str)
{
	double d;
	int j = decstr2double(str, &d);

	f[0] = d;
	return j;
}
int parsevec4(vec4 vec, u8* str)
{
	int j = 0, k;

first:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[0], str+j);

	for(k=j;k<j+16;j++){
		if(',' == str[k]){
			j = k+1;
			goto second;
		}
	}
	return 1;

second:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[1], str+j);

	for(k=j;k<j+16;j++){
		if(',' == str[k]){
			j = k+1;
			goto third;
		}
	}
	return 2;

third:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[2], str+j);

	for(k=j;k<j+16;j++){
		if(',' == str[k]){
			j = k+1;
			goto fourth;
		}
	}
	return 3;

fourth:
	while(0x20 == str[j])j++;
	j += parsefloat(&vec[3], str+j);
	return 4;
}




int role_test_arena(u8* buf, int len, struct chiplist chip[], int clen)
{
	//say("arena:\n%.*s\n", len, buf);

	u64 hash;

	u64 fmt;
	u64 file;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {

			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				propdata = j+1;
				propname = str;
				str = -1;

				//say("propname = %.*s\n", j-propname, buf+propname);
				if(0 == ncmp(buf+propname, "fmt", 3)){
					parsefmt((void*)&fmt, buf+propdata);
					//say("%llx\n", fmt);
				}
				if(0 == ncmp(buf+propname, "file", 4)){
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			parsefmt((void*)&hash, buf+nodename);
			//say("winnode=%.*s\n", j-nodename, buf+nodename);
		}
		if('}' == k) {
			if(nodename >= 0){
				nodename = -1;

				chip[clen].tier = _win_;
				chip[clen].type = fmt;

				chip[clen].hash = hash;
				chip[clen].addr = arenacreate(fmt, 0);
				//say("@%llx\n", arenabuf[arenalen]);

				clen += 1;
			}//if innode

			fmt = 0;
			file = 0;
		}//if }
	}//for

	return clen;
}
int role_test_actor(u8* buf, int len, struct chiplist chip[], int clen)
{
	//say("actor:\n%.*s\n", len, buf);
	u64 hash;

	u64 fmt;
	u64 file;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {

			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				propdata = j+1;
				propname = str;
				str = -1;

				//say("propname = %.*s\n", j-propname, buf+propname);
				if(0 == ncmp(buf+propname, "fmt", 3)){
					parsefmt((void*)&fmt, buf+propdata);
					//say("%llx\n", fmt);
				}
				if(0 == ncmp(buf+propname, "file", 4)){
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			parsefmt((void*)&hash, buf+nodename);
			//say("actnode=%.*s\n", j-nodename, buf+nodename);
		}
		if('}' == k) {
			if(nodename >= 0){
				nodename = -1;

				chip[clen].tier = _act_;
				chip[clen].type = fmt;

				chip[clen].hash = hash;
				chip[clen].addr = actorcreate(fmt, 0);
				//say("@%llx\n", act);

				clen += 1;
			}//if innode

			fmt = 0;
			file = 0;
		}//if }
	}//for

	return clen;
}
int role_test_style(u8* buf, int len, struct footlist foot[], int flen)
{
	//say("style:\n%.*s\n", len, buf);
	u64 hash;
	struct style* sty;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
/*
			if(propdata >= 0) {
				say("propctx = %.*s\n", j-propdata, buf+propdata);
				propname = propdata = -1;
			}
*/
			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				//say("propname = %.*s\n", j-str, buf+str);
				propdata = j+1;
				propname = str;
				str = -1;

				switch(buf[propname]){
					case 'l':parsevec4(sty->vl, buf+propdata);break;
					case 'r':parsevec4(sty->vr, buf+propdata);break;
					case 'b':parsevec4(sty->vb, buf+propdata);break;
					case 'u':parsevec4(sty->vu, buf+propdata);break;
					case 'n':parsevec4(sty->vn, buf+propdata);break;
					case 'f':parsevec4(sty->vf, buf+propdata);break;
					case 'q':parsevec4(sty->vq, buf+propdata);break;
					case 'c':parsevec4(sty->vc, buf+propdata);break;
				}
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			//say("stynode=%.*s\n", j-nodename, buf+nodename);

			parsefmt((void*)&hash, buf+nodename);

			sty = allocstyle();
		}
		if('}' == k) {
			if(nodename >= 0){
				nodename = -1;
/*
				say("l:%f, %f, %f\n", sty->vl[0], sty->vl[1], sty->vl[2]);
				say("r:%f, %f, %f\n", sty->vr[0], sty->vr[1], sty->vr[2]);
				say("b:%f, %f, %f\n", sty->vb[0], sty->vb[1], sty->vb[2]);
				say("u:%f, %f, %f\n", sty->vu[0], sty->vu[1], sty->vu[2]);
				say("n:%f, %f, %f\n", sty->vn[0], sty->vn[1], sty->vn[2]);
				say("f:%f, %f, %f\n", sty->vf[0], sty->vf[1], sty->vf[2]);
				say("q:%f, %f, %f\n", sty->vq[0], sty->vq[1], sty->vq[2]);
				say("c:%f, %f, %f\n", sty->vc[0], sty->vc[1], sty->vc[2]);
*/
				foot[flen].hash = hash;
				foot[flen].addr = sty;

				flen += 1;
			}//if innode
		}//if }
	}//for

	return flen;
}

int role_test_pinid(u8* buf, int len, struct footlist foot[], int flen)
{
	//say("pinid:\n%.*s\n", len, buf);
	u64 hash;
	struct pinid* pin;

	int j,k;
	int str = -1;

	int nodename = -1;
	int nodedata = -1;

	int propname = -1;
	int propdata = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			str = -1;
			continue;
		}

		if(	((k >= '0') && (k <= '9')) |
			((k >= 'a') && (k <= 'z')) )
		{
			if(str < 0)str = j;
			continue;
		}

		//propname: ...
		if(':' == k) {
			//in <type> && in node{} && have str
			if( (nodename >= 0) && (str >= 0) ) {
				say("propname = %.*s\n", j-str, buf+str);
				propdata = j+1;
				propname = str;
				str = -1;
			}
			continue;
		}

		//nodename{...}
		if('{' == k) {
			nodename = str;
			nodedata = j+1;
			str = -1;

			//say("pinnode=%.*s\n", j-nodename, buf+nodename);

			parsefmt((void*)&hash, buf+nodename);

			pin = allocpinid();
		}
		if('}' == k) {
			if(nodename >= 0) {
				nodename = -1;

				foot[flen].hash = hash;
				foot[flen].addr = pin;

				flen += 1;
			}
		}
	}//for

	return flen;
}

void role_test_relation(u8* buf, int len,
	struct chiplist chip[], int clen, struct footlist foot[], int flen)
{
	//say("relation:\n%.*s\n", len, buf);
	struct halfrel src;
	struct halfrel dst;
	struct relation* rel;

	int j,k;
	int wirellll = -1;
	int wirerrrr = -1;

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			wirellll = wirerrrr = -1;
			continue;
		}

		//(src) -> (dst)
		if('(' == k) {
			if(wirellll < 0) {
				wirellll = j+1;
			}
			else {
				wirerrrr = j+1;
			}
		}
		if(')' == k) {
			if(wirerrrr >= 0) {
				//say("r(%.*s)\n", j-wirerrrr, buf+wirerrrr);
				parserelation(buf+wirerrrr, j-wirerrrr,
					chip, clen, foot, flen,
					&dst);

				say("%llx,%llx,%.4s,%.4s -> %llx,%llx,%.4s,%.4s\n",
					src.chip, src.foot, &src.type, &src.flag,
					dst.chip, dst.foot, &dst.type, &dst.flag
				);

				rel = relationcreate(
					(void*)dst.chip, (void*)dst.foot, dst.type, dst.flag,
					(void*)src.chip, (void*)src.foot, src.type, src.flag
				);
				if(_act_ == rel->dsttype){
					actorstart(&rel->dstchip, &rel->srcchip);
				}

				wirellll = wirerrrr = -1;
			}
			else if(wirellll >= 0) {
				//say("l(%.*s) to ", j-wirellll, buf+wirellll);
				parserelation(buf+wirellll, j-wirellll,
					chip, clen, foot, flen,
					&src);
			}
		}
	}//for
}
void role_test1(u8* buf, int len)
{
	int j,k;

	int head = 0;
	int line = 1;

	int typename = -1;
	int typedata = -1;

	int clen = 0;
	struct chiplist cbuf[80];

	int flen = 0;
	struct footlist fbuf[80];

	for(j=0;j<=len;j++) {
		k = buf[j];

		if( (j == len) | ('\n' == k) ) {
			//say("%04d: %.*s\n", l, j-head, buf+head);

			head = j+1;
			line += 1;
			continue;
		}

		//<typename>...</typename>
		if('-' == k) {
			if('>' == buf[j+1]) {
				j++;
				continue;
			}
		}
		if('<' == k) {
			if(typename < 0) {
				typename = j+1;
			}
			else {
				if(0 == ncmp(buf+typename, "arena", 5)) {
					clen = role_test_arena(
						buf + typedata, j-typedata,
						cbuf, clen
					);
				}
				if(0 == ncmp(buf+typename, "actor", 5)) {
					clen = role_test_actor(
						buf + typedata, j-typedata,
						cbuf, clen
					);
				}
				if(0 == ncmp(buf+typename, "style", 5)) {
					flen = role_test_style(
						buf + typedata, j-typedata,
						fbuf, flen
					);
				}
				if(0 == ncmp(buf+typename, "pinid", 5)) {
					flen = role_test_pinid(
						buf + typedata, j-typedata,
						fbuf, flen
					);
				}
				if(0 == ncmp(buf+typename, "relation", 8)) {
					role_test_relation(
						buf + typedata, j-typedata,
						cbuf, clen,
						fbuf, flen
					);
				}
				typename = -1;
			}
		}
		if('>' == k) {
			if(typename < 0)continue;

			////say("typename = %.*s\n", j-typename, buf+typename);
			typedata = j+1;
		}
	}//for
/*
	for(j=0;j<clen;j++)
	{
		if(0 == cbuf[j].addr)break;
		say("%.8s/%.8s: %.8s@%llx\n",
			&cbuf[j].tier, &cbuf[j].type,
			&cbuf[j].hash, cbuf[j].addr
		);
	}

	for(j=0;j<flen;j++)
	{
		if(0 == fbuf[j].addr)break;
		say("foot: %.8s@%llx\n",
			&fbuf[j].hash, fbuf[j].addr
		);
	}
*/
}
void role_test(int argc, u8** argv)
{
	int j,len;
	char* str;
	u8 buf[0x1000];

	if(argc >= 2)str = (char*)argv[1];
	else str = "datafile/mainwindow.ml";

	len = openreadclose(str, 0, buf, 0x1000);
	if(len <= 0)return;
	//say("%s", buf);

	role_test1(buf, len);
}