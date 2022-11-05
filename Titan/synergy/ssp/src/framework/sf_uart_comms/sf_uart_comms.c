/***********************************************************************************************************************
 * Copyright [2015-2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/*************************************************************************//*******************************************
 * File Name    : sf_uart_comms.c
 * Description  : SSP UART framework driver API definition
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
/*LDRA_NOANALYSIS tx_api.h is not maintained by Renesas, so LDRA analysis is skipped for this file only. */
#include "tx_api.h"
/*LDRA_ANALYSIS */
#include "sf_uart_comms.h"
#include "sf_uart_comms_private_api.h"
#include "sf_uart_comms_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#ifndef SF_UART_COMMS_ERROR_RETURN
/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
#define SF_UART_COMMS_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), &g_module_name[0], &module_version)
#endif

#ifndef SF_UART_COMMS_ERROR_LOG
/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
#define SF_UART_COMMS_ERROR_LOG(err) SSP_ERROR_LOG((err),  &g_module_name[0], &module_version)
#endif

#define UART_EVENT_ERR_INTERNAL (1U << 15)

/** UART event definition for data reception and errors */
#define UART_EVENT_RECEPTION                                                        \
    ((ULONG) UART_EVENT_RX_COMPLETE | (ULONG) UART_EVENT_ERR_OVERFLOW |             \
     (ULONG) UART_EVENT_ERR_FRAMING | (ULONG) UART_EVENT_BREAK_DETECT |             \
     (ULONG) UART_EVENT_ERR_PARITY  | (ULONG) UART_EVENT_RX_CHAR      |             \
     (ULONG) UART_EVENT_ERR_INTERNAL)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/* Callback functions for SSP framework */
static void sf_uart_comms_callback (uart_callback_args_t * parg);
static ssp_err_t sf_uart_comms_receive_chars(sf_uart_comms_instance_ctrl_t * p_ctrl, uint8_t * const p_dest,
                                   uint32_t const bytes, uint32_t * p_bytes_remaining);
static ssp_err_t sf_uart_comms_tx_resource_setup(sf_uart_comms_instance_ctrl_t * p_ctrl);
static void sf_uart_comms_tx_resource_release(sf_uart_comms_instance_ctrl_t * p_ctrl);
static ssp_err_t sf_uart_comms_check_events(sf_uart_comms_instance_ctrl_t * p_ctrl,
                         uint8_t * const         p_dest,
                         uint32_t const          byte,
                         UINT const          timeout);
static ssp_err_t sf_uart_comms_check_events_sub(ULONG actual_flags_sub);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char g_module_name[] = "sf_uart_comms";
#endif

#if defined(__GNUC__)
/* This structure is affected by warnings from a GCC compiler bug.  This pragma suppresses the warnings in this
 * structure only.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** SSP UART module version data structure */
