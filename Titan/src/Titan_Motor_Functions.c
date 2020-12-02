#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Titan.h>
#include <TitanMain.h>
#include "math.h"

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
    UDPStopMotor(motorBlock);

//    if (motorBlock == motorBlockX)
//    {
//        UDPStopMotor ('x');
//    }
//    else if (motorBlock == motorBlockY)
//    {
//        UDPStopMotor ('y');
//    }
//    else if (motorBlock == motorBlockZ)
//    {
//        UDPStopMotor ('z');
//    }
//    else if (motorBlock == motorBlockA)
//    {
//        UDPStopMotor ('a');
//    }

}
