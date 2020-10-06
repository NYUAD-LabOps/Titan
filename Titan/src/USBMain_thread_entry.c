/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
#include "USBMain_thread.h"
#include "board_cfg.h"
#include "fx_api.h"
#include "Helix.h"

static UX_SLAVE_CLASS_CDC_ACM *g_cdc;
#define SEMI_HOSTING

#define     MAX_NUM_OF_TRY              (1000000)
#define     UX_STORAGE_BUFFER_SIZE      (64*1024)

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
        ux_return = ux_system_host_storage_fx_media_get (instance, &p_ux_host_class_storage_media, &g_fx_media0_ptr);

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

/* USB Thread entry function */
/* The application project demonstrates the typical use of the USBX Host Class Mass Storage module APIs.
 * The application project main thread entry waits for the connection from the callback function, reads the
 * firstdir\gcodeHelix.txt file in the USB memory, and updates the described number. If firstdir does not exist,
 * the directory information is ignored. If this file does not exist, creates a file. After updating the file,
 * the application waits until the USB memory is unplugged. The application uses LED2 LED1 and LED0 in sequence
 * to provide status of the application. (When all three or two (depends upon evaluation board) lights up, it
 * indicates operation is complete and user can disconnect the attached mass storage device.The application uses
 * SEMI-HOSTING feature, it displays relevant messages and errors to the user. */
void USBMain_thread_entry(void)
{

    CHAR volume[32];
    FX_MEDIA *p_media;
    ULONG actual_length = 0;
    ULONG actual_flags;
    int iValue = 0;
    UINT tx_return;
    UINT fx_return;
    UINT status;

    ///Setup the byte pool for handling FileX operations.
    local_buffer = initUSBBuffer_Pool (UX_STORAGE_BUFFER_SIZE);
    machineGlobalsBlock->local_buffer = local_buffer;

    while (1)
    {

        while (machineGlobalsBlock->globalsInit != 1)
        {
            tx_thread_sleep (500);
        }

        while (machineGlobalsBlock->USBPlugIn == 0)
        {
            tx_thread_sleep (500);
        }

        // Get the pointer to FileX Media Control Block for a USB flash device
        p_media = g_fx_media0_ptr;
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

                    break;
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
                    break;
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

                    break;
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
                    break;
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

            while (1)
            {
                memset (machineGlobalsBlock->USBBufferB, 0, 49);
                status = _ux_device_class_cdc_acm_read (g_cdc, machineGlobalsBlock->USBBufferB, 49, &actual_length);
                if (status == UX_SUCCESS)
                {
//                    if(DEBUG) printf ("\nDevice received...");
                    //printf(topic_buffer);
//                    if(DEBUG) printf (machineGlobalsBlock->USBBufferB);
                    if (machineGlobalsBlock->USBBufferB[0] == 'U' && machineGlobalsBlock->USBBufferB[1] == 'S'
                            && machineGlobalsBlock->USBBufferB[2] == 'B')
                    {
                        status = _ux_device_class_cdc_acm_write (g_cdc, (UCHAR *) "USB", 4, &actual_length);
                    }
                    else if (machineGlobalsBlock->USBBufferB[0] == 'T' && machineGlobalsBlock->USBBufferB[1] == 'M'
                            && machineGlobalsBlock->USBBufferB[2] == 'P')
                    {
                        char tmpUSBOut[12];
                        memset (tmpUSBOut, 0, 12);
                        tmpUSBOut[0] = 'T';
                        tmpUSBOut[1] = 'M';
                        tmpUSBOut[2] = 'P';

                        snprintf (tmpUSBOut + 3, 8, "%f", toolBlockA->tempRead);
                        status = _ux_device_class_cdc_acm_write (g_cdc, (UCHAR *) tmpUSBOut, 12, &actual_length);
                    }
                    else
                    {
                        processReceivedMsg (machineGlobalsBlock->USBBufferB);
                    }
                }
                else
                {

                }
                tx_thread_sleep (1);
            }

            //Close already opened file
            fx_return = fx_file_close (&my_file);

            tx_thread_sleep (200);

            /* flush the media */
            fx_return = fx_media_flush (machineGlobalsBlock->p_media);

            /* close the media */
            fx_return = fx_media_close (machineGlobalsBlock->p_media);

#ifdef SEMI_HOSTING
            if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
            {
                /* Debugger is connected */
                /* Call this before any calls to printf() */
                printf ("Disconnect the device \n");
            }
#endif

        }

        while (machineGlobalsBlock->USBPlugIn == 1)
        {
            tx_thread_sleep (500);
        }
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
    fx_return = fx_file_seek (&ini_file, 0);

    memcpy (local_buffer, motorBlockZ, length_tmp);
    memcpy ((local_buffer + length_tmp), motorBlockX, length_tmp);
    memcpy ((local_buffer + (2 * length_tmp)), motorBlockY, length_tmp);
    memcpy ((local_buffer + (3 * length_tmp)), toolBlockA->motorBlock, length_tmp);

    printf ("\nWriting INI data...");
    // Write the file in blocks
    fx_return = fx_file_write (&ini_file, local_buffer, (4 * length_tmp));
    if (fx_return != FX_SUCCESS)
    {
        if (DEBUGGER)
            printf ("\nINI write fail.");
    }
    else
    {
        if (DEBUGGER)
            printf ("\nINI write success.");
        fx_return = fx_media_flush (machineGlobalsBlock->p_media);
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
    fx_return = fx_file_seek (&ini_file, 0);

    if (DEBUGGER)
        printf ("\nLoading INI...");
    ///Attempt to read 500 bytes
    fx_return = fx_file_read (&ini_file, local_buffer, (4 * length_tmp), &actual_length);
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

    memcpy (motorBlockZ, local_buffer, length_tmp);
    memcpy (motorBlockX, (local_buffer + length_tmp), length_tmp);
    memcpy (motorBlockY, (local_buffer + (2 * length_tmp)), length_tmp);
    memcpy (toolBlockA->motorBlock, (local_buffer + (3 * length_tmp)), length_tmp);

    if (DEBUGGER)
        printf ("\nINI data loaded.");
///Clear local buffer to prevent errors with other USB operations
    memset (machineGlobalsBlock->local_buffer, 0, (4 * length_tmp));
}

void ux_cdc_device0_instance_activate(VOID *cdc_instance)
{
    /* Save the CDC instance.  */
    g_cdc = (UX_SLAVE_CLASS_CDC_ACM *) cdc_instance;
    tx_semaphore_put (&g_cdc_activate_semaphore);
}

void ux_cdc_device0_instance_deactivate(VOID *cdc_instance)
{
    SSP_PARAMETER_NOT_USED(cdc_instance);

    g_cdc = UX_NULL;
}
