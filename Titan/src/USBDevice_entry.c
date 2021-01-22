#include "USBDevice.h"
#include <Titan.h>
#include "board_cfg.h"
#include "fx_api.h"
static UX_SLAVE_CLASS_CDC_ACM *g_cdc;
/* USBDevice entry function */
void USBDevice_entry(void)
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

    /* TODO: add your own code here */

    tx_thread_suspend (tx_thread_identify ());
    while (1)
    {
        memset (machineGlobalsBlock->USBBufferB, 0, 49);
        printf ("\nRead Device.");
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
            else if (machineGlobalsBlock->USBBufferB[0] == 'S' && machineGlobalsBlock->USBBufferB[1] == 'T'
                    && machineGlobalsBlock->USBBufferB[2] == 'P')
            {
                setupMode ();

                char tmpUSBOut[12];
                memset (tmpUSBOut, 0, 12);
                tmpUSBOut[0] = 'S';
                tmpUSBOut[1] = 'T';
                tmpUSBOut[2] = 'P';

                //                        snprintf (tmpUSBOut + 3, 8, "%f", toolBlockA->tempRead);
                status = _ux_device_class_cdc_acm_write (g_cdc, (UCHAR *) tmpUSBOut, 12, &actual_length);
                printf ("\nSetup Complete2.");
            }
            else
            {
                processReceivedMsg (machineGlobalsBlock->USBBufferB);
            }
        }
        else
        {

        }
        tx_thread_relinquish();
    }
}

void ux_cdc_device1_instance_activate(VOID *cdc_instance)
{
    /* Save the CDC instance.  */
    g_cdc = (UX_SLAVE_CLASS_CDC_ACM *) cdc_instance;
    tx_semaphore_put (&g_cdc_activate_semaphore);
}

void ux_cdc_device1_instance_deactivate(VOID *cdc_instance)
{
    SSP_PARAMETER_NOT_USED(cdc_instance);

    g_cdc = UX_NULL;
}
