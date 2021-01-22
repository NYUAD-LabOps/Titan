/* generated thread source file - do not edit */
#include "USBHost.h"

TX_THREAD USBHost;
void USBHost_create(void);
static void USBHost_func(ULONG thread_input);
static uint8_t USBHost_stack[4096] BSP_PLACE_IN_SECTION_V2(".stack.USBHost") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_EVENT_FLAGS_GROUP g_usb_plug_events;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void USBHost_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_usb_plug_events;
    err_g_usb_plug_events = tx_event_flags_create (&g_usb_plug_events, (CHAR *) "USB Plug Event Flags");
    if (TX_SUCCESS != err_g_usb_plug_events)
    {
        tx_startup_err_callback (&g_usb_plug_events, 0);
    }

    UINT err;
    err = tx_thread_create (&USBHost, (CHAR *) "USBHost", USBHost_func, (ULONG) NULL, &USBHost_stack, 4096, 2, 2, 1,
                            TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&USBHost, 0);
    }
}

static void USBHost_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    USBHost_entry ();
}
