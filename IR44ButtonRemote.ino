//*****************************************************************************
//44 button IR remote
//NEC - IR code
//LarryD
//
//If you use the techniques shown here in, no library is needed for this discussion.
//
//This sketch demonstrates interfacing to a 44 button IR remote. http://goo.gl/nvDuhD
//These are commonly available on eBay for $3 - $5 USD.
//The remote usually comes with a controller. It is an
//all in one IR detector and amplifier similar to TSOP38238.
//Remove the detector from the controller for this discussion and wire it to +5V and GND.
//Datasheet for the IR detector: https://www.sparkfun.com/datasheets/Sensors/Infrared/tsop382.pdf
//For wiring, see: https://learn.sparkfun.com/tutorials/ir-control-kit-hookup-guide
//Interrupt 0 (pin D2) is used to achieve fast receive response.
//See also: http://forum.arduino.cc/index.php?topic=289446.msg2024776#new
//
//Most Arduino boards have two external interrupts: 0 (on digital pin 2) and 1 (on digital pin 3). 
//The table below shows the available interrupt pins on various boards. 
//Board       int.0  int.1  int.2  int.3  int.4  int.5 
//UNO, Ethernet   2      3      -      -      -      -    
//Mega2560        2      3     21     20     19     18 
//Leonardo        3      2      0      1      7      -
//
//
// Rev 1.00   January 25, 2015 functional code
// Rev 1.01
//

#include "IRremote44Button.h"
#define interruptPin 2
IRremote44Button myRemote(interruptPin); //create the object

#define tonePin 7            //Piezo speaker is connected to this pin

//*****************************************************************************

//Is this a Portrait or Landscape version of the 44 button keypad?

#define Portrait

//*****************************************************************************

//Portrait keypad definitions for the 44 button IR remote
#ifdef Portrait
const byte buttonCode[44] = {
  0x5C,0x5D,0x41,0x40,
  0x58,0x59,0x45,0x44,
  0x54,0x55,0x49,0x48,
  0x50,0x51,0x4D,0x4C,
  0x1C,0x1D,0x1E,0x1F,
  0x18,0x19,0x1A,0x1B,
  0x14,0x15,0x16,0x17,
  0x10,0x11,0x12,0x13,
  0x0C,0x0D,0x0E,0x0F,
  0x08,0x09,0x0A,0x0B,
  0x04,0x05,0x06,0x07
}; //END of buttonCode Array

const byte ASCIIcode[44] = {
  // 1    2    3    4
  0x31,0x32,0x33,0x34,
  // 5    6    7    8
  0x35,0x36,0x37,0x38,
  // 9    0    -   SP
  0x39,0x30,0x2D,0x20,
  // A    B    C    D
  0x41,0x42,0x43,0x44,
  // E    F    G    H
  0x45,0x46,0x47,0x48,
  // I    J    K    L
  0x49,0x4A,0x4B,0x4C,
  // M    N    O    P
  0x4D,0x4E,0x4F,0x50,
  // Q    R    S    T
  0x51,0x52,0x53,0x54,
  // U    V    W    X
  0x55,0x56,0x57,0x58,
  // Y    Z    <    >
  0x59,0x5A,0x3C,0x3E,
  //SH   NL    V    ^
  0xFC,0x0A,0x76,0x5E
}; //END of ASCIIcode Array 
#endif

//Landscape keypad definitions for the 44 button IR remote
#ifndef Portrait
const byte buttonCode[44] = {
  0x4,0x8,0xC,0x10,0x14,0x18,0x1C,0x50,0x54,0x58,0x5C,
  0x5,0x9,0xD,0x11,0x15,0x19,0x1D,0x51,0x55,0x59,0x5D,
  0x6,0xA,0xE,0x12,0x16,0x1A,0x1E,0x4D,0x49,0x45,0x41,
  0x7,0xB,0xF,0x13,0x17,0x1B,0x1F,0x4C,0x48,0x44,0x40
}; //END of buttonCode Array

const byte ASCIIcode[44] = {
  // 1    2    3    4    5    6    7    8    9    0    -  
  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x2D,
  // Q    W    E    R    T    Y    U    I    O    P    ^
  0x51,0x57,0x45,0x52,0x54,0x59,0x55,0x49,0x4F,0x50,0x5E,
  //Shift A    S    D    F    G    H    J    K    L    v
  0xFC,0x41,0x53,0x44,0x46,0x47,0x48,0x4A,0x4B,0x4C,0x76,
  // <    Z    X    C    V    B Space   N   M NewLine  >    
  0x3C,0x5A,0x58,0x43,0x56,0x42,0x20,0x4E,0x4D,0x0A,0x3E
}; //END of ASCIIcode Array 
#endif

//=============================================================================

void setup(void) 
{
  Serial.begin(115200);

  myRemote.beginIR();

} 
// ************************** E N D   s e t u p ( ) *****************************


void loop(void) 
{
  //**************************************
  //Check if we've received a valid IR code yet.
  if(myRemote.getIRflag())
  {
    byte byteRX = myRemote.checkCode();
    //If we get a verified button code, convert it to an ASCII code
    if(byteRX) 
    {          
      //Scan through the buttonCode Array
      for (int i = 0; i < 44; i++) 
      {
        if (buttonCode[i] == byteRX)
        {
          tone(tonePin,3400,100);
          Serial.print(char(ASCIIcode[i])); //get the ASCII code

          //we found it, no need to continue looking
          break;
        } 
      }
    }

    //get ready for the next falling edge
    myRemote.resetIRflag(); 
  }
  //**************************************

  // Other loop stuff goes here

}
// ************************** E N D   l o o p ( ) *****************************






//=============================================================================
//                             END OF CODE
//=============================================================================


