/* generated thread source file - do not edit */
#include "USBDevice.h"

TX_THREAD USBDevice;
void USBDevice_create(void);
static void USBDevice_func(ULONG thread_input);
static uint8_t USBDevice_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.USBDevice") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_SEMAPHORE g_cdc_activate_semaphore;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void USBDevice_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_cdc_activate_semaphore;
    err_g_cdc_activate_semaphore = tx_semaphore_create (&g_cdc_activate_semaphore, (CHAR *) "New Semaphore", 0);
    if (TX_SUCCESS != err_g_cdc_activate_semaphore)
    {
        tx_startup_err_callback (&g_cdc_activate_semaphore, 0);
    }

    UINT err;
    err = tx_thread_create (&USBDevice, (CHAR *) "USBDevice", USBDevice_func, (ULONG) NULL, &USBDevice_stack, 1024, 2,
                            2, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&USBDevice, 0);
    }
}

static void USBDevice_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    USBDevice_entry ();
}
