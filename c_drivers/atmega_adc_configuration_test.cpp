// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

float rec_buffer[] = {};
int counter = 0;
void setup() {
  // put your setup code here, to run once:

  //clear the prescalar bits
  //ADCSRA &= ~(bit(ADPS0)|bit(ADPS1)|bit(ADPS2));

  //what a divisor of 2, will give bit wise conversion time of 1.625 micros
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  
  Serial.begin(1000000);
  Serial.println("Beginning ADC test");
    
}

void loop() {
  // put your main code here, to run repeatedly:
  //float val = analogRead(0);
  int val = analogRead(1);  
  sendToPC(&val);
  
}
 
void sendToPC(int* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData, 2);
}
 
void sendToPC(float* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData, 4);
}