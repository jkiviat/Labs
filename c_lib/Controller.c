#include "Controller.h"
//#include "Lab5_Tasks.h"
//#include <math.h>
//#include <stdlib.h>
//#include "Filter.h"
/**
 * Function Initialize_Controller sets up the z-transform based controller for the system.
 */
void Initialize_Controller(Controller_t* p_cont, float kp, float* num, float* den, uint8_t order, float update_period){

    Filter_Init(&p_cont -> controller, num, den, order);
    p_cont -> kp = kp;
    p_cont -> update_period = update_period;
    p_cont -> target_pos = 0;
    p_cont -> target_vel = 0;
}

/**
 * Function Controller_Set_Target_Velocity sets the target velocity for the 
 * controller.
 */
void Controller_Set_Target_Velocity( Controller_t* p_cont, float vel ){
    //NEEDS TO BE CONVERTED TO ENCODER SPACE BEFORE PASSING VALUE INTO THIS FUNCTION
    //VELOCITY IN TERMS OF INCHES/SECOND
    p_cont -> target_vel = vel;

}

/**
 * Function Controller_Set_Target_Position sets the target postion for the 
 * controller, this also sets the target velocity to 0.
 */
void Controller_Set_Target_Position( Controller_t* p_cont, float vel ){
    //NEEDS TO BE CONVERTED TO ENCODER SPACE BEFORE PASSING VALUE INTO THIS FUNCTION
    /*
    int32_t Set_Encoder_Target_Right(int32_t current_encoder_count, float dist, float angle){

    //arc length = angle(in radians)*radius. radius = track_sep/2. 
    int32_t angle_enc = round(angle*(deg_to_rad)*(track_sep/2)*count_per_dist);

    int32_t straight_dist_enc = round(dist*count_per_dist);

    int32_t target_encoder_count = straight_dist_enc + angle_enc;

    return target_encoder_count;
}
    
    */
    
    p_cont -> target_vel = 0;
    p_cont -> target_pos = vel;
    
}

/**
 * Function Controller_Update takes in a new measurement and returns the
 * new control value.
 */
float Controller_Update( Controller_t* p_cont, float measurement, float dt ){

    float count_per_inch = 909.7/(2*M_PI*0.765);
    //***IF TARGET VELOCITY IS ZERO, USE POSITION AS TARGET
    float target;
    if(p_cont -> target_vel != 0){
        target = p_cont -> target_vel; 
        //update targeted encoder count for velocity mode
        int32_t target = measurement + round((target*count_per_inch*dt));
    } else {
        target = p_cont -> target_pos; //
    }

    float output = Filter_Value(&p_cont -> controller, measurement);

    float u = (p_cont -> kp)*(target - output);

    return u;
}

/**
 * Function Controller_Last returns the last control command
 */
float Controller_Last( Controller_t* p_cont){
    //float Filter_Last_Output( Filter_Data_t* p_filt )
//{
    //return rb_get_F(&p_filt->out_list, 0);
//}
    return Filter_Last_Output(&p_cont -> controller);

}

/**
 * Function Controller_SettTo sets the Filter's input and output lists
 * to match the measurement so it starts with zero error.
 */
void Controller_SetTo(Controller_t* p_cont, float measurement ){

//Function Filter_SetTo sets the initial values for the input and output lists to a constant defined value. This
 //helps to initialize or re-initialize the filter as desired.
    //void Filter_SetTo( Filter_Data_t* p_filt, float amount )
//{
    
    //for(int i = 0; i < rb_length_F(&p_filt->in_list); i++){
        //rb_set_F(&p_filt->in_list, i, amount);
    //}

    //for(int i = 0; i < rb_length_F(&p_filt->out_list); i++){
        //rb_set_F(&p_filt->out_list, i, amount);
    //}
    //return;

    //Set target equal to measurement (error = measurement - target)
    p_cont -> target_pos = measurement; //may need to zero this out depending on position of velocity mode
    p_cont -> target_vel = measurement;

    Filter_SetTo(&p_cont -> controller, measurement);

}


/**
 * Function Controller_ShiftBy shifts the Filter's input and output lists
 * by the desired amount. This is helpful when dealing with wrapping.
 */
void Controller_ShiftBy(Controller_t* p_cont, float measurement ){

    /*
 * Function Filter_ShiftBy shifts the input list and output list to keep the filter in the same frame. This especially
 * useful when initializing the filter to the current value or handling wrapping/overflow issues.
 * @param p_filt
 * @param shift_amount
 *
void Filter_ShiftBy( Filter_Data_t* p_filt, float shift_amount )
{
    for(int i = 0; i < rb_length_F(&p_filt->in_list); i++){
        float shifted = rb_get_F(&p_filt->in_list, i) + shift_amount;
        rb_set_F(&p_filt->in_list, i, shifted);
    }

    for(int i = 0; i < rb_length_F(&p_filt->out_list); i++){
        float shifted = rb_get_F(&p_filt->out_list, i) + shift_amount;
        rb_set_F(&p_filt->out_list, i, shifted);
    }
    return;
}
    */

   Filter_ShiftBy(&p_cont -> controller, measurement);

}
