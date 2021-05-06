#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Titan.h>
#include <TitanMain.h>
#include "math.h"

extern struct node *head;

/*Handles G01 commands
 * This function must track the velocity and position of the tool head.
 * Adjustments to motor speed are calculated here. The motor handler function for
 * each respective motor is responsible for making the actual adjustment to STEP timing.
 */
void G01(struct instruction *data)
{
    int endReached = 0;
    int endX, endY, endZ;
    double lineVector[3], newLineVector[3];
    double lineVectorMag, newLineVectorMag;
    double newUnitVector[3];
    double targetVelocityVector[3], targetVelocityVectorMag;
    double startPos[3], newPos[3], targetPos[3];
//    double targetSpeed;
    double tmpData;
    double extruderSpeed;
    int outOfBounds = 0;
    double time;
    UINT timeInt;
    //    ioport_level_t level;
    //    ssp_err_t err;
    //    err = g_ioport.p_api->pinRead (motorBlock->limit0Pin, &level);

    while (checkSecondaryHoming () == 1)
    {
        tx_thread_sleep (1);
    }
    UDPSendTargetPosSpeed (data->x, data->y, data->z, data->t, data->f);

//    do
//    {
//        tx_thread_sleep (1);
//        startPos[0] = UDPGetPosition (motorBlockX);
//        startPos[1] = UDPGetPosition (motorBlockY);
//        startPos[2] = UDPGetPosition (motorBlockZ);
//
//        if (data->x != ~0)
//        {
//            ///There is valid data for the X axis.
//            targetPos[0] = data->x;
//        }
//        else
//        {
//            ///We set the target position to the current position
//            /// if there is no valid data. This will mean that the motor does
//            /// not move from its current location.
//            targetPos[0] = startPos[0];
//        }
//        if (data->y != ~0)
//        {
//            ///There is valid data for the Y axis.
//            targetPos[1] = data->y;
//        }
//        else
//        {
//            ///We set the target position to the current position
//            /// if there is no valid data. This will mean that the motor does
//            /// not move from its current location.
//            targetPos[1] = startPos[1];
//        }
//        if (data->z != ~0)
//        {
//            ///There is valid data for the Z axis.
//            targetPos[2] = data->z;
//        }
//        else
//        {
//            ///We set the target position to the current position
//            /// if there is no valid data. This will mean that the motor does
//            /// not move from its current location.
//            targetPos[2] = startPos[2];
//        }
//
//        lineVector[0] = (targetPos[0] - startPos[0]);
//        lineVector[1] = (targetPos[1] - startPos[1]);
//        lineVector[2] = (targetPos[2] - startPos[2]);
//
//        ///Calculate magnitude.
//        lineVectorMag = sqrt (pow (lineVector[0], 2) + pow (lineVector[1], 2) + pow (lineVector[2], 2));
//    }
//    while (lineVectorMag > .1);
//
//
//

//
//    ///Record the current position as the starting position.
//    startPos[0] = UDPGetPosition (motorBlockX);
//    startPos[1] = UDPGetPosition (motorBlockY);
//    startPos[2] = UDPGetPosition (motorBlockZ);
//
/////Check for target speed data and set the global variable if available.
///// If no target speed has been sent, the machine will use the default speed
///// defined in "Helix.h".
//    if (data->f != ~0 && data->f > 0)
//    {
//        machineGlobalsBlock->targetSpeed = data->f;
//    }
////    targetSpeed = machineGlobalsBlock->targetSpeed;
//
///// Compare the current position with the target position.
/////To do this, calculate a line vector from the current to target position,
///// then find the percent error.
//
//    ///First, find the target position.
//    if (data->x != ~0)
//    {
//        ///There is valid data for the X axis.
//        targetPos[0] = data->x;
//    }
//    else
//    {
//        ///We set the target position to the current position
//        /// if there is no valid data. This will mean that the motor does
//        /// not move from its current location.
//        targetPos[0] = startPos[0];
//    }
//    if (data->y != ~0)
//    {
//        ///There is valid data for the Y axis.
//        targetPos[1] = data->y;
//    }
//    else
//    {
//        ///We set the target position to the current position
//        /// if there is no valid data. This will mean that the motor does
//        /// not move from its current location.
//        targetPos[1] = startPos[1];
//    }
//    if (data->z != ~0)
//    {
//        ///There is valid data for the Z axis.
//        targetPos[2] = data->z;
//    }
//    else
//    {
//        ///We set the target position to the current position
//        /// if there is no valid data. This will mean that the motor does
//        /// not move from its current location.
//        targetPos[2] = startPos[2];
//    }
//
//
//
//    ///We now have the target position and can calculate the line vector.
//    lineVector[0] = (targetPos[0] - startPos[0]);
//    lineVector[1] = (targetPos[1] - startPos[1]);
//    lineVector[2] = (targetPos[2] - startPos[2]);
//
//    ///Calculate magnitude.
//    lineVectorMag = sqrt (pow (lineVector[0], 2) + pow (lineVector[1], 2) + pow (lineVector[2], 2));
//
//    ///Calculate the latest unit vector.
//    newUnitVector[0] = (lineVector[0] / lineVectorMag);
//    newUnitVector[1] = (lineVector[1] / lineVectorMag);
//    newUnitVector[2] = (lineVector[2] / lineVectorMag);
//
//    ///The unit vector will provide the appropriate direction of each motor. Multiplying
//    /// the unit vector by the target speed will provide the target velocity vector.
//    ///Calculate the velocity vector.
//    targetVelocityVector[0] = (machineGlobalsBlock->targetSpeed * newUnitVector[0]);
//    targetVelocityVector[1] = (machineGlobalsBlock->targetSpeed * newUnitVector[1]);
//    targetVelocityVector[2] = (machineGlobalsBlock->targetSpeed * newUnitVector[2]);
//
//    if (targetVelocityVector[2] > 110.0)
//    {
//        ///The z-axis movement speed is too high. Calculate the reduction factor and recalculate
//        /// the velocity vector based on this.
//        double reductionFactor = (110.0 / targetVelocityVector[2]);
//        machineGlobalsBlock->targetSpeed *= reductionFactor;
//        targetVelocityVector[0] *= reductionFactor;
//        targetVelocityVector[1] *= reductionFactor;
//        targetVelocityVector[2] *= reductionFactor;
//    }
//
//    ///Disable freqSet.
//    /// This is something that should not be performed with every G01. A lot of extra packets from this.
//    if (machineGlobalsBlock->motorFreqSet != 0)
//    {
//        UDPSetMotorFreqSet (motorBlockX);
////        UDPSetMotorFreqSet (motorBlockY);
////        UDPSetMotorFreqSet (motorBlockZ);
//        machineGlobalsBlock->motorFreqSet = 0;
//    }
//
//    ///Get the movement time in minutes.
//    time = lineVectorMag / machineGlobalsBlock->targetSpeed;
//
//    ///Perform extruder calculations.
//    if (data->t != ~0)
//    {
//
//        ///There is valid data for tool A, the extruder.
//        ///This data is an extrusion length, which is the length of filament which
//        /// must be extruded over the movement length. The appropriate speed can be found
//        /// with the basic D=RT or distance = (rate)(time) formula.
//        ///
//        /// The rate is simply the extrusion length (or distance) divided by the total movement time, which
//        /// was previously found as the variable time in minutes.
//
//        extruderSpeed = ((data->t - UDPGetPosition (motorBlockT)) / time);
//
//        ///extruderSpeed is now the velocity required to extrude the required length over the course of the toolhead movement.
//        /// This velocity is in mm/min, as the secondary requires.
//    }
//    else
//    {
//        extruderSpeed = 0;
////        stopMotor (motorBlockA);
//    }
//
//    ///Finish time calculations for delay period.
//    time *= 60; ///Convert from minutes to seconds
//    time *= 1000; ///Convert from seconds to milliseconds
//    timeInt = time;
//
////    UDPSetMotorFreqSet ('a', 0);
////    UDPSetTargetVelocity ('a', extruderSpeed);
//
//    if (targetVelocityVector[0] != 0)
//    {
//        UDPSetTargetVelocity (motorBlockX, targetVelocityVector[0]);
//    }
//    else
//    {
//        stopMotor (motorBlockX);
//    }
//
//    if (targetVelocityVector[1] != 0)
//    {
//        UDPSetTargetVelocity (motorBlockY, targetVelocityVector[1]);
////        UDPSetTargetVelocity (motorBlockA, targetVelocityVector[1]);
//    }
//    else
//    {
//        stopMotor (motorBlockY);
////        stopMotor (motorBlockA);
//    }
//
//    if (targetVelocityVector[2] != 0)
//    {
//        UDPSetTargetVelocity (motorBlockZ, targetVelocityVector[2]);
////        UDPSetTargetVelocity (motorBlockB, targetVelocityVector[2]);
////        UDPSetTargetVelocity (motorBlockC, targetVelocityVector[2]);
////        UDPSetTargetVelocity (motorBlockD, targetVelocityVector[2]);
//    }
//    else
//    {
//        stopMotor (motorBlockZ);
////        stopMotor (motorBlockB);
////        stopMotor (motorBlockC);
////        stopMotor (motorBlockD);
//    }
//
//    if (extruderSpeed != 0)
//    {
//        UDPSetTargetVelocity (motorBlockT, extruderSpeed);
//    }
//    else
//    {
//        stopMotor (motorBlockT);
//    }
////    UDPSetTargetVelocity (motorBlockA, extruderSpeed);
//
//    UINT tmp;
////    tx_thread_priority_change(tx_thread_identify(), 0, &tmp);
//    tx_thread_sleep (timeInt);
////    tx_thread_priority_change(tx_thread_identify(), tmp, &tmp);
//    ///The above do-while loop continues as long as the percent error of
//    /// any of the motor positions, compared to the target position, is found
//    /// to be too great.
//
//    ///At this point the toolhead is close enough to the target. Here, we will
//    /// either break out of the G01 function, or set the motors to stop, depending on
//    /// whether or not there is another G01 to follow.
//    if (head->next->content[0] != 'G' || head->next->content[1] != '0' || head->next->content[2] != '1')
//    {
//        ///There is no G01 to follow, stop the motors.
//        stopMotor (motorBlockX);
//        stopMotor (motorBlockY);
////        stopMotor (motorBlockA);
//        stopMotor (motorBlockZ);
////        stopMotor (motorBlockB);
////        stopMotor (motorBlockC);
////        stopMotor (motorBlockD);
////        stopMotor (motorBlockA);
//    }

}
/**Handles behavior of g-code "G28"
 * G28 is the Homing command.
 * Character flags are available for specifying the axes to be homed.
 *
 * */
void G28(struct instruction *data)
{

    ///The motors are now in motion, and this function must only wait until all homing
    /// routines are complete.
    while (motorBlockX->homing || motorBlockY->homing || motorBlockZ->homing)
        tx_thread_sleep (1);

}
