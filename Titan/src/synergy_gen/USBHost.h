/* generated thread header file - do not edit */
#ifndef USBHOST_H_
#define USBHOST_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
extern "C" void USBHost_entry(void);
#else
extern void USBHost_entry(void);
#endif
#ifdef __cplusplus
extern "C"
{
#endif
extern TX_EVENT_FLAGS_GROUP g_usb_plug_events;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* USBHOST_H_ */
