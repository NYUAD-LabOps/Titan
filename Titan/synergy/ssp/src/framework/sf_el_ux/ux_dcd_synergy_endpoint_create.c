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
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    ux_dcd_synergy_endpoint_create                      PORTABLE C      */
/*                                                           5.6          */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function will create a physical endpoint.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    dcd_synergy                           Pointer to device controller  */
/*    endpoint                              Pointer to endpoint container */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */ 
/*                                                                        */
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    ux_dcd_synergy_endpoint_reset           Reset endpoint              */
/*    ux_dcd_synergy_register_read            Read register               */
/*    ux_dcd_synergy_register_write           Write register              */
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
 * @brief  This function creates a physical endpoint.
 *
 * @param[in]  dcd_synergy  Pointer to a DCD control block
 * @param[in]  endpoint     Pointer to a Device Controller Endpoint structure.
 *
 * @retval UX_SUCCESS           Endpoint is created successfully.
 * @retval UX_ERROR             Buffer is not free or endpoint creation is unsuccessful.
 * @retval UX_NO_ED_AVAILABLE   Endpoint is already in use.
 **********************************************************************************************************************/
UINT  ux_dcd_synergy_endpoint_create(UX_DCD_SYNERGY *dcd_synergy, UX_SLAVE_ENDPOINT *endpoint)
{
    UX_DCD_SYNERGY_ED * ed;
    ULONG               synergy_endpoint_index;
    ULONG               synergy_endpoint_mask;
    ULONG               pipe_index;
    ULONG               buffer_index;
    ULONG               current_pipe_index;
    ULONG               endpoint_direction;
    ULONG               endpoint_type = 0UL;
    ULONG               pipe_type;
    ULONG               pipe_selection_status;

TX_INTERRUPT_SAVE_AREA

    /* The endpoint index in the array of the synergy must match the endpoint number.  */
    synergy_endpoint_index =  endpoint -> ux_slave_endpoint_descriptor.bEndpointAddress & (ULONG)(~UX_ENDPOINT_DIRECTION);
    
    /* Fetch the address of the endpoint.  */
    ed =  &dcd_synergy -> ux_dcd_synergy_ed[synergy_endpoint_index];

    /* Check the endpoint status, if it is free, reserve it. If not reject this endpoint.  */
    if ((ed -> ux_dcd_synergy_ed_status & UX_DCD_SYNERGY_ED_STATUS_USED) == 0)
    {
        /* We can use this endpoint.  */
        ed -> ux_dcd_synergy_ed_status |=  UX_DCD_SYNERGY_ED_STATUS_USED;

        /* Keep the physical endpoint address in the endpoint container.  */
        endpoint -> ux_slave_endpoint_ed =  (VOID *) ed;

        /* Save the endpoint pointer.  */
        ed -> ux_dcd_synergy_ed_endpoint =  endpoint;
        
        /* Build the endpoint mask from the endpoint descriptor.  */
        synergy_endpoint_mask =  endpoint -> ux_slave_endpoint_descriptor.bmAttributes & UX_MASK_ENDPOINT_TYPE;
        
        /* Set the endpoint type and direction.  */
        switch (synergy_endpoint_mask)
        {
        case (ULONG)UX_CONTROL_ENDPOINT:

            /* Control pipe index is always 0.  */
            pipe_index = 0UL;
            ed -> ux_dcd_synergy_ed_buffer_number =  0UL;
            ed -> ux_dcd_synergy_ed_buffer_size =  (( endpoint ->  ux_slave_endpoint_descriptor.wMaxPacketSize) / 64);
            break;                

        case (ULONG)UX_BULK_ENDPOINT:

            /* Obtain the first available pipe. */
            current_pipe_index = UX_DCD_SYNERGY_MIN_PIPE;

            pipe_index = 0UL;
            
            /* Take a pessimist view on endpoint selection.  */
            pipe_selection_status = (ULONG)UX_ERROR;

            while( ( current_pipe_index <= 5UL ) && ( pipe_index ==  0UL ) )
            {
                /* Select pipe 1 - 5. S7,S3, Select pipe 4-5 for S1 */
                ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_PIPESEL, (USHORT)current_pipe_index);
               
                /* Check whether pipe is in use.  */
                pipe_type =  dcd_synergy -> ux_dcd_synergy_pipe[current_pipe_index];

                /* double check whether pipe is in use.  */
                pipe_type = pipe_type | ( (ux_dcd_synergy_register_read(dcd_synergy, UX_SYNERGY_DCD_PIPECFG)
                        & UX_SYNERGY_DCD_PIPECFG_TYPE_MASK) >> 14);

                /* Not in use, choose this one.  */
                if (pipe_type == 0UL)
                {
                    /* This is the pipe index to use.  */
                    pipe_index = current_pipe_index;  
                    
                    /* Memorize the pipe associated with the endpoint.  */
                    dcd_synergy -> ux_dcd_synergy_pipe[current_pipe_index] = synergy_endpoint_index;

                    /* We are done here.  */
                    pipe_selection_status = (ULONG)UX_SUCCESS;
                }                    
                    
                /* Increment the current index.  */
                current_pipe_index++;
            }

            /* Endpoint selected ? */
            if (pipe_selection_status != (ULONG)UX_SUCCESS)
            {
                /* Cannot proceed.  */
                return (UINT)UX_ERROR;
            }

            /* Set the endpoint type to BULK.  */
            endpoint_type =  UX_SYNERGY_DCD_PIPECFG_TYPE_BULK;

            /* Configure the  endpoint  hardware, pick up buffer size & number. */
            ed -> ux_dcd_synergy_ed_buffer_size   =  UX_DCD_SYNERGY_MAX_BULK_PAYLOAD / UX_DCD_SYNERGY_MAX_BUF_SIZE;
            ed -> ux_dcd_synergy_ed_buffer_number =  (ULONG)(UX_SYNERGY_DCD_PIPE1_BUF_START_NUM +
                                                            ((pipe_index - 1UL) * ed -> ux_dcd_synergy_ed_buffer_size));

            /* Configure PIPEBUF which is only present in HS peripheral */
#if defined(R_USBHS_BASE)
            if (R_USBHS_BASE == dcd_synergy->ux_dcd_synergy_base)
            {
                /* Set the endpoint buffer info. */
                ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_PIPEBUF,
                                       ((USHORT)((ed -> ux_dcd_synergy_ed_buffer_size - 1UL) << UX_SYNERGY_DCD_PIPEBUF_SHIFT) |
                                        (USHORT)ed -> ux_dcd_synergy_ed_buffer_number));
            }
#endif
            /* Calculate the next available free buffer.  */
            if (ed -> ux_dcd_synergy_ed_buffer_number > UX_SYNERGY_DCD_MAX_BUF_NUM)
            {
                return (UINT)UX_ERROR;
            }
            break;

        case (ULONG)UX_INTERRUPT_ENDPOINT:

            /* Set first buffer number. */
            buffer_index = UX_SYNERGY_DCD_PIPE6_BUF_START_NUM;
            pipe_index = 0UL;
            
            /* Take a pessimist view on endpoint selection.  */
            pipe_selection_status = (ULONG)UX_ERROR;

            /* Browse Pipes from 6 to 9 (S7,S3)  6 to 7 (S1) which can be interrupt.  */
            for (current_pipe_index = 6UL; current_pipe_index <= UX_DCD_SYNERGY_MAX_PIPE; current_pipe_index++)
            {
                /* Select pipe.  */
                ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_PIPESEL, (USHORT)current_pipe_index);
               
                /* Check whether pipe is in use.  */
                pipe_type =  dcd_synergy -> ux_dcd_synergy_pipe[current_pipe_index];
            
                /* Check whether pipe is in use.  */
                pipe_type = pipe_type | ( (ux_dcd_synergy_register_read(dcd_synergy, UX_SYNERGY_DCD_PIPECFG)
                        & UX_SYNERGY_DCD_PIPECFG_TYPE_MASK) >> 14);
            
                /* Not in use, choose this one.  */
                if (pipe_type == 0UL)
                {
                    /* Set the pipe index.  */
                    pipe_index = current_pipe_index;
        
                    /* Memorise the pipe associated with the endpoint.  */
                    dcd_synergy -> ux_dcd_synergy_pipe[current_pipe_index] = synergy_endpoint_index;
                   
                    /* We are done here.  */
                    pipe_selection_status = (ULONG)UX_SUCCESS;

                    /* We are done with pipe selection.  */
                    break;
                }            
        
                /* Next buffer.  */
                buffer_index++;
        
            }

            /* Endpoint selected ? */
            if (pipe_selection_status != (ULONG)UX_SUCCESS)
            {
                /* Cannot proceed.  */
                return (UINT)UX_ERROR;
            }

            /* Set the endpoint type to Interrupt.  */
            endpoint_type =  UX_SYNERGY_DCD_PIPECFG_TYPE_INTERRUPT;

            /* Configure the  endpoint  hardware, pick up buffer size. Number is fixed. */
            ed -> ux_dcd_synergy_ed_buffer_number =  buffer_index;
            ed -> ux_dcd_synergy_ed_buffer_size =  0UL;
            break;                

        default:

            return (UINT)UX_ERROR;
        }

        /* Memorize the endpoint index.  */
        ed -> ux_dcd_synergy_ed_index =  pipe_index;
        /* Note - UX_SYNERGY_DCD_FIFO_C is used for Control end point,
         * UX_SYNERGY_DCD_FIFO_D0 is used for all IN end points and
         * UX_SYNERGY_DCD_FIFO_D1 is used for all OUT end points
         * Basically D0/ D1 indicates data endpoints and not necessarily D0/ D1 HW buffers are used
         * This is checked when DMA is enabled and only in that case we use D0/ D1 HW buffers for DMA
         */
