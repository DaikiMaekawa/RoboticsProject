#include "_lcd.h"

static void LcdData(const unsigned char asci);
static void LcdClear(void);
static void LcdStrPuts(const char *string);
static void LcdNumPuts(const long number);
static void LcdOut4(int rs,unsigned char dat);
static void LcdOut8(unsigned char dat);


static void LcdData(const unsigned char asci){
	
	LcdOut4(1,asci);
	machi_usec(50);

}

static void LcdClear(void){

	LcdOut4(0,0x01);
	machi_usec(1650);

}

Lcd LcdInitFunc(void){
	Lcd lcd;
	static short first = TRUE;
	
	if(first){
		machi_msec(20);
		LcdOut8(0x23);
		machi_msec(10);
		LcdOut8(0x23);
		machi_msec(10);
		LcdOut8(0x23);
		LcdOut8(0x22);
		LcdOut4(0,0x28);  
		LcdOut4(0,0x0E);
		LcdOut4(0,0x06);
		LcdOut4(0,0x02);
		
		first = FALSE;
	}

	lcd.data = LcdData;
	lcd.clear = LcdClear;
	lcd.StrPuts = LcdStrPuts;
	lcd.NumPuts = LcdNumPuts;
	
	return lcd;
}

static void LcdStrPuts(const char *string){
	while(*string != '\0'){	
		LcdOut4(1,*string);		
		string++;				
	}
}

static void LcdNumPuts(const long number){
	  long j;
	  int k;
	  char s[6];
	  k=0;
	
	if(number == 0)
		LcdOut4(1,'0');
	if(number >= 0) 
		j=number;
	else{
	    j=-number;
	    LcdOut4(1,'-');
	}
	while(0<j){
	     s[k++]=j-((long)(j/10)*10);
	     j=(long)j/10;
	}
	while(k!=0){
	  k-=1;
		LcdOut4(1,'0'+s[k]);
	}
}

static void LcdOut8(unsigned char dat){

	LCD_RS = 0;
	LCD_E = 1;
	LCD_D4 = (dat & 0x01);
	dat = dat >> 1;
	LCD_D5 = (dat & 0x01);
	dat = dat >> 1;
	LCD_D6 = (dat & 0x01);
	dat = dat >> 1;
	LCD_D7 = (dat & 0x01);
	
	machi_usec(50);
	LCD_E = 0;

	//wait 50us
	machi_usec(50);

}

static void LcdOut4(int rs,unsigned char dat){
	unsigned char bk = dat;

	LCD_RS = rs;
	LCD_E = 1;

	bk = bk >> 4;
	LCD_D4 = (bk & 0x01);
	bk = bk >> 1;
	LCD_D5 = (bk & 0x01);
	bk = bk >> 1;
	LCD_D6 = (bk & 0x01);
	bk = bk >> 1;
	LCD_D7 = (bk & 0x01);

	machi_usec(50);
	LCD_E = 0;
	machi_usec(50);
	LCD_E = 1;

	LCD_D4 = (dat & 0x01);
	dat = dat >> 1;
	LCD_D5 = (dat & 0x01);
	dat = dat >> 1;
	LCD_D6 = (dat & 0x01);
	dat = dat >> 1;
	LCD_D7 = (dat & 0x01);

	machi_usec(50);
	LCD_E = 0;
	machi_usec(50);
	LCD_RS = 0;
}
