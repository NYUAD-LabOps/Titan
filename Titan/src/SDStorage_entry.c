#include "SDStorage.h"
#include <Titan.h>

/* SDStorage entry function */
void SDStorage_entry(void)
{
    UINT status;
    UINT fx_return;
    char *local_buffer;


    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }


    ///Setup the byte pool for handling FileX operations.
    local_buffer = initUSBBuffer_Pool (UX_STORAGE_BUFFER_SIZE);
    machineGlobalsBlock->local_buffer = local_buffer;


    ///Load or create INI file

    ///Wait for default motor data to be ready
    while (machineGlobalsBlock->motorsInit == 0)
    {
        tx_thread_sleep (50);
    }


    status = fx_media_init0_open ();

    // Try to open the file, 'Helix.ini'.
    fx_return = fx_file_open(&g_fx_media0, &machineGlobalsBlock->iniFile, "Titan.ini",
                             FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);

    if (fx_return != FX_SUCCESS)
    {
        if (DEBUGGER)
            printf ("\nTitan.ini not found. Creating...");
        //The 'Helix.ini' file is not found, so create a new file
        fx_return = fx_file_create (&g_fx_media0, "Titan.ini");
        if (fx_return != FX_SUCCESS)
        {

        }
        else
        {
            if (DEBUGGER)
                printf ("\nTitan.ini created. Initiating INI save...");
        }
        // Open that file
        fx_return = fx_file_open(&g_fx_media0, &machineGlobalsBlock->iniFile, "Titan.ini",
                                 FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);
        if (fx_return != FX_SUCCESS)
        {
//            if (DEBUGGER)
                printf ("\nCannot open Titan.ini.");
        }
        else
        {
            saveINI ();
        }
    }
    else
    {
        ///Titan.ini is already present. Load INI settings
        if (DEBUGGER)
            printf ("\nTitan.ini present. Initiating INI load...");
        loadINI ();
    }

//    CHAR dir_name[] = "/testdir";
//
//
//
//    tx_thread_sleep (10);
//
//    /* Create a directory */
//    status = fx_directory_create (&g_fx_media0, dir_name);
//    tx_thread_sleep (10);
//    status = fx_file_create (&g_fx_media0, "test.txt");
//
//    printf ("\n%i", status);
//    printf ("\n%d", sizeof(struct machineGlobals));
//    printf ("\n%d", sizeof(FX_FILE));
//
//    tx_thread_sleep (10);
//    fx_media_flush (&g_fx_media0);
//
//    tx_thread_sleep (10);

//     fx_media_close(&g_fx_media0);
}
