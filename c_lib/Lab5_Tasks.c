#include "Lab5_Tasks.h"
#include <math.h>


//axis of wheel to outer surface of track: 0.765 inches
static const float wheel_rad= 0.765f;

//outer edge of left track to outer edge of right track: 3.858 inches
static const float track_sep= 3.858f;

//909.7 counts per rotation
//Linear distance travelled per rotation: (2*pi*0.765 inches)
//Linear distance per encoder count: (2*pi*0.765)/909.7 inches
static const float dist_per_count = ((2*M_PI*0.765)/909.7);

static const float half_rot_dist = M_PI*3.858; //circumference = 2*pi*r. Travelling half the circumference = pi*r

//Return the encoder count to serve as the target for the Controller_Update function
int distance_to_encoder(float distance){
    //need to round to nearest int (encoder count)
    int target_count = round(dist_per_count*distance);

    return target_count;
}


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

    if (direction < 0.0){
        while(direction < 0.0){
            direction += direction +360.0;
        }
    } else if(direction >= 360.0){
        while(direction >= 360.0){
            direction -= direction - 360.0;
        }
    } 

    if (direction >= 180.0){
        direction = ABS(direction - 360.0);
    }
   
   // //Distance to travel for a 180 degree arc is known. Find the % of that distance for this particular turn.
    float turn_ratio = direction/180.0; 
    float distance = round(turn_ratio*half_rot_dist); 

    int32_t target_count = round(dist_per_count*distance); //Convert the arc distance to travel into encoder counts

    return target_count;
    //return turn_target;
}

//This function is linked to the task that will update the controller at the specified discretization interval.
//It will update the controller and call the Set_PWM function based on the signal.
//One of its arguments must be the controller struct (left or right) to update
//It will be deactivated in the main loop when the encoder count is close enough to its target
float send_controller_update(Controller_t* p_cont, float measurement, float dt){
    //float Controller_Update( Controller_t* p_cont, float measurement, float dt )

    /*while(ABS(updated_error > 24)){ //Encoder count of 24 correlates to 1/8 inch

                        float meas = (float)updated_enc;
                        float u = Controller_Update(&right_cont, meas, update_period);
                        u = Saturate(u,MAX_PWM);
                        int16_t input_sig = round(u);

                        Set_Motor_Values(0,input_sig);


                        updated_enc = Encoder_Counts_Right(); 
                        updated_error = updated_enc - target;
*/
    float meas = (float)updated_enc;

    
    float u = Controller_Update(p_cont, meas, dt);

}