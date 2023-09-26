/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/** The Application Project demonstrates the typical use of the GPT HAL APIs using
 * three GPT modules. The Application Project main thread entry initializes the timers,
 *  starts, and reconfigures the period and duty cycle parameters. When GPT 0 and GPT 2 expire,
 *  their respective callbacks toggle an LED. GPT 1 is configured to be a PWM timer. Information about
 *  the timers will be displayed on the Debug Console using the common semi-hosting function.
 * */

/**
 * GPT_HAL_MG.c
 * Functions to open, start, configure and close timer.
 *  Quick tips to customize this example:
 *  1) Configure the period setting of timers using the set_timer_period function
 *  2) Configure the PWM setting of g_timer_gpt_1 (pwm GPT) using the set_timer_pwm function
 *  3) Callbacks can be edited to achieve what the user has in mind. The example toggles an LED in each callback currently.
 *
 *  By default, printf is disabled. To enable printf, enable the #define SEMI_HOSTING in the hal_entry.c and GPT_HAL_MG.c file
 */

#include <Titan.h>
#include <GPT_HAL_MG.h>
#include <stdio.h>
#include "hal_data.h"
#define SEMI_HOSTING

ssp_err_t error_handle(ssp_err_t err, const char *err_msg)
{
    if (SSP_SUCCESS != err)
    {

        if (DEBUGGER)
            printf (err_msg);
        if (DEBUGGER)
            printf (" err = 0x%x", err);

        while (1)
            ;
    }
    return err;
}

/**
 * Function to call the Initialization, configuration and starting of timers
 **/
ssp_err_t init_timers(void)
{

    ssp_err_t err;

    timer_size_t gptTimer0Val;
    timer_size_t gptTimer1Val;
    timer_size_t gptTimer2Val;

    timer_info_t gptTimer0Info;
    timer_info_t gptTimer1Info;
    timer_info_t gptTimer2Info;

    err = open_timers ();
    error_handle (err, "open_timers failed");

//    err = start_timers();
//    error_handle(err, "start_timers failed");

//    err = set_timer_period(0, 2,TIMER_UNIT_PERIOD_SEC); // Set g_timer_gpt_0 to have a period of 2s
//    error_handle(err, "set_timer_period failed");

//    err = set_timer_pwm(50, TIMER_PWM_UNIT_PERCENT); // Set g_timer_gpt_1 to have a duty cycle of 30%
//    error_handle(err, "set_timer_PWM failed");
//
//
//
//    if ( (SSP_SUCCESS == err) && ( (gptTimer0Info.status == TIMER_STATUS_STOPPED) || (gptTimer1Info.status == TIMER_STATUS_STOPPED) || (gptTimer2Info.status == TIMER_STATUS_STOPPED) ) )
//    {
//        // Up to User to decide what to do
//    }

    return err;
}

/**
 * Open timer drivers and initialize the timers
 **/
ssp_err_t open_timers(void)
{

    ssp_err_t err;

    err = g_timerX.p_api->open (g_timerX.p_ctrl, g_timerX.p_cfg);
    error_handle (err, "open failed");

    err = g_timerY.p_api->open (g_timerY.p_ctrl, g_timerY.p_cfg);
    error_handle (err, "open failed");

    err = g_timerZ.p_api->open (g_timerZ.p_ctrl, g_timerZ.p_cfg); // Open timers
    error_handle (err, "open failed");

    err = g_timerA.p_api->open (g_timerA.p_ctrl, g_timerA.p_cfg); // Open timers
    error_handle (err, "open failed");

    err = g_timerC.p_api->open (g_timerC.p_ctrl, g_timerC.p_cfg); // Open timers
    error_handle (err, "open failed");

    err = g_timerT.p_api->open (g_timerT.p_ctrl, g_timerT.p_cfg); // Open timers
    error_handle (err, "open failed");
//    err = g_timer_gpt_1.p_api->open(g_timer_gpt_1.p_ctrl, g_timer_gpt_1.p_cfg);
//    error_handle(err, "open failed");

//    err = g_timer_gpt_10.p_api->open(g_timer_gpt_10.p_ctrl, g_timer_gpt_10.p_cfg);
//    error_handle(err, "open failed");

    return err;
}

/**
 * Start all timers
 **/
ssp_err_t start_timers(void)
{

    ssp_err_t err;
//
//    err = g_timer_gpt_0.p_api->start(g_timer_gpt_0.p_ctrl); // Start timer
//    if(SSP_SUCCESS != err)
//    error_handle(err, "start failed");
//
////    err = g_timer_gpt_1.p_api->start(g_timer_gpt_1.p_ctrl);
////    error_handle(err, "start failed");
//
//
//    err = g_timer_gpt_2.p_api->start(g_timer_gpt_2.p_ctrl);
//    error_handle(err, "start failed");
//
//    err = g_timer_gpt_3.p_api->start(g_timer_gpt_3.p_ctrl);
//    error_handle(err, "start failed");
//
//    err = g_timer_gpt_4.p_api->start(g_timer_gpt_4.p_ctrl);
//    error_handle(err, "start failed");
//
    return err;
}

