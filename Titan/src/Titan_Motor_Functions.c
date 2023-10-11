#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Titan.h>
#include <TitanMain.h>
#include "math.h"



///The motor handler function makes speed and direction adjustments, and tracks position. Position is
/// calculated based on number of steps taken and the step size.
void motorHandler(struct motorController *motorBlock)
{
    int i;
    int prevTargetDir;
    int targetFreq, backOffSteps;
    double prevTargetSpeed, origSpeed;

    ///Enforce soft limits prior to any motor operations.
    ///Only applies to referenced motors.
    if (motorBlock->referenced && motorBlock->hasLimits)
    {
        if (motorBlock->pos > SOFTLIMITFORWARD || motorBlock->pos < SOFTLIMITREVERSE)
        {
            stopMotor (motorBlock);
        }
    }

    ///If the frequency adjustment flag is active, set the motor speed accordingly,
    /// else calculate and set frequency according to target speed and step size.
    ///if (motorBlock->limit0State == IOPORT_LEVEL_HIGH && motorBlock->homing == 0)
    if (motorBlock->homing == 0)
    {
        ///First check if it's to be set per frequency or speed
        if (motorBlock->freqSet == 1)
        {
            ///Check and set the direction.
            if (motorBlock->dir != motorBlock->targetDir)
            {
                ///The direction is wrong. Set it.
                setDir (motorBlock, motorBlock->targetDir);
            }
            if (motorBlock->frequency != motorBlock->targetFreq)
            {
                if (motorBlock->targetFreq == 0)
                {
                    stopMotor (motorBlock);
                }
                else
                {
                    targetFreq = motorBlock->targetFreq;
                    setSpeed (motorBlock, 1, 0, targetFreq);
                }
            }
            else
            {
                ///Motor is already running at correct frequency.
                /// Do nothing.
            }

        }
        else
        {
            if (motorBlock->dir != motorBlock->targetDir)
            {
                ///If the motor is running, decelerate until stopped, then change direction
                while (motorBlock->speed != 0)
                {
                    if (fabs (0 - motorBlock->speed) <= ACCELPERTICK)
                    {
                        setSpeed (motorBlock, 0, 0, 0);
                    }
                    else
                    {
                        /// Step it up or down by ACCELPERTICK and wait 1 tick
                        if (motorBlock->speed < 0)
                        {
                            setSpeed (motorBlock, 0, (motorBlock->speed + ACCELPERTICK), 0);
                        }
                        else
                        {
                            setSpeed (motorBlock, 0, (motorBlock->speed - ACCELPERTICK), 0);
                        }
                    }
                    tx_thread_sleep (1);
                }
                setDir (motorBlock, motorBlock->targetDir);

//                if (motorBlock->speed > motorBlock->targetJerkSpeed)
//                {
//                    ///The motor is moving too fast for a direction change.
//                    //                motorBlock->accelStepSize = (0.0 - motorBlock->speed) * 1.0;
//                    setSpeed (motorBlock, 0, motorBlock->targetJerkSpeed, 0);
//                }
//                else
//                {
//                    ///The motor is moving slow enough for a direction change.
//
//                }
            }
            else
            {
                if (motorBlock->targetSpeed == 0 && motorBlock->speed != 0)
                {
                    ///Needs deceleration
                    while (motorBlock->speed != 0)
                    {
                        if (fabs (0 - motorBlock->speed) <= ACCELPERTICK)
                        {
                            setSpeed (motorBlock, 0, 0, 0);
                        }
                        else
                        {
                            /// Step it up or down by ACCELPERTICK and wait 1 tick
                            if (motorBlock->speed < 0)
                            {
                                setSpeed (motorBlock, 0, (motorBlock->speed + ACCELPERTICK), 0);
                            }
                            else
                            {
                                setSpeed (motorBlock, 0, (motorBlock->speed - ACCELPERTICK), 0);
                            }
                        }
                        tx_thread_sleep (1);
                    }
//                    stopMotor (motorBlock);
                }
                else if (motorBlock->targetSpeed != motorBlock->speed)
                {
                    ///The current speed is too far off target.

//                    motorBlock->accelStepSize = (motorBlock->targetSpeed - motorBlock->speed) * 1.0;

                    ///Check if the current direction is equal to the target direction based on the sign of the target speed.
                    if (motorBlock->targetSpeed > 0 && motorBlock->dir != motorBlock->fwdDir)
                    {
                        motorBlock->targetDir = motorBlock->fwdDir;
                    }
                    else if (motorBlock->targetSpeed < 0 && motorBlock->dir == motorBlock->fwdDir)
                    {
                        if (motorBlock->fwdDir == IOPORT_LEVEL_HIGH)
                        {
                            motorBlock->targetDir = IOPORT_LEVEL_LOW;
                        }
                        else
                        {
                            motorBlock->targetDir = IOPORT_LEVEL_HIGH;
                        }
                    }
                    else if ((motorBlock->referenced && motorBlock->pos <= SOFTLIMITFORWARD
                            && motorBlock->pos >= SOFTLIMITREVERSE && motorBlock->hasLimits == 1)
                            || motorBlock->hasLimits == 0)
                    {
                        ///Needs acceleration/deceleration here
                        if (fabs (motorBlock->targetSpeed - motorBlock->speed) <= ACCELPERTICK)
                        {
                            setSpeed (motorBlock, 0, motorBlock->targetSpeed, 0);
                        }
                        else
                        {
                            /// Step it up or down by ACCELPERTICK and wait 1 tick
                            if (motorBlock->speed < motorBlock->targetSpeed)
                            {
                                setSpeed (motorBlock, 0, (motorBlock->speed + ACCELPERTICK), 0);
                            }
                            else
                            {
                                setSpeed (motorBlock, 0, (motorBlock->speed - ACCELPERTICK), 0);
                            }
                            tx_thread_sleep (1);
                        }
//                        setSpeed (motorBlock, 0, motorBlock->targetSpeed, 0);
                    }
                }
            }
        }

    }
    else if (motorBlock->homing == 1)
    {
        ///The homing routine has been initiated.
        ///Set the direction and speed.
        setDir (motorBlock, motorBlock->defaultDir);
        setSpeed (motorBlock, 0, motorBlock->homeSpeed, 0);

        ///Let the motor run as long as the limit is inactive
        while (motorBlock->limit0State == LIMITINACTIVESTATE)
            tx_thread_relinquish ();

        ///Reverse direction and let the motor run as long as the limit
        /// remains active.
        if (motorBlock->defaultDir == IOPORT_LEVEL_HIGH)
        {
            setDir (motorBlock, IOPORT_LEVEL_LOW);
        }
        else
        {
            setDir (motorBlock, IOPORT_LEVEL_HIGH);
        }
        while (motorBlock->limit0State == LIMITACTIVESTATE)
            tx_thread_relinquish ();

        motorBlock->posSteps = 0;
        motorBlock->pos = 0;

        while (fabs (motorBlock->pos) < HOME_BACKOFF_DISTANCE)
        {
            tx_thread_relinquish ();
        }

        ///Stop the motor, reset position counter and homing flag.
        stopMotor (motorBlock);
        motorBlock->posSteps = 0;
        motorBlock->pos = 0;
        motorBlock->homing = 0;
        motorBlock->referenced = 1;
    }

}

