#include "libhard.h"
void* mmiobase();


#define SDHOST_OFFS 0x202000
//
#define SDCMD  0x00		//Command to SD card - 16 R/W
#define SDARG  0x04		//Argument to SD card - 32 R/W
#define SDTOUT 0x08		//Start value for timeout counter - 32 R/W
#define SDCDIV 0x0c		//Start value for clock divider - 11 R/W
#define SDRSP0 0x10		//SD card rsp (31:0) - 32 R
#define SDRSP1 0x14		//SD card rsp (63:32) - 32 R
#define SDRSP2 0x18		//SD card rsp (95:64) - 32 R
#define SDRSP3 0x1c		//SD card rsp (127:96) - 32 R
#define SDHSTS 0x20		//SD host status - 11 R
#define SDVDD  0x30		//SD card power control - 1 R/W
#define SDEDM  0x34		//Emergency Debug Mode - 13 R/W
#define SDHCFG 0x38		//Host configuration - 2 R/W
#define SDHBCT 0x3c		//Host byte count (debug) - 32 R/W
#define SDDATA 0x40		//Data to/from SD card - 32 R/W
#define SDHBLC 0x50		//Host block count (SDIO/SDHC) - 9 R/W
//	
#define SDCMD_NEW_FLAG 0x8000
#define SDCMD_FAIL_FLAG 0x4000
#define SDCMD_BUSYWAIT 0x800
#define SDCMD_NO_RESPONSE 0x400
#define SDCMD_LONG_RESPONSE 0x200
#define SDCMD_WRITE_CMD 0x80
#define SDCMD_READ_CMD 0x40
#define SDCMD_CMD_MASK 0x3f
#define SDCDIV_MAX_CDIV 0x7ff
#define SDHSTS_BUSY_IRPT 0x400
#define SDHSTS_BLOCK_IRPT 0x200
#define SDHSTS_SDIO_IRPT 0x100
#define SDHSTS_REW_TIME_OUT 0x80
#define SDHSTS_CMD_TIME_OUT 0x40
#define SDHSTS_CRC16_ERROR 0x20
#define SDHSTS_CRC7_ERROR 0x10
#define SDHSTS_FIFO_ERROR 0x08
//
#define SDHSTS_DATA_FLAG 0x01
#define SDHCFG_BUSY_IRPT_EN (1 << 10)
#define SDHCFG_BLOCK_IRPT_EN (1 << 8)
#define SDHCFG_SDIO_IRPT_EN (1 << 5)
#define SDHCFG_DATA_IRPT_EN (1 << 4)
#define SDHCFG_SLOW_CARD (1 << 3)
#define SDHCFG_WIDE_EXT_BUS (1 << 2)
#define SDHCFG_WIDE_INT_BUS (1 << 1)
#define SDHCFG_REL_CMD_LINE (1 << 0)
#define SDEDM_FORCE_DATA_MODE (1 << 19)
#define SDEDM_CLOCK_PULSE (1 << 20)
#define SDEDM_BYPASS (1 << 21)
#define SDEDM_WRITE_THRESHOLD_SHIFT 9
#define SDEDM_READ_THRESHOLD_SHIFT 14
#define SDEDM_THRESHOLD_MASK 0x1f
#define SDEDM_FSM_MASK 0xf
#define SDEDM_FSM_IDENTMODE 0x0
#define SDEDM_FSM_DATAMODE 0x1
#define SDEDM_FSM_READDATA 0x2
#define SDEDM_FSM_WRITEDATA 0x3
#define SDEDM_FSM_READWAIT 0x4
#define SDEDM_FSM_READCRC 0x5
#define SDEDM_FSM_WRITECRC 0x6
#define SDEDM_FSM_WRITEWAIT1 0x7
#define SDEDM_FSM_POWERDOWN 0x8
#define SDEDM_FSM_POWERUP 0x9
#define SDEDM_FSM_WRITESTART1 0xa
#define SDEDM_FSM_WRITESTART2 0xb
#define SDEDM_FSM_GENPULSES 0xc
#define SDEDM_FSM_WRITEWAIT2 0xd
#define SDEDM_FSM_STARTPOWDOWN 0xf
#define SDDATA_FIFO_WORDS 16


struct persdhost{
	void* sdhost;
};


int freesdhost()
{
	return 0;
}
int initsdhost(struct item* dev)
{
	long r,cnt,ccs=0;
	say("@initsdhost\n");

	void* mmio = mmiobase();
	void* sdhost = mmio + SDHOST_OFFS;
	printmmio(sdhost, 0x40);

	//get addr
	struct persdhost* per = (void*)dev->priv_256b;
	per->sdhost = sdhost;

	say("\n");
	return 0;
}
