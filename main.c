/*
 * main.c
 */

#include "msp430f5529.h"
#include "HAL_Dogs102x6.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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

//#define p 40.96;


unsigned int result;
float temperature;
//volatile float result1 = 0.0;
char t[15] ;

void InitLcd(void);
void InitAll(void);
void Delay400ms(void);
void Delay(unsigned char i);
void StartAD(void);
//void SendtoBluetooth(void);

void Sounds(char*);
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

	ADC12MCTL7 = ADC12INCH_7;

	ADC12IE = 0x01;
	P6SEL |= BIT7;
	ADC12CTL0 |= ADC12ENC;


}

void StartAD(void){
	ADC12Init();
	_EINT();
	ADC12CTL0 |= ADC12SC;

	temperature = (double)(result - 2048)/40.96;
//	sprintf(t,"%d",result);
	int a;
	a= floor(temperature);
	int b;
	b = a/10 + 48;
	t[0] = (char)b;
	int c;
	c = a%10 + 48;
	t[1] = (char)c;
	int d;
	d = floor((temperature-a)*10) + 48;
	t[3] = (char)d;
	t[2] = '.';
	t[4] = '\0';
	InitLcd();
	Dogs102x6_stringDraw(3, 0, t, DOGS102x6_DRAW_NORMAL);
//	SendtoBluetooth();
	Sounds(t);
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


/*void SendtoBluetooth(void){
	unsigned int i;
//	sprintf(t,"%d",result);
	UARTInit();
	while (!(UCA1IFG&UCTXIFG));
	for(i=0;i<6;i++){
		UCA1TXBUF = t[i];
	}
}  */



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


void Sounds(char*a){
	int i;
	switch(t[0]){
		case 1:
			{
				P7OUT |= BIT3+BIT1 ;
				for(i=0;i<100;i++){
					Delay1s();
				}
				P7OUT = 0x0000;
				break;
			}


		case 2:
			{
				P7OUT |= BIT1;
				for(i=0;i<100;i++){
				Delay1s();
			    }
				P7OUT = 0x0000;
				break;
			}


		case 3:
			{
				P7OUT |= BIT0 + BIT1 ;
				for(i=0;i<100;i++){
								Delay1s();
							    }
				P7OUT = 0x0000;
				break;
			}


		case 4:
			{
				P7OUT |= BIT2 ;
				for(i=0;i<100;i++){
	              Delay1s();
	             }
				P7OUT = 0x0000;
				break;
			}


		case 5:
			{
				P7OUT |= BIT0 + BIT2 ;
				for(i=0;i<100;i++){
					              Delay1s();
					             }
				P7OUT = 0x0000;
				break;
			}


		default: break;
		}

	if((t[0]!= 0) && (t[0]!= 1)){

		P7OUT |= BIT1 + BIT3;
		for(i=0;i<100;i++){
			Delay1s();
		}
	    P7OUT = 0x0000;
	}else{
		switch(t[1]){
		case 1:
		{
			P7OUT |= BIT0;
			for(i=0;i<100;i++){
						Delay1s();
					}
			P7OUT = 0x0000;
			break;
		}
		case 2:
		{
			P7OUT |= BIT1;
			for(i=0;i<100;i++){
			Delay1s();
						    }
			P7OUT = 0x0000;
			break;
		}
		case 3:
		{
			P7OUT |= BIT0 + BIT1 ;
			for(i=0;i<100;i++){
				Delay1s();
										    }
			P7OUT = 0x0000;
			break;

		}
		case 4:
		{
						P7OUT |= BIT2 ;
						for(i=0;i<100;i++){
			              Delay1s();
			             }
						P7OUT = 0x0000;
						break;
					}
		case 5:
		{
			P7OUT |= BIT0 + BIT2;
			for(i=0;i<100;i++){
				Delay1s();
			}
			P7OUT = 0x0000;
			break;
		}
		case 6:
		{
			P7OUT |= BIT1 + BIT2;
			for(i=0;i<100;i++){
							Delay1s();
						}
						P7OUT = 0x0000;
						break;
		}
		case 7:
		{
			P7OUT |= BIT0 + BIT1 + BIT2;
			for(i=0;i<100;i++){
										Delay1s();
									}
									P7OUT = 0x0000;
									break;
		}
		case 8:
		{
			P7OUT |= BIT3;
			for(i=0;i<100;i++){
										Delay1s();
									}
									P7OUT = 0x0000;
									break;
		}
		case 9:
		{
			P7OUT |= BIT0 + BIT3;
			for(i=0;i<100;i++){
										Delay1s();
									}
									P7OUT = 0x0000;
									break;
		}
		}
	}
	    P7OUT |= BIT0 + BIT1 + BIT3;
	    for(i=0;i<100;i++){
	    							Delay1s();
	    						}
	    						P7OUT = 0x0000;
	    switch(t[3]){

	    case 0:
	    {
            P7OUT |= BIT0 + BIT2 + BIT3;
            for(i=0;i<100;i++){
            							Delay1s();
            						}
            						P7OUT = 0x0000;
            						break;
	    }

	    case 1:
	    		{
	    			P7OUT |= BIT0;
	    			for(i=0;i<100;i++){
	    						Delay1s();
	    					}
	    			P7OUT = 0x0000;
	    			break;
	    		}
	    		case 2:
	    		{
	    			P7OUT |= BIT1;
	    			for(i=0;i<100;i++){
	    			Delay1s();
	    						    }
	    			P7OUT = 0x0000;
	    			break;
	    		}
	    		case 3:
	    		{
	    			P7OUT |= BIT0 + BIT1 ;
	    			for(i=0;i<100;i++){
	    				Delay1s();
	    										    }
	    			P7OUT = 0x0000;
	    			break;

	    		}
	    		case 4:
	    		{
	    						P7OUT |= BIT2 ;
	    						for(i=0;i<100;i++){
	    			              Delay1s();
	    			             }
	    						P7OUT = 0x0000;
	    						break;
	    					}
	    		case 5:
	    		{
	    			P7OUT |= BIT0 + BIT2;
	    			for(i=0;i<100;i++){
	    				Delay1s();
	    			}
	    			P7OUT = 0x0000;
	    			break;
	    		}
	    		case 6:
	    		{
	    			P7OUT |= BIT1 + BIT2;
	    			for(i=0;i<100;i++){
	    							Delay1s();
	    						}
	    						P7OUT = 0x0000;
	    						break;
	    		}
	    		case 7:
	    		{
	    			P7OUT |= BIT0 + BIT1 + BIT2;
	    			for(i=0;i<100;i++){
	    										Delay1s();
	    									}
	    									P7OUT = 0x0000;
	    									break;
	    		}
	    		case 8:
	    		{
	    			P7OUT |= BIT3;
	    			for(i=0;i<100;i++){
	    										Delay1s();
	    									}
	    									P7OUT = 0x0000;
	    									break;
	    		}
	    		case 9:
	    		{
	    			P7OUT |= BIT0 + BIT3;
	    			for(i=0;i<100;i++){
	    										Delay1s();
	    									}
	    									P7OUT = 0x0000;
	    									break;
	    		}




	    }
	    P7OUT |= BIT2 + BIT3;
	    for(i=0;i<100;i++){
	    	  Delay1s();
	    	              }
	    P7OUT = 0x0000;



}



