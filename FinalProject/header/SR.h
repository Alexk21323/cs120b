#include <avr/io.h>
#include <util/delay.h>
#define RED_PORT   PORTC
#define RED_DDR    DDRC
#define RED_DS_POS PC0      //Data pin (DS) pin location
#define RED_SH_CP_POS PC1      //Shift Clock (SH_CP) pin location
#define RED_ST_CP_POS PC2      //Store Clock (ST_CP) pin location

#define BLACK_PORT   PORTD
#define BLACK_DDR    DDRD
#define BLACK_DS_POS PD0      //Data pin (DS) pin location
#define BLACK_SH_CP_POS PD1      //Shift Clock (SH_CP) pin location
#define BLACK_ST_CP_POS PD2      //Store Clock (ST_CP) pin location

void REDInit(){
   RED_DDR|=((1<<RED_SH_CP_POS)|(1<<RED_ST_CP_POS)|(1<<RED_DS_POS));
}

#define REDDataHigh() (RED_PORT|=(1<<RED_DS_POS))
#define REDDataLow() (RED_PORT&=(~(1<<RED_DS_POS)))

void REDPulse(){
   RED_PORT|=(1<<RED_SH_CP_POS);
   RED_PORT&=(~(1<<RED_SH_CP_POS));
}

void REDLatch(){
   RED_PORT|=(1<<RED_ST_CP_POS);
   _delay_loop_1(1);

   RED_PORT&=(~(1<<RED_ST_CP_POS));
   _delay_loop_1(1);
}

void REDWrite(uint8_t data){
   for(uint8_t i=0;i<8;i++)
   {
      if(data & 0b10000000)
      {
         REDDataHigh();
      }
      else
      {
         REDDataLow();
      }
      REDPulse();  
      data=data<<1;  
   }
   REDLatch();
}

void Wait(){
   for(uint8_t i=0;i<30;i++)
   {
      _delay_loop_2(0);
   }
}

void BLACKInit(){
   BLACK_DDR|=((1<<BLACK_SH_CP_POS)|(1<<BLACK_ST_CP_POS)|(1<<BLACK_DS_POS));
}

#define BLACKDataHigh() (BLACK_PORT|=(1<<BLACK_DS_POS))

#define BLACKDataLow() (BLACK_PORT&=(~(1<<BLACK_DS_POS)))

void BLACKPulse(){
   BLACK_PORT|=(1<<BLACK_SH_CP_POS);//HIGH
   BLACK_PORT&=(~(1<<BLACK_SH_CP_POS));//LOW
}

void BLACKLatch(){
   BLACK_PORT|=(1<<BLACK_ST_CP_POS);//HIGH
   _delay_loop_1(1);

   BLACK_PORT&=(~(1<<BLACK_ST_CP_POS));//LOW
   _delay_loop_1(1);
}

void BLACKWrite(uint8_t data){
   for(uint8_t i=0;i<8;i++)
   {
      if(data & 0b10000000)
      {
         BLACKDataHigh();
      }
      else
      {
         BLACKDataLow();
      }
      BLACKPulse();  
      data=data<<1;  
   }
   BLACKLatch();
}