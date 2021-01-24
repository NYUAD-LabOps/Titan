#include "UARTWIFI.h"
#include <Titan.h>

/* UARTWIFI entry function */
void UARTWIFI_entry(void)
{
    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }
    /* TODO: add your own code here */
    UCHAR uartRx[11];
    memset (uartRx, 0, 11);
    static uart_cfg_t rx_uart_config;
    g_sf_comms0.p_api->open (g_sf_comms0.p_ctrl, g_sf_comms0.p_cfg);

    while (1)
    {
        g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, uartRx, 10, TX_WAIT_FOREVER);
        printf ("\n%s", uartRx);
        serialHandler (uartRx);

        tx_thread_relinquish ();
    }
}

void serialHandler(char *uartRx)
{
    long long fileSize;
    fileSize = 0;
    switch (uartRx[0])
    {
        case 'F':
//            memcpy (&fileSize, (uartRx + 2), 8);
            fileSize = atoi((uartRx + 2));
            rxFile (fileSize);
        break;
    }
}

///This function receives files from the GUI via UART or USB
void rxFile(long long fileSize)
{
        UINT status;
        int i, parts;
        machineGlobalsBlock->local_bufferIndex = 0;
    //    long fileSize;

        parts = fileSize / 1024;

    //    printf ("\n%s", test);

        long remainder = fileSize % 1024;
        printf ("\nsize: %lu remainder:%lu", fileSize, remainder);

    //    fx_file_op

        status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);

        if (status == FX_SUCCESS)
        {
            fx_file_delete (&g_fx_media0, "testGcode.gcode");

            fx_file_create (&g_fx_media0, "testGcode.gcode");

            fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
        }
        else
        {
            fx_file_create (&g_fx_media0, "testGcode.gcode");

            fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);

        }

        for (i = 0; i < parts; i++)
        {
            g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, machineGlobalsBlock->local_buffer, 1024, 250);

            fx_file_write (&machineGlobalsBlock->gcodeFile, machineGlobalsBlock->local_buffer, 1024);
    //        machineGlobalsBlock->fileTransferIndex += 1000;
        }

        ///Read and store remainder

        g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, machineGlobalsBlock->local_buffer, remainder, 250);
        fx_file_write (&machineGlobalsBlock->gcodeFile, machineGlobalsBlock->local_buffer, remainder);

        fx_file_close(&machineGlobalsBlock->gcodeFile);
        fx_media_flush(&g_fx_media0);

        printf("\nFile transfer completed.");
        printf("\nFile transfer completed.");

}
