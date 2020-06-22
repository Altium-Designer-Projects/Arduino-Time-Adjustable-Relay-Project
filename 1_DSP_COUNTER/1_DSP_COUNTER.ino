#include <SevenSeg.h>

/*
 Hello 
 I design developer tools for embedded electronic systems.
 You can buy my projects design files.
 https://www.altiumdesignerprojects.com
*/
SevenSeg disp(4,5,6,7,8,9,10);  // https://github.com/sigvaldm/SevenSeg
//            A,B,C,D,E,F,G
//DIGIT PORT  0 1 2 3 4 5 6     // my seven-segment display project
        
int digitPins[4]={3,2,1,0};
//               D1,D2,D3,D4
// SCAN PORT     T4,T3,T2,T1    // my seven-segment display project
 
char displaybuffer[4] = "0000";

word number = 0;

void setup() {

 disp.setDigitPins(4, digitPins);
 disp.setCommonAnode();
 disp.setActivePinState(HIGH,HIGH);
 disp.setTimer(2);
 disp.startTimer();

}  

void loop() {
  
 disp.write(displaybuffer);
 
 displaybuffer[0] = number / 1000     + 48;   //   1 
 displaybuffer[1] = number / 100 % 10 + 48;   //   2
 displaybuffer[2] = number / 10  % 10 + 48;   //   3
 displaybuffer[3] = number % 10       + 48;   //   4
 
 number++;
 delay(500);

}

ISR(TIMER2_COMPA_vect){
  disp.interruptAction();
}
