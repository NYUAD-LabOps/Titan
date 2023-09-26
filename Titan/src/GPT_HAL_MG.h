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



/**
 * Timer_HAL_MG.h
 * Header files for functions written in Timer_HAL_MG.c
 */

#include "hal_data.h"
#ifndef GPT_HAL_MG_H_
#define GPT_HAL_MG_H_
ssp_err_t error_handle(ssp_err_t err, const char* err_msg);
ssp_err_t init_timers (void);
ssp_err_t start_timers (void);
ssp_err_t open_timers (void);
ssp_err_t stop_timers (void);
ssp_err_t set_timer_period (int timer, timer_size_t period, timer_unit_t unit);
ssp_err_t set_timer_pwm (timer_size_t duty_cycle, timer_pwm_unit_t unit);
ssp_err_t get_timer_info(timer_info_t* gptTimer0_info, timer_info_t* gptTimer1_info, timer_info_t* gptTimer2_info);
ssp_err_t reset_timer(void);
ssp_err_t get_timer_value(timer_size_t* gptTimer0_value, timer_size_t* gptTimer1_value, timer_size_t* gptTimer2_value);

#endif /* TIMER_HAL_MG_H_ */
