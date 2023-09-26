#include <Titan.h>
#include <TitanMain.h>
//#include "HelixParser.h"

void motorInit(struct motorController *motorBlock);

struct motorController *motorBlockX, *motorBlockY, *motorBlockZ, *motorBlockT, *motorBlockA, *motorBlockB, *motorBlockC,
        *motorBlockD;
struct toolBlock *toolBlockA;
struct machineGlobals *machineGlobalsBlock;

extern struct node *head;
extern struct node *tail;
/* Helix Main entry function */
void TitanMain_entry(void)
{
    ssp_err_t err;
    UINT status;
    ULONG eventFlags;

    if (DEBUGGERPRIMARY)
        initialise_monitor_handles ();

    if (DEBUGGERPRIMARY)
    {
        printf ("\Titan controller initializing...");
    }
    struct instruction data;
    ioport_level_t pinValue;
    char cmd[3] = "cmd";

    initGlobalsBlock ();
    initBuff ();
//    machineGlobalsBlock->USBBufferB = initUSBBuffer_PoolB (512);
    if (DEBUGGERPRIMARY)
    {
        printf ("\nMachine Globals OK\nMotor registers initializing...");
    }
    initMotorBlocks ();
    if (DEBUGGERPRIMARY)
        printf ("\nMotor Blocks OK");
    initToolBlocks ();
    if (DEBUGGERPRIMARY)
        printf ("\nTool Blocks OK");
    initMotors ();
    if (DEBUGGERPRIMARY)
        printf ("\nMotors OK");
    initTools ();
    if (DEBUGGERPRIMARY)
        printf ("\nTools OK");

    err = init_timers ();
    if (DEBUGGERPRIMARY)
        printf ("\nTimers OK");
    if (DEBUGGERPRIMARY)
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
//        status = tx_event_flags_get (&g_linked_list_flags, 1, TX_AND, &eventFlags, 10);

//This should be in a separate thread so it does not delay the next command.
        if (machineGlobalsBlock->USBBufferHasData == 1 && machineGlobalsBlock->linkedListNodeCount < 10
                && machineGlobalsBlock->printJob == 1)
        {
//            tx_thread_priority_change (tx_thread_identify (), 0, &tmp);
            rebuildLinkedListFromSD ();
//            tx_thread_priority_change (tx_thread_identify (), tmp, &tmp);
        }

        if (head != NULL)
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

//        if (machineGlobalsBlock->getUpdate)
//        {
//            UDPGetToolUpdate ();
//            machineGlobalsBlock->getUpdate = 0;
//        }

//        tx_thread_relinquish();
    }
}

