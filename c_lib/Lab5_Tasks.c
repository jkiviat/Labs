#include "Lab5_Tasks.h"
#include <math.h>
#include <stdlib.h>


//axis of wheel to outer surface of track: 0.765 inches
static const float wheel_rad= 0.765f;

//outer edge of left track to outer edge of right track: 3.858 inches
static const float track_sep= 3.858f;

//909.7 counts per rotation
//Linear distance travelled per rotation: (2*pi*0.765 inches)
//Linear distance per encoder count: (2*pi*0.765)/909.7 inches
static const float inch_per_count = ((2*M_PI*0.765)/909.7);
//static const float count_per_inch = 909.7/(2*M_PI*0.765);

static const float half_rot_dist = M_PI*3.858; //circumference = 2*pi*r. Travelling half the circumference = pi*r

static const float deg_to_rad = M_PI/180.0;

//Return the encoder count to serve as the target for the Controller_Update function
int distance_to_encoder(float distance){
    //need to round to nearest int (encoder count)
    int target_count = round(inch_per_count*distance);

    return target_count;
}

/*
//Return the encoder count to serve as the target for the Controller_Update function
int direction_to_encoder(float direction){

    //Turn the vehicle to the desired direction (in terms of angles, CCW convention):

    int turn_target;
    //Ensure the input direction is betwen 0 and 360:
    if (direction < 0.0){
        while(direction < 0.0){
            direction += direction +360.0;
        }
    } else if(direction >= 360.0){
        while(direction >= 360.0){
            direction -= direction - 360.0;
        }
    } 

    if (direction <= 180){ //left turn, left track remains stationary while right track moves
        //match distance travelled by track to the ratio
        float turn_ratio = direction/180.0;
        turn_target = round(turn_ratio*half_rot_dist);
        //CALL THE FUNCTION TO TRAVEL STRAIGHT UNTIL A CERTAIN ENCODER COUNT HERE

    } else{
        float turn_ratio = (direction-180.0)/180.0;
        turn_target = round(turn_ratio*half_rot_dist);
        //CALL THE FUNCTION TO TRAVEL STRAIGHT UNTIL A CERTAIN ENCODER COUNT HERE
    }
    return turn_target;
}

void orient_direction(Controller_t* p_cont, float direction){

    bool left = 0; // If true, left turn. If false, right turn.
    if (direction < 0.0){
        while(direction < 0.0){
            direction += direction +360.0;
        }
    } else if(direction >= 360.0){
        while(direction >= 360.0){
            direction -= direction - 360.0;
        }
    } 

    if (direction <= 180){
        left = true;
    }

    int dir_enc = direction_to_encoder(direction);

}

void drive_dist_and_direction(Controller_t* p_cont, float distance, float direction){

    //call PWM function
    //call controller_update
    //disable motor when target reached
    //Task_Activate( &task_update_controller, &p_cont -> update_period);

    //Turn the vehicle to the desired direction first:



    //void Controller_Set_Target_Position(p_cont, ____)

}

//This function returns a true or 1 value if it is a left turn.
//Assumes facing directly forward is 0 degrees and a counterclockwise convention
//Angles between 0 and 180 degrees are deemed to be left turns
//Angles between 180 and 360 degrees are deemed to be right turns
bool determine_direction(float direction){

    bool left = 0; // If true, left turn. If false, right turn.
    if (direction < 0.0){
        while(direction < 0.0){
            direction += direction +360.0;
        }
    } else if(direction >= 360.0){
        while(direction >= 360.0){
            direction -= direction - 360.0;
        }
    } 

    if (direction <= 180){
        left = true;
    }

    return left;
}

//This function converts a desired angle (in degrees) to turn into encoder counts for a single wheel.
//For example, to turn left 45 degrees, the left motor remains stationary while the right motor activates until 
//the distance of a 45 degree arc has been travelled.
int32_t turn_to_encoder(float direction){

    //If the user enters a value outside the range of 0 to 360 degrees, these statements bring the entered value
    //back within the range of 0 to 360.
    if (direction < 0.0){
        while(direction < 0.0){
            direction += direction +360.0;
        }
    } else if(direction >= 360.0){
        while(direction >= 360.0){
            direction -= direction - 360.0;
        }
    } 

    //The if statement below exists because if the user enters 350 degrees, we want to convert it to a 10 degree turn
    if (direction >= 180.0){
        direction = ABS(direction - 360.0);
    }
   
   // //Distance to travel for a 180 degree arc is known. Find the % of that distance for this particular turn.
    float turn_ratio = direction/180.0; 
    float distance = turn_ratio*half_rot_dist; 

    int32_t target_count = round(dist_per_count*distance); //Convert the arc distance to travel into encoder counts

    return target_count;
    //return turn_target;
}
*/

