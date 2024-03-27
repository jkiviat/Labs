#include "Battery_Monitor.h"

#include "SerialIO.h"

static const float BITS_TO_BATTERY_VOLTS = 0.004882813f;
// 4th order, fs = 0.002, fc = 15 => doesn't work
// float numerator[]                        = { 1.0e-03 * 0.0624, 1.0e-03 * 0.2495, 1.0e-03 * 0.3743, 1.0e-03 * 0.2495, 1.0e-03 * 0.624 };
// float numerator[]                        = { 0.0000624, 0.0002495, 0.0003743, 0.0002495, 0.000624 };
// float denominator[]                      = { 1.0000, -3.5078, 4.6409, -2.7427, 0.6105 };

// 3th order, fs = 0.01, fc = 15 => works
// float numerator[] = {0.0495, 0.1486, 0.1486, 0.0495};
// float denominator[] = {1, -1.1619, 0.6959, -0.1378};

// 3rd order, fs = 0.001, fc = 15 => doesn't work
float numerator[]   = { 0.0000954, 0.0002863, 0.0002863, 0.0000954 };
float denominator[] = { 1, -2.8116, 2.6405, -0.8281 };

// 4th order, fs = 0.01, fc = 10 => works
// float numerator[] = {0.0048, 0.0193, 0.0289, 0.0193, 0.0048};
// float denominator[] = {1, -2.3695, 2.3140, -1.0547, 0.1874};

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{
    // setup 128 prescalar
    ADCSRA |= ( 1 << ADPS0 );
    ADCSRA |= ( 1 << ADPS1 );
    ADCSRA |= ( 1 << ADPS2 );

    ADMUX |= ( 1 << REFS0 );
    ADMUX |= ( 1 << REFS1 );

    // enable ADC
    ADCSRA |= ( 1 << ADEN );

    // initialize filter
    Filter_Init( &filt, numerator, denominator, 4 );
    Filter_SetTo( &filt, 5.0f );
}

/**
 * Function Battery_Voltage initiates the A/D measurement and returns the result for the battery voltage.
 */
float Battery_Voltage()
{
    // wait for ADC to be free
    while( ADCSRA & ( 1 << ADSC ) ) {};

    // set ADC to read from 6
    ADMUX &= ~( 1 << MUX5 );
    ADMUX &= ~( 1 << MUX4 );
    ADMUX &= ~( 1 << MUX3 );
    ADMUX |= ( 1 << MUX2 );
    ADMUX |= ( 1 << MUX1 );
    ADMUX &= ~( 1 << MUX0 );

    // start conversion
    ADCSRA |= ( 1 << ADSC );

    // wait for ADC to complete
    while( ADCSRA & ( 1 << ADSC ) ) {};

    // A Union to assist with reading the LSB and MSB in the  16 bit register
    union {
        struct {
            uint8_t LSB;
            uint8_t MSB;
        } split;
        uint16_t value;
    } data = { .value = 0 };

    data.split.LSB = ADCL;
    data.split.MSB = ADCH;

    float bat_volt_measured = data.value * BITS_TO_BATTERY_VOLTS;
    // return bat_volt_measured;
    return Filter_Value( &filt, bat_volt_measured );
}

//commit