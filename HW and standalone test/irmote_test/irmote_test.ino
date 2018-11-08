#include <IRremote.h>
#include "Timer.h"


int RECV_PIN = 14;
bool goingon = false;
IRrecv irrecv(RECV_PIN);
int goingonTimerID;
Timer t;
decode_results results;
typedef enum {vSTOP = 0, vRUNNING = 1} vStatus;
vStatus currentvStatus = vSTOP;

 

void setup()
{
  Serial.begin(9600);
  Serial.println("car working");

  //irrecv.enableIRIn(); // Start the receiver

  irrecv.enableIRIn(); // Start the receiver
}
bool isValid(int val)
{
  switch (val)
  {
    case 0xFD10EF:
    case 0xFD50AF:
    case 0xFD30CF:
    case 0xFD08F7:
    case 0xFD8877:
    case 0xFD48B7:
    case 0xFD28D7:
    case 0xFDA857:
    case 0xFD6897:
    case 0xFD18E7:
    case 0xFD9867:
    case 0xFD58A7:
      return true;
    default:
      return false;
  }
}

void runCar(int val)
{
  switch (val)
  {
    case 0xFD10EF:
      Serial.println("speed down");
   
      break;
    case 0xFD50AF:
      Serial.println("speed up");
  
      
      break;
    case 0xFD30CF:
      Serial.println("left turn");
      break;
    case 0xFD08F7:
      Serial.println("up left");
      break;
    case 0xFD8877:
      Serial.println("up");
      break;
    case 0xFD48B7:
      Serial.println("up right");
      break;
    case 0xFD28D7:
      Serial.println("left");
      break;
    case 0xFDA857:
      Serial.println("right turn");
      break;
    case 0xFD6897:
      Serial.println("right");
      break;
    case 0xFD18E7:
      Serial.println("down left");
      break;
    case 0xFD9867:
      Serial.println("down");
      break;
    case 0xFD58A7:
      Serial.println("down right");
      break;
    default:
      Serial.println("wrong");
  }
}
void loop()
{
  t.update();
  int res = irrecv.decode(&results);
  if (res)
  {
    //res == 1
    if (!isValid(results.value))
    {
      //not valid
      //do nothing
      goingon = true;
    } else
    {
      //valid
      if (currentvStatus == vSTOP)
      {
        runCar(results.value);
        currentvStatus = vRUNNING;
        goingonTimerID = t.every(125, timerIsr);
        goingon = true;
      } else
      {
        //do nothing
      }//if (currentvStatus == vSTOP)
    }// if (!isValid(results.value))
    irrecv.resume();
  }// if (res == 0)
}
//for stop
void timerIsr()
{
  //Serial.println("fired");
  if (goingon)
  {
    //goingon = false;
  } else
  {
    Serial.println("stop");
    currentvStatus = vSTOP;
    t.stop(goingonTimerID);
    //stopmotor();
  }
  goingon = false;
}
