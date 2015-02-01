
#include <Arduino.h> 

//********************************************************** 
class IRremote44Button
{
  byte IRpin_;
  byte counter;
  boolean validStart;
  unsigned long thisMicros;
  unsigned long lastMicros;
  unsigned long startMillis;
  byte irFlag;
  int data[4];

public:

  //constructor
  IRremote44Button(byte IRpin)
  {
    IRpin_ = IRpin;

  } //END of IRremote44Button()

  //********************************************************** 
  void beginIR()
  {
    pinMode(IRpin_, INPUT_PULLUP);
    counter = 0;
    validStart = false;
    thisMicros = micros();
    irFlag = 0;

  } //END of IRremote44Button()

  //********************************************************** 
  byte getIRflag()
  {
    return irFlag;

  } // END of getFlag() 

  //**********************************************************  
  void resetIRflag()
  {
    irFlag = 0;

  } //END of resetIRflag()

  //**********************************************************  
  void setEdgeTime()
  {
    //record the time the edge happened
    thisMicros = micros();
    //Flag that the an edge has happened
    irFlag = 1;

  } //END of setEdgeTime()

  //**********************************************************  

  byte checkCode()
  {
    byte byteRecived = 0;   //returned variable to the CALLing code
    byte rxBit = 0;         //the value of the received bit

    //Calculate the time between the last two falling edges.
    unsigned long temp = thisMicros - lastMicros;

    //Are we over the start bit upper limit of 13.8mS for the 44 button IR remote code?
    if (temp > 13800)
    {
      //There must have been quite some time since the last falling edge.
      //Save the current time (beginning time) as the starting edge for this message.
      startMillis = millis();
    }

    //One complete message is (actual 67.5mS) 75mS long.
    //If we go over this, reset things i.e. get ready for the next message.
    //As a benefit this removes (ignores) any repeat codes that come in.
    else if(millis() - startMillis > 75)
    {
      //Too much time has elapsed since the first edge of the message. 
      //Start over.
      validStart = false;
    }

    //Are we inside the limit of the start bit: (actual 13480) 13200uS to 13800uS
    else if((validStart == false) && (temp > 13200))
    {
      //This is a valid start time
      counter = 0;
      validStart = true;

      //4 bytes are coming, initialize their storage.
      data[0] = 0;
      data[1] = 0;
      data[2] = 0;
      data[3] = 0;
    }

    //Are we into data bits: (actual 1130) 900 to 1300uS is a 0 bit,
    //(actual 2250), 2000 to 2500uS is a 1 bit.
    else if((validStart == true) && (temp > 900) && (temp < 2500))
    {
      //Is this bit a 1?
      if(temp > 2000)
      {
        rxBit = 1;
      }

      //Is this bit a 0?
      else if(temp < 1300)
      {
        rxBit = 0;
      }

      //Timing was not valid!
      else
      {
        //The bit width was not within the above range, start over.
        validStart = false;
        counter = 0;
      }

      //Put this bit in the correct position in the current byte.
      data[counter/8] |= (rxBit << (counter%8));

      //Have we received all the bits yet?
      if(counter >= 31) 
      {
        //Was this a valid IR code? i.e. Address = ~Address && Data = ~Data
        if((data[0] == (~data[1] & 0xFF)) && (data[2] == (~data[3] & 0xFF)))
        {
          //data[2] contains the valid IR code
          byteRecived = data[2];
        }

        //We are now finished with this IR code, get ready for the next valid start.
        validStart = false;

      } //END of if(counter >= 31)

      //Get ready for the next bit.
      counter++;
    } //END of if(validStart == true && (temp > 900) && (temp < 2500))

    //Save the time when the last edge happened.
    lastMicros = thisMicros;

    return byteRecived;

  } // END of checkCode()

};  // END of  IRremote44Button class 

// In C++ you must have a ; at the end of a class definition