//This function is linked to the task that will update the controller at the specified discretization interval.
//It will update the controller and call the Set_PWM function based on the signal.
//It will be deactivated when the encoder count is close enough to its target
void send_right_controller_update_pos(){
   
    float meas = (float)Encoder_Counts_Right();

    //filter the encoder counts that have been measured to eliminate noise
    float update_interval = ((&right_cont) -> update_period);
    float u = Controller_Update(&right_cont, meas, update_interval);
    u = Saturate(u,MAX_PWM);
    int16_t input_sig = round(u);
    Set_Right_Motor(input_sig); 

    //Cancel the task if the encoder counts are within the targeted threshold 
   int32_t updated_error_right = Encoder_Counts_Right() - round(Get_Controller_Target(&right_cont));
   updated_error_right = sabs(updated_error_right);
   while(target_threshold > updated_error_right){
        Task_Cancel( &task_update_controller_right_pos );
        Task_Activate( &task_set_pwm_zero_right, -1.0 );
   }
}

//This function is linked to the task that will update the controller at the specified discretization interval.
//It will update the controller and call the Set_PWM function based on the signal.
//It will be deactivated when the encoder count is close enough to its target
void send_left_controller_update_pos(){
    
    float meas = (float)Encoder_Counts_Left();

    //filter the encoder counts that have been measured to eliminate noise
    float update_interval = ((&left_cont) -> update_period);
    float u = Controller_Update(&left_cont, meas, update_interval); //***UPDATE_PERIOD WAS DECLARED IN CONTROLLER.H
    u = Saturate(u,MAX_PWM);
    int16_t input_sig = round(u);
    Set_Left_Motor(input_sig); 

    //Cancel the task if the encoder counts are within the targeted threshold 
   int32_t updated_error_left = Encoder_Counts_Left() - round(Get_Controller_Target(&left_cont));
   updated_error_left = sabs(updated_error_left);
   while(target_threshold > updated_error_left){
        Task_Cancel( &task_update_controller_left_pos );
        Task_Activate( &task_set_pwm_zero_left, -1.0 );
   }
}

void Set_Right_Motor(int16_t right)
{
    float bat_level = Filter_Last_Output( &filt );
    if( bat_level < 0.1 ) {
        struct {
            char let[9];
        } data = { .let = { 'P', 'O', 'W', 'E', 'R', ' ', 'O', 'F', 'F' } };
        USB_Send_Msg( "c9s", '!', &data, sizeof( data ) );
    } else if( bat_level < 3.6 ) {
        struct {
            char let[7];
            float volts;
        } data = { .let = { 'B', 'A', 'T', ' ', 'L', 'O', 'W' }, .volts = bat_level };
        USB_Send_Msg( "c7sf", '!', &data, sizeof( data ) );
    } else {
        if( !MotorPWM_Is_Enabled() ) {
            MotorPWM_Enable( true );
        }
        MototPWM_Set_Right( right );
    }
}

void Set_Left_Motor(int16_t left)
{
    float bat_level = Filter_Last_Output( &filt );
    if( bat_level < 0.1 ) {
        struct {
            char let[9];
        } data = { .let = { 'P', 'O', 'W', 'E', 'R', ' ', 'O', 'F', 'F' } };
        USB_Send_Msg( "c9s", '!', &data, sizeof( data ) );
    } else if( bat_level < 3.6 ) {
        struct {
            char let[7];
            float volts;
        } data = { .let = { 'B', 'A', 'T', ' ', 'L', 'O', 'W' }, .volts = bat_level };
        USB_Send_Msg( "c7sf", '!', &data, sizeof( data ) );
    } else {
        if( !MotorPWM_Is_Enabled() ) {
            MotorPWM_Enable( true );
        }
        MototPWM_Set_Right( left );
    }
}

void Set_PWM_Zero_Left( float _time_since_last )
{
    MotorPWM_Set_Left( 0 );
    Task_Cancel( &task_set_pwm_zero_left );
}


void Set_PWM_Zero_Right( float _time_since_last )
{
    MototPWM_Set_Right( 0 );
    Task_Cancel( &task_set_pwm_zero_right );
}

float Get_Controller_Target(Controller_t* p_cont){

    float target;
    if(p_cont -> target_vel != 0){
        target = p_cont -> target_vel; //TARGET ENCODER VALUE
    } else {
        target = p_cont -> target_pos; //
    }

    return target;
}