static const ssp_version_t module_version =
{
    .api_version_minor  = SF_COMMS_API_VERSION_MINOR,
    .api_version_major  = SF_COMMS_API_VERSION_MAJOR,
    .code_version_minor = SF_UART_COMMS_CODE_VERSION_MINOR,
    .code_version_major = SF_UART_COMMS_CODE_VERSION_MAJOR,
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/***********************************************************************************************************************
 * Implementation of Interface
 **********************************************************************************************************************/
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const sf_comms_api_t g_sf_comms_on_sf_uart_comms =
{
    .open          = SF_UART_COMMS_Open,
    .close         = SF_UART_COMMS_Close,
    .read          = SF_UART_COMMS_Read,
    .write         = SF_UART_COMMS_Write,
    .lock          = SF_UART_COMMS_Lock,
    .unlock        = SF_UART_COMMS_Unlock,
    .versionGet    = SF_UART_COMMS_VersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup SF_UART_COMMS
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/******************************************************************************************************************//**
 * @brief   Open the UART for communication.
 *
 * @param[in,out]     p_api_ctrl         Pointer to the UART control block
 * @param[in]         p_cfg              Pointer to the configuration structure
 * @retval  SSP_SUCCESS                  Channel opened successfully.
 * @retval  SSP_ERR_IN_USE               Channel already in use.
 * @retval  SSP_ERR_ASSERTION            Parameter check failed for one of the  following:
 *                                       - Pointer p_api_ctrl is NULL.
 *                                       - Pointer p_cfg is NULL
 *                                       - Pointer p_cfg->p_extend is NULL
 *                                       - Pointer p_cfg_extend->p_lower_lvl_uart is NULL
 *                                       - Pointer p_cfg_extend->p_lower_lvl_uart->p_api->open is NULL
 *                                       - Pointer p_cfg_extend->p_lower_lvl_uart->p_cfg is NULL
 * @retval  SSP_ERR_INTERNAL             An internal ThreadX error has occurred. This is typically a failure
 *                                       to create/use thread mutex or failure create/use event flags or queue.
 * @return                               See @ref Common_Error_Codes or functions called by this function for other possible
 *                                       codes. This function calls:
 *                                       * uart_api_t::open
 *
 * @note This function is reentrant for any channel. Handle must be cleared by caller before calling this function.
 **********************************************************************************************************************/
ssp_err_t SF_UART_COMMS_Open (sf_comms_ctrl_t * const p_api_ctrl, sf_comms_cfg_t const * const p_cfg)
{
    sf_uart_comms_instance_ctrl_t * p_ctrl = (sf_uart_comms_instance_ctrl_t *) p_api_ctrl;
    ssp_err_t  err;
    uart_cfg_t tmp_lower_lvl_cfg;

    /**  Checks error. Further parameter checking can be done at the driver layer. */
#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_cfg);
#endif

    sf_uart_comms_cfg_t *  p_cfg_extend  = (sf_uart_comms_cfg_t *) p_cfg->p_extend;

#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_cfg_extend);
    SSP_ASSERT(NULL != p_cfg_extend->p_lower_lvl_uart->p_api);
    SSP_ASSERT(NULL != p_cfg_extend->p_lower_lvl_uart->p_api->open);
    SSP_ASSERT(NULL != p_cfg_extend->p_lower_lvl_uart->p_cfg);
#endif

    p_ctrl->p_lower_lvl_uart = p_cfg_extend->p_lower_lvl_uart;

    SF_UART_COMMS_ERROR_RETURN((SF_UART_COMMS_STATE_CLOSED == p_ctrl->state), SSP_ERR_IN_USE);

    /** Initialize and start ThreadX resources */
    err =  sf_uart_comms_tx_resource_setup(p_ctrl);
    if (SSP_SUCCESS != err)
    {
        sf_uart_comms_tx_resource_release(p_ctrl);
        return err;
    }

    /* Temporary copy config parameters during open processing */
    memcpy(&tmp_lower_lvl_cfg, p_cfg_extend->p_lower_lvl_uart->p_cfg, sizeof(uart_cfg_t));

    tmp_lower_lvl_cfg.p_callback = sf_uart_comms_callback;               /* set SSP framework level callback function */

    /* Save context for use in ISRs */
    tmp_lower_lvl_cfg.p_context = (void *) p_ctrl;

    /**  Calls open function of UART HAL driver */
    err = p_cfg_extend->p_lower_lvl_uart->p_api->open(p_ctrl->p_lower_lvl_uart->p_ctrl, &tmp_lower_lvl_cfg);
    if (SSP_SUCCESS != err)
    {
        sf_uart_comms_tx_resource_release(p_ctrl);
        return err;
    }

    p_ctrl->state           = SF_UART_COMMS_STATE_OPENED;

    return SSP_SUCCESS;
}  /* End of function SF_UART_COMMS_Open() */

/******************************************************************************************************************//**
 * @brief   Close the UART Channel and clean up the resources.
 *
 * @param[in]     p_api_ctrl     Pointer to the UART control block
 * @retval SSP_SUCCESS           UART channel is successfully closed.
 * @retval SSP_ERR_ASSERTION     Parameter check failed for one of the  following:
 *                               - Pointer p_api_ctrl is NULL.
 *                               - Pointer p_ctrl->p_lower_lvl_uart is NULL
 *                               - Pointer p_ctrl->p_lower_lvl_uart->p_api is NULL
 *                               - Pointer p_ctrl->p_lower_lvl_uart->p_api->close is NULL
 * @retval SSP_ERR_NOT_OPEN      Channel is not opened.
 * @note This function is reentrant for any channel.
 **********************************************************************************************************************/
ssp_err_t SF_UART_COMMS_Close (sf_comms_ctrl_t * const p_api_ctrl)
{
    sf_uart_comms_instance_ctrl_t * p_ctrl = (sf_uart_comms_instance_ctrl_t *) p_api_ctrl;

    /**  Checks error. Further parameter checking can be done at the driver layer. */
#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_ctrl->p_lower_lvl_uart);
    SSP_ASSERT(NULL != p_ctrl->p_lower_lvl_uart->p_api);
    SSP_ASSERT(NULL != p_ctrl->p_lower_lvl_uart->p_api->close);
#endif

    SF_UART_COMMS_ERROR_RETURN((SF_UART_COMMS_STATE_CLOSED != p_ctrl->state), SSP_ERR_NOT_OPEN);

    /**  Calls close function of UART HAL driver */
    p_ctrl->p_lower_lvl_uart->p_api->close(p_ctrl->p_lower_lvl_uart->p_ctrl);

    /** Release ThreadX resources */
    sf_uart_comms_tx_resource_release(p_ctrl);

    p_ctrl->state = SF_UART_COMMS_STATE_CLOSED;

    return SSP_SUCCESS;
}  /* End of function SF_UART_COMMS_Close() */

/******************************************************************************************************************//**
 * @brief  Read user specified number of bytes into destination buffer pointer.
 *
 * @param[in]     p_api_ctrl            Pointer to the UART control block
 * @param[in]     bytes                 No.of bytes to be read
 * @param[in]     timeout               timeout for read
 * @param[out]    p_dest                Destination buffer
 * @retval SSP_SUCCESS                  Data reception ends successfully.
 * @retval SSP_ERR_NOT_OPEN             Channel is not opened.
 * @retval SSP_ERR_HW_LOCKED            Channel is locked.
 * @retval SSP_ERR_ASSERTION            Pointer to UART control block/pointer to destination address is NULL.
 * @retval SSP_ERR_INVALID_MODE         Channel is used for non-UART mode.
 * @retval SSP_ERR_INSUFFICIENT_DATA    Not enough data in receive circular buffer.
 * @retval SSP_ERR_OVERFLOW             Hardware overflow.
 * @retval SSP_ERR_FRAMING              Framing error.
 * @retval SSP_ERR_PARITY               Parity error.
 * @retval SSP_ERR_BREAK_DETECT         Break signal detected.
 * @retval SSP_ERR_TIMEOUT              One of the following operation timed out.
 *                                      - 'Event flags get' timed out
 *                                      - 'Receive mutex get' timed out
 * @retval SSP_ERR_INTERNAL             An internal ThreadX error has occurred. This is typically a failure
 *                                      to create/use thread mutex or failure create/use event flags or queue.
 * @return                              See @ref Common_Error_Codes or functions called by this function for other possible
 *                                      return codes. This function calls:
 *                                      * uart_api_t::read
 *                                      * uart_api_t::communicationAbort
 **********************************************************************************************************************/
ssp_err_t SF_UART_COMMS_Read (sf_comms_ctrl_t * const p_api_ctrl,
                         uint8_t * const         p_dest,
                         uint32_t const          bytes,
                         UINT const          timeout)
{
    sf_uart_comms_instance_ctrl_t * p_ctrl = (sf_uart_comms_instance_ctrl_t *) p_api_ctrl;
    UINT      status = TX_SUCCESS;
    ssp_err_t err    = SSP_SUCCESS;
    uint32_t  bytes_remaining = bytes;

    /* Checks error. Further parameter checking can be done at the driver layer. */
#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_dest);
#endif

    SF_UART_COMMS_ERROR_RETURN((SF_UART_COMMS_STATE_CLOSED != p_ctrl->state), SSP_ERR_NOT_OPEN);
  
    /** Locks the UART reception hardware resource */
    status = tx_mutex_get(&p_ctrl->mutex[1], timeout);
    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == status), SSP_ERR_TIMEOUT);

    p_ctrl->state = SF_UART_COMMS_STATE_READING;

    /* Read the data from the read queue using the UART_EVENT_RX_CHAR event */
    err = sf_uart_comms_check_events(p_ctrl,p_dest,bytes_remaining,timeout);

    /** Unlock the UART reception hardware resource */
    /* The return code is not checked here because tx_mutex_put cannot fail when called with a mutex owned by the
     * current thread. The mutex is owned by the current thread because this call follows a successful call to
     * tx_mutex_get. */
    tx_mutex_put(&p_ctrl->mutex[1]);

    /* Return error only after mutex is released. */
    SF_UART_COMMS_ERROR_RETURN((SSP_SUCCESS == err), err);

    p_ctrl->state = SF_UART_COMMS_STATE_OPENED;

    return err;
}  /* End of function SF_UART_COMMS_Read() */

