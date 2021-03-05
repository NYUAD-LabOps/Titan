#include "USBHost.h"
#include <Titan.h>
#include "board_cfg.h"
#include "fx_api.h"

#define     MAX_NUM_OF_TRY              (1000000)

#define     EVENT_USB_PLUG_IN           (1UL << 0)
#define     EVENT_USB_PLUG_OUT          (1UL << 1)

static FX_FILE my_file;
static FX_FILE ini_file;
static char *local_buffer;

//static void set_led_off(int iIdx);
//static void blink_led_blink(int iIdx, int iCount);

UINT usb_host_plug_event_notification(ULONG usb_event, UX_HOST_CLASS *host_class, VOID *instance);

//callback function for USB_MSC_HOST
UINT usb_host_plug_event_notification(ULONG usb_event, UX_HOST_CLASS *host_class, VOID *instance)
{
    /* variable to hold the UX calls return values */
    UINT ux_return;

    UX_HOST_CLASS_STORAGE_MEDIA *p_ux_host_class_storage_media;

    // Check if host_class is for Mass Storage class.
    if (UX_SUCCESS
            == _ux_utility_memory_compare (_ux_system_host_class_storage_name, host_class,
                                           _ux_utility_string_length_get (_ux_system_host_class_storage_name)))
    {
        // Get the pointer to the media
        ux_return = ux_system_host_storage_fx_media_get (instance, &p_ux_host_class_storage_media, &g_fx_media1_ptr);

        if (ux_return != UX_SUCCESS)
        {
            /* This is a fairly simple error handling - it holds the
             application execution. In a more realistic scenarios
             a more robust and complex error handling solution should
             be provided. */
#ifdef SEMI_HOSTING
            if (DEBUGGER)
            {
                if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
                {
                    /* Debugger is connected */
                    /* Call this before any calls to printf() */
                    printf ("Could not get the pointer to the media, error:%d\n", ux_return);
                }
            }
#endif
//                   tx_thread_sleep(TX_WAIT_FOREVER);
        }
        else
        {
            //Check the usb_event type
            switch (usb_event)
            {
                case EVENT_USB_PLUG_IN:
                    // Notify the insertion of a USB Mass Storage device.
                    tx_event_flags_set (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_OR);
                    machineGlobalsBlock->USBPlugIn = 1;
                break;
                case EVENT_USB_PLUG_OUT:
                    // Notify the removal of a USB Mass Storage device.
                    tx_event_flags_set (&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_OR);
                    machineGlobalsBlock->USBPlugIn = 0;
                break;
                default:
                    //ignore this unsupported event
                break;
            }
        }
    }
    return UX_SUCCESS;
}