void setSpeed(struct motorController *motorBlock, int freqSet, double targetSpeed, int targetFreq)
{
    ssp_err_t err;
    int tmpTargetFreq;
    double tmp, tmpTargetSpeed;

    if (!freqSet)
    {
        ///Calculate timer frequency.
        tmp = targetSpeed;
        tmp /= 60; //Convert to mm/min to mm/s
        tmp *= motorBlock->stepsPerMM; //(mm/s) * (steps/mm) = steps/s = frequency
        tmpTargetFreq = (int) fabs (tmp);
        tmpTargetSpeed = targetSpeed;
    }
    else
    {
        tmpTargetFreq = targetFreq;
        ///To calculate the speed based on the frequency. We divide by the step size
        /// in order to obtain the speed in mm/s. Here our step size is steps/mm, not mm/step.
        tmpTargetSpeed = targetFreq;
        tmpTargetSpeed /= motorBlock->stepsPerMM;
    }

    ///Check if the motor GPT is already running. If not, it must be started.
    if (tmpTargetFreq > 0)
    {
        ///Start motor GPT
        motorBlock->start (motorBlock->g_timer_gpt_x.p_ctrl);
        err = motorBlock->periodSet (motorBlock->g_timer_gpt_x.p_ctrl, (2 * tmpTargetFreq), TIMER_UNIT_FREQUENCY_HZ);
    }
    else
    {
        motorBlock->stop (motorBlock->g_timer_gpt_x.p_ctrl);
    }

//    timer_info_t
    motorBlock->frequency = tmpTargetFreq;
    motorBlock->speed = tmpTargetSpeed;

    //    err = motorBlock->dutyCycleSet (motorBlock->g_timer_gpt_x.p_ctrl, 50, TIMER_PWM_UNIT_PERCENT, 0);
}

