#include "_outcompare.h"

static void rotat(float Duty,const short MotorType);

static void rotat(float Duty,const short MotorType){		//Duty 100% ~ -100%

	if(Duty < 0){
		Duty = -Duty;
		Duty = Duty/100;
		switch(MotorType){
			case motor1:
				PWM_1B = 0;
				PWM_2B = 1;
			break;
			case motor2:
				PWM_3A = 0;			
				PWM_4A = 1;
			break;
		}
	}else if(Duty >= 0){
		Duty = Duty/100;
		switch(MotorType){
			case motor1:
				PWM_1B = 1;
				PWM_2B = 0;
			break;
			case motor2:
				PWM_3A = 1;
				PWM_4A = 0;
			break;
		}
	}
	switch(MotorType){
		case motor1:
			SetDCOC2PWM(3125*Duty);
		break;
		case motor2:	
			SetDCOC1PWM(3125*Duty);
		break;		
	}
}


Motor MotorInitFunc(void){
	Motor motor;
	static short first = TRUE;
	
	if(first){
		OpenOC2(OC_IDLE_CON & OC_TIMER3_SRC & OC_PWM_FAULT_PIN_DISABLE,0,0);
		OpenOC1(OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE,0,0);
		
		ConfigIntTimer2(T2_INT_PRIOR_0 & T2_INT_OFF);
	
		//10msec(4/80MhzÅ~64Å~3125=10msec)
		OpenTimer2(T2_ON & T2_GATE_OFF & T2_PS_1_64 & T2_SOURCE_INT,3125-1);
		OpenTimer3(T3_ON & T3_GATE_OFF & T3_PS_1_64 & T3_SOURCE_INT,3125-1);
		first = FALSE;
	}
	
	motor.rotat = rotat;

	return motor;
	
}
