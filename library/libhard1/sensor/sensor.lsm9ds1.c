#include "libhard.h"
#define Gscale 3
#define Ascale 0
#define Mscale 0
void sleep_us(int);




enum lsm9ds1{
	WHO_AM_I	= 0x0F,

	CTRL_REG1_G     = 0x10, // Angular rate sensor Control Register 1.

	OUT_TEMP_L      = 0x15,
	OUT_TEMP_H      = 0x16,
	STATUS_REG_G    = 0x17,
	OUT_X_L_G       = 0x18,
	OUT_X_H_G       = 0x19,
	OUT_Y_L_G       = 0x1A,
	OUT_Y_H_G       = 0x1B,
	OUT_Z_L_G       = 0x1C,
	OUT_Z_H_G       = 0x1D,

	CTRL_REG4       = 0x1E,
	CTRL_REG5_XL    = 0x1F,
	CTRL_REG6_XL    = 0x20, // Linear acceleration sensor Control Register 6.
	CTRL_REG7_XL    = 0x21,
	CTRL_REG8       = 0x22, // Control register 8.
	CTRL_REG9       = 0x23, // Control register 9.

	STATUS_REG_A    = 0x27,
	OUT_X_L_XL      = 0x28,
	OUT_X_H_XL      = 0x29,
	OUT_Y_L_XL      = 0x2A,
	OUT_Y_H_XL      = 0x2B,
	OUT_Z_L_XL      = 0x2C,
	OUT_Z_H_XL      = 0x2D,
	FIFO_CTRL       = 0x2E, // FIFO control register.
	FIFO_SRC	= 0x2F, // FIFO status control register.
};
#define MAG_CTRL_REG1 0x20
#define MAG_CTRL_REG2 0x21
#define MAG_CTRL_REG3 0x22
#define MAG_CTRL_REG4 0x23
#define MAG_CTRL_REG5 0x24