/******************************************************************************************************************//**
 * @brief  Write user specified number of bytes from the source buffer.
 *
 * @param[in]     p_api_ctrl            Pointer to the UART control block
 * @param[in]     bytes                 Number of bytes to be written.
 * @param[in]     timeout               Timeout for write.  This timeout must be long enough for the write to complete.
 *                                      A timeout of 0 or TX_NO_WAIT results in a return value of SSP_ERR_TIMEOUT.
 * @param[out]    p_src                 Source buffer
 *
 * @retval SSP_SUCCESS                  Data transmission finished successfully.
 * @retval SSP_ERR_ASSERTION            Pointer to UART control block is NULL.
 *                                      Pointer to source buffer is NULL.
 * @retval SSP_ERR_NOT_OPEN             Channel is not opened.
 * @retval SSP_ERR_INVALID_MODE         Channel is used for non-UART mode or illegal mode is set in handle.
 * @retval SSP_ERR_INSUFFICIENT_SPACE   Not enough space in transmission circular buffer.
 * @retval SSP_ERR_HW_LOCKED            Could not lock hardware.
 * @retval SSP_ERR_TIMEOUT              'Transmit mutex get' timed out
 * @retval SSP_ERR_INTERNAL             An internal ThreadX error has occurred. This is typically a failure
 *                                      to create/use thread mutex or failure create/use event flags or queue.
 * @return                              See @ref Common_Error_Codes or functions called by this function for other possible
 *                                      return codes. This function calls:
 *                                      * uart_api_t::write
 *                                      * uart_api_t::communicationAbort
 **********************************************************************************************************************/
