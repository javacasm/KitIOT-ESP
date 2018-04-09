#include "MotorShield.h"

/* 
 *  
    Motor Shield  NodeMCU DevKit  GPIO  Purpose
    D1 PWMA (Motor A) D1  5 Speed
    D3 DIRA (Motor A) D3  0 Direction
    D2 PWMA (Motor B) D2  4 Speed
    D4 DIRB (Motor B) D4  2 Direction

 */



carStatus status;


void setup_motorShield(){
    pinMode(SPEED_MOTOR_A, OUTPUT); // 1,2EN aka D1 pwm left
    pinMode(SPEED_MOTOR_B, OUTPUT); // 3,4EN aka D2 pwm right
    pinMode(DIR_MOTOR_A, OUTPUT); // 1A,2A aka D3
    pinMode(DIR_MOTOR_B, OUTPUT); // 3A,4A aka D4
}

void continueMove(carStatus stat){
  
  switch(stat){
    case FORWARD:
      forward();
      break;
    case BACK:
      backward();
      break;
    default:
      stop();
      break;
  }
}

void turnLeft(){
  carStatus oldStatus = status;
  left();
  delay(bitTurn);
  continueMove(oldStatus);
  status = oldStatus;
}

void turnRight(){
  carStatus oldStatus = status;
  right();
  delay(bitTurn);
  continueMove(oldStatus);
  status = oldStatus;
}

void stop(void)
{
    analogWrite(SPEED_MOTOR_A, MIN_SPEED);
    analogWrite(SPEED_MOTOR_B, MIN_SPEED);
    status = STOP;
}
void backward()
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, HIGH);
    digitalWrite(DIR_MOTOR_B, HIGH);
    status = BACK;
}
void forward()
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, LOW);
    digitalWrite(DIR_MOTOR_B, LOW);
    status = FORWARD;
}
void left(void)
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, LOW);
    digitalWrite(DIR_MOTOR_B, HIGH);
    status = LEFT;
}
void right()
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, HIGH);
    digitalWrite(DIR_MOTOR_B, LOW);
    status = RIGHT;
}