/* USBHost entry function */
void USBHost_entry(void)
{
    CHAR volume[32];
    FX_MEDIA *p_media;
    ULONG actual_length = 0;
    ULONG actual_flags;
    int iValue = 0;
    UINT tx_return;
    UINT fx_return;
    UINT status;
    UCHAR waiting = 0;


    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }


    tx_thread_suspend (tx_thread_identify ());


    while (machineGlobalsBlock->USBPlugIn == 0)
    {
        tx_thread_sleep (500);
    }

    // Get the pointer to FileX Media Control Block for a USB flash device
    p_media = g_fx_media1_ptr;
    machineGlobalsBlock->p_media = p_media;

    // Retrieve the volume name of the opened media from the Data sector
    fx_return = fx_media_volume_get (machineGlobalsBlock->p_media, volume, FX_DIRECTORY_SECTOR);

    if (fx_return == FX_SUCCESS)
    {
        // Set the default directory in the opened media, arbitrary name called "firstdir"
        fx_directory_default_set (machineGlobalsBlock->p_media, "firstdir");

        // Suspend this thread for 200 time-ticks
        tx_thread_sleep (100);

        // Try to open the file, 'gcodeHelix.txt'.
        fx_return = fx_file_open(machineGlobalsBlock->p_media, &my_file, "gcodeHelix.txt",
                                 FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);

        if (fx_return != FX_SUCCESS)
        {
            //The 'gcodeHelix.txt' file is not found, so create a new file
            fx_return = fx_file_create (machineGlobalsBlock->p_media, "gcodeHelix.txt");
            if (fx_return != FX_SUCCESS)
            {
                // Blink the LED 1 to report an error

                tx_thread_suspend (tx_thread_identify ());
            }
            fx_return = fx_media_flush (machineGlobalsBlock->p_media);
            // Open that file
            fx_return = fx_file_open(machineGlobalsBlock->p_media, &my_file, "gcodeHelix.txt",
                                     FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);
            if (fx_return != FX_SUCCESS)
            {
                if (DEBUGGER)
                {
                    printf ("\nCould not open G-code buffer.");
                }
                tx_thread_suspend (tx_thread_identify ());
            }
            else
            {
                if (DEBUGGER)
                {
                    printf ("\nG-code buffer ready.");
                }
            }
        }
        else
        {
            if (DEBUGGER)
            {
                printf ("\nG-code buffer already found - rebuilding...");
            }
            ///File already exists - rebuild it for a clean slate
            fx_return = fx_file_close (&my_file);
            fx_return = fx_file_delete (machineGlobalsBlock->p_media, "gcodeHelix.txt");
            fx_return = fx_media_flush (machineGlobalsBlock->p_media);

            fx_return = fx_file_create (machineGlobalsBlock->p_media, "gcodeHelix.txt");
            fx_return = fx_media_flush (machineGlobalsBlock->p_media);
            if (fx_return != FX_SUCCESS)
            {
                if (DEBUGGER)
                {
                    printf ("\nCould not create G-code buffer.");
                }

                tx_thread_suspend (tx_thread_identify ());
            }
            // Open that file
            fx_return = fx_file_open(machineGlobalsBlock->p_media, &my_file, "gcodeHelix.txt",
                                     FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);
            if (fx_return != FX_SUCCESS)
            {
                if (DEBUGGER)
                {
                    printf ("\nCould not open G-code buffer.");
                }
                tx_thread_suspend (tx_thread_identify ());
            }
            else
            {
                if (DEBUGGER)
                    printf ("\nG-code buffer ready.");

            }
        }

        ///Wait for default motor data to be ready
        while (machineGlobalsBlock->motorsInit == 0)
        {
            tx_thread_sleep (50);
        }
        // Try to open the file, 'Helix.ini'.
        fx_return = fx_file_open(machineGlobalsBlock->p_media, &ini_file, "Helix.ini",
                                 FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);

        if (fx_return != FX_SUCCESS)
        {
            if (DEBUGGER)
                printf ("\nHelix.ini not found. Creating...");
            //The 'Helix.ini' file is not found, so create a new file
            fx_return = fx_file_create (machineGlobalsBlock->p_media, "Helix.ini");
            if (fx_return != FX_SUCCESS)
            {

            }
            else
            {
                if (DEBUGGER)
                    printf ("\nHelix.ini created. Initiating INI save...");
            }
            // Open that file
            fx_return = fx_file_open(machineGlobalsBlock->p_media, &ini_file, "Helix.ini",
                                     FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);
            if (fx_return != FX_SUCCESS)
            {
                if (DEBUGGER)
                    printf ("\nCannot open Helix.ini.");
            }
            else
            {
                saveINI ();
            }
        }
        else
        {
            ///Helix.ini is already present. Load INI settings
            if (DEBUGGER)
                printf ("\nHelix.ini present. Initiating INI load...");
            loadINI ();
        }

        // Already open a file, then read the file in blocks
        // Set a specified byte offset for reading
        fx_return = fx_file_seek (&my_file, 0);

        machineGlobalsBlock->USBBufferOpen = 1;
        if (DEBUGGER)
            printf ("\nUSB buffer ready.");
        if (DEBUGGER)
            printf ("\nController ready.");
    }
    while (1)
    {
        tx_thread_sleep (1);
    }
}

