#include "Management.h"

/* Management entry function */
void Management_entry(void)
{
    ///The Management thread will be responsible for the build plate, enclosure, and tool chamber.
    while (1)
    {
        tx_thread_sleep (500);
    }
}
