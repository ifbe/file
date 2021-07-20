#include <stdio.h>
#include <windows.h>
#include "libuser.h"




static _obj* working;
static WAVEFORMATEX fmt;
static int alive = 0;
//
static HWAVEIN wavein;
static WAVEHDR headin[16];
static u8* ibuf = 0;
static int icur = 0;




static void CALLBACK icb(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)
{
	struct halfrel stack[0x80];
	//printf("@icb\n");
	if(WIM_DATA == uMsg)
	{
		//printf("WIM_DATA:%d\n", icur);
		give_data_into_peer(working,_dst_, stack,0, 0,0, ibuf+(1024*2*icur),1024*2);

		waveInAddBuffer(wavein, &headin[icur], sizeof (WAVEHDR));
		icur = (icur+1)%16;
	}
	else if(WIM_OPEN == uMsg)
	{
		printf("WIM_OPEN\n");
	}
	else if(WIM_CLOSE == uMsg)
	{
		printf("WIM_CLOSE\n");
	}
}




void micphone_take(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
}
void micphone_give(_sup* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
}
void micphonestop()
{
	waveInClose(wavein);
}
void micphonestart()
{
}
void micphonedelete(_obj* win)
{
}
void micphonecreate(_obj* win, void* arg, int argc, u8** argv)
{
	int j,ret,freq;
	working = win;

	if(0 == arg)freq = 44100;
	else decstr2u32(arg, &freq);

	fmt.wFormatTag = WAVE_FORMAT_PCM;
	fmt.nAvgBytesPerSec = freq*2;
	fmt.nSamplesPerSec = freq;
	fmt.wBitsPerSample = 16;
	fmt.nChannels = 1;
	fmt.nBlockAlign = 2;
	fmt.cbSize = 0;
	ret = waveInOpen(
		&wavein, WAVE_MAPPER,
		&fmt, (DWORD_PTR)icb,
		0L, CALLBACK_FUNCTION
	);
	say("@waveInOpen:%d\n", ret);

	icur = 0;
	ibuf = malloc(1024*2*16);
	for(j=0;j<16;j++)
	{
		headin[j].lpData = ibuf + (1024*2*j);
		headin[j].dwBufferLength = 1024*2;
		headin[j].dwBytesRecorded = 0;
		headin[j].dwUser = 0;
		headin[j].dwFlags = 0;
		headin[j].dwLoops = 1;
		waveInPrepareHeader(wavein, &headin[j], sizeof(WAVEHDR));
		waveInAddBuffer(wavein, &headin[j], sizeof (WAVEHDR));
	}

	waveInStart(wavein);
}




void initmic()
{
}
void freemic()
{
}
