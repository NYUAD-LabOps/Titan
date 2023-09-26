#include "motorX_thread.h"
#include <Titan.h>

/* motorX entry function */
void motorX_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        motorHandler (motorBlockX);
        //allow next waiting thread(s) to run
        tx_thread_relinquish ();
    }
}