ssp_err_t SF_UART_COMMS_Write (sf_comms_ctrl_t * const p_api_ctrl,
                          uint8_t const * const   p_src,
                          uint32_t const          bytes,
                          UINT const          timeout)
{
    sf_uart_comms_instance_ctrl_t * p_ctrl = (sf_uart_comms_instance_ctrl_t *) p_api_ctrl;
    UINT      status = TX_SUCCESS;
    ssp_err_t err    = SSP_SUCCESS;
    ULONG     actual_flags = 0U;

    /*  Checks error. Further parameter checking can be done at the driver layer. */
#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_src);
#endif

    SF_UART_COMMS_ERROR_RETURN((SF_UART_COMMS_STATE_CLOSED != p_ctrl->state), SSP_ERR_NOT_OPEN);

    /**  Locks the UART transmission hardware resource */
    status = tx_mutex_get(&p_ctrl->mutex[0], timeout);
    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == status), SSP_ERR_TIMEOUT);

    p_ctrl->state = SF_UART_COMMS_STATE_WRITING;

    /**  Clear the event flag. */
    /* The return code is not checked here because tx_event_flags_get returns from the service
     * without waiting for the status of actual flag. This is to clear event flag. */
    tx_event_flags_get(&p_ctrl->eventflag[0], UART_EVENT_TX_COMPLETE, TX_OR_CLEAR,
                                &actual_flags, TX_NO_WAIT);

    /**  Calls write function of UART HAL driver */
    err = p_ctrl->p_lower_lvl_uart->p_api->write(p_ctrl->p_lower_lvl_uart->p_ctrl, p_src, bytes);

    if (SSP_SUCCESS == err)
    {
        /**  Wait until write operation is completed. Event is signaled in event flag object */
        status = tx_event_flags_get(&p_ctrl->eventflag[0], UART_EVENT_TX_COMPLETE, TX_OR_CLEAR,
                                    &actual_flags, timeout);
        if (TX_SUCCESS != status)
        {
            if (TX_NO_EVENTS == status)
            {
                /**  Calls communicationAbort function of UART HAL driver to abort write operation. */
                err = p_ctrl->p_lower_lvl_uart->p_api->communicationAbort(p_ctrl->p_lower_lvl_uart->p_ctrl,
                                                                          UART_DIR_TX);
                if (SSP_SUCCESS == err)
                {
                    err = SSP_ERR_TIMEOUT;
                }
            }
            else
            {
                err = SSP_ERR_INTERNAL;
            }
        }
    }

    /**  Unlock the UART transmission hardware resource */
    /* The return code is not checked here because tx_mutex_put cannot fail when called with a mutex owned by the
     * current thread. The mutex is owned by the current thread because this call follows a successful call to
     * tx_mutex_get. */
    tx_mutex_put(&p_ctrl->mutex[0]);
    SF_UART_COMMS_ERROR_RETURN((SSP_SUCCESS == err), err);

    p_ctrl->state = SF_UART_COMMS_STATE_OPENED;

    return err;
}  /* End of function SF_UART_COMMS_Write() */

