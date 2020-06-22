#include <SevenSeg.h>
#include <Button.h>
/*
 Hello 
 I design developer tools for embedded electronic systems.
 You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/

SevenSeg disp(4,5,6,7,8,9,10); //  A,B,C,D,E,F,G
int digitPins[4]={3,2,1,0};    //  D1,D2,D3,D4

char displaybuffer[4] = "0000";

Button minutePlus  = Button(A0,BUTTON_PULLDOWN); // Minute +
Button minuteMinus = Button(A1,BUTTON_PULLDOWN); // Minute -

Button secondPlus  = Button(A2,BUTTON_PULLDOWN); // Second +
Button secondMinus = Button(A3,BUTTON_PULLDOWN); // Second -

Button startButton = Button(A4,BUTTON_PULLDOWN); // RUN

#define relayPin     A5  

word MilliSecond = 0; 
byte Second = 0;
byte Minute = 0;
byte lastSecond;
bool timerStart = false;
bool relayStatus = false;

const byte wait = 125;

void setup(){
 pinMode(relayPin, OUTPUT);
 
 disp.setDigitPins(4, digitPins);
 disp.setCommonAnode();
 disp.setActivePinState(HIGH,HIGH);
 disp.setTimer(2);
 disp.startTimer();

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

if(relayStatus == false){   // status
  
 if(minutePlus.isPressed()){
    Minute++;
    delay(wait);    
     if(Minute >= 60){
        Second = 0;
        Minute = 0;
     }         
 } 

 if(minuteMinus.isPressed()){  
    Minute--;
    delay(wait);     
     if(Minute >= 255) {    
        Minute = 0;
     }    
 }

 if(secondPlus.isPressed()){
    Second++;
    delay(wait);
     if(Second >= 60){
        Second = 0;      
        Minute++;
     } 
 }

 if(secondMinus.isPressed()){
    Second--;
    delay(wait);    
     if(Minute == 0 && Second >= 255){  
        Second=0;
     } 
      if(Second >= 255 ){
         Second = 59;
         Minute--;
          if(Minute >= 255 ){            
             Minute = 0;
          }           
      } 
 }

 if(startButton.isPressed()){
    timerStart = true;
    relayStatus = true;
    digitalWrite(relayPin,HIGH);   // role on
    delay(50); 
 }
 
} // relaystatus


 displaybuffer[0] = Minute / 10  + 48;   
 displaybuffer[1] = Minute % 10  + 48;
 displaybuffer[2] = Second / 10  + 48;
 displaybuffer[3] = Second % 10  + 48;

 disp.write(displaybuffer);

} // loop

ISR(TIMER2_COMPA_vect){
  disp.interruptAction ();
}

ISR(TIMER1_COMPA_vect){   

 if(timerStart == true){ 
    MilliSecond++;
    if(MilliSecond >= 1000){
       MilliSecond = 0;
       Second--;
        if(Second >= 255){   
           Second = 59;
           Minute--;
         if(Minute >= 255){
            Second = 0;
            Minute = 0;
          }
        }
    }
 
 }
  
}
