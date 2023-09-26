#include "UARTWIFI.h"
#include <Titan.h>

static FX_FILE my_file;
static FX_FILE ini_file;
static char *local_buffer;

#define UX_BUFFER_SIZE (128)
#define CDCACM_FLAG ((ULONG)0x0001)

/* CDC-ACM reception data buffer. */
static unsigned char buffer[UX_BUFFER_SIZE];

/* A pointer to store CDC-ACM device instance. */
static UX_SLAVE_CLASS_CDC_ACM *g_cdc = UX_NULL;

/* USBX CDC-ACM Instance Activate User Callback Function */
VOID ux_cdc_device0_instance_activate(VOID *cdc_instance)
{
    /* Save the CDC instance.  */
    g_cdc = (UX_SLAVE_CLASS_CDC_ACM*) cdc_instance;
    tx_event_flags_set (&g_cdcacm_activate_event_flags0, CDCACM_FLAG, TX_OR);
}

/* USBX CDC-ACM Instance Deactivate User Callback Function */
VOID ux_cdc_device0_instance_deactivate(VOID *cdc_instance)
{
    SSP_PARAMETER_NOT_USED(cdc_instance);

    tx_event_flags_set (&g_cdcacm_activate_event_flags0, ~CDCACM_FLAG, TX_AND);
    g_cdc = UX_NULL;
}

void error_trap(char *msg, ULONG status);
void error_trap(char *msg, ULONG status)
{

    printf ("%s failed. error = %d\n", msg, (int) status);

    while (1)
    {

    }
}

/* UARTWIFI entry function */
void UARTWIFI_entry(void)
{

    UINT fx_return;
    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }
    UCHAR uartRx[WIFI_PACKET_SIZE];
    UCHAR controlCodeRxBuf[4];
    memset (controlCodeRxBuf, 0, 4);
    int i;
    i = 0;
    memset (uartRx, 0, WIFI_PACKET_SIZE);

    ULONG status;
    ULONG actual_flags;
    ULONG actual_length;

    //Call pre-programmed instructions.

    char preProgrammed[WIFI_PACKET_SIZE];
//    memset (preProgrammed, 0, WIFI_PACKET_SIZE);

    tx_thread_sleep(1000);

    memset (preProgrammed, 0, WIFI_PACKET_SIZE);
    strcpy (preProgrammed, "PRJ");
    processReceivedMsg (preProgrammed);

    tx_thread_suspend (tx_thread_identify ());

//Home axes and move forward 5s
    memset (preProgrammed, 0, WIFI_PACKET_SIZE);
    strcpy (preProgrammed, "G28 XYZ");
    processReceivedMsg (preProgrammed);

    //Give it time to process command
    tx_thread_sleep(500);

    while (motorBlockX->homing || motorBlockY->homing || motorBlockZ->homing)
        ;

    memset (preProgrammed, 0, WIFI_PACKET_SIZE);
    strcpy (preProgrammed, "M03 X500");
    processReceivedMsg (preProgrammed);
    memset (preProgrammed, 0, WIFI_PACKET_SIZE);
    strcpy (preProgrammed, "M03 Y500");
    processReceivedMsg (preProgrammed);
    memset (preProgrammed, 0, WIFI_PACKET_SIZE);
    strcpy (preProgrammed, "M03 Z500");
    processReceivedMsg (preProgrammed);
    memset (preProgrammed, 0, WIFI_PACKET_SIZE);
    strcpy (preProgrammed, "M03 A500");
    processReceivedMsg (preProgrammed);
    tx_thread_sleep (5000);

    while (1)
    {
        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M03 X500");
        processReceivedMsg (preProgrammed);
        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M03 Y500");
        processReceivedMsg (preProgrammed);
        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M03 Z500");
        processReceivedMsg (preProgrammed);
        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M03 A500");
        processReceivedMsg (preProgrammed);
        tx_thread_sleep (5000);

        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M04 X500");
        processReceivedMsg (preProgrammed);
        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M04 Y500");
        processReceivedMsg (preProgrammed);
        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M04 Z500");
        processReceivedMsg (preProgrammed);
        memset (preProgrammed, 0, WIFI_PACKET_SIZE);
        strcpy (preProgrammed, "M04 A500");
        processReceivedMsg (preProgrammed);
        tx_thread_sleep (5000);
    }

    //USB Currently disabled
    tx_thread_suspend (tx_thread_identify ());

    printf ("USBX_CDCACM_Device running\n\n");

    /* Check if a CDC device is connected */
    status = tx_event_flags_get (&g_cdcacm_activate_event_flags0, CDCACM_FLAG, TX_OR, &actual_flags, TX_NO_WAIT);
    if (status && (status != TX_NO_EVENTS))
    {
        error_trap ("tx_event_flags_get 1", status);
    }

    /* Wait for a CDC device to be connected */
    while (!(actual_flags & CDCACM_FLAG))
    {

        printf ("Waiting for a CDC device to be connected...\n");

        /* 1. Wait for the CDCACM event flag */
        status = tx_event_flags_get (&g_cdcacm_activate_event_flags0, CDCACM_FLAG, TX_OR, &actual_flags,
        TX_WAIT_FOREVER);
        if (status)
        {
            error_trap ("tx_event_flags_get 2", status);
        }

        printf ("Event detected\n");

    }

    printf ("CDC device connected\n");

    /* Perform an echo function by reading characters from the CDC device and writing them back */
    while (1)
    {
//        if (i >= 3)
//        {
//            controlCodeRxBuf[0] = controlCodeRxBuf[1];
//            controlCodeRxBuf[1] = controlCodeRxBuf[2];
//            i = 2;
//        }

        /* 2. Read from the CDC device. This is a blocking call */
        status = ux_device_class_cdc_acm_read (g_cdc, uartRx, WIFI_PACKET_SIZE, &actual_length);
        i++;
        if (status)
        {
            printf ("Device read fail");
        }
        if (uartRx[0] == '@' && uartRx[1] == '@' && uartRx[2] == '@')
        {
//            status = ux_device_class_cdc_acm_read (g_cdc, uartRx, WIFI_PACKET_SIZE, &actual_length);
//            g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, uartRx, WIFI_PACKET_SIZE, TX_WAIT_FOREVER);
            //        printf ("\n%s", uartRx);

            if (actual_length >= 53)
                serialHandler (uartRx + 3);

            //echo
            status = ux_device_class_cdc_acm_write (g_cdc, uartRx, WIFI_PACKET_SIZE, &actual_length);
            if (status)
            {
                error_trap ("ux_device_class_cdc_acm_write", status);
            }

            printf ("%d characters written\n", (int) actual_length);
            ///Clear old data and relinquish.
            memset (uartRx, 0, WIFI_PACKET_SIZE);
//            memset (controlCodeRxBuf, 0, 4);
//            i = 0;
        }
        printf ("%d characters read\n", (int) actual_length);
        tx_thread_relinquish ();
    }