/******************************************************************************************************************//**
 * @brief  Lock the UART resource
 *
 * @param[in]     p_api_ctrl    Pointer to the UART control block
 * @param[in]     lock_type     Type of Lock.
 * @param[in]     timeout       timeout for lock.
 * @retval SSP_SUCCESS          Locking UART resource successful.
 * @retval SSP_ERR_ASSERTION    Pointer to UART control block is NULL.
 * @retval SSP_ERR_NOT_OPEN     Channel is not opened.
 * @retval SSP_ERR_TIMEOUT      Timeout Error.
 *                              'Receive mutex get' timed out
 *                              'Transmit mutex get' timed out
 *
 **********************************************************************************************************************/
ssp_err_t SF_UART_COMMS_Lock (sf_comms_ctrl_t * const p_api_ctrl, sf_comms_lock_t lock_type, UINT timeout)
{
    sf_uart_comms_instance_ctrl_t * p_ctrl = (sf_uart_comms_instance_ctrl_t *) p_api_ctrl;

#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif

    SF_UART_COMMS_ERROR_RETURN((SF_UART_COMMS_STATE_CLOSED != p_ctrl->state), SSP_ERR_NOT_OPEN);

    UINT tx_err;

    /** Get both lock if requested, else get the lock type requested */
    if (SF_COMMS_LOCK_ALL  == lock_type)
    {
        tx_err = tx_mutex_get(&p_ctrl->mutex[0], timeout);
        if (TX_SUCCESS == tx_err)
        {
            tx_err = tx_mutex_get(&p_ctrl->mutex[1], timeout);

            if (TX_SUCCESS != tx_err)
            {
                /* The return code is not checked here because tx_mutex_put cannot fail when called with a mutex owned by the
                 * current thread. The mutex is owned by the current thread because this call follows a successful call to
                 * tx_mutex_get. */
                tx_mutex_put(&p_ctrl->mutex[0]);
            }
        }
    }
    else
    {
        tx_err = tx_mutex_get(&p_ctrl->mutex[lock_type], timeout);
    }
    SF_UART_COMMS_ERROR_RETURN(TX_SUCCESS == tx_err, SSP_ERR_TIMEOUT);

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief  UnLock the UART resource
 *
 * @param[in]     p_api_ctrl    Pointer to the UART control block
 * @param[in]     lock_type     Type of Lock.
 * @retval SSP_SUCCESS          Unlocking UART resource successful.
 * @retval SSP_ERR_ASSERTION    Pointer to UART control block is NULL.
 * @retval SSP_ERR_NOT_OPEN     Channel is not opened.
 * @retval SSP_ERR_INTERNAL     Failed to release the mutex.
 *
 **********************************************************************************************************************/
ssp_err_t SF_UART_COMMS_Unlock (sf_comms_ctrl_t * const p_api_ctrl, sf_comms_lock_t lock_type)
{
    sf_uart_comms_instance_ctrl_t * p_ctrl = (sf_uart_comms_instance_ctrl_t *) p_api_ctrl;

#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
#endif

    SF_UART_COMMS_ERROR_RETURN((SF_UART_COMMS_STATE_CLOSED != p_ctrl->state), SSP_ERR_NOT_OPEN);

    UINT tx_err_rx = TX_SUCCESS;
    UINT tx_err_tx = TX_SUCCESS;
    if ((SF_COMMS_LOCK_TX == lock_type) || (SF_COMMS_LOCK_ALL == lock_type))
    {
        tx_err_tx = tx_mutex_put(&p_ctrl->mutex[0]);
    }

    if ((SF_COMMS_LOCK_RX == lock_type) || (SF_COMMS_LOCK_ALL == lock_type))
    {
        tx_err_rx = tx_mutex_put(&p_ctrl->mutex[1]);
    }

    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == tx_err_tx) && (TX_SUCCESS == tx_err_rx), SSP_ERR_INTERNAL);

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @retval SSP_SUCCESS           Version number obtained successfully.
 * @retval SSP_ERR_ASSERTION     Pointer to version is NULL
 **********************************************************************************************************************/
