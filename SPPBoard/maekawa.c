#include "maekawa.h"
#include "stdio.h"
#include "string.h"

//void __attribute__((__interrupt__)) _ADCInterrupt(void);

float MotorRotat[2] = {0,0};
volatile int ServoTargetValue;

void machi_usec(int usec){
	usec = (int)(CLOCK*usec)/40;
	while(usec){
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		usec--;
	}
}

void machi_msec(const int msec){
	int i;
	for(i=0; i< msec; i++)
		machi_usec(1000);
}

void ServoInitFunc(void){
	servo(0);

	ConfigIntTimer1(T1_INT_PRIOR_1 & T1_INT_ON);
	//1msec(4/80Mhz×1×200=0.01msec)
	OpenTimer1(T1_ON & T1_GATE_OFF & T1_PS_1_1 & T1_SYNC_EXT_OFF & T1_SOURCE_INT,200-1);
}

void servo(int angle){
	if(angle < -90){
		angle = -90;
	}
	if(angle > 90){
		angle = 90;
	}
	angle += 90;
	angle += 55;

	ServoTargetValue = angle;
	
}

void SensorInitFunc(void){

	unsigned int ADconfig1 = ADC_MODULE_ON & ADC_IDLE_CONTINUE & ADC_FORMAT_INTG &     
						 ADC_CLK_AUTO & ADC_AUTO_SAMPLING_OFF & ADC_SAMP_OFF;					
	//ADC_VREF_AVDD_AVSS()					 
	unsigned int ADconfig2 = ADC_VREF_AVDD_AVSS & ADC_SCAN_OFF & ADC_SAMPLES_PER_INT_1 & 	
							ADC_ALT_BUF_OFF & ADC_ALT_INPUT_OFF;
	
	unsigned int ADconfig3 = ADC_SAMPLE_TIME_8 & ADC_CONV_CLK_SYSTEM & ADC_CONV_CLK_32Tcy;
	unsigned int ADconfigPort = ENABLE_AN0_ANA & ENABLE_AN1_ANA & ENABLE_AN2_ANA;
	unsigned int ADconfigScan = 0x000;

	//AD initialize
	OpenADC12(ADconfig1,ADconfig2,ADconfig3,ADconfigPort,ADconfigScan);

}

void SensorInitFunc2(void){
	unsigned int ADconfig1 = ADC_MODULE_ON & ADC_IDLE_STOP & ADC_FORMAT_INTG &     
						 ADC_CLK_TMR & ADC_AUTO_SAMPLING_ON & ADC_SAMP_OFF;					
	//ADC_VREF_AVDD_AVSS()					 
	unsigned int ADconfig2 = ADC_VREF_AVDD_AVSS & ADC_SCAN_ON & ADC_SAMPLES_PER_INT_3 & 	
							ADC_ALT_BUF_OFF & ADC_ALT_INPUT_OFF;
	
	unsigned int ADconfig3 = ADC_SAMPLE_TIME_2 & ADC_CONV_CLK_SYSTEM & ADC_CONV_CLK_2Tcy;
	unsigned int ADconfigPort = ENABLE_AN0_ANA & ENABLE_AN1_ANA & ENABLE_AN2_ANA;
	unsigned int ADconfigScan = 0x0007;

	//AD initialize
	OpenADC12(ADconfig1,ADconfig2,ADconfig3,ADconfigPort,ADconfigScan);
	
/*
	ADCON1bits.FORM = 3;
    ADCON1bits.SSRC = 7;
    ADCON1bits.ASAM = 1;
    //ADCON1bits.SIMSAM = 0;
	ADCON2bits.SMPI = 1;
	//ADCON2bits.CHPS = 1;
	ADCON3bits.SAMC = 0;
	ADCON3bits.ADCS = 4;	//7Tcy
	//        |----- (CH0のMUXB-入力指定:Vref--),(CH123のMUXBの+入力指定:CH1-AN0,CH2-AN1,CH3-AN2),CH123のMUXBの-入力指定:CH123-Vref
	//        ||---- AN3をCH0のMUXBの+入力に指定
	//		  |||--- (CH0のMUXA-入力指定:Vref--),(CH123のMUXAの+入力指定:CH1-AN0,CH2-AN1,CH3-AN2),CH123のMUXAの-入力指定:CH123-Vref	
	//		　 ↓↓↓↓-- AN3をCH0のMUXAの+入力に指定
	ADCHS = 0x0003;
	
	ADCSSL = 0x0000; //チャネル0の自動スキャンに含める入力:なし
	ADPCFG = 0xFFFF;
	//ADPCFGbits.PCFG3 = 0;
    IFS0bits.ADIF = 0;
    IEC0bits.ADIE = 1;
    ADCON1bits.ADON = 1;
*/
}

