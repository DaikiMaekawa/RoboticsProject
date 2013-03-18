/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file maekawa.h
 * @brief API for SPPBoard
 * @author Daiki Maekawa
 * @date 2012-04-28
 *******************************************************************/

#ifndef MAEKAWA_H
#define MAEKAWA_H

#include "p30F3014.h"
#include "_outcompare.h"
#include "timer.h"
#include "adc12.h"
#include "_uart.h"
#include "_i2c.h"
#include "_lcd.h"

typedef struct{
	float TargetSpd;
	float MotorSpd;
	float WheelSpd;
	float ErrSpd;
	float IErrSpd;
	float OldSpd;
	float Kp,Ki;
}Move;

typedef struct{
	Move right;
	Move left;
	int time;
	short MotorControlFlag;
	short ObsAvoidFlag;
	short AutoDriveFlag;
}Robot;

typedef struct{
	Lcd lcd;
	Motor motor;
	I2C i2c;
	Uart uart;
	Robot robot;
	IMU   imu;
}Device;

#define UART_RX_TX              0xFBE7
#define UART_ALTRX_ALTTX        0xFFE7
#define TRUE 1
#define FALSE 0
#define LCD_RS LATBbits.LATB5		
#define LCD_E  LATBbits.LATB4		
#define LCD_D4 LATBbits.LATB9
#define LCD_D5 LATBbits.LATB10
#define LCD_D6 LATBbits.LATB11
#define LCD_D7 LATBbits.LATB12
#define CHECK_SENSOR_VALUE(SensorValue)(((SensorValue < 1200) ? TRUE : FALSE))
#define CLOCK 80
#define MAX_UARTWORD 20
#define PWM_1A_AND_2A LATDbits.LATD1
#define PWM_1B LATFbits.LATF0
#define PWM_2B LATFbits.LATF1
#define PWM_3A LATDbits.LATD2
#define PWM_4A LATFbits.LATF6
#define SERVO LATBbits.LATB3
#define ENC_1A PORTCbits.RC13
#define ENC_1B PORTAbits.RA11
#define ENC_2A PORTCbits.RC14
#define ENC_2B PORTDbits.RD9

void machi_usec(int usec);
void machi_msec(const int msec);
void SensorInitFunc(void);
void SensorInitFunc2(void);
unsigned int GetSensorValue(const short ADChannel);
Device DeviceInitFunc(void);
Robot RobotInitFunc(void);
void ServoInitFunc(void);
void servo(int angle);
float GetMotorSpd(const short MotorType,float time);
void MotorControler(Robot *robot,Motor *motor);

enum MotionType{normal,reverse};
enum MotorType{motor1,motor2};
enum ADChannel{ch0,ch1,ch2};
enum UartChannel{uart1,uart2};
enum MotionCommandType{type1,type2,type3,type4,type5};
enum SendSpdType{WHEEL,MOTOR,ERR,IERR,NONE,BUT};


#endif		//MAEKAWA_H
