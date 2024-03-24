#include "MotorPWM.h"

#include "SerialIO.h"

/**
 * Function Initialize_Motor_PWM initializes the motor PWM on Timer 1 for PWM based voltage control of the motors.
 * The Motor PWM system shall initialize in the disabled state for safety reasons. You should specifically enable
 * Motor PWM outputs only as necessary.
 * @param [uint16_t] MAX_PWM is the maximum PWM value to use. This controls the PWM frequency.
 */
void Initialize_MotorPWM( uint16_t MAX_PWM )
{
    // set the timer mode to freq and phase correct pwm
    TCCR1A &= ~( 1 << WGM10 );
    TCCR1A &= ~( 1 << WGM11 );
    TCCR1B &= ~( 1 << WGM12 );
    TCCR1B |= ( 1 << WGM13 );

    // left motor non inverting mode: clear on match up and set on match down
    TCCR1A &= ~( 1 << COM1A0 );
    TCCR1A |= ( 1 << COM1A1 );
    // left motor data direction input for direction and pwm pin
    DDRB |= ( 1 << PB6 );
    DDRB |= ( 1 << PB2 );

    // right motor non inverting mode: clear on match up and set on match down
    // right motor data direction input for direction and pwm pin
    TCCR1A &= ~( 1 << COM1B0 );
    TCCR1A |= ( 1 << COM1B1 );
    // right motor data direction input for direction and pwm pin
    DDRB |= ( 1 << PB5 );
    DDRB |= ( 1 << PB1 );

    // set TOP value
    ICR1 = MAX_PWM;
}

/**
 * Function MotorPWM_Enable enables or disables the motor PWM outputs.
 * @param [bool] enable (true set enable, false set disable)
 */
void MotorPWM_Enable( bool enable )
{
    if( enable ) {
        // set timer value to 0
        TCNT1 = 0;

        // set to use internal clock no pre-scaling
        TCCR1B |= ( 1 << CS10 );
        TCCR1B &= ~( 1 << CS11 );
        TCCR1B &= ~( 1 << CS12 );
    } else {
        // no clock source set timer stopped
        TCCR1B &= ~( 1 << CS10 );
        TCCR1B &= ~( 1 << CS11 );
        TCCR1B &= ~( 1 << CS12 );

        // stop right and left motors
        PORTB &= ~( 1 << PB5 );
        PORTB &= ~( 1 << PB6 );
    }
}

/**
 * Function MotorPWM_Is_Enabled returns if the motor PWM is enabled for output.
 * @param [bool] true if enabled, false if disabled
 */
bool MotorPWM_Is_Enabled()
{
    return bit_is_set( TCCR1B, CS10 );
    // return (TCCR1B & (1 << CS10));
}

/**
 * Function MotorPWM_Set_Left sets the PWM duty cycle for the left motor.
 * @return [int32_t] The count number.
 */
void MotorPWM_Set_Left( int16_t pwm )
{
    // int16_t data = TCNT1;
    // USB_Send_Msg( "ch", 'l', &data, sizeof( data ) );

    if( pwm < 0 ) {
        PORTB |= ( 1 << PB2 );
        OCR1B = -pwm;
    } else {
        OCR1B = pwm;
        PORTB &= ~( 1 << PB2 );
    }
}

/**
 * Function MototPWM_Set_Right sets the PWM duty cycle for the right motor.
 * @return [int32_t] The count number.
 */
void MototPWM_Set_Right( int16_t pwm )
{
    // int16_t data = TCNT1;
    // USB_Send_Msg( "ch", 'r', &data, sizeof( data ) );
    // if( ( pwm < 0 && bit_is_clear( PINB, PB1 ) ) || ( pwm > 0 && bit_is_set( PINB, PB1 ) ) ) {
    //     OCR1A = 0;
    // }

    if( pwm < 0 ) {
        PORTB |= ( 1 << PB1 );
        OCR1A = -pwm;
    } else {
        OCR1A = pwm;
        PORTB &= ~( 1 << PB1 );
    }
}

/**
 * Function MotorPWM_Get_Left returns the current PWM duty cycle for the left motor. If disabled it returns what the
 * PWM duty cycle would be.
 * @return [int16_t] duty-cycle for the left motor's pwm
 */
int16_t MotorPWM_Get_Left()
{
    return OCR1B;
}

/**
 * Function MotorPWM_Get_Right returns the current PWM duty cycle for the right motor. If disabled it returns what the
 * PWM duty cycle would be.
 * @return [int16_t] duty-cycle for the right motor's pwm
 */
int16_t MotorPWM_Get_Right()
{
    return OCR1A;
}

/**
 * Function MotorPWM_Get_Max() returns the PWM count that corresponds to 100 percent duty cycle (all on), this is the
 * same as the value written into ICR1 as (TOP).
 */
uint16_t MotorPWM_Get_Max()
{
    return ICR1;
}

/**
 * Function MotorPWM_Set_Max sets the maximum pwm count. This function sets the timer counts to zero because
 * the ICR1 can cause undesired behaviors if change dynamically below the current counts.  See page 128 of the
 * atmega32U4 datasheat.
 */
void MotorPWM_Set_Max( uint16_t MAX_PWM )
{
    ICR1  = MAX_PWM;
    TCNT1 = 0;
}