void USB_Write_Buffer(char *data, UINT length)
{
    UINT fx_return;
    ULONG length_tmp;

    length_tmp = strlen (data);

    memcpy ((local_buffer + 1), data, length_tmp);
    *local_buffer = '\n';
//
//    if (fx_return == FX_SUCCESS)
//    {
    // Write the file in blocks
    fx_return = fx_file_write (&my_file, local_buffer, (length_tmp + 1));
    if (fx_return != FX_SUCCESS)
    {
        printf ("\nFile write fail.");
    }
    else
    {
        fx_return = fx_media_flush (machineGlobalsBlock->p_media);
    }
//    } else{
//        printf("\nFile seek fail.");
//    }
    printf ("\nUSB write complete.");

}

///This function transfers the entire contents of local_buffer to
/// to the USB drive.
void USB_Buffer_Transfer()
{
    UINT fx_return;
    ULONG length_tmp;

//    printf (machineGlobalsBlock->local_buffer);
//    printf ("\nUSB buffer transfer...");
    // Write the file in blocks
    fx_return = fx_file_write (&my_file, local_buffer, machineGlobalsBlock->local_bufferIndex);
    if (fx_return != FX_SUCCESS)
    {
        printf ("\nFile write fail.");
    }
    else
    {
//        printf ("\nFile write success.");
        fx_return = fx_media_flush (machineGlobalsBlock->p_media);
//        if (fx_return == FX_SUCCESS)
//        {
//            printf ("\nBuffer transfer complete.");
//        }
//        else
//        {
//            printf ("\nBuffer transfer flush fail.");
//        }
    }

///Clear local buffer to prevent errors with other USB operations
    memset (machineGlobalsBlock->local_buffer, 0, machineGlobalsBlock->local_bufferIndex);
    machineGlobalsBlock->USBBufferHasData = 1;
    machineGlobalsBlock->local_bufferIndex = 0;

}

///Uses data from the USB gcode buffer to rebuild the linked list buffer.
void rebuildLinkedList()
{
    UINT fx_return;
    ULONG actual_length = 0;
    ULONG length_tmp;
    int i;

    ///Go to the G-code buffer index location
    fx_return = fx_file_seek (&my_file, machineGlobalsBlock->USBFileIndex);
    if (fx_return != FX_SUCCESS)
    {
        if (DEBUGGER)
            printf ("\nFile seek fail.");
    }
    ///Attempt to read 500 bytes
    fx_return = fx_file_read (&my_file, machineGlobalsBlock->USBBufferB, 500, &actual_length);
    if (fx_return != FX_SUCCESS)
    {
        if (DEBUGGER)
            printf ("\nFile read fail.");
    }
    ///Create a new link using the string between each \n
    i = 0;
    while (i < actual_length)
    {
        if (isInRange (machineGlobalsBlock->USBBufferB[i]) == 1)
        {
            insertLink ((machineGlobalsBlock->USBBufferB + i));
            i += strlen ((machineGlobalsBlock->USBBufferB + i));
        }
        while (isInRange (machineGlobalsBlock->USBBufferB[i]) == 0 && i < actual_length)
        {
            i++;
        }

    }

    ///If the number of bytes read is less than 500, the end of the file has been reached
    if (actual_length < 500)
    {
        machineGlobalsBlock->USBBufferHasData = 0;
        machineGlobalsBlock->USBFileIndex = 0;
        ///Rebuild it
//        printf ("\nG-code buffer rebuilding...");
        ///End of file reached - rebuild it for a clean slate
        fx_return = fx_file_close (&my_file);
        if (fx_return != FX_SUCCESS)
        {
            if (DEBUGGER)
                printf ("\nCould not create G-code buffer1.");

        }
        fx_return = fx_file_delete (machineGlobalsBlock->p_media, "gcodeHelix.txt");
        if (fx_return != FX_SUCCESS)
        {
            if (DEBUGGER)
                printf ("\nCould not create G-code buffer2.");

        }
        fx_return = fx_media_flush (machineGlobalsBlock->p_media);
        if (fx_return != FX_SUCCESS)
        {
            if (DEBUGGER)
                printf ("\nCould not create G-code buffer3.");

        }
        fx_return = fx_file_create (machineGlobalsBlock->p_media, "gcodeHelix.txt");
        if (fx_return != FX_SUCCESS)
        {
            if (DEBUGGER)
                printf ("\nCould not create G-code buffer4.");

        }

        fx_return = fx_media_flush (machineGlobalsBlock->p_media);

        // Open that file
        fx_return = fx_file_open(machineGlobalsBlock->p_media, &my_file, "gcodeHelix.txt",
                                 FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE);
        if (fx_return != FX_SUCCESS)
        {
            if (DEBUGGER)
                printf ("\nCould not open G-code buffer.");

        }
    }
    else
    {
        ///Set new file index - there is more data
        machineGlobalsBlock->USBFileIndex += actual_length;
    }

    ///Clear local buffer
    memset (machineGlobalsBlock->USBBufferB, 0, actual_length);
//    printf ("\nLinked list rebuilt.");
}