int lsm9ds1_i2cread(void* it, float* measure)
{
	short t;
	u8 reg[32];

	t = relationread(it,_i2c_, 0,0, 0,0x6a0018, reg+0,1);
	t = relationread(it,_i2c_, 0,0, 0,0x6a0019, reg+1,1);
	t = relationread(it,_i2c_, 0,0, 0,0x6a001a, reg+2,1);
	t = relationread(it,_i2c_, 0,0, 0,0x6a001b, reg+3,1);
	t = relationread(it,_i2c_, 0,0, 0,0x6a001c, reg+4,1);
	t = relationread(it,_i2c_, 0,0, 0,0x6a001d, reg+5,1);
	//printf("%02x %02x %02x %02x %02x %02x\n", reg[0],reg[1],reg[2],reg[3],reg[4],reg[5]);

	t = *(short*)(reg+0);
	measure[0] = t * 500.0/32768 * PI/180.0;
	t = *(short*)(reg+2);
	measure[1] =-t * 500.0/32768 * PI/180.0;
	t = *(short*)(reg+4);
	measure[2] = t * 500.0/32768 * PI/180.0;


	relationread(it,_i2c_, 0,0, 0,0x6a0028, reg+0,1);
	relationread(it,_i2c_, 0,0, 0,0x6a0029, reg+1,1);
	relationread(it,_i2c_, 0,0, 0,0x6a002a, reg+2,1);
	relationread(it,_i2c_, 0,0, 0,0x6a002b, reg+3,1);
	relationread(it,_i2c_, 0,0, 0,0x6a002c, reg+4,1);
	relationread(it,_i2c_, 0,0, 0,0x6a002d, reg+5,1);
	//printf("%02x %02x %02x %02x %02x %02x\n", reg[0],reg[1],reg[2],reg[3],reg[4],reg[5]);

	t = *(short*)(reg+0);
	measure[3] = t * 9.8*8 / 32768.0;
	t = *(short*)(reg+2);
	measure[4] =-t * 9.8*8 / 32768.0;
	t = *(short*)(reg+4);
	measure[5] = t * 9.8*8 / 32768.0;


	relationread(it,_i2c_, 0,0, 0,0x1c0028, reg+0,1);
	relationread(it,_i2c_, 0,0, 0,0x1c0029, reg+1,1);
	relationread(it,_i2c_, 0,0, 0,0x1c002a, reg+2,1);
	relationread(it,_i2c_, 0,0, 0,0x1c002b, reg+3,1);
	relationread(it,_i2c_, 0,0, 0,0x1c002c, reg+4,1);
	relationread(it,_i2c_, 0,0, 0,0x1c002d, reg+5,1);
	//printf("%02x %02x %02x %02x %02x %02x\n", reg[0],reg[1],reg[2],reg[3],reg[4],reg[5]);

	t = *(short*)(reg+0);
	measure[6] =-t * 400.0 / 32768.0;
	t = *(short*)(reg+2);
	measure[7] =-t * 400.0 / 32768.0;
	t = *(short*)(reg+4);
	measure[8] = t * 400.0 / 32768.0;

	say("%f,%f,%f, %f,%f,%f, %f,%f,%f\n",
		measure[0], measure[1], measure[2],
		measure[3], measure[4], measure[5],
		measure[5], measure[7], measure[8]);
	return 9;
}
int lsm9ds1_i2cinit(void* it)
{
	// enable the 3-axes of the gyroscope
	relationwrite(it,_i2c_, 0,0, 0,0x6a0000|CTRL_REG4, 0,0x38);
	sleep_us(1000);

	// configure the gyroscope
	relationwrite(it,_i2c_, 0,0, 0,0x6a0000|CTRL_REG1_G, 0,(6<<5)|(Gscale<<3)|3);
	sleep_us(200);

	// enable the three axes of the accelerometer 
	relationwrite(it,_i2c_, 0,0, 0,0x6a0000|CTRL_REG5_XL, 0,0x38);
	// configure the accelerometer-specify bandwidth selection with Abw
	relationwrite(it,_i2c_, 0,0, 0,0x6a0000|CTRL_REG6_XL, 0,(6<<5)|(Ascale<<3)|0x04|3);
	sleep_us(200);

	// enable block data update, allow auto-increment during multiple byte read
	relationwrite(it,_i2c_, 0,0, 0,0x6a0000|CTRL_REG8, 0,0x44);

	// configure the magnetometer-enable temperature compensation of mag data
	relationwrite(it,_i2c_, 0,0, 0,0x1c0000|MAG_CTRL_REG1, 0,0x80|(3<<5)|(6<<2)); // select x,y-axis mode
	relationwrite(it,_i2c_, 0,0, 0,0x1c0000|MAG_CTRL_REG2, 0,Mscale<<5); // select mag full scale
	relationwrite(it,_i2c_, 0,0, 0,0x1c0000|MAG_CTRL_REG3, 0,0x00); // continuous conversion mode
	relationwrite(it,_i2c_, 0,0, 0,0x1c0000|MAG_CTRL_REG4, 0,3<<2); // select z-axis mode
	relationwrite(it,_i2c_, 0,0, 0,0x1c0000|MAG_CTRL_REG5, 0,0x40); // select block update mode
	sleep_us(200);
	return 0;
}




int lsm9ds1_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int lsm9ds1_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int ret;
	float tmp[10];
	void* it = (void*)(self->chip);

	if(_clk_ == self->flag){
		ret = lsm9ds1_i2cread(it, tmp);
		relationwrite(it,_dst_, 0,0, 0,0, tmp,ret);
	}
	return 0;
}
int lsm9ds1_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int lsm9ds1_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct driver* it;
	say("@lsm9ds1_linkup\n", &self->flag);

	it = (void*)(self->chip);
	if(0 == it)return 0;

	switch(self->flag){
		case _i2c_:lsm9ds1_i2cinit(it);break;
		case _spi_:break;
	}
	return 0;
}




int lsm9ds1_create(struct driver* ele, u8* url)
{
	say("@lsm9ds1_create\n");
	return 1;
}
int lsm9ds1_delete(struct driver* ele)
{
	return 0;
}