ssp_err_t SF_UART_COMMS_VersionGet (ssp_version_t * const p_version)
{
#if SF_UART_COMMS_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = module_version.version_id;

    return SSP_SUCCESS;
}  /* End of function SF_UART_COMMS_VersionGet() */

/*******************************************************************************************************************//**
 * @} (end addtogroup SF_UART_COMMS)
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/******************************************************************************************************************//**
 *@brief  This is a helper routine called by sf_uart_comms_check_events to receive all characters in the queue
 *
 *@param[in]     p_ctrl              Pointer to the UART control block.
 *@param[in]     bytes               No. of bytes to read
 *@param[in]     p_bytes_remaining   No of remaining bytes to be received from the queue.
 *@param[out]    p_dest              Destination buffer.
 *
 *@retval SSP_SUCCESS                All characters from the queue are received successfully.
 *@retval SSP_ERR_INTERNAL           An internal ThreadX error has occurred.This is typically a failure create/use
 *                                   event flags or queue.
 **********************************************************************************************************************/
/** Receive all characters in the queue. */
static ssp_err_t sf_uart_comms_receive_chars(sf_uart_comms_instance_ctrl_t * p_ctrl, uint8_t * const p_dest,
        uint32_t const bytes, uint32_t * p_bytes_remaining)
{
    UINT status = TX_SUCCESS;
    uint32_t data = 0U;

    while ((TX_SUCCESS == status) && (*p_bytes_remaining > 0U))
    {
        /* Receive all data from the queue. */
        status = tx_queue_receive(&p_ctrl->queue, &data, TX_NO_WAIT);
        if (TX_SUCCESS == status)
        {
            /* 9 bit data mode of operation */
            if (UART_DATA_BITS_9 == p_ctrl->p_lower_lvl_uart->p_cfg->data_bits)
            {
                p_dest[(bytes - *p_bytes_remaining) * 2U] = (uint8_t)(data & 0xFF);
                p_dest[((bytes - *p_bytes_remaining) * 2U) + 1U] = (uint8_t)((data >> 8U) & 0xFF);
            }
            else
            {
                /* 7 or 8 bit data mode of operation */
                p_dest[bytes - *p_bytes_remaining] = (uint8_t) data;
            }
            *p_bytes_remaining = *p_bytes_remaining - 1U;
        }
    }

    SF_UART_COMMS_ERROR_RETURN(((TX_SUCCESS == status) || (TX_QUEUE_EMPTY == status)), SSP_ERR_INTERNAL);
    return SSP_SUCCESS;
}
/*******************************************************************************************************************//**
 * @brief This is a helper routine called by read API to check the events received.
 *
 * @param[in]     p_ctrl          Pointer to the UART control block.
 * @param[in]     bytes           No. of bytes to be read.
 * @param[in]     timeout         Timeout for reading the Events. This timeout must be long enough for the
 *                                Event flags to get updated.
 * @param[out]    p_dest          Destination buffer.
 *
 * @retval SSP_SUCCESS            ALL received events are verified successfully
 * @retval SSP_ERR_TIMEOUT        Return timeout error when read is aborted.
 * @retval SSP_ERR_INTERNAL       An internal ThreadX error has occurred.This is typically a failure create/use
 *                                event flags or queue.
 **********************************************************************************************************************/
