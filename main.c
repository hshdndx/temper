/*
 * main.c
 */

#include "msp430f5529.h"
#include "HAL_Dogs102x6.h"
#include "config.h"
#include <stdio.h>
//using namespace std;

#define uchar unsigned char
#define uint unsigned int

#define DQ_OUT  P8DIR|=BIT0;
#define DQ_IN   P8DIR&= ~BIT0;

#define ISD_OUT P8DIR|=BIT1;
#define ISD_IN  P8DIR&= ~BIT1;

#define ISDP_IN  P8DIR&= ~BIT2;
#define ISDP_OUT P8DIR|=BIT2;

//uint temp;
//unsigned char table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//unsigned char table1[] = {0xbb,0x09,0xea,0x6b,0x59,0x73,0xf3,0x0b,0xfb,0x7b};


volatile unsigned int result = 0;
//volatile float result1 = 0.0;
char t[6];

void InitLcd(void);
void InitAll(void);
void Delay400ms(void);
void Delay(unsigned char i);
void StartAD(void);
void SendtoBluetooth(void);
void SendtoMobile();
void Sounds(unsigned int);
void Delay1s(void);
void ADC12Init(void);
void UARTInit(void);


void main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	InitAll();
    //Dogs102x6_imageDraw(Dlogo, 0, 16);           //Display logo
    Dogs102x6_stringDraw(3, 0, "   Welcome to    ", DOGS102x6_DRAW_NORMAL);

    for( ; ;){

    StartAD();
//    SendtoBluetooth();
//    SendtoMobile();
//    Sounds(result);
//    unsigned int i;
//    for(i=0;i<4;i++){
/*     char t[6];
     sprintf(t,"%d",result);
     InitLcd();
     Dogs102x6_stringDraw(3, 0, t, DOGS102x6_DRAW_NORMAL);  */
     unsigned int i;
     for(i=0;i<16000;i++){
    	 Delay1s();
     }
//     SendtoBluetooth();
//    }
    }
}

void InitAll(void){
	InitLcd();
}

void InitLcd(){
    Dogs102x6_init();                            //Init LCD
    Dogs102x6_backlightInit();                   //Init backlight

    Dogs102x6_setBacklight(Dbrightness);          //Set backlight value
    Dogs102x6_setContrast(Dcontrast);             //Set contrast value
    Dogs102x6_clearScreen();                     //Clear screen
}

void Delay400ms(void)
{
    unsigned int i=500;
	unsigned int j;

	while(i--)
	{
		j=7269;
		while(j--);
	}
}

void Delay(unsigned char n){
	while(n--)
	{
		Delay400ms();
	}
}

void ADC12Init(void){
	ADC12CTL0 &= ~ADC12ENC;

	ADC12CTL0 =  ADC12ON + ADC12SHT0_2+ADC12MSC;         // Sampling time, ADC12 on
    ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_0;                     // Use sampling timer
//    ADC12CTL1 |=  ADC12DIV_7 + ADC12SSEL_1;
//    ADC12CTL2 =0x20+ADC12SR+ADC12PDIV;
	ADC12MCTL7 = ADC12INCH_7;
	ADC12CTL0 |= ADC12REF2_5V;                      //选用内部参考电压为2.5V
    ADC12CTL0 |= ADC12REFON;                        //内部参考电压打开
	ADC12IE = 0x01;
	P6SEL |= BIT7;
	ADC12CTL0 |= ADC12ENC;


}

void StartAD(void){
	ADC12Init();
	_EINT();
	ADC12CTL0 |= ADC12SC;
//    while(1);
//	char t[6];
	sprintf(t,"%d",result);
	InitLcd();
	Dogs102x6_stringDraw(3, 0, t, DOGS102x6_DRAW_NORMAL);
	SendtoBluetooth();
}
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void){
	switch(__even_in_range(ADC12IV,34))
	  {
	  case  0: break;                           // Vector  0:  No interrupt
	  case  2: break;                           // Vector  2:  ADC overflow
	  case  4: break;                           // Vector  4:  ADC timing overflow
	  case  6:                                  // Vector  6:  ADC12IFG0
		result = ADC12MEM0;
		ADC12CTL0 |= ADC12SC;
	  default: break;
	  }
}


