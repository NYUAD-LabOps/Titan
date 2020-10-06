/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2015 by Express Logic Inc.               */
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

/***********************************************************************************************************************
 * File Name    : nx_hw_init.c
 * Description  : Ethernet hardware initialization driver
 **********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/*LDRA_NOANALYSIS tx_api.h and nx_api.h are not maintained by Renesas, so LDRA analysis is skipped for these files. */
#include "tx_api.h"
#include "nx_api.h"
/*LDRA_ANALYSIS */
#include "nx_renesas_synergy.h"
#include "sf_el_nx_cfg.h"

#include "bsp_api.h"
#include "r_ioport.h"
/* Only for KSZ8081RNB and KSZ8091RNB PHY chips */
#if defined(BSP_BOARD_PHY_KSZ8091) || defined(BSP_BOARD_PHY_KSZ8081)
#include "phy/ether_phy.h"
#endif
#include "framework/instances/sf_el_nx.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

static void nx_driver_timer_handler(ULONG data);


/***********************************************************************//**
 * @ingroup SF_Library
 * @defgroup NX_SYNERGY_PORT NetX Synergy Port
 *
 * @brief RTOS-integrated NetX Ethernet driver for the Renesas Synergy software and Synergy Ethernet IP.
 *
 * @{
 **************************************************************************/

static inline ULONG nx_ms_to_ticks(ULONG time_ms)
{
    return (((time_ms * TX_TIMER_TICKS_PER_SECOND) / 1000U) + 1U);
}

