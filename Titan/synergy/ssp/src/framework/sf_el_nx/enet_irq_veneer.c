/***********************************************************************************************************************
 * Copyright [2015-2017] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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

#include "bsp_api.h"
/*LDRA_NOANALYSIS nx_api.h is not maintained by Renesas, so LDRA analysis is skipped for this file only. */
#include "nx_api.h"
/*LDRA_ANALYSIS */
#include "nx_renesas_synergy.h"

void edmac_eint_isr (void);

/***********************************************************************//**
 * @addtogroup NX_SYNERGY_PORT
 * @{
 **************************************************************************/

/*******************************************************************************************************************//**
 * @brief   edmac_eint_isr
 *
 * This is the Ethernet interrupt service routine. It clears the interrupt flag and calls nx_ether_interrupt
 * to process the interrupt.
***********************************************************************************************************************/
void edmac_eint_isr (void)
{
    /** Save context if RTOS is used */
    SF_CONTEXT_SAVE

    ssp_vector_info_t * p_vector_info = NULL;
    R_SSP_VectorInfoGet(R_SSP_CurrentIrqGet(), &p_vector_info);
    NX_REC * p_record = (NX_REC *) *(p_vector_info->pp_ctrl);

    /** Process the interrupt. */
    nx_ether_interrupt(p_record);

    /** Clear pending interrupt flag to make sure it doesn't fire again
     * after exiting. */
    R_BSP_IrqStatusClear(R_SSP_CurrentIrqGet());

    /** Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}

/***********************************************************************//**
 * @} (end addtogroup NX_SYNERGY_PORT)
 **************************************************************************/
