#include <Titan.h>
#include <TitanMain.h>
//#include "HelixParser.h"

void motorInit(struct motorController *motorBlock);

extern struct node *head;
extern struct node *tail;
/* Helix Main entry function */
void TitanMain_entry(void)
{
    ssp_err_t err;

    if (DEBUGGER)
        initialise_monitor_handles ();

    if (DEBUGGER)
    {
        printf ("\Titan controller initializing...");
    }
    struct instruction data;
    ioport_level_t pinValue;
    char cmd[3] = "cmd";

    initialise_monitor_handles ();

    initGlobalsBlock ();
    initBuff ();
    machineGlobalsBlock->USBBufferB = initUSBBuffer_PoolB (512);
    if (DEBUGGER)
    {
        printf ("\nMotor registers initializing...");
    }
    initMotorBlocks ();
    initToolBlocks ();
    initMotors ();
    initTools ();

//    err = g_timer0.p_api->open (g_timer0.p_ctrl, g_timer0.p_cfg); // Open timers
//    g_timer0.p_api->start (g_timer0.p_ctrl);
//
//    err = g_timer0.p_api->periodSet (g_timer0.p_ctrl, 1, TIMER_UNIT_PERIOD_SEC);

    if (DEBUGGER)
    {
        printf ("\nPrimary initialization complete.");
    }

//    if(DEBUG) printf("\nInit machine globals block...");

//ensure motor init
    while (motorBlockZ->init != 1 || motorBlockX->init != 1 || motorBlockY->init != 1)
        tx_thread_sleep (1);

    while (1)
    {
        //parse instructions if there are any available and receiving is not in progress
        if (head != NULL && machineGlobalsBlock->receivingMsg == 0)
        {
            //process next instruction
            data = parseLine (head);
            removeLink (head);

            commandHandler (&data);

            ///Finally we check if the linked list needs to be rebuilt, if
            /// there is more data available, before it runs out.
//            if(machineGlobalsBlock->USBBufferHasData == 1 && machineGlobalsBlock->linkedListNodeCount < 6){
//                machineGlobalsBlock->rebuildLinkedList = 1;
//            }
        }
//        else if(machineGlobalsBlock->USBBufferHasData == 1 && machineGlobalsBlock->receivingMsg == 0){
//            machineGlobalsBlock->rebuildLinkedList = 1;
//        }

        if (machineGlobalsBlock->getUpdate)
        {
            UDPGetToolUpdate ();
            machineGlobalsBlock->getUpdate = 0;
        }
        tx_thread_relinquish();
    }
}

void motorInitX()
{
    ssp_err_t err;
    motorBlockX->homing = 0;
    motorBlockX->dirPin = IOPORT_PORT_06_PIN_04;
    motorBlockX->stepSize = STEPX;
    motorBlockX->stepPin = IOPORT_PORT_03_PIN_02;
    motorBlockX->stepState = IOPORT_LEVEL_LOW;
    motorBlockX->limit0Pin = IOPORT_PORT_00_PIN_05;
    motorBlockX->defaultDir = IOPORT_LEVEL_LOW;
    motorBlockX->targetDir = motorBlockX->defaultDir;
    motorBlockX->targetJerkSpeed = DEFAULTJERKSPEEDX;
    g_ioport.p_api->pinWrite (motorBlockX->dirPin, motorBlockX->defaultDir);
    motorBlockX->dir = motorBlockX->defaultDir;
    motorBlockX->fwdDir = IOPORT_LEVEL_LOW;
    motorBlockX->homeSpeed = HOMEVX;
    motorBlockX->rapidSpeed = HOMEVX;
    motorBlockX->limit0State = IOPORT_LEVEL_HIGH;
    motorBlockX->intervalSteps = 0;
    motorBlockX->freqSet = 0;
    motorBlockX->setPosSteps = 0;
    motorBlockX->pos = 0;
    motorBlockX->posSteps = 0;
    motorBlockX->posAbs = 0;
    motorBlockX->posStepsAbs = 0;
    motorBlockX->offsetSteps = 0;
    motorBlockX->targetSpeed = 0;
    motorBlockX->targetFreq = 0;
    motorBlockX->targetPosSteps = 0;
    motorBlockX->speed = 0;
    motorBlockX->frequency = 0;
}

