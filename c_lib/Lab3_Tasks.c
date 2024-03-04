#include "Lab3_Tasks.h"

void Send_Encoder( float _time_since_last )
{
    struct __attribute__( ( __packed__ ) ) {
        float left_counts;
        float right_counts;
    } encoder;

    encoder.left_counts  = Encoder_Counts_Left();
    encoder.right_counts = Encoder_Counts_Right();

    if( task_encoder_read.run_period > 0 )
        USB_Send_Msg( "cff", 'E', &encoder, sizeof( encoder ) );
    else
        USB_Send_Msg( "cff", 'e', &encoder, sizeof( encoder ) );
    return;
}

void Send_Battery( float _time_since_last )
{
    struct __attribute__( ( __packed__ ) ) {
        float bat_volt;
    } battery;

    //Retrieve unfiltered battery voltage
    battery.bat_volt = Battery_Voltage();

    //Add the unfiltered voltage to the filter and retrieve the filtered value
    battery.bat_volt = Filter_Value(&battery_filt, battery.bat_volt);

    if( task_battery_read.run_period > 0 )
        USB_Send_Msg( "cf", 'B', &battery, sizeof( battery ) );
    else
        USB_Send_Msg( "cf", 'b', &battery, sizeof( battery ) );
    return;
}

// Samples the current battery voltage
void Sample_Battery( float _time_since_last )
{

    Filter_Value( &battery_filt, 2.0f );

    return;
}