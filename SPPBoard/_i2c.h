#ifndef _I2C_H
#define _I2C_H

#define GYRO 0x68         
#define INT_MAX             32767  
#define ANGLE_RATE_MAX      250

typedef unsigned char I2CData;

enum Axes{
	X,	
	Y,
	Z
};	

typedef struct{
	double NewGyro[3];
	double OldGyro[3];
	double OffsetGyro[3];
	double angle[3];
	
}IMU;

typedef struct{
	void(*close)(void);
	void(*write)(I2CData address,I2CData data);
	void(*SendDataToIMU)(I2CData I2CAddress,I2CData RegisterAddress,I2CData data);
	I2CData(*read)(I2CData address);
	I2CData(*ReadDataFromIMU)(I2CData I2CAddress,I2CData RegisterAddress);
	void(*CalibrationIMU)(IMU *imu);
	void(*UpdateIMUStatus)(IMU *imu,const unsigned int time);
	
}I2C;

#include "i2c.h"
#include "maekawa.h"

I2C I2CInitFunc(void);
IMU IMUInitFunc(void);

#endif

