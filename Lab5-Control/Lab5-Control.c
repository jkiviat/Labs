/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

// put your includes here for all modules you need to use
#include "Battery_Monitor.h"
#include "Encoder.h"
#include "Lab3_Tasks.h"
#include "Message_Handling.h"  // for translating USB messages to microcontroller tasks
#include "MotorPWM.h"
#include "SerialIO.h"         // for USB communication
#include "Task_Management.h"  // for clean task management with functors
#include "Timing.h"           // for Time understanding
#include "Controller.h"
#include "Lab5_Tasks.h"

// put your task includes and/or function declarations here for future populaion

// put your global variables (only if absolutely required) here.
static const int32_t MAX_PWM= 500;
static const uint16_t TURN_THRESHOLD= 24; //24 ENCODER COUNTS = APPROX. 1/8"

int updated_error_left = 0; //need to declare this as global. It will be modified in a function and called in the main.
int updated_error_right = 0;

// Best to identify them as "static" to make them indentified as internal and start with a "_" to identify as internal.
// Try to initialize them if possible, so their values are never arbitrary.

// put your initialization function here
void Initialize_Modules( float _time_not_used_ )
{
    // Initialize (reinitialize) all global variables

    // reset USB input buffers
    USB_Flush_Input_Buffer();

    // Initialize all modules except USB (it can only be called once without messing things up)
    Initialize_Timing( 0.0 );
    Initialize_Encoders();
    Initialize_Battery_Monitor();
    Initialize_MotorPWM( MAX_PWM );
    //Initialize_Controller(Controller_t* p_cont, float kp, float* num, float* den, uint8_t order, float update_period)
    Initialize_Controller(&left_cont, kp_left, num_left, den_left, order, update_period);
    Initialize_Controller(&right_cont, kp_right, num_right, den_right, order, update_period);

    // Setup task handling
    Initialize_Task( &task_restart, &Initialize_Modules /*function pointer to call*/ );

    // Setup message handling to get processed at some desired rate.
    Initialize_Task( &task_message_handling, &Task_Message_Handling );

    // Lab 2 tasks
    Initialize_Task( &task_time_loop, &Send_Loop_Time );
    Initialize_Task( &task_send_time, &Send_Time_Now );
    Initialize_Task( &task_message_handling_watchdog, Task_Message_Handling_Watchdog );

    Task_Activate( &task_message_handling_watchdog, 0.25f );
    Task_Activate( &task_message_handling, 0.f );

    // Lab 3 tasks
    Initialize_Task( &task_send_battery_level, &Send_Battery_Level );
    Initialize_Task( &task_update_battery_filter, &Update_Battery_Filter );
    Initialize_Task( &task_send_encoder_counts, &Send_Encoder_Counts );
    Initialize_Task( &task_battery_level_watchdog, &Check_Battery_Level );
    Task_Activate( &task_update_battery_filter, 0.002f );
    Task_Activate( &task_battery_level_watchdog, 60 );

    // Lab 4 tasks
    Initialize_Task( &task_set_pwm_zero, &Set_PWM_Zero );
    Initialize_Task( &send_system_id_info, &Send_System_ID );

    //Lab 5 tasks
    Initialize_Task(&task_update_controller_left, &send_left_controller_update);
    Initialize_Task(&task_update_controller_right, &send_right_controller_update);
    Initialize_Task(&task_set_pwm_zero_left, &Set_PWM_Zero_Left);
    Initialize_Task(&task_set_pwm_zero_right, &Set_PWM_Zero_Right);
}

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main( void )
{
    Initialize_Modules( 0.0 );
    // call initialization stuff

    Initialize_USB();
    Initialize_Modules( 0.0 );
    Task_Activate( &task_message_handling_watchdog, 0.25f );
    Task_Activate( &task_message_handling, 0.f );

    while( true ) {
        Task_USB_Upkeep();

        Task_Run_If_Ready( &task_time_loop );
        Task_Run_If_Ready( &task_send_time );
        Task_Run_If_Ready( &task_message_handling );
        Task_Run_If_Ready( &task_restart );
        Task_Run_If_Ready( &task_send_encoder_counts );
        Task_Run_If_Ready( &task_update_battery_filter );
        Task_Run_If_Ready( &task_send_battery_level );

        Task_Run_If_Ready( &task_message_handling_watchdog );
        Task_Run_If_Ready( &task_battery_level_watchdog );

        Task_Run_If_Ready( &task_set_pwm_zero );
        Task_Run_If_Ready( &send_system_id_info );

        Task_Run_If_Ready (&task_update_controller_left);
        Task_Run_If_Ready (&task_update_controller_right);

        //If encoder count reaches the target, cancel the controller update task and set PWM to zero for that motor
        updated_error_left = Encoder_Counts_Left() - round(Get_Controller_Target(&left_cont));
        while(ABS(TURN_THRESHOLD > updated_error_left)){
            Task_Cancel( &task_update_controller_left );
            Task_Activate( &task_set_pwm_zero_left, -1.0 );
        }
        updated_error_right = Encoder_Counts_Right() - round(Get_Controller_Target(&right_cont));
        while(ABS(TURN_THRESHOLD > updated_error_right)){
            Task_Cancel( &task_update_controller_right );
            Task_Activate( &task_set_pwm_zero_right, -1.0 );
        }


    }
}

// put your task function definitions here