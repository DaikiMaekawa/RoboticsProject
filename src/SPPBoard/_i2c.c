#include "i2c.h"
#include "_i2c.h"
#include "_lcd.h"

#define IDLE_I2C do{ }while(I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN || I2CCONbits.ACKEN || I2CSTATbits.TRSTAT)
#define NACK_I2C do{I2CCONbits.ACKDT = 1; I2CCONbits.ACKEN = 1;}while(0)

static void    _CloseI2C(void);
static void    WriteI2C(I2CData address,I2CData data);
static I2CData ReadI2C(I2CData address);
static void    SendDataToIMU(I2CData I2CAddress,I2CData RegisterAddress,I2CData data);
static I2CData ReadDataFromIMU(I2CData I2CAddress,I2CData RegisterAddress);
static void    SendDataI2C(I2CData data);
static I2CData ReadDataI2C(void);
static void    SendAddressI2C(I2CData address,char AddressType);
static void    CalibrationIMU(IMU *imu);
static void    UpdateIMUStatus(IMU *imu,const unsigned int time);
enum AddressType{SEND, READ};

I2C I2CInitFunc(void){
	I2C i2c;
	
	unsigned int config1 = MI2C_INT_OFF & SI2C_INT_OFF;
	unsigned int config2 = 399;
	static short first = TRUE;
	
	if(first){
		
		I2CBRG = config2;
		
		I2CCONbits.I2CSIDL = 0;
		I2CCONbits.SCLREL  = 1;
		I2CCONbits.IPMIEN  = 0;
		I2CCONbits.A10M    = 0;
		I2CCONbits.DISSLW  = 1;
		I2CCONbits.SMEN    = 0;
		I2CCONbits.GCEN    = 0;
		I2CCONbits.STREN   = 1;	
		I2CCONbits.ACKDT   = 0;
		I2CCONbits.ACKEN   = 0;
		I2CCONbits.RCEN    = 1;
		I2CCONbits.PEN     = 1;
		I2CCONbits.RSEN    = 0;
		I2CCONbits.SEN     = 1;
		I2CCONbits.I2CEN   = 1;

		IDLE_I2C;
		I2CADD = 0x09;
		
		//ConfigIntI2C(MI2C_INT_OFF & SI2C_INT_OFF);
		_SI2CIF = 0;
	    _MI2CIF = 0;
	    _SI2CIP = (config1 & 0x0007);
	    _MI2CIP = (config1 & 0x0070) >> 4;  
	    _SI2CIE = (config1 & 0x0008) >> 3;  
	    _MI2CIE = (config1 & 0x0080) >> 7;	
		
		SendDataToIMU(GYRO,0x16,0x0);
		SendDataToIMU(GYRO,0x3D,0x28);			
		
		first = FALSE;
	}

	//set address
	i2c.close           = _CloseI2C;
	i2c.write           = WriteI2C;
	i2c.read            = ReadI2C;
	i2c.SendDataToIMU   = SendDataToIMU;
	i2c.ReadDataFromIMU = ReadDataFromIMU;
	i2c.CalibrationIMU  = CalibrationIMU;
	i2c.UpdateIMUStatus = UpdateIMUStatus;
	
	return i2c;	
}

static void _CloseI2C(void)
{ 
	I2CCONbits.I2CEN = 0;

	_SI2CIE = 0;
	_MI2CIE = 0;
 
	_SI2CIF = 0;
	_MI2CIF = 0;
}

static void SendDataToIMU(I2CData I2CAddress,I2CData RegisterAddress,I2CData data){
	
	I2CCONbits.SEN = 1;
	IDLE_I2C;
	
	SendAddressI2C(I2CAddress,SEND);
	SendDataI2C(RegisterAddress);
	SendDataI2C(data);
	
	I2CCONbits.PEN = 1;
	IDLE_I2C;
	
}

static void SendAddressI2C(I2CData address,char AddressType){
	address = address << 1;
	
	if(AddressType == SEND){
		address &= 0b11111110;
	}else{
		address |= 0b00000001;
	}
	SendDataI2C(address);
}	

static void SendDataI2C(I2CData data){
	
	I2CTRN = data;
	if(I2CSTATbits.IWCOL){        
    	
 	}else{
        while(I2CSTATbits.TRSTAT);   
        IDLE_I2C;                  
    }
	
	while(I2CSTATbits.TBF);
	while(I2CSTATbits.ACKSTAT);
	IDLE_I2C;
}

static I2CData ReadDataI2C(void){
	I2CCONbits.RCEN = 1;
	while(I2CCONbits.RCEN);
	I2CSTATbits.I2COV = 0;
	return (I2CRCV);
}	

static void WriteI2C(I2CData address,I2CData data){
	
	I2CCONbits.SEN = 1;
	IDLE_I2C;
	
	SendAddressI2C(address,SEND);
	SendDataI2C(data);
	
	I2CCONbits.PEN = 1;
	IDLE_I2C;
}

