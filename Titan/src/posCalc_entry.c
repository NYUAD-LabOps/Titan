#include <Titan.h>
#include "posCalc.h"

/* Position Calculator entry function */
extern struct node *head;

void posCalc_entry(void)
{
    /* TODO: add your own code here */
//    long int posStepsX, posStepsY, posStepsZ;
    ///The position calculator thread is responsible for translating the
    /// position of each motor, in steps, into a position in mm, etc...
    while (1)
    {
        if (machineGlobalsBlock->USBBufferHasData == 1 && machineGlobalsBlock->linkedListNodeCount < 6)
        {
            rebuildLinkedList ();
        }
//
//        if (machineGlobalsBlock->rebuildLinkedList == 1)
//        {
//
//            machineGlobalsBlock->rebuildLinkedList = 0;
//        }
        tx_thread_sleep (10);
    }
//    while(machineGlobalsBlock->motorsInit != 1) tx_thread_sleep(1);
//    posStepsX = motorBlockX->posSteps;
//    posStepsY = motorBlockY->posSteps;
//    posStepsZ = motorBlockZ->posSteps;
//    while (1)
//    {
//        ///The position in steps for each motor is monitored for any change. If
//        /// a change is found, the posUpdater() function is called to update the
//        /// rest of the position data accordingly.
//        if(posStepsX != motorBlockX->posSteps) posUpdater(motorBlockX);
//        if(posStepsY != motorBlockY->posSteps) posUpdater(motorBlockY);
//        if(posStepsZ != motorBlockZ->posSteps) posUpdater(motorBlockZ);
//        tx_thread_sleep (1);
//    }
}

void posUpdater(struct motorController *motorBlock){
    ///The GPT timer for each motor calls an IRQ which handles
    /// toggling the level of the STEP output, thus producing the pulse wave. At the same
    /// time, this function also increments or decrements a counter which tracks the position
    /// of the motor, in steps.
    ///
    /// The calculations required to translate this step count into position information
    /// are performed here.
    ///
    /// These operations are not included in the STEP IRQ in an effort to minimize its execution time.
    ///
    /// Currently there is no function for switching between absolute and
    /// relative position. Once those features are included, those calculations
    /// will need to be here as well.

    motorBlock->pos = motorBlock->posSteps * motorBlock->stepSize;
}
