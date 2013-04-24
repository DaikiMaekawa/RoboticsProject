/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file main.h
 * @brief Implementation of SPPCommand
 * @author Daiki Maekawa
 * @date 2012-04-28
 *******************************************************************/

#include "maekawa.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "SPPCommand.h"
#define  CONTROL_CYCLE 10 //msec

_FOSC(CSW_FSCM_OFF & XT_PLL8);
_FWDT(WDT_OFF);
_FBORPOR(PBOR_ON & BORV_20 & PWRT_64 & MCLR_EN);
_FGS(CODE_PROT_OFF);

void SPPCommandSystem(Device *device,unsigned int time);

int main(void){
	
	Device device = DeviceInitFunc();
	machi_msec(1000);
	device.i2c.CalibrationIMU(&device.imu);
	
	while(1){
		if(device.uart.CheckCommandChanged()) SPPCommandSystem(&device,CONTROL_CYCLE);
		device.i2c.UpdateIMUStatus(&device.imu,CONTROL_CYCLE);
		machi_msec(CONTROL_CYCLE);
	}
	
	return 0;
}

void SPPCommandSystem(Device *device,unsigned int time){
	char Command[MAX_UARTWORD];	
	char SendGyro_X[MAX_UARTWORD];
	char SendGyro_Y[MAX_UARTWORD];
	char SendGyro_Z[MAX_UARTWORD];
	strcpy(Command,device->uart.GetMotionCommand(type1));
	switch(atoi(Command)){
		case COMMAND_CANCEL:
			device->motor.rotat(0,motor2);
			device->motor.rotat(0,motor1);
		break;
		case GET_MOTOR_SPD:
		break;
		case SET_MOTOR_GAIN:
		break;
		case SET_MOTOR_DUTY:
			device->motor.rotat(atoi(device->uart.GetMotionCommand(type3)),atoi(device->uart.GetMotionCommand(type2)));
			//device->motor.rotat(atoi(device->uart.GetMotionCommand(type3)),motor1);
		break;
		case SET_SERVO_ANGLE:
			servo(atoi(device->uart.GetMotionCommand(type2)));
			device->lcd.StrPuts(device->uart.GetMotionCommand(type2));
		break;
		case CALIBRATION_GYRO:
			device->i2c.CalibrationIMU(&device->imu);
		break;
		case LCD_PUTS:
			device->lcd.StrPuts(device->uart.GetMotionCommand(type2));
		break;
		case LCD_CLEAR:
			device->lcd.clear();
		break;
		case GET_GYRO:
			sprintf(SendGyro_X,"%d,%f\r",GET_GYRO_X,device->imu.angle[X]);
			sprintf(SendGyro_Y,"%d,%f\r",GET_GYRO_Y,device->imu.angle[Y]);
			sprintf(SendGyro_Z,"%d,%f\r",GET_GYRO_Z,device->imu.angle[Z]);
			device->uart.SendString(uart2,(unsigned int*)SendGyro_X);
			device->uart.SendString(uart2,(unsigned int*)SendGyro_Y);
			device->uart.SendString(uart2,(unsigned int*)SendGyro_Z);
		break;
	}
}	








