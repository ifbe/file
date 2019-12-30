#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
double squareroot(double);

#define T 0.001
#define Kp 1.0
#define Ki 0.001

//
static float q[4];
#define qx q[0]
#define qy q[1]
#define qz q[2]
#define qw q[3]
//
float integralx, integraly, integralz;




//https://github.com/PaulStoffregen/MahonyAHRS
static float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	y = y * (1.5f - (halfx * y * y));
	return y;
}
void mahonyupdate6(
	float gx, float gy, float gz,
	float ax, float ay, float az)
{
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Convert gyroscope degrees/sec to radians/sec
	gx *= 0.0174533f;
	gy *= 0.0174533f;
	gz *= 0.0174533f;

	// Compute feedback only if accelerometer measurement valid
	// (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Estimated direction of gravity
		halfvx = qx * qz - qw * qy;
		halfvy = qw * qx + qy * qz;
		halfvz = qw * qw - 0.5f + qz * qz;

		// Error is sum of cross product between estimated
		// and measured direction of gravity
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		if(Ki > 0.0f) {
			// integral error scaled by Ki
			integralx += Ki * halfex * T;
			integraly += Ki * halfey * T;
			integralz += Ki * halfez * T;
			gx += integralx;	// apply integral feedback
			gy += integraly;
			gz += integralz;
		} else {
			integralx = 0.0f;	// prevent integral windup
			integraly = 0.0f;
			integralz = 0.0f;
		}

		// Apply proportional feedback
		gx += Kp * halfex;
		gy += Kp * halfey;
		gz += Kp * halfez;
	}

	// Integrate rate of change of quaternion
	gx *= (0.5f * T);		// pre-multiply common factors
	gy *= (0.5f * T);
	gz *= (0.5f * T);
	qa = qw;
	qb = qx;
	qc = qy;
	qw += (-qb * gx - qc * gy - qz * gz);
	qx += (qa * gx + qc * gz - qz * gy);
	qy += (qa * gy - qb * gz + qz * gx);
	qz += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion
	recipNorm = invSqrt(qw * qw + qx * qx + qy * qy + qz * qz);
	qw *= recipNorm;
	qx *= recipNorm;
	qy *= recipNorm;
	qz *= recipNorm;
}
void mahonyupdate9(
	float gx, float gy, float gz,
	float ax, float ay, float az,
	float mx, float my, float mz)
{
	float norm;
	float qwqw, qwqx, qwqy, qwqz, qxqx, qxqy, qxqz, qyqy, qyqz, qzqz;  
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;


	// Normalise acc, mag
	norm = squareroot(ax * ax + ay * ay + az * az);
	ax /= norm;
	ay /= norm;
	az /= norm;     

	norm = squareroot(mx * mx + my * my + mz * mz);
	mx /= norm;
	my /= norm;
	mz /= norm;   


	// Auxiliary variables to avoid repeated arithmetic
	qwqw = qw * qw;
	qwqx = qw * qx;
	qwqy = qw * qy;
	qwqz = qw * qz;
	qxqx = qx * qx;
	qxqy = qx * qy;
	qxqz = qx * qz;
	qyqy = qy * qy;
	qyqz = qy * qz;
	qzqz = qz * qz;   


	// Reference direction of Earth's magnetic field
	hx = 2.0f * (mx * (0.5f - qyqy - qzqz) + my * (qxqy - qwqz) + mz * (qxqz + qwqy));
	hy = 2.0f * (mx * (qxqy + qwqz) + my * (0.5f - qxqx - qzqz) + mz * (qyqz - qwqx));

	bx = squareroot(hx * hx + hy * hy);
	bz = 2.0f * (mx * (qxqz - qwqy) + my * (qyqz + qwqx) + mz * (0.5f - qxqx - qyqy));


	// Estimated direction of gravity and magnetic field
	halfvx = qxqz - qwqy;
	halfvy = qwqx + qyqz;
	halfvz = qwqw - 0.5f + qzqz;

	halfwx = bx * (0.5f - qyqy - qzqz) + bz * (qxqz - qwqy);
	halfwy = bx * (qxqy - qwqz) + bz * (qwqx + qyqz);
	halfwz = bx * (qwqy + qxqz) + bz * (0.5f - qxqx - qyqy);  


	// Error is sum of cross product between estimated direction and measured direction of field vectors
	halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
	halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
	halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);


	// Compute and apply integral feedback if enabled
	if(Ki > 0.0f)
	{
		// integral error scaled by Ki
		integralx += Ki * halfex * T;
		integraly += Ki * halfey * T;
		integralz += Ki * halfez * T;

		gx += integralx;	// apply integral feedback
		gy += integraly;
		gz += integralz;
	}
	else
	{
		integralx = 0.0f;	// prevent integral windup
		integraly = 0.0f;
		integralz = 0.0f;
	}

	// Apply proportional feedback
	gx += Kp * halfex;
	gy += Kp * halfey;
	gz += Kp * halfez;
	

	// Integrate rate of change of quaternion
	gx *= 0.5f * T;		// pre-multiply common factors
	gy *= 0.5f * T;
	gz *= 0.5f * T;
	qa = qw;
	qb = qx;
	qc = qy;
	qw += (-qb * gx - qc * gy - qz * gz);
	qx += (qa * gx + qc * gz - qz * gy);
	qy += (qa * gy - qb * gz + qz * gx);
	qz += (qa * gz + qb * gy - qc * gx); 
	

	// Normalise quaternion
	norm = squareroot(qw * qw + qx * qx + qy * qy + qz * qz);
	qw /= norm;
	qx /= norm;
	qy /= norm;
	qz /= norm;


	//gx = arctan2(2*(qw*qx+qy*qz),1-2*(qx*qx+qy*qy))*180/3.141592653;
	//gy = arcsin(2*qw*qy - 2*qx*qz)*180/3.141592653;
	//gz = arctan2(2*(qw*qz+qx*qy),1-2*(qy*qy+qz*qz))*180/3.141592653;
	//say("euler:	%f	%f	%f\n", gx, gy, gz);
}




int mahony_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	struct artery* ele;
	say("@mahony_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	relationread(ele, _src_, 0, 0, f, 10);
	return 0;
}
int mahony_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float* f;
	struct artery* ele;
	say("@mahony_write\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	f = (void*)buf;
	switch(len){
		case 9:mahonyupdate9(f[0],f[1],f[2], f[3],f[4],f[5], f[6],f[7],f[8]);break;
		case 6:mahonyupdate6(f[0],f[1],f[2], f[3],f[4],f[5]);break;
		default:say("err@mahony_write:len=%d\n", len);return 0;
	}

	relationwrite(ele, _dst_, 0, 0, q, 4);
	return 0;
}
int mahony_stop(struct halfrel* self, struct halfrel* peer)
{
	say("@mahony_stop\n");
	return 0;
}
int mahony_start(struct halfrel* self, struct halfrel* peer)
{
	say("@mahony_start\n");
	return 0;
}




int mahony_delete(struct artery* ele)
{
	return 0;
}
int mahony_create(struct artery* ele, u8* url)
{
	say("@mahony_create\n");

	qw = 1.0;
	qx = qy = qz = 0.0;
	integralx = integraly = integralz = 0.0;
	return 1;
}
