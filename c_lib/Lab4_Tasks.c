#include "Lab4_Tasks.h"

void Set_Motor_Values( int16_t left, int16_t right )
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
        MotorPWM_Set_Left( left );
    }
}

void Set_PWM_Zero( float _time_since_last )
{
    MototPWM_Set_Right( 0 );
    MotorPWM_Set_Left( 0 );
    Task_Cancel( &task_set_pwm_zero );
}

void Motor_Stop_And_Disable()
{
    MototPWM_Set_Right( 0 );
    MotorPWM_Set_Left( 0 );
    MotorPWM_Enable( false );
}

void Send_System_ID( float _time_since_last )
{
    struct {
        float time;
        int16_t pwm_left;
        int16_t pwm_right;
        int16_t encoder_left;
        int16_t encoder_right;
    } data;

    data.time          = Timing_Get_Time_Sec();
    data.pwm_left      = MotorPWM_Get_Left();
    data.pwm_right     = MotorPWM_Get_Right();
    data.encoder_left  = Encoder_Counts_Left();
    data.encoder_right = Encoder_Counts_Right();

    USB_Send_Msg( "cfhhhh", 'Q', &data, sizeof( data ) );
}
//commit