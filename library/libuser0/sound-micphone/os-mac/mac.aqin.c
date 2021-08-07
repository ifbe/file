#include "libuser.h"
#include "AudioToolbox/AudioToolbox.h"




static void micphonecallback(void* ptr, AudioQueueRef aqref, AudioQueueBufferRef aqbufref, const AudioTimeStamp* inStartTime, UInt32 inNumberPacketDescriptions, const AudioStreamPacketDescription* inPacketDescs)
{
	_obj* spk = ptr;
	void* buf = aqbufref->mAudioData;
	int len = aqbufref->mAudioDataByteSize;
	say("@aqin.callback: spk=%p, aqref=%p,aqbufref=%p, len=%x,buf=%p, time=%lld\n",
	spk, aqref, aqbufref, len,buf, inStartTime->mHostTime);

	//deliver
	struct halfrel stack[0x80];
	give_data_into_peer(spk,_dst_, stack,0, 0,0, buf,len);

	//reuse
	AudioQueueEnqueueBuffer(aqref, aqbufref, 0, NULL);
}




int micphone_take(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int micphone_give(_obj* sup,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf, int len)
{
	return 0;
}
int micphonestop()
{
	return 0;
}
int micphonestart()
{
	return 0;
}
int micphonedelete(_obj* spk)
{
	return 0;
}
int micphonecreate(_obj* spk)
{
	AudioStreamBasicDescription fmt = {0};
	fmt.mSampleRate = 44100;
	fmt.mFormatID = kAudioFormatLinearPCM;
	fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	fmt.mFramesPerPacket = 1;
	fmt.mChannelsPerFrame = 1; // 2 for stereo
	fmt.mBytesPerPacket = fmt.mBytesPerFrame = 2; // x2 for stereo
	fmt.mBitsPerChannel = 16;

	AudioQueueRef aqref;
	OSStatus status = AudioQueueNewInput(&fmt, (void*)micphonecallback, spk, 0, 0, 0, &aqref);
	if(status == kAudioFormatUnsupportedDataFormatError){
		puts("oops!");
		return 0;
	}
	printf("NewInput status: %d\n", status);
	spk->appleaq.aqref = aqref;

	int j;
	AudioQueueBufferRef* aqbufref = (void*)spk->appleaq.aqbufref;
	for(j=0;j<16;j++){
		status = AudioQueueAllocateBuffer(aqref, 2*1024, &aqbufref[j]);
		printf("[%d]Allocate status: %d\n", j, status);

		status = AudioQueueEnqueueBuffer(aqref, aqbufref[j], 0, NULL);
		printf("[%d]Enqueue status: %d\n", j, status);
	}

	status = AudioQueueSetParameter(aqref, kAudioQueueParam_Volume, 1.0);
	printf ("Volume status: %d\n", status);

	status = AudioQueueStart(aqref, NULL);
	printf ("Start status: %d\n", status);

	return 0;
}




void initmicphone()
{
}
void freemicphone()
{
}
