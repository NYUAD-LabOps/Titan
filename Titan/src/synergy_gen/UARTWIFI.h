/* generated thread header file - do not edit */
#ifndef UARTWIFI_H_
#define UARTWIFI_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void UARTWIFI_entry(void);
                #else
extern void UARTWIFI_entry(void);
#endif
#ifdef __cplusplus
extern "C" {
#endif
extern TX_SEMAPHORE g_cdc_activate_semaphore;
extern TX_EVENT_FLAGS_GROUP g_cdcacm_activate_event_flags0;
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* UARTWIFI_H_ */
