
#ifndef _OUTCOMPARE_H
#define _OUTCOMPARE_H

typedef struct{

	void(*rotat)(float Duty,const short MotorType);

}Motor;

#include "outcompare.h"
#include "maekawa.h"

Motor MotorInitFunc(void);

#endif //_OUTCOMPARE_H