/*
int ADResult1 = 0;
int ADResult2 = 0;
int ADResult3 = 0;

void __attribute__((__interrupt__)) __ADCInterrupt(void){	
	IFS0bits.ADIF = 0;
	
	ADResult1 = ReadADC12(0);
	ADResult2 = ReadADC12(1);
	ADResult3 = ReadADC12(2);
}
*/

unsigned int GetSensorValue(const short ADChannel){
	
	unsigned int Channel0 = ADC_CH0_POS_SAMPLEA_AN0 & ADC_CH0_NEG_SAMPLEA_NVREF;
	unsigned int Channel1 = ADC_CH0_POS_SAMPLEA_AN1 & ADC_CH0_NEG_SAMPLEA_NVREF;
	unsigned int Channel2 = ADC_CH0_POS_SAMPLEA_AN2 & ADC_CH0_NEG_SAMPLEA_NVREF;
	unsigned int ADvalue[3],i,ave=0;
	
	for(i=0;i<20;i++){
		switch(ADChannel){
			case ch0:
				SetChanADC12(Channel0);
			break;
			case ch1:
				SetChanADC12(Channel1);							
			break;
			case ch2:
				SetChanADC12(Channel2);
			break;
		}
		ADCON1bits.SAMP = 1;
		while(ADCON1bits.DONE == 0);
		ADvalue[i] = ReadADC12(0);
		ave += ADvalue[i];
	}
		ave /= 20;
	
	if(ave <= 200){		
		ave = 0;
	}
	
	return ave;
}

Device DeviceInitFunc(void){
	Device device;
	TRISA = 0x800;			//0000 1000 0000 0000
	TRISB = 0x1C7; 			//0000 0001 1100 0111
	TRISC = 0x6000;			//0110 0000 0000 0000
	TRISD = 0x200;			//0000 0010 0000 0000
//	TRISF = 0x0010;
	
	SensorInitFunc();
	ServoInitFunc();
	device.lcd   = LcdInitFunc();
	device.uart  = UartInitFunc();
	device.robot = RobotInitFunc();
	device.i2c   = I2CInitFunc();
	device.motor = MotorInitFunc();
	device.imu   = IMUInitFunc();
	
	device.lcd.clear();
	device.motor.rotat(0,motor1);
	device.motor.rotat(0,motor2);
	
	INTCON2 = 0;       
    IEC0bits.INT0IE = 1;    /*Enable INT0 Interrupt Service Routine */
    IEC1bits.INT2IE = 1;    /*Enable INT2 Interrupt Service Routine */

	return device;
}

Robot RobotInitFunc(void){
	Robot robot;
	
	robot.right.TargetSpd = 0;
	robot.left.TargetSpd = 0;
	robot.right.Kp = 75.09f;
	robot.left.Kp = 170.00f;
	robot.right.Ki = 0.06f;
	robot.left.Ki = 0.06f;
	robot.right.MotorSpd = robot.left.MotorSpd = 0;
	robot.right.WheelSpd = robot.left.WheelSpd = 0;
	robot.right.ErrSpd = robot.left.ErrSpd = 0;
	robot.right.IErrSpd = robot.left.IErrSpd = 0;
	robot.right.OldSpd = robot.left.OldSpd = 0;	

	robot.time = 100;
	robot.MotorControlFlag = TRUE;	

	return robot;
}

unsigned int TimeCount = 0;

void _ISR _T1Interrupt(void){
	IFS0bits.T1IF = 0;
	TimeCount++;
					
	if(TimeCount < ServoTargetValue){
			SERVO = 1;
	}else if(TimeCount <= 2000){
			SERVO = 0;
	}else{
		TimeCount = 0;
	}
}