/**
 * Stop all timers
 **/
ssp_err_t stop_timers(void)
{

    ssp_err_t err;

//    err = g_timer_gpt_0.p_api->stop(g_timer_gpt_0.p_ctrl); // Stop timers
//    error_handle(err, "stop failed");
//
////    err = g_timer_gpt_1.p_api->stop(g_timer_gpt_1.p_ctrl);
////    error_handle(err, "stop failed");
//
//    err = g_timer_gpt_2.p_api->stop(g_timer_gpt_2.p_ctrl);
//    error_handle(err, "stop failed");

    return err;
}

/**
 * Set period and period unit for specified timer
 * timer    - Which timer to set period/period unit
 *              1 - g_timer_gpt_0
 *              2 - g_timer_gpt_1
 *              3 - g_timer_agt_0
 *              4 - g_timer_agt_1
 * period   - Value of time until timer expires
 * unit     - Unit of period paramter. Find the enumeration for this in the r_timer_api.h
 **/
ssp_err_t set_timer_period(int timer, timer_size_t period, timer_unit_t unit)
{
#ifdef SEMI_HOSTING
    if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
    {
        if (DEBUGGER)
            printf ("Setting timer period ... \r\n");
    }
#endif
    ssp_err_t err;

//    if(0 == timer)
//    {
//        err = g_timer_gpt_0.p_api->periodSet(g_timer_gpt_0.p_ctrl, period, unit);
//    }

//    else if (1 == timer)
//    {
//        err = g_timer_gpt_1.p_api->periodSet(g_timer_gpt_1.p_ctrl, period, unit);
//
//    }

//    else if (2 == timer)
//    {
//        err = g_timer_gpt_2.p_api->periodSet(g_timer_gpt_2.p_ctrl, period, unit);
//    }

//    else
//    {
//#ifdef SEMI_HOSTING
//        if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
//        {
//            printf("Requested timer not found\r\n!");
//        }
//#endif
//
//        err = SSP_ERR_ASSERTION;
//    }

    error_handle (err, "periodSet failed");

    return err;
}

/**
 * Set period and period unit for PWM timer (g_timer_gpt_1)
 * duty_cycle   - Duty cycle of PWM timer
 * unit         - Unit of PWM paramter. Find the enumeration for this in the r_timer_api.h file
 **/
//ssp_err_t set_timer_pwm(timer_size_t duty_cycle, timer_pwm_unit_t unit)
//{
//    ssp_err_t err;
//    err = g_timer_gpt_3.p_api->dutyCycleSet (g_timer_gpt_3.p_ctrl, duty_cycle, unit, 0);
//    err = g_timer_gpt_4.p_api->dutyCycleSet (g_timer_gpt_4.p_ctrl, duty_cycle, unit, 0);
//    err = g_timer_gpt_7.p_api->dutyCycleSet (g_timer_gpt_7.p_ctrl, duty_cycle, unit, 0);
////    err = g_timer_gpt_10.p_api->dutyCycleSet(g_timer_gpt_10.p_ctrl, duty_cycle, unit, 0);
//    return err;
//}
/**
 * Get counter value of timer
 * gptTimer0_value  - Parameter to pass counter value of g_timer_gpt_0
 * gptTimer1_value  - Parameter to pass counter value of g_timer_gpt_1
 * agtTimer0_value  - Parameter to pass counter value of g_timer_apt_0
 * agtTimer0_value  - Parameter to pass counter value of g_timer_apt_1
 **/

ssp_err_t get_timer_value(timer_size_t *gptTimer0_value, timer_size_t *gptTimer1_value, timer_size_t *gptTimer2_value)
{

    ssp_err_t err;

//    err = g_timer_gpt_7.p_api->counterGet(g_timer_gpt_7.p_ctrl, gptTimer7_value);

//    err = g_timer_gpt_1.p_api->counterGet(g_timer_gpt_0.p_ctrl, gptTimer1_value);

//    err = g_timer_gpt_10.p_api->counterGet(g_timer_gpt_10.p_ctrl, gptTimer10_value);
    if (SSP_SUCCESS == error_handle (err, "get_timer_value failed"))
    {
#ifdef SEMI_HOSTING
        if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
        {
//            printf("Timer Value for Timer 0 is %d\r\n", (int)*gptTimer7_value);
//            printf("Timer Value for Timer 1 is %d\r\n", (int)*gptTimer10_value);
//            printf("Timer Value for Timer 2 is %d\r\n\r\n", (int)*gptTimer3_value);
        }
#endif
    }

    return err;
}

/**
 * Reset all Timers
 **/
