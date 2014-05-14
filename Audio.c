#include "msp430.h"
#include "Audio.h"

void Sounds(char*t){
  int i;
  P4DIR |= BIT0;
  P7DIR |= BIT0 +BIT1 +BIT2 +BIT3;

//  READ TEN BIT
  P4OUT |= BIT0;
  switch((int)t[0] - 48){
    case 1:
      {
        P7OUT = 0x000a ;
        break;
      }
    case 2:
      {
        P7OUT = 0x0002;
        break;
      }
    case 3:
      {
        P7OUT = 0x0003 ;
        break;
      }
    case 4:
      {
        P7OUT = 0x0004 ;
        break;
      }
    case 5:
      {
        P7OUT =0x0005 ;
        break;
      }
    default: break;
  }
  for(i=0;i<10;i++){
      Delay1s();
  }
  P4OUT &= ~BIT0;
  for(i=0;i<20;i++){
    Delay1s();
  }

//  READ TEN
  P4OUT |= BIT0;
  for(i=0;i<10;i++){
      Delay1s();
    }
  if((((int)t[0]-48)!= 0) && (((int)t[0]-48)!= 1)){
    P7OUT = 0x000a;
    P4OUT &= ~BIT0;
    for(i=0;i<20;i++){
      Delay1s();
    }
    P4OUT |= BIT0;
  }

//  READ ONE BIT
  P4OUT |= BIT0;
  for(i=0;i<10;i++){
    Delay1s();
  }
  switch((int)t[1]-48){
    case 1:
    {
      P7OUT = 0x0001;
      break;
    }
    case 2:
    {
      P7OUT = 0x0002;
      break;
    }
    case 3:
    {
      P7OUT = 0x0003 ;
      break;
    }
    case 4:
    {
      P7OUT = 0x0004 ;
      break;
    }
    case 5:
    {
      P7OUT = 0x0005;
      break;
    }
    case 6:
    {
      P7OUT =0x0006;
      break;
    }
    case 7:
    {
      P7OUT = 0x0007;
      break;
    }
    case 8:
    {
      P7OUT = 0x0008;
      break;
    }
    case 9:
    {
      P7OUT = 0x0009;
      break;
    }
  }
  P4OUT &= ~BIT0;
  for(i=0;i<20;i++){
    Delay1s();
  }

  //READ .
  P4OUT |= BIT0;
  for(i=0;i<10;i++){
    Delay1s();
  }
  P7OUT = 0x000b;
  P4OUT &= ~BIT0;
  for(i=0;i<20;i++){
    Delay1s();
  }

  //READ PERCENT
  P4OUT |= BIT0;
  for(i=0;i<10;i++){
    Delay1s();
  }
  switch((int)t[3]-48){
    case 0:
    {
      P7OUT = 0x000d;
      break;
    }
    case 1:
    {
      P7OUT = 0x0001;
      break;
    }
    case 2:
    {
      P7OUT = 0x0002;
      break;
    }
    case 3:
    {
      P7OUT = 0x0003 ;
      break;
    }
    case 4:
    {
      P7OUT = 0x0004 ;
      break;
    }
    case 5:
    {
      P7OUT = 0x0005;
      break;
    }
    case 6:
    {
      P7OUT =0x0006;
      break;
    }
    case 7:
    {
      P7OUT = 0x0007;
      break;
    }
    case 8:
    {
      P7OUT = 0x0008;
      break;
    }
    case 9:
    {
      P7OUT = 0x0009;
      break;
    }
  }
  P4OUT &= ~BIT0;
  for(i=0;i<20;i++){
    Delay1s();
  }

  //READ DEGREE
  P4OUT |= BIT0;
  for(i=0;i<10;i++){
    Delay1s();
  }
  P7OUT = 0x000c;
  P4OUT &= ~BIT0;
  for(i=0;i<20;i++){
    Delay1s();
  }
}
