#include "UARTWIFI.h"
#include <Titan.h>

/* UARTWIFI entry function */
void UARTWIFI_entry(void)
{
    /* TODO: add your own code here */
    initialise_monitor_handles ();
    while (1)
    {
//        UCHAR uartRX[11];
//        static uart_cfg_t rx_uart_config;
//        g_sf_comms0.p_api->open (g_sf_comms0.p_ctrl, g_sf_comms0.p_cfg);
//
//        g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, uartRX, 10, TX_WAIT_FOREVER);
//        printf ("\n%s", uartRX);
//        serialHandler (uartRX);

        tx_thread_relinquish ();
    }
}

///This function receives files from the GUI via UART or USB
//void rxFile(long fileSize)
//{
//    UINT status;
//    int i, parts;
//    machineGlobalsBlock->fileTransferIndex = 0;
////    long fileSize;
//    parts = fileSize / 1000;
//
////    printf ("\n%s", test);
//
//    long remainder = fileSize % 1000;
//    printf ("\nsize: %lu remainder:%lu", fileSize, remainder);
//
////    fx_file_op
//
//    status = fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
//
//    if (status == FX_SUCCESS)
//    {
//        fx_file_delete (&g_fx_media0, "testGcode.gcode");
//
//        fx_file_create (&g_fx_media0, "testGcode.gcode");
//
//        fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
//    }
//    else
//    {
//        fx_file_create (&g_fx_media0, "testGcode.gcode");
//
//        fx_file_open(&g_fx_media0, &machineGlobalsBlock->gcodeFile, "testGcode.gcode", FX_OPEN_FOR_WRITE);
//
//    }
//
//    for (i = 0; i < parts; i++)
//    {
//        g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, machineGlobalsBlock->storageBuffer, 1000, 250);
//
//        fx_file_write (&machineGlobalsBlock->gcodeFile, machineGlobalsBlock->storageBuffer, 1000);
////        machineGlobalsBlock->fileTransferIndex += 1000;
//    }
//
//    ///Read and store remainder
//
//    g_sf_comms0.p_api->read (g_sf_comms0.p_ctrl, machineGlobalsBlock->storageBuffer, remainder, 250);
//    fx_file_write (&machineGlobalsBlock->gcodeFile, machineGlobalsBlock->storageBuffer, remainder);
//
//    fx_media_flush(&g_fx_media0);
//
//}
