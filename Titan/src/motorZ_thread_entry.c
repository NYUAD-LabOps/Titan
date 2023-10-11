#include "motorZ_thread.h"
#include <Titan.h>

/* motorZ entry function */
void motorZ_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
 //       motorHandler (motorBlockZ);
        //allow next waiting thread(s) to run
        tx_thread_relinquish ();
    }
}
