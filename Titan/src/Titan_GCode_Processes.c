#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <Titan.h>
//#include <TitanMain.h>
#include "math.h"
/*
extern struct node *head;

void G00(struct instruction *data)
{
    double targetSpeed;
    targetSpeed = DEFAULTSPEED;

//    while (checkSecondaryHoming () == 1)
//    {
//        tx_thread_sleep (1);
//    }

    if (machineGlobalsBlock->chainedMovement == 1)
    {
        ///There is a G01 to follow, and the chainedMovement flag has not already been raised, send the command
        /// and raise the flag.
//        UDPChainedMovement (0);
        machineGlobalsBlock->chainedMovement = 0;
    }
//    if (head->next->content[0] == 'G' && head->next->content[1] == '0' && head->next->content[2] == '1'
//            && machineGlobalsBlock->chainedMovement == 0)
//    {
//        ///There is a G01 to follow, and the chainedMovement flag has not already been raised, send the command
//        /// and raise the flag.
//        UDPChainedMovement (1);
//        machineGlobalsBlock->chainedMovement = 1;
//    }
//    else if ((head->next->content[0] != 'G' || head->next->content[1] != '0' || head->next->content[2] != '1')
//            && machineGlobalsBlock->chainedMovement != 0)
//    {
//        UDPChainedMovement (0);
//        machineGlobalsBlock->chainedMovement = 0;
//    }
//    UDPSendTargetPosSpeed (data->x, data->y, data->z, data->t, targetSpeed);
}
*/
/*Handles G01 commands
 * This function must track the velocity and position of the tool head.
 * Adjustments to motor speed are calculated here. The motor handler function for
 * each respective motor is responsible for making the actual adjustment to STEP timing.
 */

