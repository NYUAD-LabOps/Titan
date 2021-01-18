#include "UARTWIFI.h"

/* UARTWIFI entry function */
void UARTWIFI_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        char test[150];
        static uart_cfg_t rx_uart_config;
        g_sf_comms0.p_api->open(g_sf_comms0.p_ctrl, g_sf_comms0.p_cfg);

        g_sf_comms0.p_api->read(g_sf_comms0.p_ctrl, test, 125, TX_WAIT_FOREVER);
        printf("\n%s", test);
        tx_thread_sleep (1);
    }
}
