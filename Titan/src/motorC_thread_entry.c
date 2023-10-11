#include "motorC_thread.h"
#include <Titan.h>

/* motorC entry function */
void motorC_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
 //       motorHandler (motorBlockC);
        //allow next waiting thread(s) to run
        tx_thread_relinquish ();
    }
}