ssp_err_t reset_timer(void)
{
    ssp_err_t err;

//    err = g_timer_gpt_0.p_api->reset(g_timer_gpt_0.p_ctrl);
//    error_handle(err, "reset failed");
//
////    err = g_timer_gpt_1.p_api->reset(g_timer_gpt_1.p_ctrl);
////    error_handle(err, "reset failed");
//
//    err = g_timer_gpt_2.p_api->reset(g_timer_gpt_2.p_ctrl);
//    error_handle(err, "reset failed");

    return err;
}

/**
 * Get information of timer
 *          Member status in timer_info_t (timer_info_t.status) will return a value of 0 when the timer is counting,
 *          and a value of 1 when it is stopped.
 *
 * gptTimer0_value  - Parameter to pass information of g_timer_gpt_0
 * gptTimer1_value  - Parameter to pass information of g_timer_gpt_1
 * agtTimer0_value  - Parameter to pass information of g_timer_apt_0
 * agtTimer0_value  - Parameter to pass information of g_timer_apt_1
 **/
ssp_err_t get_timer_info(timer_info_t *gptTimer0_info, timer_info_t *gptTimer1_info, timer_info_t *gptTimer2_info)
{
    ssp_err_t err;
    timer_info_t tempGptTimer0_info;
    timer_info_t tempGptTimer1_info;
    timer_info_t tempGptTimer2_info;

//    err = g_timer_gpt_0.p_api->infoGet(g_timer_gpt_0.p_ctrl, &tempGptTimer0_info);
//    error_handle(err, "infoget failed");
//
//
////    err = g_timer_gpt_1.p_api->infoGet(g_timer_gpt_1.p_ctrl, &tempGptTimer1_info);
////    error_handle(err, "infoget failed");
//
//    err = g_timer_gpt_2.p_api->infoGet(g_timer_gpt_2.p_ctrl, &tempGptTimer2_info);
//    error_handle(err, "infoget failed");

    *gptTimer0_info = tempGptTimer0_info;
    *gptTimer1_info = tempGptTimer1_info;
    *gptTimer2_info = tempGptTimer2_info;

#ifdef SEMI_HOSTING
    if (SSP_SUCCESS != error_handle (err, "infoget failed"))
    {
        while (1)
            ;
    }

    else
    {
        if (DEBUGGER)
        {
            if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
            {
                printf ("Timer Info for Timer 0: \r\n");
                printf ("Timer Direction: %d\r\n", (int) tempGptTimer0_info.count_direction);
                printf ("Clock Frequency: %d\r\n", (int) tempGptTimer0_info.clock_frequency);
                printf ("Period Counts: %d\r\n", (int) tempGptTimer0_info.period_counts);
                printf ("Status: %d\r\n\r\n", (int) tempGptTimer0_info.status);

                printf ("Timer Info for Timer 1: \r\n");
                printf ("Timer Direction: %d\r\n", (int) tempGptTimer1_info.count_direction);
                printf ("Clock Frequency: %d\r\n", (int) tempGptTimer1_info.clock_frequency);
                printf ("Period Counts: %d\r\n", (int) tempGptTimer1_info.period_counts);
                printf ("Status: %d\r\n\r\n", (int) tempGptTimer1_info.status);

                printf ("Timer Info for Timer 2: \r\n");
                printf ("Timer Direction: %d\r\n", (int) tempGptTimer2_info.count_direction);
                printf ("Clock Frequency: %d\r\n", (int) tempGptTimer2_info.clock_frequency);
                printf ("Period Counts: %d\r\n", (int) tempGptTimer2_info.period_counts);
                printf ("Status: %d\r\n\r\n", (int) tempGptTimer2_info.status);
            }
        }
    }
#endif
    return err;

}

/**
 * User callback function for g_timer_gpt_0. Callback function blinks the LED0
 * p_args   - Unused
 **/

void motor1_callback(timer_callback_args_t *p_args)
{

}

void motor2_callback(timer_callback_args_t *p_args)
{

}

void motor3_callback(timer_callback_args_t *p_args)
{

}

/**
 * User callback function for g_timer_gpt_0. Callback function blinks the LED0
 * p_args   - Unused
 **/
//void timer2_callback(timer_callback_args_t * p_args)
//{
//
//    bsp_leds_t leds;
//    ioport_level_t level;
//    if (TIMER_EVENT_EXPIRED  == p_args->event)
//    {
//        R_BSP_LedsGet(&leds);
//
//        g_ioport.p_api->pinRead(leds.p_leds[1], &level); // Read output of LED
//
//        if (IOPORT_LEVEL_LOW == level)
//        {
//            level = IOPORT_LEVEL_HIGH;
//        }
//
//        else
//        {
//            level = IOPORT_LEVEL_LOW;
//        }
//
//        g_ioport.p_api->pinWrite(leds.p_leds[1], level); // Toggle LED output
//    }
//}
