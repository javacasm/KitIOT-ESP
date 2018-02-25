#include "MotorShield.h"

/* 
 *  
    Motor Shield  NodeMCU DevKit  GPIO  Purpose
    D1 PWMA (Motor A) D1  5 Speed
    D3 DIRA (Motor A) D3  0 Direction
    D2 PWMA (Motor B) D2  4 Speed
    D4 DIRB (Motor B) D4  2 Direction

 */

void setup_motorShield(){
    pinMode(SPEED_MOTOR_A, OUTPUT); // 1,2EN aka D1 pwm left
    pinMode(SPEED_MOTOR_B, OUTPUT); // 3,4EN aka D2 pwm right
    pinMode(DIR_MOTOR_A, OUTPUT); // 1A,2A aka D3
    pinMode(DIR_MOTOR_B, OUTPUT); // 3A,4A aka D4
}


void stop(void)
{
    analogWrite(SPEED_MOTOR_A, MIN_SPEED);
    analogWrite(SPEED_MOTOR_B, MIN_SPEED);
}
void forward()
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, HIGH);
    digitalWrite(DIR_MOTOR_B, HIGH);
}
void backward()
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, LOW);
    digitalWrite(DIR_MOTOR_B, LOW);
}
void left(void)
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, LOW);
    digitalWrite(DIR_MOTOR_B, HIGH);
}
void right()
{
    analogWrite(SPEED_MOTOR_A, MAX_SPEED);
    analogWrite(SPEED_MOTOR_B, MAX_SPEED);
    digitalWrite(DIR_MOTOR_A, HIGH);
    digitalWrite(DIR_MOTOR_B, LOW);
}