/*
void G01(struct instruction *data)
{
    int endReached = 0;
    int endX, endY, endZ;
    double lineVector[5], newLineVector[5];
    double lineVectorMag, newLineVectorMag;
    double newUnitVector[5];
    double targetVelocityVector[5], targetVelocityVectorMag;
    double startPos[5], newPos[5], targetPos[5];
//    double targetSpeed;
    double tmpData;
    double extruderSpeed;
    int outOfBounds = 0;
    double time;
    UINT timeInt;
    //    ioport_level_t level;
    //    ssp_err_t err;
    //    err = g_ioport.p_api->pinRead (motorBlock->limit0Pin, &level);

//    while (checkSecondaryHoming () == 1)
//    {
//        tx_thread_sleep (1);
//    }

    if (data->f != ~0)
    {
        machineGlobalsBlock->targetSpeed = data->f;
    }

//    if (head->next->content[0] == 'G' && head->next->content[1] == '0' && head->next->content[2] == '1'
//            && machineGlobalsBlock->chainedMovement == 0)
//    {
//        ///There is a G01 to follow, and the chainedMovement flag has not already been raised, raise the flag.
////        UDPChainedMovement (1);
//        machineGlobalsBlock->chainedMovement = 1;
//    }
//    else if ((head->next->content[0] != 'G' || head->next->content[1] != '0' || head->next->content[2] != '1')
//            && machineGlobalsBlock->chainedMovement != 0)
//    {
////        UDPChainedMovement (0);
//        machineGlobalsBlock->chainedMovement = 0;
//    }
//    UDPSendTargetPosSpeed (data->x, data->y, data->z, data->t, machineGlobalsBlock->targetSpeed);

    startPos[0] = motorBlockX->pos;
    startPos[1] = motorBlockY->pos;
    startPos[2] = motorBlockZ->pos;
    startPos[3] = motorBlockA->pos;
    startPos[4] = motorBlockC->pos;

    if (data->x != ~0)
    {
        ///There is valid data for the X axis.
        targetPos[0] = data->x;
    }
    else
    {
        ///We set the target position to the current position
        /// if there is no valid data. This will mean that the motor does
        /// not move from its current location.
        targetPos[0] = startPos[0];
    }
    if (data->y != ~0)
    {
        ///There is valid data for the Y axis.
        targetPos[1] = data->y;
    }
    else
    {
        ///We set the target position to the current position
        /// if there is no valid data. This will mean that the motor does
        /// not move from its current location.
        targetPos[1] = startPos[1];
    }
    if (data->z != ~0)
    {
        ///There is valid data for the Z axis.
        targetPos[2] = data->z;
    }
    else
    {
        ///We set the target position to the current position
        /// if there is no valid data. This will mean that the motor does
        /// not move from its current location.
        targetPos[2] = startPos[2];
    }
    if (data->a != ~0)
    {
        ///There is valid data for the Z axis.
        targetPos[3] = data->a;
    }
    else
    {
        ///We set the target position to the current position
        /// if there is no valid data. This will mean that the motor does
        /// not move from its current location.
        targetPos[3] = startPos[3];
    }
    if (data->c != ~0)
    {
        ///There is valid data for the Z axis.
        targetPos[4] = data->c;
    }
    else
    {
        ///We set the target position to the current position
        /// if there is no valid data. This will mean that the motor does
        /// not move from its current location.
        targetPos[4] = startPos[4];
    }

    lineVector[0] = (targetPos[0] - startPos[0]);
    lineVector[1] = (targetPos[1] - startPos[1]);
    lineVector[2] = (targetPos[2] - startPos[2]);
    lineVector[3] = (targetPos[3] - startPos[3]);
    lineVector[4] = (targetPos[4] - startPos[4]);

    ///Calculate magnitude. This is the distance to be moved.
    lineVectorMag = sqrt (
            pow (lineVector[0], 2) + pow (lineVector[1], 2) + pow (lineVector[2], 2) + pow (lineVector[3], 2)
                    + pow (lineVector[4], 2));

    ///Calculate the latest unit vector.
    newUnitVector[0] = (lineVector[0] / lineVectorMag);
    newUnitVector[1] = (lineVector[1] / lineVectorMag);
    newUnitVector[2] = (lineVector[2] / lineVectorMag);
    newUnitVector[3] = (lineVector[3] / lineVectorMag);
    newUnitVector[4] = (lineVector[4] / lineVectorMag);

    ///The unit vector will provide the appropriate direction of each motor. Multiplying
    /// the unit vector by the target speed will provide the target velocity vector.
    ///Calculate the velocity vector.
    targetVelocityVector[0] = (machineGlobalsBlock->targetSpeed * newUnitVector[0]);
    targetVelocityVector[1] = (machineGlobalsBlock->targetSpeed * newUnitVector[1]);
    targetVelocityVector[2] = (machineGlobalsBlock->targetSpeed * newUnitVector[2]);
    targetVelocityVector[3] = (machineGlobalsBlock->targetSpeed * newUnitVector[3]);
    targetVelocityVector[4] = (machineGlobalsBlock->targetSpeed * newUnitVector[4]);

    //Check the Z velocity to ensure it's not too high. If so, adjust everything down proportionally.
    if (targetVelocityVector[2] > 110.0)
    {
        ///The z-axis movement speed is too high. Calculate the reduction factor and recalculate
        /// the velocity vector based on this.
        double reductionFactor = (110.0 / targetVelocityVector[2]);
        machineGlobalsBlock->targetSpeed *= reductionFactor;
        targetVelocityVector[0] *= reductionFactor;
        targetVelocityVector[1] *= reductionFactor;
        targetVelocityVector[2] *= reductionFactor;
        targetVelocityVector[3] *= reductionFactor;
        targetVelocityVector[4] *= reductionFactor;
    }

    ///Disable freqSet if necessary to ensure the motors can be set according to a target speed.
    /// This is something that should not be performed with every G01.
    if (motorBlockX->freqSet || motorBlockY->freqSet || motorBlockZ->freqSet || motorBlockA->freqSet
            || motorBlockC->freqSet || motorBlockT->freqSet)
    {
        motorBlockX->freqSet = 0;
        motorBlockY->freqSet = 0;
        motorBlockZ->freqSet = 0;
        motorBlockA->freqSet = 0;
        motorBlockC->freqSet = 0;
        motorBlockT->freqSet = 0;
        machineGlobalsBlock->motorFreqSet = 0;
    }

    ///Get the movement time in minutes. GCode speeds are in mm/min.
    time = lineVectorMag / machineGlobalsBlock->targetSpeed;

    ///Perform extruder calculations.
    if (data->t != ~0)
    {

        ///There is valid data for tool A, the extruder.
        ///This data is an extrusion length, which is the length of filament which
        /// must be extruded over the movement length. The appropriate speed can be found
        /// with the basic D=RT or distance = (rate)(time) formula.
        /// **data->t is the target extruder position, not the length**
        ///
        /// The rate is simply the extrusion length (or distance) divided by the total movement time, which
        /// was previously found as the variable time in minutes.

        extruderSpeed = ((data->t - motorBlockT->pos) / time);

        ///extruderSpeed is now the velocity required to extrude the required length over the course of the toolhead movement.
        /// This velocity is in mm/min, as in GCode.
    }
    else
    {
        extruderSpeed = 0;
//        stopMotor (motorBlockA);
    }

    ///Finish time calculations for delay period.
    time *= 60; ///Convert from minutes to seconds
    time *= 1000; ///Convert from seconds to milliseconds
    timeInt = time;

////    UDPSetTargetVelocity ('a', extruderSpeed);

    if (targetVelocityVector[0] != 0)
    {
        motorBlockX->targetSpeed = targetVelocityVector[0];
//        UDPSetTargetVelocity (motorBlockX, targetVelocityVector[0]);
    }
    else
    {
        stopMotor (motorBlockX);
    }

    if (targetVelocityVector[1] != 0)
    {
        motorBlockY->targetSpeed = targetVelocityVector[1];
//        UDPSetTargetVelocity (motorBlockY, targetVelocityVector[1]);
//        UDPSetTargetVelocity (motorBlockA, targetVelocityVector[1]);
    }
    else
    {
        stopMotor (motorBlockY);
//        stopMotor (motorBlockA);
    }

    if (targetVelocityVector[2] != 0)
    {
        motorBlockZ->targetSpeed = targetVelocityVector[2];
//        UDPSetTargetVelocity (motorBlockZ, targetVelocityVector[2]);
//        UDPSetTargetVelocity (motorBlockB, targetVelocityVector[2]);
//        UDPSetTargetVelocity (motorBlockC, targetVelocityVector[2]);
//        UDPSetTargetVelocity (motorBlockD, targetVelocityVector[2]);
    }
    else
    {
        stopMotor (motorBlockZ);
//        stopMotor (motorBlockB);
//        stopMotor (motorBlockC);
//        stopMotor (motorBlockD);
    }
    if (targetVelocityVector[3] != 0)
    {
        motorBlockA->targetSpeed = targetVelocityVector[2];
    }
    else
    {
        stopMotor (motorBlockA);
    }
    if (targetVelocityVector[4] != 0)
    {
        motorBlockC->targetSpeed = targetVelocityVector[2];
    }
    else
    {
        stopMotor (motorBlockC);
    }

    if (extruderSpeed != 0)
    {
        motorBlockT->targetSpeed = extruderSpeed;
    }
    else
    {
        stopMotor (motorBlockT);
    }

    //Wait for the movement to complete.
    //Probably a better method would be to schedule an interrupt which sets an event flag, and wait for the event flag.
    UINT tmp;
    //The following alteration of thread priority was probably to ensure accurate timing.
    //This is not necessary at this time.

//    tx_thread_priority_change(tx_thread_identify(), 0, &tmp);
    if(timeInt > 10000){
        printf("\nLong wait?");
    }
    tx_thread_sleep (timeInt);
//    tx_thread_priority_change(tx_thread_identify(), tmp, &tmp);

    //The alloted time has completed. The tool should be at the target location.
    //Stop the motors if a G01 does not follow. This is a simplistic form of movement chaining.
    //Any motor that is not run on the next G01 will be stopped at the start. Any motor that continues running
    //will simply have its speed altered.

    //Currently wrong. The link is removed prior to the command handler.
    if (head->next->content[0] != 'G' || head->next->content[1] != '0' || head->next->content[2] != '1')
    {
        ///There is no G01 to follow, stop the motors.
        stopMotor (motorBlockX);
        stopMotor (motorBlockY);
        stopMotor (motorBlockZ);
        stopMotor (motorBlockA);
        stopMotor (motorBlockC);
        stopMotor (motorBlockT);
//        stopMotor (motorBlockB);
//        stopMotor (motorBlockD);
    }

}
*/
/**Handles behavior of g-code "G28"
 * G28 is the Homing command.
 * Character flags are available for specifying the axes to be homed.
 *
 * */
/*
void G28(struct instruction *data)
{

    ///The motors are now in motion, and this function must only wait until all homing
    /// routines are complete.
    while (motorBlockX->homing || motorBlockY->homing || motorBlockZ->homing)
        tx_thread_sleep (1);

}
*/