//    UINT fx_return;
//    while (machineGlobalsBlock->globalsInit != 1)
//    {
//        tx_thread_sleep (500);
//    }
//    /* TODO: add your own code here */
//    UCHAR uartRx[WIFI_PACKET_SIZE];
//    UCHAR controlCodeRxBuf[4];
//    memset (controlCodeRxBuf, 0, 4);
//    int i;
//    i = 0;
//    memset (uartRx, 0, WIFI_PACKET_SIZE);
//    static uart_cfg_t rx_uart_config;
//    g_sf_comms0.p_api->open (g_sf_comms0.p_ctrl, g_sf_comms0.p_cfg);
//
//    while (1)
//    {
//        if (i >= 3)
//        {
//            controlCodeRxBuf[0] = controlCodeRxBuf[1];
//            controlCodeRxBuf[1] = controlCodeRxBuf[2];
//            i = 2;
//        }
//
//        g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, (controlCodeRxBuf + i), 1, TX_WAIT_FOREVER);
//        i++;
//
//        if (strcmp (controlCodeRxBuf, "@@@") == 0)
//        {
//            g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, uartRx, WIFI_PACKET_SIZE, TX_WAIT_FOREVER);
//            //        printf ("\n%s", uartRx);
//            serialHandler (uartRx);
//
//            ///Clear old data and relinquish.
//            memset (uartRx, 0, WIFI_PACKET_SIZE);
//            memset (controlCodeRxBuf, 0, 3);
//        }
//
//        tx_thread_relinquish ();
//    }
}

void serialHandler(char *uartRx)
{
    UINT status;
    ULONG actual_length;
    long long fileSize;
    fileSize = 0;
    UCHAR sendACK[WIFI_PACKET_SIZE];
    memset (sendACK, 0, WIFI_PACKET_SIZE);
    sendACK[0] = 'A';
    sendACK[1] = 'C';
    sendACK[2] = 'K';

    ///In the case of a file transfer, don't ACK until the gcode file is ready for writing.
    /// For all other messages, send ACK at the end of processReceivedMsg(), to indicate to the GUI
    /// that the message has been received, and the controller is ready for more.

    fileSize = 0;
    switch (uartRx[0])
    {
        case 'F':

            memcpy (&fileSize, (uartRx + 2), 8);
//            fileSize = atoi ((uartRx + 2));
            rxFile (fileSize);
        break;
        default:
            ux_device_class_cdc_acm_write (g_cdc, sendACK, WIFI_PACKET_SIZE, &actual_length);
            processReceivedMsg (uartRx);
    }

}