#if defined(BSP_MCU_GROUP_S124) || defined(BSP_MCU_GROUP_S128) || defined(BSP_MCU_GROUP_S1JA)
        /* For pipes, use FIFO_C/ FIFO_C0/ FIFO_C1  */
        if (synergy_endpoint_mask == (ULONG)UX_CONTROL_ENDPOINT)
        {
            /* For Control pipes, use FIFO_C.  */
            ed->ux_dcd_synergy_ed_fifo_index =  UX_SYNERGY_DCD_FIFO_CD;
        }
        else
         {
             /* For non-Control pipes, use FIFO_C0 for IN end-point */
             if ((endpoint -> ux_slave_endpoint_descriptor.bEndpointAddress &
                     (ULONG)UX_ENDPOINT_DIRECTION) == (ULONG)UX_ENDPOINT_IN)
             {
                     ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_C0;
             }
             else
             {
                     ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_C1;
             }
         }

#else
        if (synergy_endpoint_mask == (ULONG)UX_CONTROL_ENDPOINT)
        {
            /* For Control pipes, use FIFO_C.  */
            ed->ux_dcd_synergy_ed_fifo_index =  UX_SYNERGY_DCD_FIFO_CD;
        }
        else
        {
            /* For non-Control pipes, use FIFO_D if DMA transfer enabled. */
            if ((endpoint -> ux_slave_endpoint_descriptor.bEndpointAddress &
                    (ULONG)UX_ENDPOINT_DIRECTION) == (ULONG)UX_ENDPOINT_IN)
            {
                /* Use FIFO_D0 for IN PIPE (Transmission from Synergy to a USB Host) if DMA transfer enabled. */
                if (dcd_synergy->ux_dcd_synergy_transfer.ux_synergy_transfer_tx != NULL)
                {
                    /* Only one pipe can use D0/D1 fifo for DMA */
                    if (dcd_synergy->ux_dcd_synergy_D0_fifo_state == UX_DCD_SYNERGY_FIFO_FOR_DMA_FREE)
                    {
                        ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_D0;
                        dcd_synergy->ux_dcd_synergy_D0_fifo_state = UX_DCD_SYNERGY_FIFO_FOR_DMA_USED;
                    }
                    else
                    {
                        ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_C0;
                    }
                }
                else
                {
                    ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_C0;
                }
            }
            else
            {
                /* Use FIFO_D0 for OUT PIPE (Reception from a USB Host to Synergy) if DMA transfer enabled. */
                if (dcd_synergy->ux_dcd_synergy_transfer.ux_synergy_transfer_rx != NULL)
                {
                    /* Only one pipe can use D0/D1 fifo for DMA */
                    if (dcd_synergy->ux_dcd_synergy_D1_fifo_state == UX_DCD_SYNERGY_FIFO_FOR_DMA_FREE)
                    {
                        ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_D1;
                        dcd_synergy->ux_dcd_synergy_D1_fifo_state = UX_DCD_SYNERGY_FIFO_FOR_DMA_USED;
                    }
                    else
                    {
                        ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_C1;
                    }
                }
                else
                {
                    ed->ux_dcd_synergy_ed_fifo_index = UX_SYNERGY_DCD_FIFO_C1;
                }
            }
        }