static ssp_err_t sf_uart_comms_check_events(sf_uart_comms_instance_ctrl_t * p_ctrl,
                              uint8_t * const         p_dest,
                              uint32_t const          bytes,
                              UINT const          timeout)
{

    ULONG     actual_flags = 0U;
    uint32_t  bytes_remaining = bytes;
    UINT      status = TX_SUCCESS;
    ssp_err_t err    = SSP_SUCCESS;

    /* Check to see if characters are already in the queue. */
    sf_uart_comms_receive_chars(p_ctrl, p_dest, bytes, &bytes_remaining);

    /** Wait until read operation is completed. Event is signaled in event flag object. */
    while (bytes_remaining > 0U)
    {
        status = tx_event_flags_get(&p_ctrl->eventflag[1], UART_EVENT_RECEPTION,
                TX_OR_CLEAR, &actual_flags, timeout);

        if (TX_SUCCESS != status)
        {
            if (TX_NO_EVENTS == status)
            {
                /**  Calls communicationAbort function of UART HAL driver to abort read operation. */
                err = p_ctrl->p_lower_lvl_uart->p_api->communicationAbort(p_ctrl->p_lower_lvl_uart->p_ctrl,
                                                                          UART_DIR_RX);
                if (SSP_SUCCESS == err)
                {
                    err = SSP_ERR_TIMEOUT;
                }
            }
            else
            {
                err = SSP_ERR_INTERNAL;
            }
            return err;
        }

        /* Check which event(s) occurred. */
        /* Check for errors first. */
        err = sf_uart_comms_check_events_sub(actual_flags);

        if (SSP_SUCCESS != err)
        {
            return err;
        }

        /* Check to see if characters were received. */
        sf_uart_comms_receive_chars(p_ctrl, p_dest, bytes, &bytes_remaining);
    }

    return err;
}
/*******************************************************************************************************************//**
 * @brief This is a helper routine called by sf_uart_comms_check_events to check the error events received.
 *
 * @param[in]  actual_flags_sub   Holds status of all events.
 *
 * @retval SSP_SUCCESS            All events received successfully.
 * @retval SSP_ERR_OVERFLOW       FIFO Overflow error occurred.
 * @retval SSP_ERR_FRAMING        Mode fault error occurred.
 * @retval SSP_ERR_PARITY         Parity error occurred.
 * @retval SSP_ERR_BREAK_DETECT   Break Detect error occurred.
 * @retval SSP_ERR_INTERNAL       An internal ThreadX error has occurred.This is typically a failure create/use
 *                                event flags or queue.
 **********************************************************************************************************************/
static ssp_err_t sf_uart_comms_check_events_sub(ULONG actual_flags_sub)
{
    ssp_err_t err    = SSP_SUCCESS;
    if ((ULONG) UART_EVENT_RX_CHAR != actual_flags_sub)
    {
        ULONG test_flag = (ULONG) UART_EVENT_ERR_OVERFLOW;
        if ((test_flag & actual_flags_sub) > 0U)
        {
             err = SSP_ERR_OVERFLOW;
        }

        test_flag = (ULONG) UART_EVENT_ERR_FRAMING;
        if ((test_flag & actual_flags_sub) > 0U)
        {
            err = SSP_ERR_FRAMING;
        }

        test_flag = (ULONG) UART_EVENT_ERR_PARITY;
        if ((test_flag & actual_flags_sub) > 0U)
        {
            err = SSP_ERR_PARITY;
        }

        test_flag = (ULONG) UART_EVENT_BREAK_DETECT;
        if ((test_flag & actual_flags_sub) > 0U)
        {
            err = SSP_ERR_BREAK_DETECT;
        }

        test_flag = (ULONG) UART_EVENT_ERR_INTERNAL;
        if ((test_flag & actual_flags_sub) > 0U)
        {
            SF_UART_COMMS_ERROR_LOG(SSP_ERR_INTERNAL);
        }

    }
    return err;
}


/*******************************************************************************************************************//**
 * @brief This is a helper routine called by open API to initialize ThreadX resources.
 * @param[in] p_ctrl        - Pointer to a control structure allocated by user.
 **********************************************************************************************************************/