static I2CData ReadDataFromIMU(I2CData I2CAddress,I2CData RegisterAddress){
	I2CData buffer;
	
	I2CCONbits.SEN = 1;
	IDLE_I2C;
	SendAddressI2C(I2CAddress,SEND);
	SendDataI2C(RegisterAddress);
	
	I2CCONbits.RSEN = 1;
	while(I2CCONbits.RSEN);
	
	SendAddressI2C(I2CAddress,READ);
	//buffer = MasterReadI2C();
	buffer = ReadDataI2C();
	while(I2CSTATbits.ACKSTAT);
	IDLE_I2C;
	
    I2CCONbits.ACKDT = 1;
    I2CCONbits.ACKEN = 1;
	IDLE_I2C;
	I2CCONbits.PEN = 1;
	IDLE_I2C;
	
	return buffer;
	
}

static I2CData ReadI2C(I2CData address){
	unsigned char received_data;
	
	I2CCONbits.SEN = 1;
	IDLE_I2C;
	
	SendAddressI2C(address,READ);
	//received_data = MasterReadI2C();
	received_data = ReadDataI2C();
	IDLE_I2C;
	
	I2CCONbits.ACKDT = 1;
	I2CCONbits.ACKEN = 1;
	IDLE_I2C;
	
	I2CCONbits.PEN = 1;
	IDLE_I2C;
	
	return received_data;
} 

static void CalibrationIMU(IMU *imu){
	int buffer[6],i;
	
	for(i=0;i < 156;i++){
		buffer[0] = (int)ReadDataFromIMU(GYRO,0x1D);
		buffer[1] = (int)ReadDataFromIMU(GYRO,0x1E);
		buffer[2] = (int)ReadDataFromIMU(GYRO,0x1F);
		buffer[3] = (int)ReadDataFromIMU(GYRO,0x20);
		buffer[4] = (int)ReadDataFromIMU(GYRO,0x21);
		buffer[5] = (int)ReadDataFromIMU(GYRO,0x22);
		
		imu->OffsetGyro[X] += (double)(buffer[0] << 8 | buffer[1]);
		imu->OffsetGyro[Y] += (double)(buffer[2] << 8 | buffer[3]);
		imu->OffsetGyro[Z] += (double)(buffer[4] << 8 | buffer[5]);
		
	}
	
	imu->OffsetGyro[X] /= 156.0;
	imu->OffsetGyro[Y] /= 156.0;
	imu->OffsetGyro[Z] /= 156.0;
	
}	

static void UpdateIMUStatus(IMU *imu,const unsigned int time){ 	//time[msec]
	int buf[6];
	double angle;
	
	buf[0] = (int)ReadDataFromIMU(GYRO,0x1D);
	buf[1] = (int)ReadDataFromIMU(GYRO,0x1E);
	buf[2] = (int)ReadDataFromIMU(GYRO,0x1F);
	buf[3] = (int)ReadDataFromIMU(GYRO,0x20);
	buf[4] = (int)ReadDataFromIMU(GYRO,0x21);
	buf[5] = (int)ReadDataFromIMU(GYRO,0x22);
	imu->NewGyro[X] = (buf[0] << 8 | buf[1]) - imu->OffsetGyro[X];
	imu->NewGyro[Y] = (buf[2] << 8 | buf[3]) - imu->OffsetGyro[Y];
	imu->NewGyro[Z] = (buf[4] << 8 | buf[5]) - imu->OffsetGyro[Z];
	
	angle = ((imu->NewGyro[X] + imu->OldGyro[X]) * (double)ANGLE_RATE_MAX * (double)time) / (double)INT_MAX / 1000.0;
	imu->angle[X] += angle >= -0.009 && angle <= 0.009 ? 0.0 : angle; 
	
	angle = ((imu->NewGyro[Y] + imu->OldGyro[Y]) * (double)ANGLE_RATE_MAX * (double)time) / (double)INT_MAX / 1000.0;
	imu->angle[Y] += angle >= -0.009 && angle <= 0.009 ? 0.0 : angle;
	
	angle = ((imu->NewGyro[Z] + imu->OldGyro[Z]) * (double)ANGLE_RATE_MAX * (double)time) / (double)INT_MAX / 1000.0;
	imu->angle[Z] += angle >= -0.009 && angle <= 0.009 ? 0.0 : angle;
	
	imu->OldGyro[X] = imu->NewGyro[X];
	imu->OldGyro[Y] = imu->NewGyro[Y];
	imu->OldGyro[Z] = imu->NewGyro[Z];
	
}

IMU IMUInitFunc(void){
	IMU imu;
	int i;
	
	for(i=0;i < 3;i++){
		imu.NewGyro[i]    = 0.0;
		imu.OldGyro[i]    = 0.0;
		imu.OffsetGyro[i] = 0.0;
		imu.angle[i]      = 0.0;
	}
	
	return imu;
}	

