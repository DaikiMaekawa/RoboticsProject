#ifndef _UART_H
#define _UART_H

typedef struct{

	void(*SendLineBreak)(const short UartChannel);
	void(*SendString)(const short UartChannel,unsigned int *string);
	void(*WaitSendString)(const short UartChannel);
	void(*WaitGetString)(const short UartChannel);
	char *(*GetStrData)(const short UartChannel);
	char *(*GetMotionCommand)(const short CommandType);
	short(*CheckCommandChanged)(void);

}Uart;

#include "uart.h"
#include "maekawa.h"
#define MOTION_NUM 5

Uart UartInitFunc();

#endif // ifndef _UART_H
