#include <IRremote.h>
#include "Timer.h"
#include <AFMotor.h>

AF_DCMotor motor[] = {AF_DCMotor(1), AF_DCMotor(2), AF_DCMotor(3), AF_DCMotor(4)};
uint8_t motor_speed = 200;
int RECV_PIN = 14;
bool goingon = false;
IRrecv irrecv(RECV_PIN);
int goingonTimerID;
Timer t;
decode_results results;
typedef enum {vSTOP = 0, vRUNNING = 1} vStatus;
vStatus currentvStatus = vSTOP;

void setspeed(uint8_t spd)
{
  for (int i = 0; i < 4; i++)
  {
    motor[i].setSpeed(spd);
  }
}
void stopmotor()
{
  for (int i = 0; i < 4; i++)
  {
    motor[i].run(RELEASE);
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("car working");

  //irrecv.enableIRIn(); // Start the receiver
  setspeed(motor_speed);
  stopmotor();
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

      if (motor_speed < 10)
        motor_speed = 0;
      else
        motor_speed -= 10;
      setspeed(motor_speed);
      break;
    case 0xFD50AF:
      Serial.println("speed up");

      if (motor_speed > 245)
        motor_speed = 255;
      else
        motor_speed += 10;
      setspeed(motor_speed);

      break;
    case 0xFD30CF:
      Serial.println("clock turn");
      motor[0].run(FORWARD);
      motor[1].run(BACKWARD);
      motor[2].run(BACKWARD);
      motor[3].run(FORWARD);
      break;
    case 0xFD08F7:
      Serial.println("up left");
      motor[0].run(FORWARD);
      motor[2].run(FORWARD);
      break;
    case 0xFD8877:
      Serial.println("up");
      motor[0].run(FORWARD);
      motor[1].run(FORWARD);
      motor[2].run(FORWARD);
      motor[3].run(FORWARD);
      break;
    case 0xFD48B7:
      Serial.println("up right");
      motor[1].run(FORWARD);
      motor[3].run(FORWARD);
      break;
    case 0xFD28D7:
      Serial.println("left");
      motor[0].run(FORWARD);
      motor[1].run(BACKWARD);
      motor[2].run(FORWARD);
      motor[3].run(BACKWARD);
      break;
    case 0xFDA857:
      Serial.println("anticlock turn");
      motor[0].run(BACKWARD);
      motor[1].run(FORWARD);
      motor[2].run(FORWARD);
      motor[3].run(BACKWARD);
      break;
    case 0xFD6897:
      Serial.println("right");
      motor[0].run(BACKWARD);
      motor[1].run(FORWARD);
      motor[2].run(BACKWARD);
      motor[3].run(FORWARD);
      break;
    case 0xFD18E7:
      Serial.println("down left");
      motor[1].run(BACKWARD);
      motor[3].run(BACKWARD);
      break;
    case 0xFD9867:
      Serial.println("down");
      motor[0].run(BACKWARD);
      motor[1].run(BACKWARD);
      motor[2].run(BACKWARD);
      motor[3].run(BACKWARD);
      break;
    case 0xFD58A7:
      Serial.println("down right");
      motor[0].run(BACKWARD);
      motor[2].run(BACKWARD);
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
    stopmotor();
  }
  goingon = false;
}

