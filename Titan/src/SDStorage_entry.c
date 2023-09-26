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
    if (status != FX_SUCCESS)
    {
        printf ("\nCould not open SD");
    }
    else
    {
        printf ("\nSD card ready.");
    }

    //INI functions are currently disabled.
    tx_thread_suspend (tx_thread_identify ());

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
            printf ("\nCould not create Titan.ini");
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

    machineGlobalsBlock->iniInit = 1;
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

void rebuildLinkedListFromSD()
{
    UINT fx_return;
    ULONG actual_length = 0;
    ULONG length_tmp;
    int i, j;
    char isInRangeReturn;
//    memset (machineGlobalsBlock->USBBufferB, 0, 100);

    j = 0;
    do
    {
        i = 0;
        do
        {

            fx_return = fx_file_read (&machineGlobalsBlock->gcodeFile, (machineGlobalsBlock->USBBufferB + i), 1,
                                      &actual_length);
            if (fx_return != FX_SUCCESS)
            {
                printf ("\nRead F.");
            }
            else
            {
                //Potential here for actual length greater than one, which is not handled.
                isInRangeReturn = isInRange (machineGlobalsBlock->USBBufferB[i]);
                i += actual_length;
            }
        }
        while (actual_length >= 1 && isInRangeReturn == 1 && fx_return == FX_SUCCESS);

        if (i > 3)
        {
            ///Minimum line length met.
            insertLink (machineGlobalsBlock->USBBufferB);
            j++;
        }
        memset (machineGlobalsBlock->USBBufferB, 0, i);
    }
    while (j < 10 && actual_length >= 1 && fx_return == FX_SUCCESS);

    if (actual_length >= 1 && fx_return == FX_SUCCESS)
    {
        ///End of file not reached. Maybe more data.
        machineGlobalsBlock->USBBufferHasData = 1;

    }
    else
    {
        ///End of file reached, or read error.
        machineGlobalsBlock->USBBufferHasData = 0;
        if (DEBUGGERPRIMARY)
            printf ("\nEnd of file.");
    }
}
