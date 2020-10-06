/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2012 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               www.expresslogic.com          */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/

/***********************************************************************************************************************
 * Copyright [2017] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   SYNERGY Controller Driver                                           */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_hcd_synergy.h"

/*******************************************************************************************************************//**
 * @addtogroup sf_el_ux
 * @{
 **********************************************************************************************************************/

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    ux_hcd_synergy_port_status_get                      PORTABLE C      */
/*                                                           5.6          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*     This function will return the status for each port attached to the */
/*     root HUB.                                                          */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    hcd_synergy                           Pointer to Synergy controller */
/*    port_index                            Port index                    */
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*  Synergy Port Status                                                   */
/*                                                                        */ 
/*    Where port status has the following format:                         */
/*                                                                        */ 
/*               bit 0         device connection status                   */
/*                             if 0 : no device connected                 */
/*                             if 1 : device connected to the port        */
/*               bit 1         port enable status                         */
/*                             if 0 : port disabled                       */
/*                             if 1 : port enabled                        */
/*               bit 2         port suspend status                        */
/*                             if 0 : port is not suspended               */
/*                             if 1 : port is suspended                   */
/*               bit 3         port overcurrent status                    */
/*                             if 0 : port has no overcurrent condition   */
/*                             if 1 : port has overcurrent condition      */
/*               bit 4         port reset status                          */
/*                             if 0 : port is not in reset                */
/*                             if 1 : port is in reset                    */
/*               bit 5         port power status                          */
/*                             if 0 : port power is off                   */
/*                             if 1 : port power is on                    */
/*               bit 6-7       device attached speed                      */
/*                             if 00 : low speed device attached          */
/*                             if 01 : full speed device attached         */
/*                             if 10 : high speed device attached         */
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    ux_hcd_synergy_register_read                Read Synergy register   */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Synergy Controller Driver                                           */
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  10-10-2012     TCRG                     Initial Version 5.6           */ 
/*                                                                        */ 
/**************************************************************************/ 

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  This function will return the status for each port attached to the
 *         root HUB.
 *
 * @param[in,out]  hcd_synergy : Pointer to a HCD control block
 * @param[in]      port_index  : Port Index
 *
 * @retval UX_PORT_INDEX_UNKNOWN   Invalid port.
 * @retval port_status             Synergy Port status
 **********************************************************************************************************************/
ULONG  ux_hcd_synergy_port_status_get(UX_HCD_SYNERGY *hcd_synergy, ULONG port_index)
{
    UX_HCD      * hcd;
    ULONG       synergy_register_port_status;
    ULONG       port_status;
    ULONG       speed;



    /* Check to see if this port is valid on this controller.  */
    if (hcd_synergy -> ux_hcd_synergy_nb_root_hubs < (UINT)port_index)
    {
        /* If trace is enabled, insert this event into the trace buffer.  */
        UX_TRACE_IN_LINE_INSERT(UX_TRACE_ERROR, UX_PORT_INDEX_UNKNOWN, port_index, 0, 0, UX_TRACE_ERRORS, 0, 0)

        return (UINT)UX_PORT_INDEX_UNKNOWN;
    }

    /* The port is valid, build the status mask for this port. This function
       returns a controller agnostic bit field.  */ 
    port_status =  0UL;

    /* Get the pointers to the generic HCD and synergy specific areas.  */
    hcd = hcd_synergy->ux_hcd_synergy_hcd_owner;

    /* Now check the low level filter.  */
    if (hcd_synergy -> ux_synergy_device_attached == (ULONG)UX_TRUE)
    {
        /* Get the Device State Control Register.  */
        synergy_register_port_status =  ux_hcd_synergy_register_read(hcd_synergy, (USHORT)UX_SYNERGY_HC_DVSTCTR0);

        /* Isolate speed.  */
        speed =  synergy_register_port_status & (ULONG)UX_SYNERGY_HC_DVSTCTR0_RHST;

        /* Detect Port Device Attached speed.   */
        switch(speed)
        {
            case (ULONG)UX_SYNERGY_HC_DVSTCTR0_SPEED_FULL:
                port_status |= (ULONG)UX_PS_DS_FS;
                break;

            case (ULONG)UX_SYNERGY_HC_DVSTCTR0_SPEED_HIGH:
                port_status |= (ULONG)UX_PS_DS_HS;
                break;

            default:
                port_status |= (ULONG)UX_PS_DS_LS;
                break;
        }      

        /*  Device connection detected .  */
        port_status |= (ULONG)UX_PS_CCS;

        /* Port Enabled.  */
        hcd_synergy -> ux_hcd_synergy_port_status = (ULONG)UX_SYNERGY_HC_PORT_ENABLED;

        /* Port Enable Status.  */
        if (synergy_register_port_status & (ULONG)UX_SYNERGY_HC_DVSTCTR0_UACT)
        {
            port_status |= (ULONG)UX_PS_PES;
        }

        /* Port Reset Status.  */
        if (synergy_register_port_status & (ULONG)UX_SYNERGY_HC_DVSTCTR0_USBRST)
        {
            port_status |= (ULONG)UX_PS_PRS;
        }
    }
    else
    {
        /* Check for port EOFERR. In this case, we report port disable to have the root
           hub remove the device, but we need to send another signal that will force
           a fake root hub insertion and a reset to the device.  */
        if (hcd_synergy -> ux_hcd_synergy_port_status == (ULONG)UX_SYNERGY_HC_PORT_INEOFERR)
        {

            /* Signal to the root hub thread a fake new device insertion.  */
            hcd -> ux_hcd_root_hub_signal[0]++;

            /* Something is attached now.  */
            hcd_synergy -> ux_synergy_device_attached = (ULONG)UX_TRUE;

            /* Reset the address of the device connected to the root hub.  */
            ux_hcd_synergy_register_set(hcd_synergy, UX_SYNERGY_HC_DCPMAXP, 0U);

            /* Update the device address in the HCD structure.  We will force a resetting later. */
            hcd_synergy -> ux_hcd_synergy_root_hub_device_address = 0xFFFFFFFFUL;

            /* We wake up the root hub thread to simulate device insertion.  */
            _ux_utility_semaphore_put(&_ux_system_host -> ux_system_host_enum_semaphore);

        }

        else
        {

            /* The port was not in EOFERR, it is just disabled.  */
            hcd_synergy -> ux_hcd_synergy_port_status = (ULONG)UX_SYNERGY_HC_PORT_DISABLED;
        }
    }

    /* Return port status.  */
    return (UINT)port_status;
}
/*******************************************************************************************************************//**
 * @} (end addtogroup sf_el_ux)
 **********************************************************************************************************************/