/*******************************************************************************************************************//**
 * @brief   nx_synergy_ethernet_init
 *
 * This function initializes the specified Ethernet port.
 *
 * @param[in]  nx_rec_ptr     Pointer to Ethernet record structure.
 * @param[in]  hw_padding     Flag to indicate hardware 2 byte padding is used or not.
 * @param[in]  sf_el_nx_cfg_ptr  Pointer to configuration structure.
 *
 * @retval  SSP_SUCCESS           Call successful.
 * @retval  NX_NOT_SUCCESSFUL     Call not successful.
***********************************************************************************************************************/
UINT nx_synergy_ethernet_init(NX_REC * nx_rec_ptr, sf_el_nx_cfg_t * sf_el_nx_cfg_ptr, bool hw_padding)
{
    int16_t          phydata = 0;
    UINT             status  = 0;
    R_ETHERC0_Type * etherc  = NULL;
    R_EDMAC0_Type  * edmac   = NULL;

    ssp_feature_t ssp_feature = {{(ssp_ip_t) 0U}};
    ssp_feature.id = SSP_IP_ETHER;
    ssp_feature.unit = 0U;

    nx_rec_ptr->etherc_ptr = sf_el_nx_cfg_ptr->etherc_ptr;
    nx_rec_ptr->edmac_ptr  = sf_el_nx_cfg_ptr->edmac_ptr;
    ssp_feature.channel    = sf_el_nx_cfg_ptr->channel;

    fmi_feature_info_t info = {0U};
    ssp_err_t err = g_fmi_on_fmi.productFeatureGet(&ssp_feature, &info);
    if (SSP_SUCCESS != err)
    {
        /* IP does not exist on this MCU. */
        return (UINT) NX_NOT_SUCCESSFUL;
    }

    fmi_event_info_t event_info = {(IRQn_Type) 0U};
    g_fmi_on_fmi.eventInfoGet(&ssp_feature, SSP_SIGNAL_EDMAC_EINT, &event_info);
    nx_rec_ptr->irq = event_info.irq;
    if (SSP_INVALID_VECTOR == nx_rec_ptr->irq)
    {
        /* Requires the Ethernet interrupt to be enabled. */
        return (UINT) NX_NOT_SUCCESSFUL;
    }

    /** Configure the Ethernet interrupt. */
    ssp_vector_info_t * p_vector_info;
    R_SSP_VectorInfoGet(nx_rec_ptr->irq, &p_vector_info);
    NVIC_SetPriority(nx_rec_ptr->irq, SF_EL_NX_CFG_IRQ_IPL);
    *(p_vector_info->pp_ctrl) = nx_rec_ptr;

    etherc = nx_rec_ptr->etherc_ptr;
    edmac  = nx_rec_ptr->edmac_ptr;

    /** Enable clock */
    R_BSP_ModuleStart(&ssp_feature);

    if (etherc == R_ETHERC1)
    {
        /** Reset PHY */
        g_ioport_on_ioport.pinEthernetModeCfg(IOPORT_ETHERNET_CHANNEL_1, IOPORT_ETHERNET_MODE_RMII);

        g_ioport_on_ioport.pinWrite(SF_EL_NX_CFG_ENET1_RESET_PIN,IOPORT_LEVEL_LOW);
        tx_thread_sleep(nx_ms_to_ticks(10U));
        g_ioport_on_ioport.pinWrite(SF_EL_NX_CFG_ENET1_RESET_PIN,IOPORT_LEVEL_HIGH);
    }
    else if (etherc == R_ETHERC0)
    {
        /* PHY reset pin for channel 0 */
        g_ioport_on_ioport.pinEthernetModeCfg(IOPORT_ETHERNET_CHANNEL_0, IOPORT_ETHERNET_MODE_RMII);

        g_ioport_on_ioport.pinWrite(SF_EL_NX_CFG_ENET0_RESET_PIN,IOPORT_LEVEL_LOW);
        tx_thread_sleep(nx_ms_to_ticks(10U));
        g_ioport_on_ioport.pinWrite(SF_EL_NX_CFG_ENET0_RESET_PIN,IOPORT_LEVEL_HIGH);
    }

    /** Initialize & reset EDMAC and ETHERC  */
    edmac->EDMR_b.SWR = 1U;

    /** Wait at least 64 cycles of PCLKA to reset the EDMAC and ETHERC.
     * PCLKA must be at least 12.5 MHz to use Ethernet, so wait at least 5.12 us. */
    tx_thread_sleep(nx_ms_to_ticks(1U));

    /** Set ETHERC default modes 100 Mbps, Full duplex */
    etherc->ECMR_b.DM = 1U;
    etherc->ECMR_b.RTM = 1U;

    /** Set to little Endian. */
    edmac->EDMR_b.DE = 1U;

    /** Initialize controller */

    /** Set up descriptor addresses */
    edmac->RDLAR    =  (UINT)(nx_rec_ptr->driver_rx_bd);    /* initialize Rx Descriptor List Address */
    edmac->TDLAR    =  (UINT)(nx_rec_ptr->driver_tx_bd);    /* initialize Tx Descriptor List Address */

    /** Configure FIFO */
    edmac->TRSCER  = 0x00000000U;         /* copy-back status for RRF & RMAF */
    edmac->TFTR    = 0x0000000DU;         /* threshold of Tx_FIFO is 52 bytes */
    edmac->FDR     = 0x0000070FU;         /* transmit fifo (2048) & receive fifo (4096) is fixed in manual  */

    edmac->EESR    = 0x47FF0F9FU;         /* clear all ETHERC and EDMAC status bits */
    etherc->ECSR   = 0x00000037U;         /* clear all ETHERC status BFR, PSRTO, LCHNG, MPD, ICD */

    edmac->RMCR_b.RNR   = 1U;             /* RR in EDRRR is under driver control */
    edmac->FCFTR        = 0x00070000U;

    /* Mac Address configuration */
    etherc->MAHR = (uint32_t) ((nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_msw << 16)
                               | (ULONG) ((nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_lsw >> 16)
                                       & 0x0000FFFFU));
    etherc->MALR = (uint32_t) (nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_lsw &
                      0x0000FFFFU);

    etherc->RFLR   = NX_ENET_MAX_MTU; /* ether payload is 1514 + CRC */
    etherc->IPGR = 0x14U;             /* 96 bit interpacket gap */

    if (false == hw_padding)
    {
        edmac->RPADIR_b.PADS = 0U;
    }
    else
    {
        edmac->RPADIR_b.PADS = 2U;
    }

    edmac->RPADIR_b.PADR = 14U; /* insert 2 bytes of padding before IP header NX_ETHERNET_SIZE*/

    /** Enable EDMAC receive */
    edmac->EDRRR_b.RR  = 1U;

    /** Enable receive, transmit, ECI interrupts. */
    etherc->ECSIPR_b.LCHNGIP = 1U; /* enable ETHERC status change interrupt */
    edmac->EESIPR_b.TCIP = 1U;     /* transmit complete */
    edmac->EESIPR_b.FRIP = 1U;     /* frame receive */
    edmac->EESIPR_b.ECIIP = 1U;    /* ECI interrupt  required for link status reporting */

    nx_rec_ptr -> link_established = (UINT) NX_FALSE;

    /** Initialize PHY. */
    phydata = bsp_ethernet_phy_init(ssp_feature.channel);

    if (phydata == R_PHY_ERROR)
    {
        return (UINT) NX_NOT_SUCCESSFUL;
    }

    /** Start PHY auto negotiation */
    bsp_ethernet_phy_start_autonegotiate(ssp_feature.channel, ETHER_FLAG_OFF);

    /** Create a timer to poll for completion of autonegotiation. */
    status = tx_timer_create(&nx_rec_ptr->driver_event_polling_timer, (CHAR *) "nx driver timer",
            nx_driver_timer_handler, (ULONG) nx_rec_ptr,
            nx_ms_to_ticks(NX_DRIVER_EVENT_POLLING_INTERVAL_SHORT),
            nx_ms_to_ticks(NX_DRIVER_EVENT_POLLING_INTERVAL_SHORT), TX_AUTO_ACTIVATE);

    return status;
}