void UARTInit(void){
	UCA1CTL1 &= ~UCSWRST;
	UCA1CTL1 = 0x10;
	UCA1BR0 = 6;                              // 1MHz 9600 (see User's Guide)
    UCA1BR1 = 0;                              // 1MHz 9600
    UCA1MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
//	UTCTL1 = 0x10;
//	ME2 |= 0x20;
    UCA1IE |= UCTXIE;

}

void SendtoBluetooth(void){
	unsigned int i;
//	sprintf(t,"%d",result);
	UARTInit();
	while (!(UCA1IFG&UCTXIFG));
	for(i=0;i<6;i++){
		UCA1TXBUF = t[i];
	}
}


//sounds function
void delay(unsigned char i)
{
	while(--i);
}
void Delay1s()
	{
	int i;
	for (i=0;i<10000;i++)
	delay(300);
	}


//ISD0-P7.0  ISD1-P7.1  ISD2-P7.2  ISD3-P7.3
void Sounds(unsigned int temp)
{
//	uchar point;
//	uchar tmp;
	uchar str[3];
	str[0]= temp/100;
	str[1]= temp%100/10;
	str[2]= temp%10;
	str[3]=0;
//	P7DIR |= 0x07;                            // P7.7 output
	P7OUT |= BIT7;
	switch(str[0]){
	case 1: P7OUT |= BIT3 + BIT7; break;
	case 2: P7OUT |= BIT2 + BIT7; break;
	case 3: P7OUT |= BIT2 + BIT3 + BIT7; break;
	case 4: P7OUT |= BIT1 + BIT7; break;
	case 5: P7OUT |= BIT1 + BIT3 +BIT7; break;
	case 6: P7OUT |= BIT1 + BIT2 + BIT7; break;
	case 7: P7OUT |= BIT1 + BIT2 + BIT3 + BIT7; break;
	case 8: P7OUT |= BIT0 + BIT7; break;
	case 9: P7OUT |= BIT0 + BIT3 + BIT7; break;
	}
    P7OUT &= ~BIT7;
    Delay1s();
    P7OUT |= BIT7;
    if(str[0]!=0) P7OUT |= BIT0 + BIT2 + BIT7;
    P7OUT &= ~BIT7;
    Delay1s();
    P7OUT |= BIT7;
    switch (str[1]){
       case 0:P7OUT |= BIT7;break;
       case 1:P7OUT |= BIT3+BIT7;break;
       case 2:P7OUT |= BIT2+BIT7;break;
       case 3:P7OUT |= BIT3+BIT7+BIT2;break;
       case 4:P7OUT |= BIT1+BIT7;break;
       case 5:P7OUT |= BIT3+BIT7+BIT1;break;
       case 6:P7OUT |= BIT1+BIT7+BIT2;break;
       case 7:P7OUT |= BIT3+BIT7+BIT1+BIT2;break;
       case 8:P7OUT |= BIT0+BIT7;break;
       case 9:P7OUT |= BIT3+BIT7+BIT0;break;
       }
    P7OUT &= ~BIT7;
    Delay1s();
    P7OUT |= BIT7;
    P7OUT |= BIT3+BIT7+BIT0+BIT2;
    P7OUT &= ~BIT7;
    Delay1s();
    P7OUT |= BIT7;
    switch (str[2]){
       case 0:P7OUT |= BIT7;break;
       case 1:P7OUT |= BIT7+BIT3;break;
       case 2:P7OUT |= BIT7+BIT2;break;
       case 3:P7OUT |= BIT7+BIT2+BIT3;break;
       case 4:P7OUT |= BIT7+BIT1;break;
       case 5:P7OUT |= BIT7+BIT1+BIT3;break;
       case 6:P7OUT |= BIT7+BIT1+BIT2;break;
       case 7:P7OUT |= BIT7+BIT1+BIT2+BIT3;break;
       case 8:P7OUT |= BIT7+BIT0;break;
       case 9:P7OUT |= BIT7+BIT0+BIT3;break;
       }
    P7OUT &= ~BIT7;
    Delay1s();
    P7OUT |= BIT7;
    P7OUT |= BIT0+BIT1+BIT7;
    P7OUT &= ~BIT7;
    Delay1s();
    P7OUT |= BIT7;

}





