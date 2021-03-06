#include "UARTWIFI.h"
#include <Titan.h>

/* UARTWIFI entry function */
void UARTWIFI_entry(void)
{
    UINT fx_return;
    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }
    /* TODO: add your own code here */
    UCHAR uartRx[WIFI_PACKET_SIZE];
    memset (uartRx, 0, WIFI_PACKET_SIZE);
    static uart_cfg_t rx_uart_config;
    g_sf_comms0.p_api->open (g_sf_comms0.p_ctrl, g_sf_comms0.p_cfg);



    while (1)
    {
        g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, uartRx, WIFI_PACKET_SIZE, TX_WAIT_FOREVER);
//        printf ("\n%s", uartRx);
        serialHandler (uartRx);

        ///Clear old data and relinquish.
        memset (uartRx, 0, WIFI_PACKET_SIZE);
        tx_thread_relinquish ();
    }
}

void serialHandler(char *uartRx)
{
    UINT status;
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
            g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, 250);
            processReceivedMsg (uartRx);
    }

}

///This function receives files from the GUI via UART or USB
void rxFile(long long fileSize)
{
    UINT status;
    int i, parts;
    UCHAR rxBuf[WIFI_PACKET_SIZE];
    UCHAR sendACK[WIFI_PACKET_SIZE];
    memset (sendACK, 0, WIFI_PACKET_SIZE);
    sendACK[0] = 'A';
    sendACK[1] = 'C';
    sendACK[2] = 'K';
    memset (rxBuf, 0, WIFI_PACKET_SIZE);
    machineGlobalsBlock->local_bufferIndex = 0;
    //    long fileSize;

    parts = fileSize / WIFI_PACKET_SIZE;

    //    printf ("\n%s", test);

    long remainder = fileSize % 1024;
    printf ("\nsize: %lu remainder:%lu", fileSize, remainder);

    //    fx_file_op

    status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);

    if (status == FX_SUCCESS)
    {
        printf ("\nFile open s");
        fx_file_close (&machineGlobalsBlock->gcodeFile);
        status = fx_file_delete (&g_fx_media0, "testGcode.gcode");
        if (status == FX_SUCCESS)
        {
            printf ("\nFile del s");
        }
        else
        {
            printf ("\nFile del f");
        }
        status = fx_file_create (&g_fx_media0, "testGcode.gcode");
        if (status == FX_SUCCESS)
        {
            printf ("\nFile create s");
        }
        else
        {
            printf ("\nFile create f");
        }
        status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
        if (status == FX_SUCCESS)
        {
            printf ("\nFile open s");
        }
        else
        {
            printf ("\nFile open f");
        }
    }
    else
    {
        printf ("\nFile open f");
        status = fx_file_create (&g_fx_media0, "testGcode.gcode");
        if (status == FX_SUCCESS)
        {
            printf ("\nFile create s");
        }
        else
        {
            printf ("\nFile create f");
        }
        status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
        if (status == FX_SUCCESS)
        {
            printf ("\nFile open s");
        }
        else
        {
            printf ("\nFile open f");
        }
    }

    g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, TX_NO_WAIT);

//    i = 0;
    while (rxBuf[0] != 'E' || rxBuf[1] != 'N' || rxBuf[2] != 'D')
    {
        status = g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, rxBuf, WIFI_PACKET_SIZE, 250);

        if (status == SSP_SUCCESS)
        {
            status = fx_file_write (&machineGlobalsBlock->gcodeFile, rxBuf, WIFI_PACKET_SIZE);
            if (status != FX_SUCCESS)
            {
                printf ("\nFile write f");
            }
//            i++;
//            printf("\n%d", i);
            g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, 250);
        }
        else if (status == 20)
        {
            printf ("\nTO");
//            g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, 250);
        }
        else
        {
            printf ("\nUART read error:%d", status);
        }

        //        machineGlobalsBlock->fileTransferIndex += 1000;
    }

    ///Send Final ACK
    g_sf_comms0.p_api->write (g_sf_comms0.p_ctrl, sendACK, WIFI_PACKET_SIZE, 250);
//    ///Read and store remainder
//
//    status = g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, rxBuf, WIFI_PACKET_SIZE, 250);
//    if (status == SSP_SUCCESS)
//    {
//        fx_file_write (&machineGlobalsBlock->gcodeFile, rxBuf, remainder);
//    }

    fx_media_flush (&g_fx_media0);
    fx_file_close (&machineGlobalsBlock->gcodeFile);

    printf ("\nFile transfer completed.");
    printf ("\nFile transfer completed.");

}
