#include <SevenSeg.h>

/*
 Hello 
 I design developer tools for embedded electronic systems.
 You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/

SevenSeg disp(4,5,6,7,8,9,10);  // https://github.com/sigvaldm/SevenSeg
//            A,B,C,D,E,F,G

int digitPins[4]={3,2,1,0};
//               D1,D2,D3,D4

char displaybuffer[4] = "0000";

byte Minute;
byte Second;

void setup() {

 disp.setDigitPins(4, digitPins);
 disp.setCommonAnode();
 disp.setActivePinState(HIGH,HIGH);
 disp.setTimer(2);
 disp.startTimer();

 Minute = 12;
 Second = 34;

}  

void loop(){

 displaybuffer[0] = Minute / 10  + 48;   
 displaybuffer[1] = Minute % 10  + 48;
 displaybuffer[2] = Second / 10  + 48;
 displaybuffer[3] = Second % 10  + 48;

 disp.write(displaybuffer);

}

ISR(TIMER2_COMPA_vect){
  disp.interruptAction();
}
