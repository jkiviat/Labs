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

#include "Message_Handling.h"

/**
 * Function _Message_Length returns the number of bytes associated with a command string per the
 * class documentation; This is only accessable from this file.
 * @param cmd
 * @return Size of expected string. Returns 0 if unrecognized.
 */
static uint8_t _Message_Length( char cmd );

/**
 * Function Task_Message_Handling processes USB messages as necessary and sets
 * status flags to control the flow of the program.
 */
void Task_Message_Handling( float _time_since_last )
{
    // *** MEGN540  ***
    // YOUR CODE HERE. I suggest you use your peak function and a switch interface
    // Either do the simple stuff strait up, set flags to have it done later.
    // If it just is a USB thing, do it here, if it requires other hardware, do it
    // in the main and set a flag to have it done here.

    // Check to see if their is data in waiting
    if( !USB_Msg_Length() )
        return;  // nothing to process...

    // Get Your command designator without removal so if their are not enough
    // bytes yet, the command persists
    char command = USB_Msg_Peek();

    // /* MEGN540 -- LAB 2 */
    bool command_processed = false;

    // process command
    switch( command ) {
        case '*':
            if( USB_Msg_Length() >= _Message_Length( '*' ) ) {
                // then process your multiplication...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Multiply_And_Send( data.v1, data.v2 );

                // /* MEGN540 -- LAB 2 */
                command_processed = true;
            }
            break;
        case '/':
            if( USB_Msg_Length() >= _Message_Length( '/' ) ) {
                // then process your divide...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Divide_And_Send( data.v1, data.v2 );

                // /* MEGN540 -- LAB 2 */
                command_processed = true;
            }
            break;
        case '+':
            if( USB_Msg_Length() >= _Message_Length( '+' ) ) {
                // then process your plus...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Add_And_Send( data.v1, data.v2 );

                // /* MEGN540 -- LAB 2 */
                command_processed = true;
            }
            break;
        case '-':
            if( USB_Msg_Length() >= _Message_Length( '-' ) ) {
                // then process your minus...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Subtract_And_Send( data.v1, data.v2 );

                // /* MEGN540 -- LAB 2 */
                command_processed = true;
            }
            break;
        case '~':
            if( USB_Msg_Length() >= _Message_Length( '~' ) ) {
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h
                Task_Activate( &task_restart, -1 );
                // /* MEGN540 -- LAB 2 */
                command_processed = true;
            }
            break;
        case 't':
            if( USB_Msg_Length() >= _Message_Length( 't' ) ) {
                // then process your minus...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                uint8_t type = USB_Msg_Get();

                // Call MEGN540_Lab_Task Function
                switch( type ) {
                    case 0: Send_Time( 't', 0, Timing_Get_Time_Sec() ); break;
                    case 1: Send_Time( 't', 1, _time_since_last ); break;
                    default: break;
                }

                // /* MEGN540 -- LAB 2 */
                command_processed = true;
            }
            break;
        case 'T':
            if( USB_Msg_Length() >= _Message_Length( 'T' ) ) {
                // then process your minus...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    uint8_t type;
                    float run_period;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                switch( data.type ) {
                    case 0: Task_Activate( &task_send_time, data.run_period ); break;
                    case 1:
                        // USB_Send_Msg("f",'r',&data.run_period, sizeof(data.run_period));
                        Task_Activate( &task_time_loop, data.run_period );
                        break;
                    default: break;
                }

                // /* MEGN540 -- LAB 2 */
                command_processed = true;
            }
            break;

        case 'e':
            if( USB_Msg_Length() >= _Message_Length( 'e' ) ) {
                USB_Msg_Get();

                Send_Encoder_Message( 'e' );
            }
            break;

        case 'E':
            if( USB_Msg_Length() >= _Message_Length( 'E' ) ) {
                USB_Msg_Get();

                float repetition_time;

                USB_Msg_Read_Into( &repetition_time, sizeof( repetition_time ) );

                if( repetition_time > 0 ) {
                    Task_Activate( &task_send_encoder_counts, repetition_time );
                } else {
                    Task_Cancel( &task_send_encoder_counts );
                }

                command_processed = true;
            }
            break;
        case 'b':
            if( USB_Msg_Length() >= _Message_Length( 'b' ) ) {
                USB_Msg_Get();

                Send_Battery_Message( 'b' );

                command_processed = true;
            }
            break;
        case 'B':
            if( USB_Msg_Length() >= _Message_Length( 'B' ) ) {
                USB_Msg_Get();

                float repetition_time;

                USB_Msg_Read_Into( &repetition_time, sizeof( repetition_time ) );

                if( repetition_time > 0 ) {
                    Task_Activate( &task_send_battery_level, repetition_time );
                } else {
                    Task_Cancel( &task_send_battery_level );
                }

                command_processed = true;
            }
            break;

        case 'p':
            if( USB_Msg_Length() >= _Message_Length( 'p' ) ) {
                USB_Msg_Get();

                struct __attribute__( ( __packed__ ) ) {
                    int16_t left;
                    int16_t right;
                } pwm_values;

                USB_Msg_Read_Into( &pwm_values, sizeof( pwm_values ) );

                Set_Motor_Values( pwm_values.left, pwm_values.right );

                command_processed = true;
            }
            break;

        case 'P':
            if( USB_Msg_Length() >= _Message_Length( 'P' ) ) {
                USB_Msg_Get();

                struct __attribute__( ( __packed__ ) ) {
                    int16_t left;
                    int16_t right;
                    float time;
                } data;

                USB_Msg_Read_Into( &data, sizeof( data ) );

                Set_Motor_Values( data.left, data.right );
                Task_Activate( &task_set_pwm_zero, data.time );

                command_processed = true;
            }
            break;

        case 's':
            if( USB_Msg_Length() >= _Message_Length( 's' ) ) {
                USB_Msg_Get();

                Motor_Stop_And_Disable();

                command_processed = true;
            }
            break;

        case 'S':
            if( USB_Msg_Length() >= _Message_Length( 'S' ) ) {
                USB_Msg_Get();

                Motor_Stop_And_Disable();

                command_processed = true;
            }
            break;

        case 'q':
            if( USB_Msg_Length() >= _Message_Length( 'q' ) ) {
                USB_Msg_Get();

                Send_System_ID( 0.0 );

                command_processed = true;
            }
            break;

        case 'Q':
            if( USB_Msg_Length() >= _Message_Length( 'Q' ) ) {
                USB_Msg_Get();

                float repetition_time;

                USB_Msg_Read_Into( &repetition_time, sizeof( repetition_time ) );

                if( repetition_time < 0 ) {
                    Task_Cancel( &send_system_id_info );
                } else {
                    Task_Activate( &send_system_id_info, repetition_time );
                }

                command_processed = true;
            }
            break;

        case 'd':
            if( USB_Msg_Length() >= _Message_Length( 'd' ) ) {
                USB_Msg_Get();

                struct __attribute__( ( __packed__ ) ) {
                    float distance;
                    float direction;
                } dist_dir;

                USB_Msg_Read_Into( &dist_dir, sizeof( dist_dir ) );

                //Set the targeted encoder counts, based on the user-input distance and direction
                int32_t target_enc_left = Set_Encoder_Target_Left(Encoder_Counts_Left(), dist_dir.distance, dist_dir.direction);
                int32_t target_enc_right = Set_Encoder_Target_Right(Encoder_Counts_Right(), dist_dir.distance, dist_dir.direction);

                Controller_Set_Target_Position(&left_cont, target_enc_left); //Controller_Set_Target_Position takes a float while this is an int32_t
                Controller_Set_Target_Position(&right_cont, target_enc_right); //Controller_Set_Target_Position takes a float while this is an int32_t

                Task_Activate(&task_update_controller_right_pos, (&right_cont) -> update_period);
                Task_Activate(&task_update_controller_left_pos, (&left_cont) -> update_period);

                //Determine whether the turn is to the left or right
                //It is necessary to calculate this here because there are seperate functions and 
                //structs for the left and right motors, and we need to know which motor to activate

                //Retrieve encoder counts
                //int32_t cur_enc_left = Encoder_Counts_Left();
                //int32_t cur_enc_right = Encoder_Counts_Right();

                //Set_Encoder_Target_Right(int32_t current_encoder_count, float dist, float angle)

                //bool left = determine_direction(dist_dir.direction); //Returns "true" if it is a left turn

                //if (left == true){

                   // int32_t current_enc = Encoder_Counts_Right(); //Current encoder counts

                    //Add the arc length (in terms of encoder counts) that must be travelled to the 
                    //current encoder count to come up with the new encoder count to complete the turn
                   // int32_t target = current_enc + turn_to_encoder(dist_dir.direction);

                    //Set this encoder count as the new target for the right controller
                    //Controller_Set_Target_Position(&right_cont, target);

                    //Initialize variables that will be needed to update the controller and send PWM signals
                    //int32_t updated_enc = current_enc;
                    //int32_t updated_error = updated_enc - target;
                    //float meas = (float)updated_enc; //Controller_Update function takes a float for the measurement

                    //Activate the right controller at intervals specified by the update period
                    //Task_Activate(&task_update_controller_right, update_period);
                    //In the main loop, want to deactivate the task and turn off the motor when the target
                    //has been reached.

                //}
                command_processed = true;
            }
            break;

        case 'D':
            if( USB_Msg_Length() >= _Message_Length( 'D' ) ) {
                USB_Msg_Get();

                struct __attribute__( ( __packed__ ) ) {
                    float distance;
                    float direction;
                    float time;
                } dist_dir;

                USB_Msg_Read_Into( &dist_dir, sizeof( dist_dir ) );

                //Set the targeted encoder counts, based on the user-input distance and direction
                int32_t target_enc_left = Set_Encoder_Target_Left(Encoder_Counts_Left(), dist_dir.distance, dist_dir.direction);
                int32_t target_enc_right = Set_Encoder_Target_Right(Encoder_Counts_Right(), dist_dir.distance, dist_dir.direction);

                Controller_Set_Target_Position(&left_cont, target_enc_left); //Controller_Set_Target_Position takes a float while this is an int32_t
                Controller_Set_Target_Position(&right_cont, target_enc_right); //Controller_Set_Target_Position takes a float while this is an int32_t

                Task_Activate(&task_terminate_controller_left, dist_dir.time);
                Task_Activate(&task_terminate_controller_right, dist_dir.time);

                if( dist_dir.time > 0 ) {
                    Task_Activate(&task_update_controller_right_pos, (&right_cont) -> update_period);
                    Task_Activate(&task_update_controller_left_pos, (&left_cont) -> update_period);
                } else {
                    Task_Cancel( &task_update_controller_right_pos );
                    Task_Cancel( &task_update_controller_left_pos );
                }
    
                command_processed = true;
            }
            break;

        case 'v':
            if( USB_Msg_Length() >= _Message_Length( 'v' ) ) {
                USB_Msg_Get();

                struct __attribute__( ( __packed__ ) ) {
                    float linear_velocity;
                    float angular_velocity;
                } velocity;

                USB_Msg_Read_Into( &velocity, sizeof( velocity ) );

                //Calculate the velocities of the left and right tracks, based on the user-input linear and angular velocity
                float target_vel_left = Set_Target_Velocity_Left(velocity.linear_velocity, velocity.angular_velocity);
                float target_vel_right = Set_Target_Velocity_Right(velocity.linear_velocity, velocity.angular_velocity);

                //Put controllers in velocity mode
                Controller_Set_Target_Velocity(&left_cont, target_vel_left);
                Controller_Set_Target_Velocity(&right_cont, target_vel_right);

                Task_Activate(&task_update_controller_right_vel, (&right_cont) -> update_period);
                Task_Activate(&task_update_controller_left_vel, (&left_cont) -> update_period);

                command_processed = true;
            }
            break;

        case 'V':
            if( USB_Msg_Length() >= _Message_Length( 'V' ) ) {
                USB_Msg_Get();

                struct __attribute__( ( __packed__ ) ) {
                    float linear_velocity;
                    float angular_velocity;
                    float time;
                } velocity;

                USB_Msg_Read_Into( &velocity, sizeof( velocity ) );

                //Calculate the velocities of the left and right tracks, based on the user-input linear and angular velocity
                float target_vel_left = Set_Target_Velocity_Left(velocity.linear_velocity, velocity.angular_velocity);
                float target_vel_right = Set_Target_Velocity_Right(velocity.linear_velocity, velocity.angular_velocity);

                //Put controllers in velocity mode
                Controller_Set_Target_Velocity(&left_cont, target_vel_left);
                Controller_Set_Target_Velocity(&right_cont, target_vel_right);

                if( velocity.time > 0 ) {
                    Task_Activate(&task_update_controller_right_vel, (&right_cont) -> update_period);
                    Task_Activate(&task_update_controller_left_vel, (&left_cont) -> update_period);
                } else {
                    Task_Cancel( &task_update_controller_right_vel );
                    Task_Cancel( &task_update_controller_left_vel );
                }

                //Task_Activiate(&task_terminate_controller_left, velocity.time);
                //Task_Activiate(&task_terminate_controller_right, velocity.time);

                command_processed = true;
            }
            break;


        default:
            // What to do if you dont recognize the command character
            ;
            char cmd = (char)USB_Msg_Get();

            char message[] = "error\0";

            USB_Send_Msg( "c6c", cmd, &message, 6 );

            USB_Flush_Input_Buffer();
            break;
    }

    //********* MEGN540 -- LAB 2 ************//
    if( command_processed ) {
        // RESET the WATCHDOG TIMER
        Task_Activate( &task_message_handling_watchdog, 0.1 );
    }
}