///This function receives files from the GUI via UART or USB
void rxFile(long long fileSize)
{
//    UINT status;
//    int i, parts;
//    UCHAR rxBuf[WIFI_PACKET_SIZE];
//    UCHAR sendACK[WIFI_PACKET_SIZE];
//    memset (sendACK, 0, WIFI_PACKET_SIZE);
//    sendACK[0] = 'A';
//    sendACK[1] = 'C';
//    sendACK[2] = 'K';
//    memset (rxBuf, 0, WIFI_PACKET_SIZE);
//    machineGlobalsBlock->local_bufferIndex = 0;
//    //    long fileSize;
//
//    parts = fileSize / WIFI_PACKET_SIZE;
//
//    //    printf ("\n%s", test);
//
//    long remainder = fileSize % 1024;
//    printf ("\nsize: %lu remainder:%lu", fileSize, remainder);
//
//    //    fx_file_op
//
//    status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
//
//    if (status == FX_SUCCESS)
//    {
//        printf ("\nFile open s");
//        fx_file_close (&machineGlobalsBlock->gcodeFile);
//        status = fx_file_delete (&g_fx_media0, "testGcode.gcode");
//        if (status == FX_SUCCESS)
//        {
//            printf ("\nFile del s");
//        }
//        else
//        {
//            printf ("\nFile del f");
//        }
//        status = fx_file_create (&g_fx_media0, "testGcode.gcode");
//        if (status == FX_SUCCESS)
//        {
//            printf ("\nFile create s");
//        }
//        else
//        {
//            printf ("\nFile create f");
//        }
//        status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
//        if (status == FX_SUCCESS)
//        {
//            printf ("\nFile open s");
//        }
//        else
//        {
//            printf ("\nFile open f");
//        }
//    }
//    else
//    {
//        printf ("\nFile open f");
//        status = fx_file_create (&g_fx_media0, "testGcode.gcode");
//        if (status == FX_SUCCESS)
//        {
//            printf ("\nFile create s");
//        }
//        else
//        {
//            printf ("\nFile create f");
//        }
//        status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
//        if (status == FX_SUCCESS)
//        {
//            printf ("\nFile open s");
//        }
//        else
//        {
//            printf ("\nFile open f");
//        }
//    }
//
//    g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, TX_NO_WAIT);
//
////    i = 0;
//    while (rxBuf[0] != 'E' || rxBuf[1] != 'N' || rxBuf[2] != 'D')
//    {
//        status = g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, rxBuf, WIFI_PACKET_SIZE, 250);
//
//        if (status == SSP_SUCCESS)
//        {
//            status = fx_file_write (&machineGlobalsBlock->gcodeFile, rxBuf, WIFI_PACKET_SIZE);
//            if (status != FX_SUCCESS)
//            {
//                printf ("\nFile write f");
//            }
////            i++;
////            printf("\n%d", i);
//            g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, 250);
//        }
//        else if (status == 20)
//        {
//            printf ("\nTO");
////            g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, 250);
//        }
//        else
//        {
//            printf ("\nUART read error:%d", status);
//        }
//
//        //        machineGlobalsBlock->fileTransferIndex += 1000;
//    }
//
//    ///Send Final ACK
//    g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, 250);
////    ///Read and store remainder
////
////    status = g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, rxBuf, WIFI_PACKET_SIZE, 250);
////    if (status == SSP_SUCCESS)
////    {
////        fx_file_write (&machineGlobalsBlock->gcodeFile, rxBuf, remainder);
////    }
//
//    fx_media_flush (&g_fx_media0);
//    fx_file_close (&machineGlobalsBlock->gcodeFile);
//
//    printf ("\nFile transfer completed.");
//    printf ("\nFile transfer completed.");

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
    memcpy ((machineGlobalsBlock->local_buffer + (3 * length_tmp)), motorBlockA, length_tmp);
    memcpy ((machineGlobalsBlock->local_buffer + (4 * length_tmp)), motorBlockB, length_tmp);
    memcpy ((machineGlobalsBlock->local_buffer + (5 * length_tmp)), motorBlockC, length_tmp);
    memcpy ((machineGlobalsBlock->local_buffer + (6 * length_tmp)), motorBlockD, length_tmp);
    memcpy ((machineGlobalsBlock->local_buffer + (7 * length_tmp)), motorBlockT, length_tmp);

//    printf ("\nWriting INI data...");
    // Write the file in blocks
    fx_return = fx_file_write (&machineGlobalsBlock->iniFile, machineGlobalsBlock->local_buffer, (8 * length_tmp));
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
    memset (machineGlobalsBlock->local_buffer, 0, (8 * length_tmp));

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

    fx_return = fx_file_read (&machineGlobalsBlock->iniFile, machineGlobalsBlock->local_buffer, (8 * length_tmp),
                              &actual_length);
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
    memcpy (motorBlockA, (machineGlobalsBlock->local_buffer + (3 * length_tmp)), length_tmp);
    memcpy (motorBlockB, (machineGlobalsBlock->local_buffer + (4 * length_tmp)), length_tmp);
    memcpy (motorBlockC, (machineGlobalsBlock->local_buffer + (5 * length_tmp)), length_tmp);
    memcpy (motorBlockD, (machineGlobalsBlock->local_buffer + (6 * length_tmp)), length_tmp);
    memcpy (motorBlockT, (machineGlobalsBlock->local_buffer + (7 * length_tmp)), length_tmp);

    if (DEBUGGER)
        printf ("\nINI data loaded.");
///Clear local buffer to prevent errors with other USB operations
    memset (machineGlobalsBlock->local_buffer, 0, (8 * length_tmp));
}
