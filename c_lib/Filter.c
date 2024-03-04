/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2023.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*js
    Copyright (c) 2023 Andrew Petruska at Colorado School of Mines

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

#include "Filter.h"

/**
 * Function Filter_Init initializes the filter given two float arrays and the order of the filter.  Note that the
 * size of the array will be one larger than the order. (First order systems have two coefficients).
 *
 *  1/A_0*( SUM( B_i * input_i )  -   SUM( A_i * output_i) )
 *         i=0..N                    i=1..N
 *
 *  Note a 5-point moving average filter has coefficients:
 *      numerator_coeffs   = { 5 0 0 0 0 };
 *      denominator_coeffs = { 1 1 1 1 1 };
 *      order = 4;
 *
 * @param p_filt pointer to the filter object
 * @param numerator_coeffs The numerator coefficients (B/beta traditionally)
 * @param denominator_coeffs The denominator coefficients (A/alpha traditionally)
 * @param order The filter order
 */
void Filter_Init( Filter_Data_t* p_filt, float* numerator_coeffs, float* denominator_coeffs, uint8_t order )
{
    int length = order + 1;
    rb_initialize_F(&(p_filt -> numerator));
    rb_initialize_F(&(p_filt -> denominator));
    rb_initialize_F(&(p_filt -> in_list));
    rb_initialize_F(&(p_filt -> out_list));
    
    for(int i = 0; i < length; i++){
        rb_push_back_F(&(p_filt -> numerator), numerator_coeffs[i]);
        rb_push_back_F(&(p_filt -> denominator), denominator_coeffs[i]);
        rb_push_back_F(&(p_filt -> in_list), 0); //INITIALIZE TO ZERO? SAME LENFTH AS NUMERATOR AND DENOMINATOR
        rb_push_back_F(&(p_filt -> out_list), 0); //INITIALIZE TO ZERO? SAME LENFTH AS NUMERATOR AND DENOMINATOR
    }
    return;
}

/**
 * Function Filter_ShiftBy shifts the input list and output list to keep the filter in the same frame. This especially
 * useful when initializing the filter to the current value or handling wrapping/overflow issues.
 * @param p_filt
 * @param shift_amount
 */
void Filter_ShiftBy( Filter_Data_t* p_filt, float shift_amount )
{
    int in_list_length = rb_length_F(&(p_filt -> in_list));
    int out_list_length = rb_length_F(&(p_filt -> out_list));
    for(int i = 0; i <= in_list_length; i++){
        rb_set_F(&(p_filt -> in_list),i,shift_amount);
        rb_set_F(&(p_filt -> out_list),i,shift_amount);
    }

    //WHY IS "SHIFT AMOUNT" A FLOAT AND WHY WOULD IT EVER BE ANYTING OTHER THAN 1?
    //WHY IS THE FILTER_SHIFTBY TEST THE SAME AS THE FILTER_SETTO LIST?
    //WOULD THINK THAT THE A AND B COEFFICIENTS ARE SET BY THE DESIRED FILTER TYPE AND CUTOFF FREQ, 
    //  SO THOSE WOULD BE CONSTANT FROM THE TIME THEY ARE INITIALIZED,
    //  A NEW ERROR AND IS RECORDED FROM THE PREVIOUS BATTERY READING, ALL THE IN_LIST AND OUT_LIST
    //  VALUES SHOULD BE PUSHED DOWN BY ONLY 1
    return;
}

/**
 * Function Filter_SetTo sets the initial values for the input and output lists to a constant defined value. This
 * helps to initialize or re-initialize the filter as desired.
 * @param p_filt Pointer to a Filter_Data sturcture
 * @param amount The value to re-initialize the filter to.
 */
void Filter_SetTo( Filter_Data_t* p_filt, float amount )
{
    //based on the main.c testing function, this appears to do exactly the same thing as ShiftBy?
    int in_list_length = rb_length_F(&(p_filt -> in_list));
    int out_list_length = rb_length_F(&(p_filt -> out_list));
    for(int i = 0; i <= in_list_length; i++){
        rb_set_F(&(p_filt -> in_list),i,amount);
        rb_set_F(&(p_filt -> out_list),i,amount);
    }
    return;
}

float Filter_Value( Filter_Data_t* p_filt, float value )
{
    //"value" is the new error (input_list)
    //pop the input list from the back
    rb_pop_back_F(&(p_filt -> in_list));
    //push the new error value to the front of the input list
    rb_push_front_F(&(p_filt -> in_list), value);

    float A_0 = rb_get_F(&(p_filt -> denominator),0);
    float ans = 0.0;
    float B_input = 0.0;
    float A_output = 0.0;
    float B_input_tmp = 0.0;
    float A_output_tmp = 0.0;

    int in_list_length = rb_length_F(&(p_filt -> in_list));
    for(int i = 0; i < in_list_length; i++){
        B_input_tmp = (rb_get_F(&(p_filt -> numerator),i)*rb_get_F(&(p_filt -> in_list),i)); 
        B_input = B_input + B_input_tmp;
    }

    for(int i = 0; i <= in_list_length; i++){ //want this to be only 4 iterations long
        A_output_tmp = rb_get_F(&(p_filt -> denominator),(i+1))*rb_get_F(&(p_filt -> out_list),i);
        A_output = A_output + A_output_tmp;
    }
    ans = (B_input - A_output)/A_0;

    //push this value to the front of the out_list
    rb_push_front_F(&(p_filt -> out_list),ans);
    //pop the output_list from the back
    rb_pop_back_F(&(p_filt -> out_list));
    float test_output = rb_get_F(&(p_filt -> out_list),0);
    return rb_get_F(&(p_filt -> out_list),0);
}

/**
 * Function Filter_Last_Output returns the most up-to-date filtered value without updating the filter.
 * @return The latest filtered value
 */
float Filter_Last_Output( Filter_Data_t* p_filt )
{
    float ans = rb_get_F(&(p_filt -> out_list),0);
    return ans;
}
