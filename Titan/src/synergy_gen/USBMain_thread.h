/* generated thread header file - do not edit */
#ifndef USBMAIN_THREAD_H_
#define USBMAIN_THREAD_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
extern "C" void USBMain_thread_entry(void);
#else
extern void USBMain_thread_entry(void);
#endif
#ifdef __cplusplus
extern "C"
{
#endif
extern TX_SEMAPHORE g_cdc_activate_semaphore;
extern TX_EVENT_FLAGS_GROUP g_usb_plug_events;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* USBMAIN_THREAD_H_ */
