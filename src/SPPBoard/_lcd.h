#ifndef _LCD_H
#define _LCD_H

typedef struct{
	void(*data)(const unsigned char asci);
	void(*clear)(void);
	void(*StrPuts)(const char *string);
	void(*NumPuts)(const long number);
}Lcd;

#include "maekawa.h"

Lcd LcdInitFunc(void);

#endif
