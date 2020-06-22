
/*
 Hello 
 I design developer tools for embedded electronic systems.
 You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/

word MilliSecond = 0; 
byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool timerStart = false;

char rx_byte;

void setup(){
 Serial.begin(9600);

 noInterrupts();         // disable all interrupts
 TCCR1A = 0;             // set entire TCCR1A register to 0  //set timer1 interrupt at 1kHz  // 1 ms
 TCCR1B = 0;             // same for TCCR1B
 TCNT1  = 0;             // set timer count for 1khz increments
 OCR1A = 1999;           // = (16*10^6) / (1000*8) - 1
 //had to use 16 bit timer1 for this bc 1999>255, but could switch to timers 0 or 2 with larger prescaler
 // turn on CTC mode
 TCCR1B |= (1 << WGM12); // Set CS11 bit for 8 prescaler
 TCCR1B |= (1 << CS11);  // enable timer compare interrupt
 TIMSK1 |= (1 << OCIE1A);
 interrupts();           // enable

}

void loop(){
 
  if(Serial.available() > 0){       
    rx_byte = Serial.read();       
    if(rx_byte == '1'){
       timerStart = true; 
    }
    
    if(rx_byte == '2'){
       timerStart = false;
    }
    
    if(rx_byte == '3'){
       Second = 0;
    }
 }

  
} // loop

ISR(TIMER1_COMPA_vect){   
 
 if(timerStart == true){ 
    MilliSecond++;
    if(MilliSecond >= 1000){
       MilliSecond = 0;
       Second++;
       if(Second >= 60){
          Second = 0;
          Minute++;
          if(Minute >= 60){
             Minute = 0;
          }
       }
    }

   if(Second != lastSecond){
      Serial.println("Time -- " + String(Minute) +":"+ String(Second));  
   }
   lastSecond = Second;    
 }
 
}