void motorInitX()
{
    ssp_err_t err;
    err = g_external_irqX.p_api->open (g_external_irqX.p_ctrl, g_external_irqX.p_cfg);
    motorBlockX->homing = 0;
    motorBlockX->dirPin = IOPORT_PORT_05_PIN_13;
    motorBlockX->start = g_timerX.p_api->start;
    motorBlockX->stop = g_timerX.p_api->stop;
    motorBlockX->dutyCycleSet = g_timerX.p_api->dutyCycleSet;
    motorBlockX->periodSet = g_timerX.p_api->periodSet;
    motorBlockX->g_timer_gpt_x = g_timerX;
    motorBlockX->stepsPerMM = STEPX;
    motorBlockX->stepSize = (1.0 / STEPY);
    motorBlockX->stepPin = IOPORT_PORT_08_PIN_05;
    motorBlockX->stepState = IOPORT_LEVEL_LOW;
    motorBlockX->limit0Pin = IOPORT_PORT_00_PIN_08;
    motorBlockX->defaultDir = IOPORT_LEVEL_HIGH; //Towards Home
    motorBlockX->targetDir = motorBlockX->defaultDir;
    motorBlockX->targetJerkSpeed = DEFAULTJERKSPEEDX;
    g_ioport.p_api->pinWrite (motorBlockX->dirPin, motorBlockX->defaultDir);
    motorBlockX->dir = motorBlockX->defaultDir;
    motorBlockX->fwdDir = IOPORT_LEVEL_LOW;
    motorBlockX->homeSpeed = HOMEVX;
    motorBlockX->rapidSpeed = HOMEVX;
    motorBlockX->limit0State = LIMITINACTIVESTATE;
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
    motorBlockY->controlCode = 'y';
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
    motorBlockZ->controlCode = 'z';
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

void motorInitT()
{
    ssp_err_t err;
    motorBlockT->controlCode = 't';
    motorBlockT->homing = 0;
    motorBlockT->dirPin = IOPORT_PORT_06_PIN_04;
    motorBlockT->stepSize = STEPX;
    motorBlockT->stepPin = IOPORT_PORT_03_PIN_02;
    motorBlockT->stepState = IOPORT_LEVEL_LOW;
    motorBlockT->limit0Pin = IOPORT_PORT_00_PIN_05;
    motorBlockT->defaultDir = IOPORT_LEVEL_LOW;
    motorBlockT->targetDir = motorBlockT->defaultDir;
    motorBlockT->targetJerkSpeed = DEFAULTJERKSPEEDX;
    g_ioport.p_api->pinWrite (motorBlockT->dirPin, motorBlockT->defaultDir);
    motorBlockT->dir = motorBlockT->defaultDir;
    motorBlockT->fwdDir = IOPORT_LEVEL_LOW;
    motorBlockT->homeSpeed = HOMEVX;
    motorBlockT->rapidSpeed = HOMEVX;
    motorBlockT->limit0State = IOPORT_LEVEL_HIGH;
    motorBlockT->intervalSteps = 0;
    motorBlockT->freqSet = 0;
    motorBlockT->setPosSteps = 0;
    motorBlockT->pos = 0;
    motorBlockT->posSteps = 0;
    motorBlockT->posAbs = 0;
    motorBlockT->posStepsAbs = 0;
    motorBlockT->offsetSteps = 0;
    motorBlockT->targetSpeed = 0;
    motorBlockT->targetFreq = 0;
    motorBlockT->targetPosSteps = 0;
    motorBlockT->speed = 0;
    motorBlockT->frequency = 0;
}

void genericMotorInit(struct motorController *motorBlock)
{
    ssp_err_t err;
    motorBlock->homing = 0;
    motorBlock->stepsPerMM = STEPX;
    motorBlock->stepSize = (1.0 / STEPZ);
    motorBlock->stepState = IOPORT_LEVEL_LOW;
    motorBlock->targetDir = motorBlock->defaultDir;
    motorBlock->targetJerkSpeed = DEFAULTJERKSPEEDZ;
    g_ioport.p_api->pinWrite (motorBlock->dirPin, motorBlock->defaultDir);
    motorBlock->dir = motorBlock->defaultDir;
    motorBlock->fwdDir = IOPORT_LEVEL_HIGH;
    motorBlock->defaultDir = IOPORT_LEVEL_LOW;
    motorBlock->fwdDir = IOPORT_LEVEL_HIGH;
    motorBlock->posSteps = 0;
    motorBlock->targetPosSteps = 0;
    motorBlock->offsetSteps = 0;
    motorBlock->homeSpeed = HOMEVX;
    motorBlock->rapidSpeed = HOMEVX;
    motorBlock->limit0State = LIMITINACTIVESTATE;
    motorBlock->intervalSteps = 0;
    motorBlock->freqSet = 0;
    motorBlock->setPosSteps = 0;
    motorBlock->pos = 0;
    motorBlock->posSteps = 0;
    motorBlock->posAbs = 0;
    motorBlock->posStepsAbs = 0;
    motorBlock->offsetSteps = 0;
    motorBlock->targetSpeed = 0;
    motorBlock->targetFreq = 0;
    motorBlock->targetPosSteps = 0;
    motorBlock->speed = 0;
    motorBlock->frequency = 0;
    motorBlock->encoderActive = 0;
}

void toolInitA()
{
    toolBlockA->motorBlock = motorBlockT;
}

void getTemp_callback(timer_callback_args_t *p_args)
{
    machineGlobalsBlock->getUpdate = 1;
}

void gpt_X_callback(timer_callback_args_t *p_args)
{
    stepHandler (motorBlockX);
}

void gpt_Y_callback(timer_callback_args_t *p_args)
{
    stepHandler (motorBlockY);
}

void gpt_Z_callback(timer_callback_args_t *p_args)
{
    stepHandler (motorBlockZ);
}

void gpt_A_callback(timer_callback_args_t *p_args)
{
    stepHandler (motorBlockA);
}

void gpt_C_callback(timer_callback_args_t *p_args)
{
    stepHandler (motorBlockC);
}

void gpt_T_callback(timer_callback_args_t *p_args)
{
    stepHandler (motorBlockT);
}
void ext_irqX_callback(external_irq_callback_args_t *p_args)
{
    limitHit (motorBlockX);
}

void ext_irqY_callback(external_irq_callback_args_t *p_args)
{
    limitHit (motorBlockY);
}

void ext_irqZ_callback(external_irq_callback_args_t *p_args)
{
    limitHit (motorBlockZ);
}