/// Saves all persistent machine information, including motor calibration settings
void saveINI()
{
    UINT fx_return;
    UINT tmpIndex;
    ULONG length_tmp;

    tmpIndex = 0;
    length_tmp = sizeof(struct motorController);
    ///Go to INI file start
    fx_return = fx_file_seek (&machineGlobalsBlock->iniFile, 0);

    memcpy (machineGlobalsBlock->local_buffer, motorBlockZ, length_tmp);
    memcpy ((machineGlobalsBlock->local_buffer + length_tmp), motorBlockX, length_tmp);
    memcpy ((machineGlobalsBlock->local_buffer + (2 * length_tmp)), motorBlockY, length_tmp);
    memcpy ((machineGlobalsBlock->local_buffer + (3 * length_tmp)), toolBlockA->motorBlock, length_tmp);

//    printf ("\nWriting INI data...");
    // Write the file in blocks
    fx_return = fx_file_write (&machineGlobalsBlock->iniFile, machineGlobalsBlock->local_buffer, (4 * length_tmp));
    if (fx_return != FX_SUCCESS)
    {
        if (DEBUGGER)
            printf ("\nINI write fail.");
    }
    else
    {
        if (DEBUGGER)
            printf ("\nINI write success.");
        fx_return = fx_media_flush (&g_fx_media0);
    }
    if (DEBUGGER)
        printf ("\nINI save complete.");

///Clear local buffer to prevent errors with other USB operations
    memset (machineGlobalsBlock->local_buffer, 0, (4 * length_tmp));

    if (DEBUGGER)
        printf ("\nINI save complete.");
}

/// Saves all persistent machine information, including motor calibration settings
void loadINI()
{
    UINT fx_return;
    UINT tmpIndex;
    ULONG length_tmp;
    ULONG actual_length;

    tmpIndex = 0;
    length_tmp = sizeof(struct motorController);
    ///Go to INI file start
    fx_return = fx_file_seek (&machineGlobalsBlock->iniFile, 0);

    if (DEBUGGER)
        printf ("\nLoading INI...");
    ///Attempt to read 500 bytes
    fx_return = fx_file_read (&machineGlobalsBlock->iniFile, machineGlobalsBlock->local_buffer, (4 * length_tmp), &actual_length);
    if (fx_return != FX_SUCCESS)
    {
        if (DEBUGGER)
            printf ("\nINI read fail.");
    }
    else
    {
        if (DEBUGGER)
            printf ("\nINI read success.");
    }

    memcpy (motorBlockZ, machineGlobalsBlock->local_buffer, length_tmp);
    memcpy (motorBlockX, (machineGlobalsBlock->local_buffer + length_tmp), length_tmp);
    memcpy (motorBlockY, (machineGlobalsBlock->local_buffer + (2 * length_tmp)), length_tmp);
    memcpy (toolBlockA->motorBlock, (machineGlobalsBlock->local_buffer + (3 * length_tmp)), length_tmp);

    if (DEBUGGER)
        printf ("\nINI data loaded.");
///Clear local buffer to prevent errors with other USB operations
    memset (machineGlobalsBlock->local_buffer, 0, (4 * length_tmp));
}