void setDir(struct motorController *motorBlock, ioport_level_t targetDir)
{
    ssp_err_t err;
    int targetFreq;
    double tmp;

    g_ioport.p_api->pinWrite (motorBlock->dirPin, targetDir);
    motorBlock->dir = targetDir;
}

///This function will stop a motor by stopping its GPT timer. It will also
/// reset the speed and frequency information to zero.
void stopMotor(struct motorController *motorBlock)
{
    ssp_err_t err;

    err = motorBlock->stop (motorBlock->g_timer_gpt_x.p_ctrl); // Stop timer
    motorBlock->targetSpeed = 0;
    motorBlock->targetFreq = 0;
    motorBlock->speed = 0;
    motorBlock->frequency = 0;

}


/**Handles behavior when limit switch is activated*/

void limitHit(struct motorController *motorBlock)
{
    ioport_level_t level;
    ssp_err_t err;
    R_BSP_SoftwareDelay (DEBOUNCE_TIME, BSP_DELAY_UNITS_MILLISECONDS);

    err = g_ioport.p_api->pinRead (motorBlock->limit0Pin, &level);
    motorBlock->limit0State = level;

    ///If the limit has been activated, and the motor is not in a homing routine,
    /// stop the motor.
    if (level == LIMITACTIVESTATE && motorBlock->homing == 0)
    {
        stopMotor (motorBlock);
    }
}

///Handles counting steps, stopping the motor when target position is reached, and toggling the STEP pin.
void stepHandler(struct motorController *motorBlock)
{
    ioport_level_t level;
    ssp_err_t err;
    if (motorBlock->stepState == IOPORT_LEVEL_HIGH)
    {
        err = g_ioport.p_api->pinWrite (motorBlock->stepPin, IOPORT_LEVEL_LOW);
        motorBlock->stepState = IOPORT_LEVEL_LOW;
    }
    else
    {
        err = g_ioport.p_api->pinWrite (motorBlock->stepPin, IOPORT_LEVEL_HIGH);
        motorBlock->stepState = IOPORT_LEVEL_HIGH;

        if (motorBlock->encoderActive != 1)
        {
            ///Without an active encoder, position must be tracked open-loop, via the step output.
            ///Increment or decrement the motor position by the stepSize
            if (motorBlock->dir == motorBlock->fwdDir)
            {
                motorBlock->posSteps++;
                motorBlock->pos += motorBlock->stepSize;
            }
            else
            {
                motorBlock->posSteps--;
                motorBlock->pos -= motorBlock->stepSize;
            }
        }

    }
}

