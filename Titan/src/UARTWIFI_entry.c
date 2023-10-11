//#include "UARTWIFI.h"
#include <Titan.h>

void error_trap(char *msg, ULONG status)
{

    printf ("%s failed. error = %d\n", msg, (int) status);

    while (1)
    {
        tx_thread_relinquish ();
    }
}

void UARTWIFI_entry(void)
{
    while (1)
    {
        tx_thread_relinquish ();
    }
}
