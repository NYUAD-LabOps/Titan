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
#include "ux_dcd_synergy.h"

/*******************************************************************************************************************//**
 * @addtogroup sf_el_ux
 * @{
 **********************************************************************************************************************/

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    ux_dcd_synergy_data_buffer_size                     PORTABLE C      */
/*                                                           5.6          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function returns the size of the buffer data                   */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    dcd_synergy                 Pointer to synergy controller           */ 
/*    ed                                 Register to the ed               */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    buffer size                                                         */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
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
 * @brief  This function returns the size of the data buffer and selects the specified pipe.
 *
 * @param[in] dcd_synergy  Pointer to a DCD control block
 * @param[in] ed           Pointer to a physical Endpoint(ED) control block
 *
 * @retval buffer_size     Maximum packet size.
 **********************************************************************************************************************/
ULONG  ux_dcd_synergy_data_buffer_size(UX_DCD_SYNERGY *dcd_synergy, UX_DCD_SYNERGY_ED *ed)
{
    ULONG   buffer_size;
    ULONG   synergy_register;

    /* First get the pipe umber. If this is the control endpoint, we
       treat it differently. */
    if (ed -> ux_dcd_synergy_ed_index == 0UL)
    {
        /* Read the DCPCFG register.  */
        synergy_register =  ux_dcd_synergy_register_read(dcd_synergy, UX_SYNERGY_DCD_DCPCFG);

        /* Check if we are in countinuous mode.  */
        if (synergy_register & UX_SYNERGY_DCD_DCPCFG_CNTMD)
        {
            /* Set the buffer size to the maximum.  */
            buffer_size =  UX_SYNERGY_DCD_PIPE0_SIZE;
        }
        else
        {
            /* Use the endpoint max packet size.  */
            buffer_size = ed -> ux_dcd_synergy_ed_endpoint -> ux_slave_endpoint_descriptor.wMaxPacketSize;
        }
    }
    else
    {
        /* Select the pipe.  */
        ux_dcd_synergy_register_clear(dcd_synergy, UX_SYNERGY_DCD_PIPESEL, UX_SYNERGY_DCD_PIPESEL_NO_PIPE);
        ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_PIPESEL, (USHORT)ed -> ux_dcd_synergy_ed_index);

        /* Read the DCPCFG register.  */
        synergy_register =  ux_dcd_synergy_register_read(dcd_synergy, UX_SYNERGY_DCD_DCPCFG);

        /* Use the endpoint max packet size.  */
        buffer_size = ed -> ux_dcd_synergy_ed_endpoint -> ux_slave_endpoint_descriptor.wMaxPacketSize;
    }

    /* Return the buffer size.  */
    return (buffer_size);
}
/*******************************************************************************************************************//**
 * @} (end addtogroup sf_el_ux)
 **********************************************************************************************************************/