void UDPZeroAxes()
{
    while (motorBlockX->homing || motorBlockY->homing || motorBlockZ->homing)
    {
        tx_thread_sleep (1);
    }

    motorBlockX->posSteps = 0;
    motorBlockY->posSteps = 0;
    motorBlockZ->posSteps = 0;
    motorBlockA->posSteps = 0;
    motorBlockC->posSteps = 0;
    motorBlockT->posSteps = 0;
//    motorBlockB->posSteps = 0;
//    motorBlockD->posSteps = 0;
//    toolBlockA->motorBlock->posSteps = 0;

    motorBlockX->pos = 0;
    motorBlockY->pos = 0;
    motorBlockZ->pos = 0;
    motorBlockA->pos = 0;
    motorBlockC->pos = 0;
    motorBlockT->pos = 0;
//    motorBlockB->pos = 0;
//    motorBlockD->pos = 0;
//    toolBlockA->motorBlock->pos = 0;
}

void encoderHandler(struct motorController *motorBlock)
{
    ioport_level_t levelA, levelB;
    ssp_err_t err;

    err = g_ioport.p_api->pinRead (motorBlock->encoderAPin, &levelA);
    err = g_ioport.p_api->pinRead (motorBlock->encoderBPin, &levelB);

    if (levelA == IOPORT_LEVEL_LOW && levelB == IOPORT_LEVEL_LOW)
    {
        if (motorBlock->encoderAState == IOPORT_LEVEL_HIGH && motorBlock->encoderBState == IOPORT_LEVEL_LOW)
        {
            ///CW
            if (motorBlock->encoderCWFWD)
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
        else if (motorBlock->encoderAState == IOPORT_LEVEL_LOW && motorBlock->encoderBState == IOPORT_LEVEL_HIGH)
        {
            ///CCW
            if (!(motorBlock->encoderCWFWD))
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
    }
    else if (levelA == IOPORT_LEVEL_LOW && levelB == IOPORT_LEVEL_HIGH)
    {
        if (motorBlock->encoderAState == IOPORT_LEVEL_LOW && motorBlock->encoderBState == IOPORT_LEVEL_LOW)
        {
            ///CW
            if (motorBlock->encoderCWFWD)
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
        else if (motorBlock->encoderAState == IOPORT_LEVEL_HIGH && motorBlock->encoderBState == IOPORT_LEVEL_HIGH)
        {
            ///CCW
            if (!(motorBlock->encoderCWFWD))
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
    }
    else if (levelA == IOPORT_LEVEL_HIGH && levelB == IOPORT_LEVEL_LOW)
    {
        if (motorBlock->encoderAState == IOPORT_LEVEL_HIGH && motorBlock->encoderBState == IOPORT_LEVEL_HIGH)
        {
            ///CW
            if (motorBlock->encoderCWFWD)
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
        else if (motorBlock->encoderAState == IOPORT_LEVEL_LOW && motorBlock->encoderBState == IOPORT_LEVEL_LOW)
        {
            ///CCW
            if (!(motorBlock->encoderCWFWD))
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
    }
    else if (levelA == IOPORT_LEVEL_HIGH && levelB == IOPORT_LEVEL_HIGH)
    {
        if (motorBlock->encoderAState == IOPORT_LEVEL_LOW && motorBlock->encoderBState == IOPORT_LEVEL_HIGH)
        {
            ///CW
            if (motorBlock->encoderCWFWD)
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
        else if (motorBlock->encoderAState == IOPORT_LEVEL_HIGH && motorBlock->encoderBState == IOPORT_LEVEL_LOW)
        {
            ///CCW
            if (!(motorBlock->encoderCWFWD))
            {
                motorBlock->pos += motorBlock->encoderMMPerPulse;
                motorBlock->posSteps += motorBlock->stepsPerEncoderPulse;
            }
            else
            {
                motorBlock->pos -= motorBlock->encoderMMPerPulse;
                motorBlock->posSteps -= motorBlock->stepsPerEncoderPulse;
            }
        }
    }

    motorBlock->encoderAState = levelA;
    motorBlock->encoderBState = levelB;
}

