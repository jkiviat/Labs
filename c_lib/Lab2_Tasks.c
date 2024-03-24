#include "Lab2_Tasks.h"

void Send_Time_Now( float _time_since_last ){

    Send_Time('T', 0, Timing_Get_Time_Sec());

}

void Send_Loop_Time( float _time_since_last ){

    Send_Time('T', 1, loop_time);

}

void Send_Time( char command, uint8_t type, float time ){
    struct __attribute__( ( __packed__ ) ) {
        uint8_t type;
        float time;
    } data;

    data.time = time;
    data.type = type;

    USB_Send_Msg("ccf", command, &data, sizeof(data));
}