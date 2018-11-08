
#include <AFMotor.h>

AF_DCMotor motor[] = {AF_DCMotor(1,200),AF_DCMotor(2,200),AF_DCMotor(3,200),AF_DCMotor(4,200)};
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");
  for (int i = 0; i < 4; i++)
  {
    //motor[i] = new AF_DCMotor(i + 1);
    motor[i].setSpeed(200);
    motor[i].run(RELEASE);
  }
  for (int i = 0; i < 4; i++)
  {
    motor[i].setSpeed(200);
     motor[i].run(FORWARD);
  }
  delay(1000);
   for (int i = 0; i < 4; i++)
  {
     motor[i].run(RELEASE);
  }
  delay(100);
   for (int i = 0; i < 4; i++)
  {
    motor[i].setSpeed(200);
     motor[i].run(BACKWARD);
  }
  delay(1000);
   for (int i = 0; i < 4; i++)
  {
     motor[i].run(RELEASE);
  }
  delay(100);
}

void loop() {
 
}
