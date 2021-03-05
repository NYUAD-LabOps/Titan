/* generated thread source file - do not edit */
#include "TitanMain.h"

TX_THREAD TitanMain;
void TitanMain_create(void);
static void TitanMain_func(ULONG thread_input);
static uint8_t TitanMain_stack[2048] BSP_PLACE_IN_SECTION_V2(".stack.TitanMain") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
TX_EVENT_FLAGS_GROUP g_linked_list_flags;
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void TitanMain_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */
    UINT err_g_linked_list_flags;
    err_g_linked_list_flags = tx_event_flags_create (&g_linked_list_flags, (CHAR *) "List Events");
    if (TX_SUCCESS != err_g_linked_list_flags)
    {
        tx_startup_err_callback (&g_linked_list_flags, 0);
    }

    UINT err;
    err = tx_thread_create (&TitanMain, (CHAR *) "TitanMain", TitanMain_func, (ULONG) NULL, &TitanMain_stack, 2048, 2,
                            2, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&TitanMain, 0);
    }
}

static void TitanMain_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    TitanMain_entry ();
}
