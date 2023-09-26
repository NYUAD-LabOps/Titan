/* generated thread source file - do not edit */
#include "UARTWIFI.h"

TX_THREAD UARTWIFI;
void UARTWIFI_create(void);
static void UARTWIFI_func(ULONG thread_input);
static uint8_t UARTWIFI_stack[4096] BSP_PLACE_IN_SECTION_V2(".stack.UARTWIFI") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_SEMAPHORE g_cdc_activate_semaphore;
TX_EVENT_FLAGS_GROUP g_cdcacm_activate_event_flags0;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void UARTWIFI_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_cdc_activate_semaphore;
    err_g_cdc_activate_semaphore = tx_semaphore_create (&g_cdc_activate_semaphore, (CHAR*) "New Semaphore", 0);
    if (TX_SUCCESS != err_g_cdc_activate_semaphore)
    {
        tx_startup_err_callback (&g_cdc_activate_semaphore, 0);
    }
    UINT err_g_cdcacm_activate_event_flags0;
    err_g_cdcacm_activate_event_flags0 = tx_event_flags_create (&g_cdcacm_activate_event_flags0,
                                                                (CHAR*) "New Event Flags");
    if (TX_SUCCESS != err_g_cdcacm_activate_event_flags0)
    {
        tx_startup_err_callback (&g_cdcacm_activate_event_flags0, 0);
    }

    UINT err;
    err = tx_thread_create (&UARTWIFI, (CHAR*) "UARTWIFI", UARTWIFI_func, (ULONG) NULL, &UARTWIFI_stack, 4096, 1, 1, 1,
                            TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&UARTWIFI, 0);
    }
}

static void UARTWIFI_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    UARTWIFI_entry ();
}