/**
 * @brief Function Task_Message_Handling_Watchdog clears the USB recieve (deleting all messages) to flush the buffer if a complete message is not recieved
 * whithin an appropriate amount of time (say 250ms)
 *
 * @param _unused_
 */
void Task_Message_Handling_Watchdog( float _unused_ )
{
    if( USB_Msg_Length() ) {
        USB_Flush_Input_Buffer();
        char message[] = "Format Error";
        USB_Send_Msg( "c7c6c", '?', &message, sizeof( message ) );
    }
}

/**
 * Function _Message_Length returns the number of bytes associated with a
 * command string per the class documentation;
 * @param cmd
 * @return Size of expected string. Returns 0 if unreconized.
 */
static uint8_t _Message_Length( char cmd )
{
    switch( cmd ) {
        case '~': return 1; break;
        case '*': return 9; break;
        case '/': return 9; break;
        case '+': return 9; break;
        case '-': return 9; break;
        case 't': return 2; break;
        case 'T': return 6; break;
        case 'e': return 1; break;
        case 'E': return 5; break;
        case 'b': return 1; break;
        case 'B': return 5; break;
        case 'p': return 5; break;
        case 'P': return 9; break;
        case 's': return 1; break;
        case 'S': return 1; break;
        case 'q': return 1; break;
        case 'Q': return 5; break;
        case 'd': return 9; break;
        case 'D': return 13; break;
        case 'v': return 9; break;
        case 'V': return 13; break;
        default: return 0; break;
    }
}

//commit