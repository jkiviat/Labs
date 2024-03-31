/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.

*/

/*
 * This file defines lab-spcific tasks for execution
 */

#ifndef LAB5_TASKS_H
#define LAB5_TASKS_H

#include "Task_Management.h"
#include "Controller.h"
#include "Message_Handling.h"
#include "MotorPWM.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>

// Include your lab-specific headers here
// e.g. #include "SerialIO.h"  // necessary for sending arithmatic results back to user

// Put your lab-specific tasks here
// e.g. Task_t task_restart;  ///<-- Lab 1: This flag indicates that the device received a restart command from the hoast. Default inactive.

// Put your lab-specific task functionality and data_structures (if necessary) here so it is accessable to both
// message handeling and the Lab main loops.
// e.g. void Send_Time_Now( float _time_since_last );

//this is the number of encoder counts from the target that will trigger the motor to stop
//24 ENCODER COUNTS = APPROX. 1/8"
int32_t target_threshold = 24; 
float count_per_inch = 909.7/(2*M_PI*0.765);

Task_t task_update_controller_left_pos;
Task_t task_update_controller_right_pos;
Task_t task_update_controller_left_vel;
Task_t task_update_controller_right_vel;
Task_t task_set_pwm_zero_left;
Task_t task_set_pwm_zero_right;
Task_t task_terminate_controller_left;
Task_t task_terminate_controller_right;

int distance_to_encoder(float distance);

//int direction_to_encoder(float direction);
//void drive_dist_and_direction(Controller_t* p_cont, float distance, float direction);
//int turn_to_encoder(float direction);
//bool determine_direction(float direction);

void send_right_controller_update_pos();
void send_left_controller_update_pos();
void Set_Right_Motor(int16_t right);
void Set_Left_Motor(int16_t left);
void Set_PWM_Zero_Left( float _time_since_last );
void Set_PWM_Zero_Right( float _time_since_last );
float Get_Controller_Target(Controller_t* p_cont);
int32_t Set_Encoder_Target_Right(int32_t current_encoder_count, float dist, float angle);
int32_t Set_Encoder_Target_Left(int32_t current_encoder_count, float dist, float angle);
void Terminate_Controller_Left();
void Terminate_Controller_Right();
float Set_Target_Velocity_Left(float linear_vel, float angular_vel);
float Set_Target_Velocity_Right(float linear_vel, float angular_vel);
void send_right_controller_update_vel();
void send_left_controller_update_vel();
int32_t sabs(int32_t i);

#endif  // ifndef LAB5_TASKS_H