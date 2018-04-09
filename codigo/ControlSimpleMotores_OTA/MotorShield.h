#ifndef _MOTORSHIELD_H_ 
#define _MOTORSHIELD_H_

/* 
 *  
    Motor Shield  NodeMCU DevKit  GPIO  Purpose
    D1 PWMA (Motor A) D1  5 Speed
    D3 DIRA (Motor A) D3  0 Direction
    D2 PWMA (Motor B) D2  4 Speed
    D4 DIRB (Motor B) D4  2 Direction

 */
#define SPEED_MOTOR_A D1
#define DIR_MOTOR_A   D3
#define SPEED_MOTOR_B D2
#define DIR_MOTOR_B   D4

#define MAX_SPEED 1023
#define MIN_SPEED 0

#define bitTurn 100

enum carStatus {FORWARD,BACK,LEFT,RIGHT,TURNLEFT,TURNRIGHT,STOP};

void right();
void left();
void forward();
void backward();
void stop();
void turnRight();
void turnLeft();

#endif _MOTORSHIELD_H_
