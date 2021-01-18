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
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "sf_uart_comms.h"
#ifdef __cplusplus
extern "C"
{
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer4;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart0;
#ifdef NULL
#else
extern void NULL(uint32_t channel, uint32_t level);
#endif
#ifndef NULL
void NULL(uart_callback_args_t *p_args);
#endif
/* UART Communications Framework Instance. */
extern const sf_comms_instance_t g_sf_comms0;
void g_sf_comms0_err_callback(void *p_instance, void *p_data);
void sf_comms_init0(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* UARTWIFI_H_ */
