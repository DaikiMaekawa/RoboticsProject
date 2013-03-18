#include "_uart.h"
#include "string.h"

static void SendLineBreak(const short UartChannel);
static void SendString(const short UartChannel,unsigned int *string);
static void WaitSendString(const short UartChannel);
static void WaitGetString(const short UartChannel);
static char *GetStrData(const short UartChannel);
static char *GetMotionCommand(const short CommandType);
static short CheckCommandChanged(void);
static volatile unsigned short CommandChanged = FALSE;
char MotionCommand[MOTION_NUM][MAX_UARTWORD];


static void SendLineBreak(const short UartChannel){
	SendString(UartChannel,(unsigned int *)"\r");
}

static void SendString(const short UartChannel,unsigned int *string){
	switch(UartChannel){
		case uart1:
			putsUART1(string);
		break;
		case uart2:
			putsUART2(string);
		break;
	}
	WaitSendString(UartChannel);
}

static void WaitSendString(const short UartChannel){
	switch(UartChannel){
		case uart1:
			while(BusyUART1());
		break;
		case uart2:
			while(BusyUART2());
		break;
	}	
}


static void WaitGetString(const short UartChannel){
	switch(UartChannel){
		case uart1:
			while(!DataRdyUART1());
		break;
		case uart2:
			while(!DataRdyUART2());
		break;
	}
}

static char *GetStrData(const short UartChannel){
	unsigned char moji;
	int count = 0;
	static char string[MAX_UARTWORD];
	
	WaitGetString(UartChannel);
	switch(UartChannel){
		case uart1:
			moji = ReadUART1();
		break;
		case uart2:
			moji = ReadUART2();
		break;
	}

	while((moji != '\r') && (count < MAX_UARTWORD)){	
		string[count] = moji;
		count++;
		WaitGetString(UartChannel);
		switch(UartChannel){
			case uart1:
				moji = ReadUART1();
			break;
			case uart2:
				moji = ReadUART2();
			break;
		}
	}
	string[count] = 0x00;
	return string;
}

static char *GetMotionCommand(const short CommandType){	
	CommandChanged = FALSE;
	return &MotionCommand[CommandType][0];	
}

static short CheckCommandChanged(void){
	
	return CommandChanged;
}

Uart UartInitFunc(void){
	Uart uart;
	int i;
	unsigned int config1 = UART_EN & UART_IDLE_CON & UART_RX_TX & UART_NO_PAR_8BIT & UART_1STOPBIT
							& UART_DIS_WAKE & UART_DIS_LOOPBACK & UART_DIS_ABAUD;
	unsigned int config2 = UART_INT_TX_BUF_EMPTY & UART_TX_PIN_NORMAL & UART_TX_ENABLE & UART_INT_RX_CHAR
							& UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR;
	static short first = TRUE;
	
	if(first){
		OpenUART2(config1,config2,10);	//115kbps
		
		//ConfigIntUART1(UART_RX_INT_DIS & UART_TX_INT_DIS);
		ConfigIntUART2(UART_RX_INT_EN & UART_TX_INT_DIS);
		
		for(i=0;i < MOTION_NUM;i++){
			memset(GetMotionCommand(i),0,sizeof(GetMotionCommand(i)));
		}
		first = FALSE;	
	}
	uart.SendLineBreak = SendLineBreak;
	uart.SendString = SendString;
	uart.WaitSendString = WaitSendString;
	uart.WaitGetString = WaitGetString;
	uart.GetStrData = GetStrData;
	uart.GetMotionCommand = GetMotionCommand;
	uart.CheckCommandChanged = CheckCommandChanged;
	
	return uart;
}

void __attribute__((interrupt,auto_psv))_U1RXInterrupt(void){
	IFS0bits.U1RXIF = 0;

}

void __attribute__((interrupt,auto_psv))_U2RXInterrupt(void){
	char moji;
	static int i = 0;
	static int CommandType = type1;
	
	IFS1bits.U2RXIF = 0;		
	moji = ReadUART2();

	if((moji != '\r') && (i < MAX_UARTWORD) &&(moji != ',')){
		MotionCommand[CommandType][i] = moji;
		i++;
	}else if(moji == ','){
		MotionCommand[CommandType][i] = 0x00;
		i=0;
		CommandType++;
	}else{
		MotionCommand[CommandType][i] = 0x00;
		i = 0;
		CommandType = type1;
		CommandChanged = TRUE;
	}
}



