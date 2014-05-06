/*
 * main.c
 */

#include "msp430f5529.h"
#include "HAL_Dogs102x6.h"
#include "config.h"

volatile unsigned int result = 0;

void InitLcd(void);
void InitAll(void);
void Delay400ms(void);
void Delay(unsigned char i);
void StartAD(void);
void SendtoBluetooth();
//void SendtoMobile();

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	InitAll();
    //Dogs102x6_imageDraw(Dlogo, 0, 16);           //Display logo
    Dogs102x6_stringDraw(3, 0, "   Welcome to    ", DOGS102x6_DRAW_NORMAL);
    Delay(10);
  //  for( ; ;){
    StartAD();
    SendtoBluetooth();
//    SendtoMobile();

  //  }
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

void StartAD(void)
{

	  ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on
	  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
	  ADC12IE = 0x01;                           // Enable interrupt
	  ADC12CTL0 |= ADC12ENC;
	  P6SEL |= 0x80;                            // P6.7 ADC option select
//	  P1DIR |= 0x01;                            // P1.0 output

	  while (1)
	  {
	    ADC12CTL0 |= ADC12SC;                   // Start sampling/conversion

	    __bis_SR_register(LPM0_bits + GIE);     // LPM0, ADC12_ISR will force exit
	    __no_operation();                       // For debugger
	  }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:                                  // Vector  6:  ADC12IFG0
	result = ADC12MEM0;

    __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
  case  8: break;                           // Vector  8:  ADC12IFG1
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12: break;                           // Vector 12:  ADC12IFG3
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break;
  }
}
void SendtoBluetooth()
{
	  P4SEL = BIT4+BIT5;                        // P4.5,4 = USCI_A1 TXD/RXD
	  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
	  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
	  UCA1BR0 = 6;                              // 1MHz 9600 (see User's Guide)
	  UCA1BR1 = 0;                              // 1MHz 9600
	  UCA1MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
	                                            // over sampling
	  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	  UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

	  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
	  __no_operation();                         // For debugger
}
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA1IFG&UCTXIFG));             // USCI_A1 TX buffer ready?
    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}






/*void SendtoMobile()
{
	String BT_DATA = "";
	NewSoftSerial blueToothSerial(RxD,TxD);
	void setup()
	{
	 Serial.begin(38400);
	 pinMode(RxD, INPUT);
	 pinMode(TxD, OUTPUT);
	 blueToothSerial.begin(38400);
	}
	void loop()
	{
	while(blueToothSerial.available()) //Receivedata              {
	    Serial.print(char(blueToothSerial.read()));
	 }

	if(Serial.available()){  //Tx data
	  do{
	     BT_DATA += char(Serial.read());
	     delay(2);
	  }while (Serial.available() > 0);

	 if (BT_DATA.length() > 0){
	     blueToothSerial.println(BT_DATA);
	     Serial.println(BT_DATA);
	     BT_DATA = "";
	 }}


}*/