/*******************************************************************************************************************//**
 * @brief   nx_driver_timer_handler
 *
 * This function is called from the timer expiration event. On every time event, this routine register
 * a deferred event in IP helper thread
 *
 * @param[in]  data     Ethernet record data.
***********************************************************************************************************************/
static void nx_driver_timer_handler(ULONG data)
{

    NX_REC *  p_nx_rec = (NX_REC *) data;
    /** Mark the polling flag.  */
    p_nx_rec->nx_driver_phy_polling_requested = TX_TRUE;

    /** Call NetX to register a deferred event.  */
    _nx_ip_driver_deferred_processing(p_nx_rec->ip_ptr);
}

/*******************************************************************************************************************//**
 * @brief   nx_driver_event_handler
 *
 * This function is called from the IP thread deferred event.  On every deferred event, this routine checks
 * for Phy link status and handles link up/down and link change.  During initialization this routine is
 * responsible for checking for autonegotiation.
 *
 * @param[in]  nx_rec_ptr    : Pointer to Ethernet record structure.
***********************************************************************************************************************/
void nx_driver_event_handler(NX_REC *nx_rec_ptr)
{
    int16_t  phydata = 0;
    USHORT line_speed_dplx = 0;
    USHORT local_pse = 0 ;
    USHORT partner_pse = 0;
    R_ETHERC0_Type *etherc;
#if (0 == NX_IP_HARDWARE_INT_CALLBACK_ENABLED)
    UINT   interface_index = 0;
#endif
    /** Determine previous link status.  */
    if (nx_rec_ptr -> link_established != (UINT) NX_TRUE)
    {
        etherc = nx_rec_ptr -> etherc_ptr;

        phydata = bsp_ethernet_phy_get_autonegotiate(nx_rec_ptr->channel, &line_speed_dplx, &local_pse, &partner_pse);

        if (phydata == R_PHY_ERROR)
        {
            return;
        }

        if(line_speed_dplx == PHY_LINK_100F)
        {
            /* Full duplex */
            etherc->ECMR_b.DM = 1U;
            /* 100 Mbps */
            etherc->ECMR_b.RTM = 1U;
        }
        else if(line_speed_dplx == PHY_LINK_100H)
        {
            /* Half duplex */
            etherc->ECMR_b.DM = 0U;
            /* 100 Mbps */
            etherc->ECMR_b.RTM = 1U;
        }
        else  if(line_speed_dplx == PHY_LINK_10F)
        {
            /* Full duplex */
            etherc->ECMR_b.DM = 1U;
            /* 10 Mbps */
            etherc->ECMR_b.RTM = 0U;
        }
        else if(line_speed_dplx == PHY_LINK_10H)
        {
            /* Half duplex */
            etherc->ECMR_b.DM = 0U;
            /* 10 Mbps */
            etherc->ECMR_b.RTM = 0U;
        }

        /** Save link status and changed polling interval.  */
        nx_rec_ptr -> link_established = (UINT) NX_TRUE;
        if (nx_rec_ptr -> nx_state & NX_LINK_ENABLED)
        {
            nx_rec_ptr -> ether_interface_ptr -> nx_interface_link_up = (UCHAR) NX_TRUE;
        }
        else
        {
            nx_rec_ptr -> ether_interface_ptr -> nx_interface_link_up = (UCHAR) NX_FALSE;
        }

#if (0 == NX_IP_HARDWARE_INT_CALLBACK_ENABLED)

        while ( interface_index < (UINT) NX_MAX_IP_INTERFACES )
        {
            if( &nx_rec_ptr->ip_ptr->nx_ip_interface[interface_index] == nx_rec_ptr->ether_interface_ptr )
            {
                _nx_ip_driver_link_status_event(nx_rec_ptr->ip_ptr, interface_index);
                break;
            }
            else
            {
                interface_index++;
            }
        }
#endif

        /* The return codes are not checked here because timer deactivate, change and activate call
         * cannot fail when called with created RTOS objects. This object was successfully created
         * during link initialization. */
        tx_timer_deactivate(&nx_rec_ptr -> driver_event_polling_timer);
        tx_timer_change(&nx_rec_ptr -> driver_event_polling_timer,
                nx_ms_to_ticks(NX_DRIVER_EVENT_POLLING_INTERVAL_LONG),
                nx_ms_to_ticks(NX_DRIVER_EVENT_POLLING_INTERVAL_LONG));
        tx_timer_activate(&nx_rec_ptr -> driver_event_polling_timer);

    }
    else
    {
        /** Check PHY link status.  */
        phydata = bsp_ethernet_phy_get_link_status((uint32_t)nx_rec_ptr->channel);
        if (phydata == R_PHY_OK)
        {
            return;
        }
        else
        {
            /** Save link status and changed polling interval.  */
            nx_rec_ptr -> link_established = (UINT) NX_FALSE;
            nx_rec_ptr -> ether_interface_ptr -> nx_interface_link_up = (UCHAR) NX_FALSE;

#if (0 == NX_IP_HARDWARE_INT_CALLBACK_ENABLED)
            while ( interface_index < (UINT) NX_MAX_IP_INTERFACES )
            {
                if( &nx_rec_ptr->ip_ptr->nx_ip_interface[interface_index] == nx_rec_ptr->ether_interface_ptr )
                {
                    _nx_ip_driver_link_status_event(nx_rec_ptr->ip_ptr, interface_index);
                    break;
                }
                else
                {
                    interface_index++;
                }
            }
#endif

            /* The return codes are not checked here because timer deactivate, change and activate call
             * cannot fail when called with created RTOS objects. This object was successfully created
             * during link initialization. */
            tx_timer_deactivate(&nx_rec_ptr -> driver_event_polling_timer);
            tx_timer_change(&nx_rec_ptr -> driver_event_polling_timer,
                    nx_ms_to_ticks(NX_DRIVER_EVENT_POLLING_INTERVAL_SHORT),
                    nx_ms_to_ticks(NX_DRIVER_EVENT_POLLING_INTERVAL_SHORT));
            tx_timer_activate(&nx_rec_ptr -> driver_event_polling_timer);
        }
    }

}

