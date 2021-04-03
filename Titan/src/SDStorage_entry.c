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

//void rebuildLinkedListFromSD()
//{
//    UINT fx_return;
//    ULONG actual_length = 0;
//    ULONG length_tmp;
//    int i;
//
//    ///Go to the G-code buffer index location
//    fx_return = fx_file_seek (&my_file, machineGlobalsBlock->USBFileIndex);
//    if (fx_return != FX_SUCCESS)
//    {
//        if (DEBUGGER)
//            printf ("\nFile seek fail.");
//    }
//    ///Attempt to read 500 bytes
//    fx_return = fx_file_read (&my_file, machineGlobalsBlock->USBBufferB, 500, &actual_length);
//    if (fx_return != FX_SUCCESS)
//    {
//        if (DEBUGGER)
//            printf ("\nFile read fail.");
//    }
//    ///Create a new link using the string between each \n
//    i = 0;
//    while (i < actual_length)
//    {
//        if (isInRange (machineGlobalsBlock->USBBufferB[i]) == 1)
//        {
//            insertLink ((machineGlobalsBlock->USBBufferB + i));
//            i += strlen ((machineGlobalsBlock->USBBufferB + i));
//        }
//        while (isInRange (machineGlobalsBlock->USBBufferB[i]) == 0 && i < actual_length)
//        {
//            i++;
//        }
//
//    }
//
//    ///If the number of bytes read is less than 500, the end of the file has been reached
//    if (actual_length < 500)
//    {
//        machineGlobalsBlock->USBBufferHasData = 0;
//        machineGlobalsBlock->USBFileIndex = 0;
//        ///Rebuild it
////        printf ("\nG-code buffer rebuilding...");
//        ///End of file reached - rebuild it for a clean slate
//        fx_return = fx_file_close (&my_file);
//        if (fx_return != FX_SUCCESS)
//        {
//            if (DEBUGGER)
//                printf ("\nCould not create G-code buffer1.");
//
//        }
//        fx_return = fx_file_delete (machineGlobalsBlock->p_media, "gcodeHelix.txt");
//        if (fx_return != FX_SUCCESS)
//        {
//            if (DEBUGGER)
//                printf ("\nCould not create G-code buffer2.");
//
//        }
//        fx_return = fx_media_flush (machineGlobalsBlock->p_media);
//        if (fx_return != FX_SUCCESS)
//        {
//            if (DEBUGGER)
//                printf ("\nCould not create G-code buffer3.");
//
//        }
//        fx_return = fx_file_create (machineGlobalsBlock->p_media, "gcodeHelix.txt");
//        if (fx_return != FX_SUCCESS)
//        {
//            if (DEBUGGER)
//                printf ("\nCould not create G-code buffer4.");
//
//        }
//
//        fx_return = fx_media_flush (machineGlobalsBlock->p_media);
//
//        // Open that file
//        fx_return = fx_file_open(machineGlobalsBlock->p_media, &my_file, "gcodeHelix.txt",
//                                 FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);
//        if (fx_return != FX_SUCCESS)
//        {
//            if (DEBUGGER)
//                printf ("\nCould not open G-code buffer.");
//
//        }
//    }
//    else
//    {
//        ///Set new file index - there is more data
//        machineGlobalsBlock->USBFileIndex += actual_length;
//    }
//
//    ///Clear local buffer
//    memset (machineGlobalsBlock->USBBufferB, 0, actual_length);
////    printf ("\nLinked list rebuilt.");
//}
