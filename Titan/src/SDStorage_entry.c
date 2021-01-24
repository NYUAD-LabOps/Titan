#include "SDStorage.h"
#include <Titan.h>

/* SDStorage entry function */
void SDStorage_entry(void)
{
    UINT status;

    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }

     CHAR dir_name[] = "/testdir";

     status = fx_media_init0_open ();




     tx_thread_sleep(10);

     /* Create a directory */
     status = fx_directory_create(&g_fx_media0, dir_name);
     tx_thread_sleep(10);
     status = fx_file_create(&g_fx_media0, "test.txt");





     printf("\n%i", status);
     printf("\n%d", sizeof(struct machineGlobals));
     printf("\n%d", sizeof(FX_FILE));

     tx_thread_sleep(10);
     fx_media_flush(&g_fx_media0);

     tx_thread_sleep(10);

     fx_media_close(&g_fx_media0);
}