/*******************************************************************************************************************//**
 * @brief   enet_hw_enable_interrupt
 *
 * This function enables interrupts for the given Ethernet port.
 *
 * @param[in]  nx_rec_ptr    : Pointer to Ethernet record structure.
 * @note       The pointer parameter passed to this function is already validated at the higher level.
***********************************************************************************************************************/
void enet_hw_enable_interrupt(NX_REC * nx_rec_ptr)
{
    R_ETHERC0_Type * etherc;

    etherc = nx_rec_ptr->etherc_ptr;

    /** Enable interrupts at the NVIC. */
    IRQn_Type irq = nx_rec_ptr->irq;
    R_BSP_IrqStatusClear(irq);
    NVIC_ClearPendingIRQ(irq);
    NVIC_EnableIRQ(irq);

    /** Enable interrupts at the Ethernet controller */
    etherc->ECMR_b.RE = 1U;
    etherc->ECMR_b.TE = 1U;
}
/*******************************************************************************************************************//**
 * @brief   nx_synergy_ethernet_deinit
 *
 * This function performs reset of the ethernet controller and PTP controller.
 *
 * @param[in]  nx_rec_ptr    : Pointer to Ethernet record structure.
 * @param[in]  sf_el_nx_cfg_ptr  : sf_el_nx configuration structure pointer
 * @retval     NX_SUCCESS : Call successful
 * @retval     NX_NOT_SUCCESSFUL : Call not successful
***********************************************************************************************************************/
UINT nx_synergy_ethernet_deinit(NX_REC * nx_rec_ptr, sf_el_nx_cfg_t * sf_el_nx_cfg_ptr)
{
    R_ETHERC0_Type * etherc  = NULL;
    R_EDMAC0_Type  * edmac   = NULL;

    R_EPTPC_CFG_Type * eptpc_cfg = NULL;
    R_EPTPC0_Type    * eptpc0    = NULL;
    R_EPTPC0_Type    * eptpc1    = NULL;

    ssp_feature_t ssp_feature = {{(ssp_ip_t) 0U}};
    ssp_feature.id = SSP_IP_ETHER;
    ssp_feature.unit = 0U;

    nx_rec_ptr->etherc_ptr = sf_el_nx_cfg_ptr->etherc_ptr;
    nx_rec_ptr->edmac_ptr  = sf_el_nx_cfg_ptr->edmac_ptr;
    ssp_feature.channel    = sf_el_nx_cfg_ptr->channel;

    fmi_feature_info_t info = {NULL,{0U,0U,0U,0U,0U},NULL};
    ssp_err_t err = g_fmi_on_fmi.productFeatureGet(&ssp_feature, &info);
    if (SSP_SUCCESS != err)
    {
        /* IP does not exist on this MCU. */
        return (UINT)NX_NOT_SUCCESSFUL;
    }

    etherc = nx_rec_ptr->etherc_ptr;
    edmac  = nx_rec_ptr->edmac_ptr;

    if ((NULL == etherc) || (NULL == edmac))
    {
        return (UINT)NX_NOT_SUCCESSFUL;
    }

    /** Disable transmission and reception at the Ethernet controller */
    etherc->ECMR_b.RE = 0U;
    etherc->ECMR_b.TE = 0U;

    ssp_feature.id = SSP_IP_EPTPC;
    ssp_feature.unit = 1U;
    ssp_feature.channel = 0U;
    ssp_err_t eptpc_err = g_fmi_on_fmi.productFeatureGet(&ssp_feature, &info);

    /** Reset the EPTPC, Resetting after detection of an erroneous flag  */
    /* 30.2.37 SYNFP Status Register (SYSR) S7G2 manual r01um0001eu0140-synergy-s7g2  */
    /* 30.2.30 SYNFP Status Register (SYSR) S5D9 manual r01um0004eu0110-synergy-s5d9  */
    /* For S5D5 MCU, EPTPC is a part of ETHERC */
    if (SSP_SUCCESS == eptpc_err)
    {
        eptpc_cfg = (R_EPTPC_CFG_Type *) info.ptr;
        eptpc_cfg->PTRSTR_b.RESET = 1U;
    }

    /** Reset the ETHERC and EDMAC */
    edmac->EDMR_b.SWR = 1U;

    /** Wait for  64 PCLKA cycles to reset ETHERC and EDMAC */
    tx_thread_sleep(2U);

    if (SSP_SUCCESS == eptpc_err)
    {
        eptpc_cfg = (R_EPTPC_CFG_Type *) info.ptr;
        eptpc_cfg->PTRSTR_b.RESET = 0U;
    }

    /** Double check the Interrupts are disabled */
    /** Disable interrupts at the NVIC. */
    IRQn_Type irq = nx_rec_ptr->irq;
    NVIC_DisableIRQ(irq);
    R_BSP_IrqStatusClear(irq);
    NVIC_ClearPendingIRQ(irq);

    /** Check for the ether channel */
    if (R_ETHERC1 == etherc)
    {
        /** Hold PHY in Reset for channel 1 */
        g_ioport_on_ioport.pinWrite(SF_EL_NX_CFG_ENET1_RESET_PIN,IOPORT_LEVEL_LOW);

        ssp_feature.unit = 3U;
        ssp_feature.channel = 1U;
        eptpc_err = g_fmi_on_fmi.productFeatureGet(&ssp_feature, &info);

        if (SSP_SUCCESS == eptpc_err)
        {
            eptpc1 = (R_EPTPC0_Type *) info.ptr;
            if (1U == eptpc1->SYSR_b.INFABT)
            {
                 eptpc1->SYSR_b.INFABT = 0U;
            }
        }
    }
    else if (R_ETHERC0 == etherc)
    {
        /** Hold PHY in Reset for channel 0 */
        g_ioport_on_ioport.pinWrite(SF_EL_NX_CFG_ENET0_RESET_PIN,IOPORT_LEVEL_LOW);

        ssp_feature.unit = 3U;
        ssp_feature.channel = 0U;
        eptpc_err = g_fmi_on_fmi.productFeatureGet(&ssp_feature, &info);

        if (SSP_SUCCESS == eptpc_err)
        {
            eptpc0 = (R_EPTPC0_Type *) info.ptr;
            if (1U == eptpc0->SYSR_b.INFABT)
            {
                eptpc0->SYSR_b.INFABT = 0U;
            }
        }
    }

    /** Disable clock to the module */
    R_BSP_ModuleStop(&ssp_feature);

    return (UINT)NX_SUCCESS;
}

/***********************************************************************//**
 * @} (end defgroup NX_SYNERGY_PORT)
 **************************************************************************/
