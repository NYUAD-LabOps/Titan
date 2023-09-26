#include "motorT_thread.h"
#include <Titan.h>

/* motorT entry function */
void motorT_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        motorHandler (motorBlockT);
        //allow next waiting thread(s) to run
        tx_thread_relinquish ();
    }
}