void motorInitY()
{
    ssp_err_t err;
    motorBlockY->homing = 0;
    motorBlockY->dirPin = IOPORT_PORT_06_PIN_07;
    motorBlockY->stepSize = STEPY;
    motorBlockY->stepPin = IOPORT_PORT_03_PIN_04;
    motorBlockY->stepState = IOPORT_LEVEL_LOW;
    motorBlockY->limit0Pin = IOPORT_PORT_05_PIN_12;
    motorBlockY->defaultDir = IOPORT_LEVEL_HIGH;
    motorBlockY->targetDir = motorBlockY->defaultDir;
    motorBlockY->targetJerkSpeed = DEFAULTJERKSPEEDY;
    g_ioport.p_api->pinWrite (motorBlockY->dirPin, motorBlockY->defaultDir);
    motorBlockY->dir = motorBlockY->defaultDir;
    motorBlockY->fwdDir = IOPORT_LEVEL_HIGH;
    motorBlockY->homeSpeed = HOMEVY;
    motorBlockY->rapidSpeed = HOMEVY;
    motorBlockY->limit0State = IOPORT_LEVEL_HIGH;
    motorBlockY->intervalSteps = 0;
    motorBlockY->freqSet = 0;
    motorBlockY->setPosSteps = 0;
    motorBlockY->pos = 0;
    motorBlockY->posSteps = 0;
    motorBlockY->posAbs = 0;
    motorBlockY->posStepsAbs = 0;
    motorBlockY->offsetSteps = 0;
    motorBlockY->targetSpeed = 0;
    motorBlockY->targetFreq = 0;
    motorBlockY->targetPosSteps = 0;
    motorBlockY->speed = 0;
    motorBlockY->frequency = 0;
}

void motorInitZ()
{
    ssp_err_t err;
    motorBlockZ->homing = 0;
    motorBlockZ->dirPin = IOPORT_PORT_06_PIN_01;
    motorBlockZ->stepSize = STEPZ;
    motorBlockZ->stepPin = IOPORT_PORT_01_PIN_11;
    motorBlockZ->stepState = IOPORT_LEVEL_LOW;
    motorBlockZ->limit0Pin = IOPORT_PORT_00_PIN_00;
    motorBlockZ->defaultDir = IOPORT_LEVEL_LOW;
    motorBlockZ->targetDir = motorBlockZ->defaultDir;
    motorBlockZ->targetJerkSpeed = DEFAULTJERKSPEEDZ;
    g_ioport.p_api->pinWrite (motorBlockZ->dirPin, motorBlockZ->defaultDir);
    motorBlockZ->dir = motorBlockZ->defaultDir;
    motorBlockZ->fwdDir = IOPORT_LEVEL_HIGH;
    motorBlockZ->posSteps = 0;
    motorBlockZ->targetPosSteps = 0;
    motorBlockZ->offsetSteps = 0;
    motorBlockZ->homeSpeed = HOMEVZ;
    motorBlockZ->rapidSpeed = HOMEVZ;
    motorBlockZ->limit0State = IOPORT_LEVEL_HIGH;
    motorBlockZ->intervalSteps = 0;
    motorBlockZ->freqSet = 0;
    motorBlockZ->setPosSteps = 0;
    motorBlockZ->pos = 0;
    motorBlockZ->posSteps = 0;
    motorBlockZ->posAbs = 0;
    motorBlockZ->posStepsAbs = 0;
    motorBlockZ->offsetSteps = 0;
    motorBlockZ->targetSpeed = 0;
    motorBlockZ->targetFreq = 0;
    motorBlockZ->targetPosSteps = 0;
    motorBlockZ->speed = 0;
    motorBlockZ->frequency = 0;
}

void motorInitA()
{
    ssp_err_t err;
    motorBlockA->homing = 0;
    motorBlockA->dirPin = IOPORT_PORT_06_PIN_04;
    motorBlockA->stepSize = STEPX;
    motorBlockA->stepPin = IOPORT_PORT_03_PIN_02;
    motorBlockA->stepState = IOPORT_LEVEL_LOW;
    motorBlockA->limit0Pin = IOPORT_PORT_00_PIN_05;
    motorBlockA->defaultDir = IOPORT_LEVEL_LOW;
    motorBlockA->targetDir = motorBlockA->defaultDir;
    motorBlockA->targetJerkSpeed = DEFAULTJERKSPEEDX;
    g_ioport.p_api->pinWrite (motorBlockA->dirPin, motorBlockA->defaultDir);
    motorBlockA->dir = motorBlockA->defaultDir;
    motorBlockA->fwdDir = IOPORT_LEVEL_LOW;
    motorBlockA->homeSpeed = HOMEVX;
    motorBlockA->rapidSpeed = HOMEVX;
    motorBlockA->limit0State = IOPORT_LEVEL_HIGH;
    motorBlockA->intervalSteps = 0;
    motorBlockA->freqSet = 0;
    motorBlockA->setPosSteps = 0;
    motorBlockA->pos = 0;
    motorBlockA->posSteps = 0;
    motorBlockA->posAbs = 0;
    motorBlockA->posStepsAbs = 0;
    motorBlockA->offsetSteps = 0;
    motorBlockA->targetSpeed = 0;
    motorBlockA->targetFreq = 0;
    motorBlockA->targetPosSteps = 0;
    motorBlockA->speed = 0;
    motorBlockA->frequency = 0;
}

void toolInitA()
{
    toolBlockA->motorBlock = motorBlockA;
}

void getTemp_callback(timer_callback_args_t *p_args)
{
    machineGlobalsBlock->getUpdate = 1;
}
