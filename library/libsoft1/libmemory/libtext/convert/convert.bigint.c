#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int bigmul(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* answer, int max,
	u8* temp, int rsvd);
int bigdiv(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* quotient, int max1,
	u8* remainder, int max2);




int hexstr2bigint(u8* p, u8* q)
{
        int j=0,k=0;
        while(p[j] != 0)j++;
        while(1)
        {
                //byte.low4
                j--;
                if(j < 0)break;
                if(p[j] == 0)break;
                if( (p[j] >= 0x30) && (p[j] <= 0x39) )
                {
                        q[k] = p[j] - 0x30;
                        k++;
                }
                else return 0;

                //byte.high4
                j--;
                if(j < 0)break;
                if(p[j] == 0)break;
                if( (p[j] >= 0x30) && (p[j] <= 0x39) )
                {
                        q[k-1] += (p[j]-0x30) << 4;
                }
                else return 0;
        }

        return k;
}
int decstr2bigint(u8* p, u8* q)
{
}




int bigint2hexstr(u8* p, u8* q, int len)
{
	u8 ch;
	int j,k=2;
	if(len<=0)return 0;

	q[0] = '0';
	q[1] = 'x';
	for(j=len-1;j>=0;j--)
	{
		ch = (p[j] >> 4) & 0xf;
		ch += 0x30;
		if(ch > 0x39)ch += 7;
		q[k] = ch;
		k++;

		ch = p[j] &0xf;
		ch += 0x30;
		if(ch > 0x39)ch += 7;
		q[k] = ch;
		k++;
	}

	q[k] = 0;
	return k;
}
int bigint2decstr(u8* p, u8* q, int len)
{
	u8 ten[1] = {10};
	u8 buf1[0x1000];
	u8 buf2[0x1000];
	u8* quotient;
	u8* result;
	u8* temp;
	int j,k;
	if(len>=0x1000)return 0;

	for(j=0;j<len;j++)quotient[j] = p[j];
	quotient = buf1;
	result = buf2;

	for(j=0;j<len;j++)
	{
		//div10
		bigdiv(
			quotient, len,
			ten, 1,
			result, 0x1000,
			q+j, 0x1000	//temp
		);

		//
		q[j] = result[0] + 0x30;
		if(j>len*2)break;

		//next
		temp = quotient;
		quotient = result;
		result = temp;
	};

	q[j] = 0;
	return j;
}
