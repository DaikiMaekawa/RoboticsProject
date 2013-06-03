#ifndef SPPCOMMAND_H
#define SPPCOMMAND_H

typedef enum{
	COMMAND_CANCEL  ,     
	GET_MOTOR_SPD   ,     
	SET_MOTOR_GAIN  ,     
	SET_MOTOR_DUTY  ,     
	SET_SERVO_ANGLE ,   
	CALIBRATION_GYRO,    
	LCD_PUTS        ,    
	LCD_CLEAR       ,
	GET_GYRO        ,
	GET_GYRO_X      ,
	GET_GYRO_Y      ,
	GET_GYRO_Z      ,
}SPPCommand;

#endif //SPPCOMMAND_H
