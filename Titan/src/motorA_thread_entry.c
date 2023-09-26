#include "motorA_thread.h"
#include <Titan.h>

/* motorA entry function */
void motorA_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        motorHandler (motorBlockA);
        //allow next waiting thread(s) to run
        tx_thread_relinquish ();
    }
}
