
#include<avr/io.h>
#include<avr/pgmspace.h>
#include<avr/interrupt.h>
#define F_CPU 8000000UL
#include<util/delay.h>
#include"C25Ddisplay.h"

//#define uint8 

//This function read the value of ADC from ADC0 channel
unsigned int read_adc(void);

unsigned char call(unsigned char n);

//This function maps the digits to statusonoff array
void display(
        unsigned char digit1,
        unsigned char digit2,
        unsigned char digit3,
        unsigned char point,
        unsigned char minus);

volatile unsigned char count=0,statusonoff[20];
//                     m,b,c,p, a,b,c,d,e,f,g,p, a,b,c,d,e,f,g,p                
const unsigned char anode[20]PROGMEM  = {4,4,4,4,2,2,2,2,3,3,3,3,0,0,0,0,1,1,1,1};
const unsigned char cathode[20]PROGMEM ={3,0,1,2,0,1,3,4,0,1,2,4,1,2,3,4,0,2,3,4};
// kNameName
const unsigned char digit_map[16] PROGMEM = {
  SEVEN_SEGMENT_PATTERN_0,
  SEVEN_SEGMENT_PATTERN_1,
  SEVEN_SEGMENT_PATTERN_2,
  SEVEN_SEGMENT_PATTERN_3,
  SEVEN_SEGMENT_PATTERN_4,
  SEVEN_SEGMENT_PATTERN_5,
  SEVEN_SEGMENT_PATTERN_6,
  SEVEN_SEGMENT_PATTERN_7,
  SEVEN_SEGMENT_PATTERN_8,
  SEVEN_SEGMENT_PATTERN_9,
  SEVEN_SEGMENT_PATTERN_A,
  SEVEN_SEGMENT_PATTERN_b,
  SEVEN_SEGMENT_PATTERN_C,
  SEVEN_SEGMENT_PATTERN_d,
  SEVEN_SEGMENT_PATTERN_E,
  SEVEN_SEGMENT_PATTERN_F,
};

volatile unsigned int voltage;
volatile unsigned char point,c,d,e;
int main(void) {
  //Timer Init
  TCCR0A = 0x00;
  TCCR0B = 0x02;
  TIMSK = 0x02;

  //ADC init
  ADMUX =  0b10010000;  // ADC0,2.56V reference
  ADCSRA = 0b10000111;  // Prescaled by 128;
  
  //Neglect first reading
  ADCSRA |= 1<<ADSC;
  while(ADCSRA&(1<<ADSC));  // wait
  count=ADCL;
  count=ADCH;
  count=0;
  
  sei();
  
  while(1) {
    voltage = read_adc();
    
    //Here the voltage is 100 times the actual voltage
    
    //Perform the rounding and scale the result
    if(voltage <= 199) {
      point = 1;
    } else if(voltage<1995) {
      if(voltage%10 >= 5) {
        voltage = voltage+10;
      }
      voltage = voltage/10;
      point = 2;
    } else {
      if(voltage%100>=50)      {
        voltage = voltage+100;
      }
      voltage = voltage/100;
      point = 3;
    }
    
    c = voltage/100;
    voltage = voltage%100;
    d = voltage/10;
    voltage = voltage%10;
    e = voltage;
    
    display(c,d,e,point,0);
    _delay_ms(100);
  }
}


//Overflow routine for timer0
ISR(TIM0_OVF_vect)
{
  DDRB = 0;
  
  PORTB = 1<<pgm_read_byte(&anode[count])|(call(count))<<
    pgm_read_byte(&cathode[count]);
  DDRB = 1<<pgm_read_byte(&anode[count])|1<<
    pgm_read_byte(&cathode[count]);
  count++;
  if(count==20)
    count = 0;
}

//This function checks whether the led should be switched on or off when its turn comes
unsigned char call(unsigned char n)
{
  unsigned char j;
  j = statusonoff[n];
  j = ~j;
  j = j&(1<<0); //Extract LSB
  return j;
}


//This function maps the digits to statusonoff array
void display(unsigned char digit1,unsigned char digit2,unsigned char digit3,unsigned char point,unsigned char minus)
{
  
  unsigned char i;
  statusonoff[0]=minus;
  statusonoff[1]=digit1;
  statusonoff[2]=digit1;
  statusonoff[3]=(point==1);
  
  for(i=0;i<8;i++)
  {
    statusonoff[4+i]= ((pgm_read_byte(&digit_map[digit2]))&(1<<i))>>i;
    statusonoff[12+i]= ((pgm_read_byte(&digit_map[digit3]))&(1<<i))>>i;  
  }
  statusonoff[11]= (point==2);
  statusonoff[19]= (point==3);
  
}

//This function read the value of ADC from ADC0 channel
unsigned int read_adc(void)
{
  
  unsigned char k;
  unsigned int adcvalue=0;
  float finaladc=0;
  for(k=0;k<=9;k++)
  {
    ADCSRA |= 1<<ADSC;
    while(ADCSRA&(1<<ADSC));//Wait
    adcvalue += ADCL;
    adcvalue += ADCH*256;
  }
  adcvalue = adcvalue/10;
  //100 times the voltage computation
  finaladc = adcvalue/4.0;///4.0;//(2.56/1024)*100
  finaladc = finaladc*4.9;//potential divider
  adcvalue = (unsigned int)finaladc;
  return adcvalue; //100 times the voltage
  
}