//This function takes the current encoder count and the desired distance and direction (in angles)
int32_t Set_Encoder_Target_Right(int32_t current_encoder_count, float dist, float angle){

    //find arc length in terms of encoder counts
    //arc length = angle(in radians)*radius. radius = track_sep/2. 
    int32_t angle_enc = round(angle*(deg_to_rad)*(track_sep/2)*count_per_inch);

    //find straight distance in terms of encoder counts
    int32_t straight_dist_enc = round(dist*count_per_inch);

    //Add the length of the turn and the straight distance in terms of encoder counts (CCW turn convention)
    int32_t target_encoder_count = (straight_dist_enc + angle_enc) + current_encoder_count;

    return target_encoder_count;

}

//This function takes the current encoder count and the desired distance and direction (in angles)
int32_t Set_Encoder_Target_Left(int32_t current_encoder_count, float dist, float angle){

    //find arc length in terms of encoder counts
    //arc length = angle(in radians)*radius. radius = track_sep/2. 
    int32_t angle_enc = round(angle*(deg_to_rad)*(track_sep/2)*count_per_inch);

    //find straight distance in terms of encoder counts
    int32_t straight_dist_enc = round(dist*count_per_inch);

    //Subtract the length of the turn and the straight distance in terms of encoder counts (CCW turn convention)
    int32_t target_encoder_count = (straight_dist_enc - angle_enc) + current_encoder_count;

    return target_encoder_count;
}


//This function stops the motor and cancels the controller update task after the time limit has been reached
void Terminate_Controller_Left(){

    MotorPWM_Set_Left( 0 );
    Task_Cancel( &task_update_controller_left_pos);
    
}

void Terminate_Controller_Right(){

    MototPWM_Set_Right( 0 );
    Task_Cancel( &task_update_controller_right_pos);
    
}

float Set_Target_Velocity_Left(float linear_vel, float angular_vel){

    float target_vel_left = linear_vel - (angular_vel*track_sep/2);

    return target_vel_left;

}

float Set_Target_Velocity_Right(float linear_vel, float angular_vel){

    float target_vel_right = linear_vel + (angular_vel*track_sep/2);

    return target_vel_right;

}

//Target vel argument is in inches per second
void send_right_controller_update_vel(){
   
    float meas = (float)Encoder_Counts_Right();

    //float target_vel = Get_Controller_Target(&right_cont);

    //target_vel: inches per second. dt: seconds. count_per_inch: encoder counts per inch. Result is encoder counts
    //int32_t target_enc_pos = Encoder_Counts_Right() + (target_vel*count_per_inch*update_period);
    //filter the encoder counts that have been measured to eliminate noise
    float update_interval = ((&right_cont) -> update_period);
    float u = Controller_Update(&right_cont, meas, update_interval);
    u = Saturate(u,MAX_PWM);
    int16_t input_sig = round(u);
    Set_Right_Motor(input_sig); 

    //Cancel the task if the encoder counts are within the targeted threshold 
   int32_t updated_error_right = Encoder_Counts_Right() - round(Get_Controller_Target(&right_cont));
   updated_error_right = sabs(updated_error_right);
   while(target_threshold > updated_error_right){
        Task_Cancel( &task_update_controller_right_pos );
        Task_Activate( &task_set_pwm_zero_right, -1.0 );
   }
}

//Target vel argument is in inches per second
void send_left_controller_update_vel(){
   
    float meas = (float)Encoder_Counts_Left();

    //float target_vel = Get_Controller_Target(&left_cont);

    //target_vel: inches per second. dt: seconds. count_per_inch: encoder counts per inch. Result is encoder counts
    //int32_t target_enc_pos = Encoder_Counts_Right() + (target_vel*count_per_inch*update_period);
    //filter the encoder counts that have been measured to eliminate noise
    float update_interval = ((&left_cont) -> update_period);
    float u = Controller_Update(&left_cont, meas, update_interval);
    u = Saturate(u,MAX_PWM);
    int16_t input_sig = round(u);
    Set_Left_Motor(input_sig); 

    //Cancel the task if the encoder counts are within the targeted threshold 
   int32_t updated_error_left = Encoder_Counts_Left() - round(Get_Controller_Target(&left_cont));
   updated_error_left = sabs(updated_error_left);
   while(target_threshold > updated_error_left){
        Task_Cancel( &task_update_controller_left_pos );
        Task_Activate( &task_set_pwm_zero_left, -1.0 );
   }
}

//This stands for "safe ABS", compiler is having issues with the ABS function, even with math.h and stblib.h
int32_t sabs(int32_t i){
    int32_t res;

    if (i < 0)
    {
        res = -i;
    }
    else
    {
        res = i;
    }

    return res;

}