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

#define UX_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_dcd_synergy.h"
#include "ux_device_stack.h"

/*******************************************************************************************************************//**
 * @addtogroup sf_el_ux
 * @{
 **********************************************************************************************************************/

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                                 RELEASE      */
/*                                                                        */
/*    ux_dcd_synergy_endpoint_stall                         PORTABLE C    */
/*                                                             5.6        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function will stall a physical endpoint.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    dcd_synergy                         Pointer to device controller    */
/*    endpoint                            Pointer to endpoint container   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */ 
/*                                                                        */
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    ux_dcd_synergy_register_read        Read register                   */
/*    ux_dcd_synergy_register_write       Write register                  */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    SYNERGY Controller Driver                                           */
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
 * @brief  This function will stall a physical endpoint.
 *
 * @param[in]  dcd_synergy     Pointer to a DCD control block
 * @param[in]  endpoint        Pointer to a Device Controller Endpoint structure.
 *
 * @retval UX_SUCCESS           Endpoint is stalled successfully.
 * @retval UX_NO_ED_AVAILABLE   Device Controller Endpoint control pointer is Null.
 **********************************************************************************************************************/
UINT  ux_dcd_synergy_endpoint_stall(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_ENDPOINT *endpoint)
{
    UX_DCD_SYNERGY_ED   * ed;
    if(endpoint == UX_NULL)
    {
        return (UINT)UX_NO_ED_AVAILABLE;
    }

    /* Get the physical endpoint address in the endpoint container.  */
    ed =  (UX_DCD_SYNERGY_ED *) endpoint -> ux_slave_endpoint_ed;

    /* Force STALL on this endpoint.  */
    switch (ed -> ux_dcd_synergy_ed_index)
    {
        case 0UL :
            /* Set STALL in DCPCTR register.  */
            ux_dcd_synergy_register_set(dcd_synergy, UX_SYNERGY_DCD_DCPCTR, UX_SYNERGY_DCD_DCPCTR_PIDSTALL);
            break;        
        
        default : 
            /* Set STALL in PIPExCTR register.  */
            ux_dcd_synergy_register_set(dcd_synergy,
                                        UX_SYNERGY_DCD_PIPE1CTR + ((ed -> ux_dcd_synergy_ed_index - 1UL) * 2UL),
                                        UX_SYNERGY_DCD_DCPCTR_PIDSTALL);
            break;        
    }

    /* Set the endpoint to stall.  */
    ed -> ux_dcd_synergy_ed_status |=  UX_DCD_SYNERGY_ED_STATUS_STALLED;

    /* This function never fails.  */
    return (UX_SUCCESS);
}
/*******************************************************************************************************************//**
 * @} (end addtogroup sf_el_ux)
 **********************************************************************************************************************/