static ssp_err_t sf_uart_comms_tx_resource_setup(sf_uart_comms_instance_ctrl_t * p_ctrl)
{
    UINT status = (UINT) TX_SUCCESS;

    /**  Create UART mutexes and put it into UART context */
    status = tx_mutex_create(&p_ctrl->mutex[0], (CHAR *) "UART Write", TX_INHERIT);
    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == status), SSP_ERR_INTERNAL);

    status = tx_mutex_create(&p_ctrl->mutex[1], (CHAR *) "UART Read", TX_INHERIT);
    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == status), SSP_ERR_INTERNAL);

    /**  Create UART event flag and put it into UART context */
    status = tx_event_flags_create(&p_ctrl->eventflag[0], (CHAR *) "UART Write");
    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == status), SSP_ERR_INTERNAL);

    status = tx_event_flags_create(&p_ctrl->eventflag[1], (CHAR *) "UART Read");
    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == status), SSP_ERR_INTERNAL);

    /** Create queue to buffer read data. */
    status = tx_queue_create(&p_ctrl->queue, (CHAR *) "UART Read", 1,
                &p_ctrl->queue_mem, (uint32_t)SF_UART_COMMS_CFG_QUEUE_SIZE_WORDS * sizeof(uint32_t));
    SF_UART_COMMS_ERROR_RETURN((TX_SUCCESS == status), SSP_ERR_INTERNAL);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief This is a helper routine called by Close API to release ThreadX resources.
 * @param[in] p_ctrl        - Pointer to a control structure allocated by user.
 **********************************************************************************************************************/
static void sf_uart_comms_tx_resource_release(sf_uart_comms_instance_ctrl_t * p_ctrl)
{
    /* The return codes are not checked here because all the threadX API calls here
     * cannot fail when called with successfully created RTOS objects.
     * These objects were successfully created in open function. */
    /**  Delete the ThreadX objects. */

    /** Delete transmit mutex. */
    tx_mutex_delete(&p_ctrl->mutex[0]);

    /** Delete receive mutex. */
    tx_mutex_delete(&p_ctrl->mutex[1]);

    /** Delete event flags group. */
    tx_event_flags_delete(&p_ctrl->eventflag[0]);

    tx_event_flags_delete(&p_ctrl->eventflag[1]);

    /** Delete queue instance. */
    tx_queue_delete(&p_ctrl->queue);

}

/******************************************************************************************************************//**
 * @brief UART SSP framework level callback
 * @param[in]  pcb_arg            Pointer to callback parameters.
 **********************************************************************************************************************/
static void sf_uart_comms_callback (uart_callback_args_t * pcb_arg)
{
    sf_uart_comms_instance_ctrl_t * p_ctrl = (sf_uart_comms_instance_ctrl_t *) pcb_arg->p_context;
    UINT tx_err;

    /* Initialize to the transmit event flags group.  This will be used if the event is UART_EVENT_TX_COMPLETE. */
    TX_EVENT_FLAGS_GROUP          * p_flag = &p_ctrl->eventflag[0];

    if (UART_EVENT_TX_DATA_EMPTY == (uint32_t) pcb_arg->event)
    {
        /* The transmit data empty event is not handled in the framework. */
        return;
    }
    if (UART_EVENT_TX_COMPLETE != pcb_arg->event)
    {
        /* Reception complete event/Error event occurs, wake up the suspended thread.  */
        p_flag = &p_ctrl->eventflag[1];   /* Points location for event flag for reception */
        uint32_t test_flag = (uint32_t) UART_EVENT_RX_CHAR;
        if (test_flag & (uint32_t) pcb_arg->event)
        {
            tx_err = tx_queue_send(&p_ctrl->queue, &pcb_arg->data, TX_NO_WAIT);
            if(TX_SUCCESS != tx_err)
            {
              /* The return code is not checked here because tx_event_flags_set cannot fail when called
               * with valid input arguments. */
                tx_event_flags_set(p_flag, UART_EVENT_ERR_INTERNAL, TX_OR);
                return;
            }
        }
    }
    /* The return code is not checked here because tx_event_flags_set cannot fail when called with valid
     * input arguments. */
    tx_event_flags_set(p_flag, pcb_arg->event, TX_OR);
}  /* End of function sf_uart_comms_callback() */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_UART_COMMS_Library)
 **********************************************************************************************************************/

/* End of file */