#endif
        /* Continue the configuration for Bulk and Interrupt only.  */
        /* Check endpoint type.  */

        if (synergy_endpoint_mask == (ULONG)UX_CONTROL_ENDPOINT)
        {
            /* Continue the configuration for control endpoint only.  */
            /* Initialize the DCPMAX register.  */
            ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_DCPMAXP,
                                              (USHORT)endpoint -> ux_slave_endpoint_descriptor.wMaxPacketSize);
        }
        else
        {
            /* Continue the configuration for Bulk and Interrupt only.  */
            /* Check if IN or OUT.  */
            if ((endpoint -> ux_slave_endpoint_descriptor.bEndpointAddress &
                    (ULONG)UX_ENDPOINT_DIRECTION) == (ULONG)UX_ENDPOINT_IN)
            {
                /* Set the direction to OUT.  */
                endpoint_direction = UX_SYNERGY_DCD_PIPECFG_DIR;
            }
            else
            {
                /* Reset the endpoint direction as bit 4 of PIPECFG is 0 for IN.  */
                endpoint_direction = 0UL;
            }

            /* Set PID to NAK. */
            ux_dcd_synergy_endpoint_nak_set(dcd_synergy, ed);

            /* Critical section, even ISR cannot be disrupted.  */
            TX_DISABLE

            /* Select the endpoint register to map all registers.  */
            ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_PIPESEL, (USHORT)ed -> ux_dcd_synergy_ed_index);

            /* Set Pipe configuration registers.  */
            ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_PIPECFG,
                                    (USHORT)((USHORT)endpoint_type |
                                             (USHORT)UX_SYNERGY_DCD_PIPECFG_SHTNAK |
                                             (USHORT)endpoint_direction |
                                             (USHORT)(endpoint ->
                                                     ux_slave_endpoint_descriptor.bEndpointAddress & 0xFUL)));

            /* Inititalize the PIPEMAX register.  */
            ux_dcd_synergy_register_write(dcd_synergy, UX_SYNERGY_DCD_PIPEMAXP,
                                                 (USHORT)endpoint -> ux_slave_endpoint_descriptor.wMaxPacketSize);

            /* Free the INTs now.  */
            TX_RESTORE
        }

        /* Reset the endpoint.  */
        ux_dcd_synergy_endpoint_reset(dcd_synergy, endpoint);
        
        /* Return successful completion.  */
        return (UINT)UX_SUCCESS;
    }

    /* The endpoint is already used.  */
    return (UINT)UX_NO_ED_AVAILABLE;
}
/*******************************************************************************************************************//**
 * @} (end addtogroup sf_el_ux)
 **********************************************************************************************************************/

