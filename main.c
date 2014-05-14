/*
 * main.c
 */

#include "msp430f5529.h"
#include "HAL_Dogs102x6.h"
#include "Audio.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define uchar unsigned char
#define uint unsigned int

unsigned int result;
float temperature;
char t[6] = {'3','5','3','4','\0','0'};

void InitLcd(void);
void InitUart();
void InitAll(void);
void InitPort();
void InitADC(void);

void StartAD(void);
void Sounds(char*);
void Delay1s(void);
void delay(unsigned int i);

void main(void) {
  WDTCTL = WDTPW + WDTHOLD;
//  P1DIR |= BIT0;                            // P1.0 output
//  P1OUT &= ~BIT0;
//  P1OUT |= BIT0;
//  P4DIR |= BIT0;
//  P4OUT |= BIT0;
  _EINT();
  InitAll();
  Dogs102x6_stringDraw(3, 0, "Temperature: ", DOGS102x6_DRAW_NORMAL);
  for(;;){
    StartAD();
    unsigned int i;
    for(i=0;i<64000;i++){
     Delay1s();
    }
    for(i=0;i<64000;i++){
      Delay1s();
    }
  }
  __bis_SR_register(LPM4_bits + GIE);     // Enter LPM4 w/interrupt
  __no_operation();            // For debugger
}
void InitAll(void){
  InitPort();
  InitUart();
  InitLcd();
  InitLcd();
}
void InitPort(){
  P4DIR |= BIT0;                            // Set P1.0 to output direction
  P4OUT &= ~BIT0;

  P1REN |= BIT7;                            // Enable P1.4 internal resistance
  P1OUT |= BIT7;                            // Set P1.4 as pull-Up resistance
  P1IES &= ~BIT7;                           // P1.4 Lo/Hi edge
  P1IFG &= ~BIT7;                           // P1.4 IFG cleared
  P1IE |= BIT7;                             // P1.4 interrupt enabled
}
void InitUart(){
  P4SEL = BIT4+BIT5;                        // P3.4,5 = USCI_A0 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
  UCA1BR0 = 6;                              // 1MHz 9600 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 9600
  UCA1MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
                                            // over sampling
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}
void InitLcd(){
  Dogs102x6_init();                            //Init LCD
  Dogs102x6_backlightInit();                   //Init backlight
  Dogs102x6_setBacklight(Dbrightness);          //Set backlight value
  Dogs102x6_setContrast(Dcontrast);             //Set contrast value
  Dogs102x6_clearScreen();                     //Clear screen
}

void InitADC(void){
  ADC12CTL0 &= ~ADC12ENC;
  ADC12CTL0 =  ADC12ON + ADC12SHT0_2+ADC12MSC;         // Sampling time, ADC12 on
  ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_0;                     // Use sampling timer
  ADC12MCTL7 = ADC12INCH_7;
  ADC12IE = 0x01;
  P6SEL |= BIT7;
  ADC12CTL0 |= ADC12ENC;
}

void StartAD(void){

  InitADC();

  ADC12CTL0 |= ADC12SC;
  temperature = (double)(result - 2100)/38.98;

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
  Dogs102x6_stringDraw(4, 0, t, DOGS102x6_DRAW_NORMAL);

  // P1DIR |= 0x01;                            // P1.0 output
  //  P1OUT &= ~BIT0;
  // P1OUT |= BIT0;
  unsigned int j=0;
  while(t[j]!='\0'){
    while((UCA1IFG&UCTXIFG)==0);
    UCA1TXBUF = t[j];
    UCA1IFG &= ~UCTXIFG;
    j++;
  }

}
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  unsigned int i;
  for(i=0;i<12000;i++);

  if(P1IFG&BIT7){
    Sounds(t);
    P1IFG &= ~BIT7;
  }
  _BIC_SR_IRQ(LPM4_bits);                 // Exit LPM4
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
//    if (result >3000)                 // ADC12MEM = A0 > 0.5AVcc?
//          {
//      P1OUT |= BIT0;                        // P1.0 = 1
//          }
//        else
//          P1OUT &= ~BIT0;                       // P1.0 = 0

//        __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
  default: break;
  }
}

//delay function
void delay(unsigned int i)
{
  while(--i);
}
void Delay1s()
{
  int j;
  for (j=0;j<10000;j++)
  delay(300);
}

//ISD0-P7.0  ISD1-P7.1  ISD2-P7.2  ISD3-P7.3