void _ISR _T2Interrupt(void){
	IFS0bits.T2IF = 0;
}

float GetMotorSpd(const short MotorType,float time){

	float MotorSpd = (MotorRotat[MotorType] / time);
	
	MotorRotat[MotorType] = 0;
	return MotorSpd;	
}
 
//motor2-b
void __attribute__((__interrupt__)) _INT2Interrupt(void)
{
	if((ENC_2A == 1 && ENC_2B == 1) || (ENC_2A == 0 && ENC_2B == 0)){
		MotorRotat[motor2] += 0.07853981; 		
	}else{
		MotorRotat[motor2] -= 0.07853981;
	}
	if(MotorRotat[motor2] >= 1000.0f) MotorRotat[motor2] = 0.0f;
	IFS1bits.INT2IF = 0;    
}		

//motor1-b
void __attribute__((__interrupt__)) _INT0Interrupt(void)
{
	if((ENC_1A == 1 && ENC_1B == 1) || (ENC_1A == 0 && ENC_1B == 0)){
		MotorRotat[motor1] += 0.07853981;
	}else{
		MotorRotat[motor1] -= 0.07853981;
	}
	if(MotorRotat[motor1] >= 1000.0f) MotorRotat[motor1] = 0.0f;
	IFS0bits.INT0IF = 0;    
}

void MotorControler(Robot *robot,Motor *motor){

	if(robot->right.OldSpd != robot->right.TargetSpd)
		robot->right.IErrSpd = 0;	

	if(robot->left.OldSpd != robot->left.TargetSpd)	
		robot->left.IErrSpd = 0;
	
	robot->right.MotorSpd = -GetMotorSpd(motor2,(float)robot->time);	//[rad/ms]
	robot->left.MotorSpd = GetMotorSpd(motor1,(float)robot->time);	
	
	robot->right.WheelSpd = 0.6f * robot->right.WheelSpd + 0.4f * robot->right.MotorSpd * 25.0f;//[mm/ms]
	
	if(robot->MotorControlFlag){
		robot->right.ErrSpd = robot->right.TargetSpd - robot->right.WheelSpd;
		robot->right.IErrSpd += robot->right.Ki * robot->right.ErrSpd;
		if(robot->right.IErrSpd > 100.0f)
			robot->right.IErrSpd = 100.0f;
		else if(robot->right.IErrSpd < -100.0f)
			robot->right.IErrSpd = -100.0f;
	
		if((robot->right.MotorSpd < 0.1 && robot->right.MotorSpd > -0.1) && (robot->right.IErrSpd < 10 && robot->right.IErrSpd > -10)){
			motor->rotat((int)(robot->right.Kp * robot->right.ErrSpd),motor2);
		}else{
			motor->rotat((int)(robot->right.IErrSpd + robot->right.Kp * robot->right.ErrSpd),motor2);
		}
	}
	robot->left.WheelSpd = 0.6f * robot->left.WheelSpd + 0.4f *  robot->left.MotorSpd * 25.0f;
	if(robot->MotorControlFlag){
		robot->left.ErrSpd = robot->left.TargetSpd - robot->left.WheelSpd;
		robot->left.IErrSpd += robot->left.Ki * robot->left.ErrSpd;
		if(robot->left.IErrSpd > 100.0f)
			robot->left.IErrSpd = 100.0;
		else if(robot->left.IErrSpd < -100.0f)
			robot->left.IErrSpd = -100.0;

		if((robot->left.MotorSpd < 0.1 && robot->left.MotorSpd > -0.1) && (robot->left.IErrSpd < 10 && robot->left.IErrSpd > -10)){
			motor->rotat((int)(robot->left.Kp * robot->left.ErrSpd),motor1);
		}else{
			motor->rotat((int)(robot->left.IErrSpd + robot->left.Kp * robot->left.ErrSpd),motor1);
		}
	}
	
	robot->right.OldSpd = robot->right.TargetSpd;
	robot->left.OldSpd = robot->left.TargetSpd;
	machi_msec(robot->time);
}


