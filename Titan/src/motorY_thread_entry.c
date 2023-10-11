#include "motorY_thread.h"
#include <Titan.h>

/* motorY entry function */
void motorY_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
//        motorHandler (motorBlockY);
        //allow next waiting thread(s) to run
        tx_thread_relinquish ();
    }
}
