/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2016 by Express Logic Inc.               */
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
* File Name    : nx_renesas_synergy.c
* Description  : Primary source code for NX Synergy Port driver.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

/*LDRA_NOANALYSIS tx_api.h and nx_api.h are not maintained by Renesas, so LDRA analysis is skipped for these files. */
#include "tx_api.h"
#include "nx_api.h"
/*LDRA_ANALYSIS */
#include "nx_renesas_synergy.h"
#include "sf_el_nx_cfg.h"

/* Define Ethernet address format.  This is prepended to the incoming IP
   and ARP/RARP messages.  The frame beginning is 14 bytes, but for speed
   purposes, we are going to assume there are 16 bytes free in front of
   the prepend pointer and that the prepend pointer is 32-bit aligned.

   Byte Offset     Size            Meaning

   0           6           Destination Ethernet Address
   6           6           Source Ethernet Address
   12          2           Ethernet Frame Type, where:

   0x0800->IP Datagram
   0x86DD->IPv6 Datagram
   0x0806->ARP Request/Reply
   0x0835->RARP request reply

   42          18          Padding on ARP and RARP messages only.
*/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef NX_ETHERNET_ERROR_RETURN
/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
#define NX_ETHERNET_ERROR_RETURN(a, err)  SSP_ERROR_RETURN((a), (err), &g_module_name[0], &s_sf_el_nx_version)
#endif

#define NX_ETHERNET_IP      0x0800U
#define NX_ETHERNET_IPV6    0x86DDU
#define NX_ETHERNET_ARP     0x0806U
#define NX_ETHERNET_RARP    0x8035U

#define NX_ETHERNET_SIZE    14U
#define NX_CRC_SIZE         4U

/***********************************************************************************************************************
 * Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
***********************************************************************************************************************/
static UINT nx_ether_send(NX_PACKET *packet, NX_REC *nx_rec_ptr);

static UINT nx_ether_write(NX_PACKET *pkt, NX_REC *nx_rec_ptr);

static UINT nx_storage_init(NX_IP *ip_ptr, NX_REC *nx_rec_ptr);

static void nx_storage_deinit(NX_REC * nx_rec_ptr);

static void nx_rx_interrupt(NX_REC *nx_rec_ptr);

static void nx_tx_interrupt(NX_REC *nx_rec_ptr);

static void nx_receive(NX_REC *nx_rec_ptr, NX_PACKET *packet_ptr);

static void nx_driver_deferred_processing(NX_IP_DRIVER *driver_req_ptr, NX_REC *nx_rec_ptr);

ssp_err_t nx_ethernet_version_get(ssp_version_t * const p_version);


#if defined(__GNUC__)
/* This structure is affected by warnings from a GCC compiler bug. This pragma suppresses the warnings in this
 * structure only.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t s_sf_el_nx_version =
{
    .api_version_minor  = SF_EL_NX_API_VERSION_MINOR,
    .api_version_major  = SF_EL_NX_API_VERSION_MAJOR,
    .code_version_major = SF_EL_NX_CODE_VERSION_MAJOR,
    .code_version_minor = SF_EL_NX_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/* Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char g_module_name[] = "sf_el_nx";
#endif

/***********************************************************************//**
 * @addtogroup NX_SYNERGY_PORT
 * @{
 **************************************************************************/

/** Flag to indicate 2 byte pad setting required for target MCU*/
static bool g_hw_padding_enable = false;



/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief   nx_ether_custom_packet_send
*
* Ethernet driver routine to send a user data in raw Ethernet packet frame from a Source HW address to
* destination HW address with the requested EtherType through a Ethernet channel
*
* @param[in]  pool_ptr                    : Pointer to packet pool to use
* @param[in]  nx_record_ptr               : Pointer to Ethernet record structure
* @param[in]  data                        : Pointer to data to be send
* @param[in]  length                      : length of data to send
* @param[in]  ether_type                  : Type of Ethernet packet
* @param[in]  dest_mac_address            : Destination hardware address to send the user data
*
* @retval     SSP_SUCCESS                 : Call successful
* @retval     SSP_ERR_ASSERTION           : - A parameter pointers point to NULL
*                                           - Invalid Ethernet record structure pointer
*                                           - Invalid data length
* @retval     SSP_ERR_OUT_OF_MEMORY       : No memory is free to allocate the packet in the pool buffer
* @retval     SSP_ERR_NOT_OPEN            : Link not enabled
* @retval     SSP_ERR_INTERNAL            : - Maximum transmit queue depth has been reached
*                                           - No packet is available to append the data
***********************************************************************************************************************/

ssp_err_t nx_ether_custom_packet_send( NX_PACKET_POOL *pool_ptr,
                                     NX_REC * nx_record_ptr,
                                     UCHAR  * data,
                                     UINT length,
                                     USHORT ether_type,
                                     nx_mac_address_t dest_mac_address)
{
#if (1 == SF_EL_NX_CFG_PARAM_CHECKING_ENABLE)
      /** Verify the pointers are valid */
      SSP_ASSERT (NULL != pool_ptr);
      SSP_ASSERT (NULL != nx_record_ptr);
      SSP_ASSERT (NULL != data);
      SSP_ASSERT (0    != length);
#endif
    UINT nx_status             = NX_SUCCESS;
    NX_PACKET * packet_ptr     = NULL;
    ULONG * ethernet_frame_ptr = NULL;

    if ( !(nx_record_ptr->nx_state & NX_LINK_ENABLED) )
    {
      /** Free the packet that we will not send.  */
      nx_packet_transmit_release(packet_ptr);
      return SSP_ERR_NOT_OPEN;
    }

    nx_status = nx_packet_allocate(pool_ptr, &packet_ptr, NX_PHYSICAL_HEADER, NX_NO_WAIT);
    NX_ETHERNET_ERROR_RETURN(((UINT)NX_SUCCESS == nx_status), SSP_ERR_OUT_OF_MEMORY);
    nx_status = nx_packet_data_append(packet_ptr, (void *) data, length, packet_ptr->nx_packet_pool_owner, NX_NO_WAIT);
    NX_ETHERNET_ERROR_RETURN(((UINT)NX_SUCCESS == nx_status), SSP_ERR_INTERNAL);


    /* Adjust the prepend pointer.  */
    packet_ptr->nx_packet_prepend_ptr = packet_ptr->nx_packet_prepend_ptr - NX_ETHERNET_SIZE;

    /** Setup the prepend pointer in order to build the Ethernet frame. Backup another 2
    *  bytes to get 32-bit word alignment.  */
    ethernet_frame_ptr =  (ULONG*) (packet_ptr->nx_packet_prepend_ptr - 2);

    /** Build the actual Ethernet frame.  */
    *ethernet_frame_ptr       =  dest_mac_address.nx_mac_address_h;
    *(ethernet_frame_ptr + 1) =  dest_mac_address.nx_mac_address_l;

    /** Build the sender's MAC access.  */
    *(ethernet_frame_ptr+2) =  (nx_record_ptr->ether_interface_ptr->nx_interface_physical_address_msw << 16) |
          (nx_record_ptr->ether_interface_ptr->nx_interface_physical_address_lsw >> 16);
    *(ethernet_frame_ptr+3) = (nx_record_ptr->ether_interface_ptr->nx_interface_physical_address_lsw << 16) ;

    /** Set the Ethernet frame type.  */
    *(ethernet_frame_ptr+3) |= ether_type;
    /** Endian swapping if necessary.  */
    /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr));
    /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+1));
    /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+2));
    /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
    NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+3));

    /** Put the frame on the wire.  */
    nx_status = nx_ether_send(packet_ptr, nx_record_ptr);
    NX_ETHERNET_ERROR_RETURN(((UINT)NX_SUCCESS == nx_status), SSP_ERR_INTERNAL);
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief   nx_ether_driver
 *
 * Ethernet driver function for Renesas Synergy.
 *
 * @param[in]  driver_req_ptr    : Pointer to driver request structure
 * @param[in]  nx_rec_ptr        : Pointer to Ethernet record structure
 * @param[in]  sf_el_nx_cfg_ptr  : sf_el_nx configuration structure pointer.
                                   This is similar to SSP configuration structure
 *
 * @retval     SSP_SUCCESS       : Call successful.
 * @retval     SSP_ERR_ASSERTION : One or more pointers point to NULL.
 * @return     See NX user manual for all possible return values.
***********************************************************************************************************************/
ssp_err_t nx_ether_driver(NX_IP_DRIVER *driver_req_ptr, NX_REC * nx_rec_ptr, sf_el_nx_cfg_t * sf_el_nx_cfg_ptr)
{
    UINT             status;
    INT              driver_command;
    ULONG           *ethernet_frame_ptr;
    NX_IP           *ip_ptr;
    NX_PACKET       *packet_ptr;
    NX_INTERFACE    *interface_ptr;
    nx_mac_address_t getmac = {0U};
    fmi_product_info_t *product_info_ptr = NULL;

    /**  Perform parameter checking */
#if (1 == SF_EL_NX_CFG_PARAM_CHECKING_ENABLE)
    /** Verify the pointers are valid */
    SSP_ASSERT (NULL != driver_req_ptr);
    SSP_ASSERT (NULL != driver_req_ptr->nx_ip_driver_ptr);
    SSP_ASSERT (NULL != nx_rec_ptr);
    SSP_ASSERT (NULL != sf_el_nx_cfg_ptr);
#endif

    /** Setup the IP pointer from the driver request.  */
    ip_ptr = driver_req_ptr->nx_ip_driver_ptr;

    /** Default to successful return.  */
    driver_req_ptr->nx_ip_driver_status =  (UINT) NX_SUCCESS;

    /** Extract driver command.  */
    driver_command = (INT) driver_req_ptr->nx_ip_driver_command;

    /** Process the driver request.  */
    switch (driver_command)
    {
        case NX_LINK_INTERFACE_ATTACH:
        {
            memset(nx_rec_ptr, 0, sizeof(NX_REC));

            /** Save the callback record pointer in the record structure. */
            nx_rec_ptr->p_callback_rec = sf_el_nx_cfg_ptr->p_callback_rec;

            /** Record the interface structure in the driver record. */
            nx_rec_ptr->ether_interface_ptr = driver_req_ptr->nx_ip_driver_interface;

            /** This command is used in multi homed devices - return NX_SUCCESS by default. */
            break;
        }
        case NX_LINK_INITIALIZE:
        {
            /** Process driver initialization.  */
            if (nx_rec_ptr->nx_state == (UINT) NX_NOT_INITIALIZED)
            {
                /**  Initialize BDs.  */
                status = nx_storage_init(ip_ptr, nx_rec_ptr);
                if (status != (UINT) NX_SUCCESS)
                {
                    nx_rec_ptr->nx_state = NX_CANNOT_INITSTORAGE;
                    driver_req_ptr->nx_ip_driver_status = (UINT) NX_NOT_SUCCESSFUL;
                    break;
                }

                /** Record the interface structure in the driver record.  */
                nx_rec_ptr->ether_interface_ptr = driver_req_ptr->nx_ip_driver_interface;

                interface_ptr = driver_req_ptr->nx_ip_driver_interface;

                /** Update associated channel information in driver record */
                nx_rec_ptr->channel = sf_el_nx_cfg_ptr->channel;

                /** Configure mac address.  */
                if ((NULL != nx_rec_ptr->p_callback_rec) && (NULL != nx_rec_ptr->p_callback_rec->nx_ether_mac_address_change_callback))
                {
                    /** Get MAC address from user.  */
                    nx_rec_ptr->p_callback_rec->nx_ether_mac_address_change_callback(&getmac);
                    nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_msw = getmac.nx_mac_address_h;
                    nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_lsw = getmac.nx_mac_address_l;
                }
                else
                {
                    nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_msw = sf_el_nx_cfg_ptr->nx_mac_address.nx_mac_address_h;
                    nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_lsw = sf_el_nx_cfg_ptr->nx_mac_address.nx_mac_address_l;
                }

                /** Detect S5D5 Mask Rev 02 to enable software padding for it*/

                /* Get factory flash information */
                g_fmi_on_fmi.productInfoGet(&product_info_ptr);

                /* Test for S5D5 Mask Rev. 2 to disable padding */
                if ((product_info_ptr->product_name[4] == (uint8_t) '5') &&
                    (product_info_ptr->product_name[5] == (uint8_t) 'D') &&
                    (product_info_ptr->product_name[6] == (uint8_t) '5') &&
                    ((uint8_t)(product_info_ptr->mask_revision) == 2U))
                {
                    g_hw_padding_enable = false;
                }
                else
                {
                    g_hw_padding_enable = true;
                }

                /** Initialize the ETHERC and E-DMAC hardware.  */
                status = nx_synergy_ethernet_init(nx_rec_ptr, sf_el_nx_cfg_ptr, g_hw_padding_enable);
                interface_ptr->nx_interface_physical_address_msw = nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_msw;
                interface_ptr->nx_interface_physical_address_lsw = nx_rec_ptr->ether_interface_ptr->nx_interface_physical_address_lsw;

                if (status != (UINT) NX_SUCCESS)
                {
                    nx_rec_ptr->nx_state = NX_CANNOT_INITHARDWARE ;
                    driver_req_ptr->nx_ip_driver_status =  (UINT) NX_NOT_SUCCESSFUL;
                }

                /** Setup the link maximum IP layer transfer unit.  */
                interface_ptr->nx_interface_ip_mtu_size = (ULONG) (NX_ENET_MAX_MTU - NX_ETHERNET_SIZE - NX_CRC_SIZE);
                interface_ptr->nx_interface_address_mapping_needed = (UCHAR) NX_TRUE;
                interface_ptr->nx_interface_link_up = (UCHAR) NX_FALSE;

                nx_rec_ptr->ip_ptr = driver_req_ptr->nx_ip_driver_ptr;
                nx_rec_ptr->nx_state = nx_rec_ptr->nx_state  | NX_INITIALIZE_DONE;
            }
            break;
        }
        case NX_LINK_ENABLE:
        {
            interface_ptr = driver_req_ptr->nx_ip_driver_interface;

            /** See if we can honor the NX_LINK_ENABLE request.
             * NX_ALREADY_ENABLED: Device has already been enabled  */
            if (!(nx_rec_ptr->nx_state & NX_INITIALIZE_DONE))
            {
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_NOT_SUCCESSFUL;
                break;
            }
            if (nx_rec_ptr->nx_state & NX_LINK_ENABLED)
            {
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_ALREADY_ENABLED;
            }
            else
            {
                nx_rec_ptr->nx_state = nx_rec_ptr->nx_state | NX_LINK_ENABLED ;
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_SUCCESS;
            }

            interface_ptr->nx_interface_link_up = (UCHAR) nx_rec_ptr->link_established;

            /** Enable the interrupts, at the interrupt controller and Ethernet controller.  */
            enet_hw_enable_interrupt(nx_rec_ptr);

            break;
        }
        case NX_LINK_DISABLE:
        {
            interface_ptr = driver_req_ptr->nx_ip_driver_interface;

            /** Make sure we are in the right state to do the NX_LINK_DISABLE.  */
            if ( !(nx_rec_ptr->nx_state & NX_LINK_ENABLED))
            {
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_NOT_ENABLED;
                break;
            }

            NVIC_DisableIRQ(nx_rec_ptr->irq);

            /** Disable receive and transmit.  */
            nx_rec_ptr->etherc_ptr->ECMR_b.RE = 0U;
            nx_rec_ptr->etherc_ptr->ECMR_b.TE = 0U;

            /** Clear link enabled flag.  */
            nx_rec_ptr->nx_state =  nx_rec_ptr->nx_state & ~(NX_LINK_ENABLED);

            /** Clear the enabled flag since there is no-one else.  */
            interface_ptr->nx_interface_link_up = (UCHAR) NX_FALSE;

            break;
        }

        case NX_LINK_PACKET_SEND:
        case NX_LINK_PACKET_BROADCAST:
        case NX_LINK_ARP_SEND:
        case NX_LINK_ARP_RESPONSE_SEND:
        case NX_LINK_RARP_SEND:
        {
            if ( !(nx_rec_ptr->nx_state & NX_LINK_ENABLED))
            {
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_NOT_ENABLED;

                /** Free the packet that we will not send.  */
                nx_packet_transmit_release(driver_req_ptr->nx_ip_driver_packet);
                break;
            }

            /** Process driver send packet.
             * Place the Ethernet frame at the front of the packet.  */
            packet_ptr =  driver_req_ptr->nx_ip_driver_packet;

            /** Adjust the prepend pointer to accommodate Ethernet header.  */
            packet_ptr->nx_packet_prepend_ptr =
                    packet_ptr->nx_packet_prepend_ptr - NX_ETHERNET_SIZE;

            /** Check if there is enough space in packet to append data.  */
            if (((ULONG)packet_ptr -> nx_packet_prepend_ptr - 2) < (ULONG)packet_ptr -> nx_packet_data_start)
            {
                /** Packet underflow. */
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_NOT_SUCCESSFUL;

                /** Free the packet as there is not enough space to append data in it.  */
                nx_packet_transmit_release(driver_req_ptr->nx_ip_driver_packet);
                break;
            }

            /** Adjust packet length.  */
            packet_ptr->nx_packet_length = packet_ptr->nx_packet_length + NX_ETHERNET_SIZE;

            /** Setup the prepend pointer in order to build the Ethernet frame. Backup another 2
             *  bytes to get 32-bit word alignment.  */
            ethernet_frame_ptr =  (ULONG*) (packet_ptr->nx_packet_prepend_ptr - 2);

            /** Build the actual Ethernet frame.  */
            if((driver_command == NX_LINK_PACKET_SEND) || (driver_command == NX_LINK_ARP_RESPONSE_SEND))
            {
                *ethernet_frame_ptr       =  driver_req_ptr->nx_ip_driver_physical_address_msw;
                *(ethernet_frame_ptr + 1) =  driver_req_ptr->nx_ip_driver_physical_address_lsw;
            }
            else
            {
                /** Use MAC broadcast for this frame.  */
                *ethernet_frame_ptr       =  0xFFFFU;          /* Broadcast!  */
                *(ethernet_frame_ptr + 1) =  0xFFFFFFFFU;
            }

            interface_ptr = nx_rec_ptr->ether_interface_ptr;

            /** Build the sender's MAC access.  */
            *(ethernet_frame_ptr+2) =  (interface_ptr->nx_interface_physical_address_msw << 16) |
                    (interface_ptr->nx_interface_physical_address_lsw >> 16);
            *(ethernet_frame_ptr+3) = (interface_ptr->nx_interface_physical_address_lsw << 16) ;

            /** Set the Ethernet frame type.  */
            if ((driver_command == NX_LINK_ARP_SEND)||
                    (driver_command == NX_LINK_ARP_RESPONSE_SEND))
            {
                *(ethernet_frame_ptr+3) |= NX_ETHERNET_ARP;
            }
            else if (driver_command == NX_LINK_RARP_SEND)
            {
                *(ethernet_frame_ptr+3) |= NX_ETHERNET_RARP;
            }
#ifdef FEATURE_NX_IPV6
            else if(packet_ptr->nx_packet_ip_version == NX_IP_VERSION_V6)
            {
                *(ethernet_frame_ptr + 3) |= NX_ETHERNET_IPV6;
            }
#endif
            else
            {
                *(ethernet_frame_ptr + 3) |= NX_ETHERNET_IP;
            }

            /** Endian swapping if necessary.  */
            /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr));
            /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+1));
            /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+2));
            /*LDRA_INSPECTED 496 S Use of NetX provided macro, which we can rely on. */
            NX_CHANGE_ULONG_ENDIAN(*(ethernet_frame_ptr+3));

            /** Put the frame on the wire.  */
            driver_req_ptr->nx_ip_driver_status =  nx_ether_send(packet_ptr, nx_rec_ptr);
            break;
        }

        case NX_LINK_MULTICAST_JOIN:
        case NX_LINK_MULTICAST_LEAVE:
        {
            /** Enable multicast.  */
            if ( !(nx_rec_ptr->nx_state & NX_LINK_ENABLED))
            {
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_NOT_ENABLED;
            }
            else
            {
                /** The device automatically receives multicast packets.
                 *  Nothing needs to be done. */
                driver_req_ptr->nx_ip_driver_status =  (UINT) NX_SUCCESS;
            }
            break;
        }

        case NX_LINK_GET_STATUS:
        {
            *(driver_req_ptr->nx_ip_driver_return_ptr) =  nx_rec_ptr->ether_interface_ptr->nx_interface_link_up;
            break;
        }

        case NX_LINK_DEFERRED_PROCESSING:
        {

            /** Process driver deferred requests.
             *  Process a device driver function on behalf of the IP thread. */
            nx_driver_deferred_processing(driver_req_ptr, nx_rec_ptr);

            break;
        }
#ifdef __PRODUCT_NETXDUO__
        case NX_LINK_INTERFACE_DETACH:
#endif
        case NX_LINK_UNINITIALIZE:
        {
            /* The return codes are not checked here because timer delete call cannot fail when called with
             * created RTOS objects. This object was successfully created during initialization of ethernet_init function. */
            tx_timer_delete(&nx_rec_ptr->driver_event_polling_timer);
            /** Un-Initialize the ETHERC, E-DMAC and EPTPC hardware */
            status = nx_synergy_ethernet_deinit(nx_rec_ptr, sf_el_nx_cfg_ptr);
            nx_storage_deinit(nx_rec_ptr);
            break;
        }

        default:
        {
            /** Return the unhandled command status.  */
            driver_req_ptr->nx_ip_driver_status =  (UINT) NX_UNHANDLED_COMMAND;

            /** Return NULL in the supplied return pointer.  */
            *(driver_req_ptr->nx_ip_driver_return_ptr) = 0;
            break;
        }
    }
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief   nx_ether_interrupt
 *
 * This function is the main Renesas Ethernet interrupt handler.
 *
 * @param[in]  nx_rec_ptr        : Pointer to Ethernet record structure
***********************************************************************************************************************/
void nx_ether_interrupt(NX_REC *nx_rec_ptr)
{
    uint32_t  reesr;
    uint32_t  ecsr;
    R_ETHERC0_Type *etherc;
    R_EDMAC0_Type  *edmac;

    if (NULL == nx_rec_ptr)
    {
        return;
    }

    etherc = nx_rec_ptr->etherc_ptr;
    edmac  = nx_rec_ptr->edmac_ptr;

    /** Read EDMAC and EtherC status register.  */
    reesr = edmac->EESR;
    ecsr  = etherc->ECSR;

    /** Clear all interrupts.  */
    edmac->EESR  = reesr;
    etherc->ECSR = ecsr;

    /** Frame transmit completed interrupt
     * The MAC sets Transmit Complete flag only if all frames are transmitted.  This creates a delay in processing transmitted frames.
     * The work around: this driver process transmitted packets when frame receive interrupt occurs. This way the transmitted packets
     * can be released before processing received packets, reducing the delay.  */
    if (reesr & (EESR_TC | EESR_FR))
    {
        nx_tx_interrupt(nx_rec_ptr);
    }

    /** Frame received.  */
    if (reesr & EESR_FR)
    {
        nx_rx_interrupt(nx_rec_ptr);
    }

    /** Special case for ECI interrupt + link change.  */
    if (reesr & EESR_ECI)
    {
        /** To report link status changes to the application, add a semaphore put or event flag set to the statement below.  */
        if (etherc->PSR_b.LMON == 0U)
        {
            /** Link present.  */
            nx_rec_ptr->ether_interface_ptr->nx_interface_link_up = (UCHAR) NX_TRUE;
            nx_rec_ptr->nx_state = nx_rec_ptr->nx_state | NX_LINK_ENABLED;
        }
        else
        {
            nx_rec_ptr->ether_interface_ptr->nx_interface_link_up = (UCHAR) NX_FALSE;
            nx_rec_ptr->nx_state = nx_rec_ptr->nx_state & ~(NX_LINK_ENABLED);
        }

#if (1 == NX_IP_HARDWARE_INT_CALLBACK_ENABLED)
        UINT interface_index = 0;

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
    }

    return;
}   /* nx_ether_interrupt() */

/*******************************************************************************************************************//**
 * @brief   nx_ether_send
 *
 * This function decides whether to queue or send the packet
 *
 * @param[in]  packet            : Pointer to the data packet
 * @param[in]  nx_rec_ptr        : Pointer to Ethernet record structure
 *
 * @return     NX_SUCCESS        : Success
 * @return     NX_TX_QUEUE_DEPTH : Queue depth
 * @return     NX_NOT_ENABLED    : Link is not enabled
 * @return     See NX user manual for all possible return values
***********************************************************************************************************************/
static UINT nx_ether_send(NX_PACKET *packet, NX_REC *nx_rec_ptr)
{
    /** Check if the link is enabled.  */
    if (nx_rec_ptr->nx_state & NX_LINK_ENABLED)
    {
        /** Call write function to send the packet, then check the result.  */
        if (nx_ether_write(packet, nx_rec_ptr) != (UINT) NX_SUCCESS)
        {
            /** Transmit failed, drop the packet.  */
            NX_BACKOFF_RELEASE(packet);
            return (UINT) NX_TX_QUEUE_DEPTH;
        }
    }
    else
    {
        NX_BACKOFF_RELEASE(packet);
        return (UINT) NX_NOT_ENABLED;
    }

    return NX_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief   nx_ether_write
 *
 * This function writes the data to the TX FIFO buffer descriptor.
 * This function can only be called when there is no active transmission.
 *
 * @param[in]  packet             : Pointer to the data packet
 * @param[in]  nx_rec_ptr         : Pointer to Ethernet record structure
 *
 * @return     NX_SUCCESS         : Success
 * @return     NX_NO_MORE_ENTRIES : No more entries(BD's) allowed
 * @return     See NX user manual for all possible return values
 * @note       The pointer parameters passed to this function are already validated at higher level.
***********************************************************************************************************************/
static UINT nx_ether_write(NX_PACKET *packet, NX_REC *nx_rec_ptr)
{
    NX_PACKET           *current_packet;
    INT                 j;
    INT                 length;
    UCHAR               *new_prepend_ptr;
    UCHAR               *new_append_ptr;
    ULONG               bd_index;
    ULONG               bd_count;
    TX_INTERRUPT_SAVE_AREA

    current_packet = packet;

    bd_index = nx_rec_ptr->driver_tx_bd_index;
    bd_count = 0U;

    while (current_packet)
    {
        /** Check if the BD is free.  */
        if (nx_rec_ptr->driver_tx_bd[bd_index].bd_status & DESCRIPTOR_FLAG_ACTIVE)
        {
            return (UINT) NX_NO_MORE_ENTRIES;
        }

        /** Set up a transfer descriptor for each NX_PACKET in the packet chain.  */
        length = current_packet->nx_packet_append_ptr - current_packet->nx_packet_prepend_ptr;

        /** Special case: alignment requirement to 5 bits for data 16 bytes or less.
         * To accommodate this special case, NX_PACKETs need to be minimum 32+16 in size.  */
        if (length < 17)
        {
            /** Check alignment for packets 16 bytes or less.  */
            if( ((ULONG)(current_packet->nx_packet_prepend_ptr)) & 0x1F)
            {
                new_prepend_ptr = (UCHAR*) (((ULONG)(current_packet->nx_packet_prepend_ptr) + 0x1F) & 0xFFFFFFE0) ;
                new_append_ptr = current_packet->nx_packet_append_ptr + (new_prepend_ptr - current_packet->nx_packet_prepend_ptr) ;

                /** Loop from back to front to copy data in the NX_PACKET.  */
                for(j = length ; j > 0 ; j-- )
                {
                    new_prepend_ptr[j-1] = current_packet->nx_packet_prepend_ptr [j-1];
                }

                /** Set new prepend and append pointers.  */
                current_packet->nx_packet_prepend_ptr = new_prepend_ptr;
                current_packet->nx_packet_append_ptr = new_append_ptr;
            }
        }

        /** Set up the transmit BD.  */
        nx_rec_ptr->driver_tx_bd[bd_index].bd_buffer_ptr = current_packet->nx_packet_prepend_ptr;
        nx_rec_ptr->driver_tx_bd[bd_index].bd_bufsize = (USHORT) length ;

        if (current_packet->nx_packet_next)
        {
            if (bd_index == (ULONG) (NUM_TX_DESC - 1))
            {
                nx_rec_ptr->driver_tx_bd[bd_index].bd_status =  DESCRIPTOR_FLAG_LAST;
            }
            else
            {
                nx_rec_ptr->driver_tx_bd[bd_index].bd_status =  0U;
            }
        }
        else
        {
            /** This was the last packet in the chain.
             * Store the original packet pointer in the last BD.  */
            nx_rec_ptr->driver_tx_bd[bd_index].bd_nx_packet = packet;

            if (bd_index == (ULONG) (NUM_TX_DESC - 1))
            {
                nx_rec_ptr->driver_tx_bd[bd_index].bd_status =  FRAME_POSITION_END | DESCRIPTOR_FLAG_LAST;
            }
            else
            {
                nx_rec_ptr->driver_tx_bd[bd_index].bd_status =   FRAME_POSITION_END;
            }
        }

        if (current_packet == packet)
        {
            nx_rec_ptr->driver_tx_bd[bd_index].bd_status |= FRAME_POSITION_HEAD;
        }
        else
        {
            /* Do nothing here */
        }

        /** Get next NetX packet in the chain.  */
        current_packet =  current_packet->nx_packet_next;

        bd_count++;

        /** Move to next BD.  */
        bd_index =  ((bd_index + 1U) % (ULONG) NUM_TX_DESC);

    } /* End of while loop.  */

    /** Disable interrupts.  */
    TX_DISABLE

    nx_rec_ptr->driver_tx_bd_in_use += bd_count;

    /** Restore interrupts.  */
    TX_RESTORE

    nx_rec_ptr->driver_tx_bd_index = bd_index;

    /** Set active flag to indicate BDs are ready.  */
    for (; bd_count > 0U; bd_count--)
    {
        /** Set active flag in reverse order, move to previous BD.  */
        bd_index =  ((bd_index - 1U) % (ULONG) NUM_TX_DESC);

        /** Set active flag.  */
        nx_rec_ptr->driver_tx_bd[bd_index].bd_status |= DESCRIPTOR_FLAG_ACTIVE;
    }

    /** If the transmission is suspended, resume transmission.  */
    if (nx_rec_ptr->edmac_ptr->EDTRR_b.TR == 0U)
    {
        /** Start transmission. */
        nx_rec_ptr->edmac_ptr->EDTRR_b.TR = 1U;
    }

    return NX_SUCCESS;
} /* nx_ether_write */

/*******************************************************************************************************************//**
 * @brief   nx_storage_init
 *
 * This function initializes byte pool, packet pool, list of descriptors and Tx & Rx FIFO.
 *
 * @param[in]  ip_ptr           Pointer to the data packet
 * @param[in]  nx_rec_ptr       Pointer to Ethernet record structure
 *
 * @return     NX_SUCCESS       Successful send
 * @return     NX_NO_PACKET     No packet available
 * @return     NX_WAIT_ABORTED  Requested suspension was aborted by a call to tx_thread_wait_abort.
 * @return     See NX user manual for all possible return values
 * @note       The pointer parameters passed to this function are already validated at higher level.
 * @note       The number of receive buffer descriptors NUM_RX_DESC is a user configurable value which is present in
               sf_el_nx_cfg header file and it defaults to 8.
***********************************************************************************************************************/
static UINT nx_storage_init(NX_IP *ip_ptr, NX_REC *nx_rec_ptr)
{
    UINT i;
    UINT status;

    NX_PACKET *packet_ptr = NULL;

    /** All memory is assumed to be allocated at link time in the .bss section
     *  to avoid memory allocation errors at runtime.  */

    /**  16 byte alignment for BDs.  */
    nx_rec_ptr->driver_rx_bd = (EMAC_BD*) (((ULONG)nx_rec_ptr->rx_bd_space + 0xFU) & 0xFFFFFFF0U);
    nx_rec_ptr->driver_tx_bd = (EMAC_BD*) (((ULONG)nx_rec_ptr->tx_bd_space + 0xFU) & 0xFFFFFFF0U);

    /** Initialize receive buffer descriptors.  */
    for(i = 0U; i < (UINT) NUM_RX_DESC; i++)
    {
        /** Allocate packets from the pool to be used for reception.  */
        status = nx_packet_allocate(ip_ptr->nx_ip_default_packet_pool, &packet_ptr, NX_RECEIVE_PACKET, 0);

        /** Check if the allocation was successful.  */
        if(status)
        {
            /** Release allocated packets.  */
            while(i)
            {
                i--;
                nx_packet_release(nx_rec_ptr->driver_rx_bd[i].bd_nx_packet);
            }
            return status;
        }

        /** Set buffer length, payload address (32byte align), initial status.  */
        packet_ptr->nx_packet_prepend_ptr = (UCHAR *) (ULONG)(((ULONG)(packet_ptr->nx_packet_prepend_ptr ) + 0x1FU) & (0xFFFFFFE0U));

        nx_rec_ptr->driver_rx_bd[i].bd_nx_packet = packet_ptr;

        nx_rec_ptr->driver_rx_bd[i].bd_buffer_ptr = packet_ptr->nx_packet_prepend_ptr;

        /** Length has to be multiple of 32.  */
        nx_rec_ptr->driver_rx_bd[i].bd_bufsize = (USHORT)((UINT) (nx_rec_ptr->driver_rx_bd[i].bd_nx_packet->nx_packet_data_end   - (nx_rec_ptr->driver_rx_bd[i].bd_nx_packet->nx_packet_prepend_ptr)) & (0xFFFFFFE0U)) ;

        nx_rec_ptr->driver_rx_bd[i].bd_status = DESCRIPTOR_FLAG_ACTIVE;
    }

    /** Set DL bit in last descriptor.  */
    nx_rec_ptr->driver_rx_bd[(UINT) NUM_RX_DESC - 1U].bd_status |= DESCRIPTOR_FLAG_LAST;

    return NX_SUCCESS;
}   /* nx_storage_init */

/*******************************************************************************************************************//**
 * @brief   nx_storage_deinit
 *
 * This function performs the release of receiver descriptor packets.
 *
 * @param[in]  nx_rec_ptr    : Pointer to Ethernet record structure.
***********************************************************************************************************************/
static void nx_storage_deinit(NX_REC * nx_rec_ptr)
{
    UINT i;

    /** Free the packets used as receive buffer descriptors */
    for (i = 0U; i < (UINT) NUM_RX_DESC; i++)
    {
        nx_packet_release(nx_rec_ptr->driver_rx_bd[i].bd_nx_packet);
    }
}

/*******************************************************************************************************************//**
 * @brief   nx_tx_interrupt
 *
 * This function handles the Tx done interrupt.
 *
 * @param[in]  nx_rec_ptr      : Pointer to Ethernet record structure
 * @note       The pointer parameter passed to this function is already validated at the higher level.
***********************************************************************************************************************/
static void nx_tx_interrupt(NX_REC *nx_rec_ptr)
{
    ULONG num_of_bd_in_use;

    if (NULL == nx_rec_ptr->ip_ptr)
    {
        return;
    }

    num_of_bd_in_use = nx_rec_ptr->driver_tx_bd_in_use;

    /** Loop through buffers in use.  */
    while (num_of_bd_in_use)
    {
        num_of_bd_in_use--;

        /** Check if the BD is in non active state.  */
        if ((nx_rec_ptr->driver_tx_bd[nx_rec_ptr->driver_tx_release_index].bd_status & DESCRIPTOR_FLAG_ACTIVE) == 0)
        {
            /** Check if this is the last BD in a chain.  */
            if ((nx_rec_ptr->driver_tx_bd[nx_rec_ptr->driver_tx_release_index].bd_status & FRAME_POSITION_END))
            {
                /** Release packet transmitted.  */
                NX_BACKOFF_RELEASE(nx_rec_ptr->driver_tx_bd[nx_rec_ptr->driver_tx_release_index].bd_nx_packet);
            }
            nx_rec_ptr->driver_tx_bd_in_use = num_of_bd_in_use;

            /** Move to next BD.  */
            nx_rec_ptr->driver_tx_release_index =  ((nx_rec_ptr->driver_tx_release_index + 1U) % (ULONG) NUM_TX_DESC);
        }
        else
        {
            break;
        }
    }
}  /* nx_tx_interrupt */

/*******************************************************************************************************************//**
 * @brief   nx_rx_interrupt
 *
 * This function handles the receive interrupt.
 *
 * @param[in]  nx_rec_ptr      : Pointer to Ethernet record structure
 * @note       The pointer parameter passed to this function is already validated at the higher level.
***********************************************************************************************************************/
static void nx_rx_interrupt(NX_REC *nx_rec_ptr)
{
    ULONG       i;
    ULONG       j;
    ULONG       index;
    ULONG       status;
    ULONG       bd_index;
    ULONG       rxbd_status;
    ULONG       bds_in_chain = 1U;
    ULONG       remaining_bytes;

    NX_PACKET   *packet_ptr;
    NX_PACKET   *last_packet_ptr;
    NX_PACKET   *allocated_packet_ptr = NULL;

    if (NULL == nx_rec_ptr->ip_ptr)
    {
        return;
    }

    /** Do for every complete frame in the receive buffer. */
    /** Each frame consists of 1 or more BDs. */
    /** The driver should be able to process at least 1 completed frame when it gets here. */
    rxbd_status = nx_rec_ptr->driver_rx_bd[nx_rec_ptr->driver_rx_bd_index].bd_status;

    do
    {
        ULONG last_descriptor_index = ((nx_rec_ptr->driver_rx_bd_index + bds_in_chain) - 1U) % (ULONG) NUM_RX_DESC;

        /** Get the number of BDs in this receive chain:
         *  Last BD has FRAME_POSITION_END set, BDs preceding should have DESCRIPTOR_FLAG_ACTIVE bit cleared.
         *  The value of FRAME_POSITION_END is 0x10000000U
         *  The value of TX/RX DESCRIPTOR_FLAG_ACTIVE is 0x80000000U */
        while (!(rxbd_status & FRAME_POSITION_END))
        {
            if(rxbd_status & DESCRIPTOR_FLAG_ACTIVE)
            {
                /** This frame has a BD that is still active and is not fully received, break out here.  */
                return;
            }

            /** Move to next BD.  */
            bds_in_chain++;
            last_descriptor_index = ((nx_rec_ptr->driver_rx_bd_index + bds_in_chain) - 1U) % (ULONG) NUM_RX_DESC;
            rxbd_status = nx_rec_ptr->driver_rx_bd[last_descriptor_index].bd_status;
        }

        /** Get first packet from first BD.  */
        packet_ptr = nx_rec_ptr->driver_rx_bd[nx_rec_ptr->driver_rx_bd_index].bd_nx_packet;

        /** Store the total frame length from the last BD + the padding in the NetX packet.  */
        remaining_bytes = nx_rec_ptr->driver_rx_bd[last_descriptor_index].bd_rxdatalength + NX_PADDING_SIZE;
        packet_ptr->nx_packet_length = remaining_bytes;

        /** Get the last packet in the chain, we need this to build NetX packet chain.  */
        last_packet_ptr = nx_rec_ptr->driver_rx_bd[last_descriptor_index].bd_nx_packet;

        /** For each packet in the chain, except the last one: */
        for (i = 0U; i < (bds_in_chain - 1U); i++)
        {
            index = (nx_rec_ptr->driver_rx_bd_index + i) % (ULONG) NUM_RX_DESC;

            /** Adjust the append pointer according to the size in BD.  */
            nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_append_ptr =  nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr + nx_rec_ptr->driver_rx_bd[index].bd_bufsize;

            /** Update remaining_bytes. */
            remaining_bytes =  remaining_bytes - nx_rec_ptr->driver_rx_bd[index].bd_bufsize;

            /** Chain packet (set _next pointer). */
            nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_next = nx_rec_ptr->driver_rx_bd[(index + 1U) % (ULONG) NUM_RX_DESC].bd_nx_packet;

            /** Set nx_packet_last pointer to point to last packet in chain.  */
            nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_last = last_packet_ptr;

            /** Insert a new NetX packet into the BD.  */
            status = nx_packet_allocate(nx_rec_ptr->ip_ptr->nx_ip_default_packet_pool , &allocated_packet_ptr , NX_RECEIVE_PACKET, NX_NO_WAIT);

            if (status == (UINT) NX_SUCCESS)
            {
                nx_rec_ptr->driver_rx_bd[index].bd_nx_packet = allocated_packet_ptr;

                /** Set buffer length and address and align the address to 32 bytes boundary.  */
                nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr = (UCHAR*) (ULONG)(((ULONG)(nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr ) + 0x1F ) & (0xFFFFFFE0));
                nx_rec_ptr->driver_rx_bd[index].bd_buffer_ptr  = nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr ;

                /** Make the size to be multiple of 32.  */
                nx_rec_ptr->driver_rx_bd[index].bd_bufsize = (USHORT)((UINT) (nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_data_end - nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr) & (0xFFFFFFE0U)) ;

                nx_rec_ptr->driver_rx_bd[index].bd_rxdatalength = 0U;

                if (index == (ULONG) (NUM_RX_DESC - 1))
                {
                    nx_rec_ptr->driver_rx_bd[index].bd_status  =  DESCRIPTOR_FLAG_LAST | DESCRIPTOR_FLAG_ACTIVE;
                }
                else
                {
                    nx_rec_ptr->driver_rx_bd[index].bd_status  =  DESCRIPTOR_FLAG_ACTIVE ;
                }
            }
            else
            {
                /** Check if we had some successful allocation.  */
                if (i > 0U)
                {
                    /** Release the packet.  */
                    nx_rec_ptr->driver_rx_bd[(index - 1U) % (ULONG) NUM_RX_DESC].bd_nx_packet->nx_packet_next = NX_NULL;
                    nx_packet_release(packet_ptr);
                }

                /** Set the BDs to active.  */
                for (j = i; j < bds_in_chain; j++)
                {
                    index = (nx_rec_ptr->driver_rx_bd_index + j) % (ULONG) NUM_RX_DESC;

                    /** Set buffer length and address.
                     *  Align to 32 byte address */
                    nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr = (UCHAR*)
                        (((ULONG)(nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr ) + 0x1F ) & (0xFFFFFFE0));
                    nx_rec_ptr->driver_rx_bd[index].bd_buffer_ptr  = nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr ;

                    /** Make the size to be multiple of 32.  */
                    nx_rec_ptr->driver_rx_bd[index].bd_bufsize = (USHORT) ((UINT) (nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_data_end - nx_rec_ptr->driver_rx_bd[index].bd_nx_packet->nx_packet_prepend_ptr) & (0xFFFFFFE0U)) ;

                    nx_rec_ptr->driver_rx_bd[index].bd_rxdatalength = 0U;

                    if (index == (ULONG) (NUM_RX_DESC - 1))
                    {
                        nx_rec_ptr->driver_rx_bd[index].bd_status  =  DESCRIPTOR_FLAG_LAST | DESCRIPTOR_FLAG_ACTIVE;
                    }
                    else
                    {
                        nx_rec_ptr->driver_rx_bd[index].bd_status  =  DESCRIPTOR_FLAG_ACTIVE ;
                    }
                }
                break;
            }
        }

        /** Check if we had allocation failures.  */
        if (i < (bds_in_chain - 1U))
        {
            /** Advance the index for next reception.  */
            nx_rec_ptr->driver_rx_bd_index = (nx_rec_ptr->driver_rx_bd_index + bds_in_chain) % (ULONG) NUM_RX_DESC;
            break;
        }

        /** Last BD or packet.  */
        /** Set append pointer for the last packet.  */
        last_packet_ptr->nx_packet_append_ptr = last_packet_ptr->nx_packet_prepend_ptr + remaining_bytes;

        /** Clear _next pointer. */
        last_packet_ptr->nx_packet_next = NX_NULL;

        /** Insert a new NetX packet into the BD.  */
        bd_index = ((nx_rec_ptr->driver_rx_bd_index + bds_in_chain) - 1U) % (ULONG) NUM_RX_DESC;

        status = nx_packet_allocate(nx_rec_ptr->ip_ptr->nx_ip_default_packet_pool , &allocated_packet_ptr , NX_RECEIVE_PACKET, NX_NO_WAIT);

        if (status == (UINT) NX_SUCCESS)
        {
            /** Send the packet up the stack.  */
            packet_ptr->nx_packet_ip_interface = nx_rec_ptr->ether_interface_ptr;
            nx_receive( nx_rec_ptr, packet_ptr);
        }
        else
        {
            /** Allocation failed.  Assign last packet to the BD. */
            allocated_packet_ptr = last_packet_ptr;
        }

        /** Prepare the BD for receiving data.  */
        nx_rec_ptr->driver_rx_bd[bd_index].bd_nx_packet = allocated_packet_ptr;

        /** Set buffer length and address align to 32 byte address.  */
        allocated_packet_ptr->nx_packet_prepend_ptr = (UCHAR*) (ULONG)(((ULONG)(allocated_packet_ptr->nx_packet_prepend_ptr ) + 0x1F ) & (0xFFFFFFE0));
        nx_rec_ptr->driver_rx_bd[bd_index].bd_buffer_ptr  = allocated_packet_ptr->nx_packet_prepend_ptr ;

        /** Make the size to be multiple of 32.  */
        nx_rec_ptr->driver_rx_bd[bd_index].bd_bufsize = (USHORT)((UINT) (allocated_packet_ptr->nx_packet_data_end - allocated_packet_ptr->nx_packet_prepend_ptr) & (0xFFFFFFE0U));

        nx_rec_ptr->driver_rx_bd[bd_index].bd_rxdatalength = 0U;

        if (bd_index == (ULONG) (NUM_RX_DESC - 1))
        {
            nx_rec_ptr->driver_rx_bd[bd_index].bd_status  =  DESCRIPTOR_FLAG_LAST | DESCRIPTOR_FLAG_ACTIVE;
        }
        else
        {
            nx_rec_ptr->driver_rx_bd[bd_index].bd_status  = DESCRIPTOR_FLAG_ACTIVE;
        }

        /** advance the index for next reception */
        nx_rec_ptr->driver_rx_bd_index = (nx_rec_ptr->driver_rx_bd_index + bds_in_chain) % (ULONG) NUM_RX_DESC;

        rxbd_status = nx_rec_ptr->driver_rx_bd[nx_rec_ptr->driver_rx_bd_index].bd_status;

    } while (!(rxbd_status & DESCRIPTOR_FLAG_ACTIVE));

    /** If reception is in suspended state, resume it.  */
    if (nx_rec_ptr->edmac_ptr->EDRRR_b.RR == 0U)
    {
        /** Resume reception.  */
        nx_rec_ptr->edmac_ptr->EDRRR_b.RR = 1U;
    }
}   /** nx_rx_interrupt() */

/*******************************************************************************************************************//**
 * @brief   nx_receive
 *
 * This function processing incoming packets.  This routine would be called from the receive packet ISR.
 *
 * @param[in]  nx_rec_ptr    : Pointer to Ethernet record structure
 * @param[in]  packet_ptr    : Packet pointer
 * @note       The pointer parameters passed to this function are already validated at higher level.
***********************************************************************************************************************/
static void nx_receive(NX_REC *nx_rec_ptr, NX_PACKET *packet_ptr)
{
    USHORT    packet_type;

    /** Pickup the packet header to determine where the packet needs to be sent.  */
    packet_type =  (USHORT) ((((UINT) (*(packet_ptr->nx_packet_prepend_ptr + 12))) << 8)
                    | ((UINT) (*(packet_ptr->nx_packet_prepend_ptr + 13))));

    /* If EDMAC padding is not enabled - implement it in software */
    if (false == g_hw_padding_enable)
    {
        /* The payload needs to be shifted up by NX_PADDING_SIZE. To do this we will start at the end and work back
         * to start so we do not lose data. */

        /* Round up the payload length to the nearest 32 byte boundary. The '- 4' is used because below we will be
         * transferring data 4 bytes at time and '- 4' moves the pointer from pointing to memory after the payload to
         * the last 4 bytes of the payload. */
        ULONG data_len_32byte_multiple = (((packet_ptr->nx_packet_length - NX_ETHERNET_SIZE - NX_PADDING_SIZE) + 31) & ~31U) - 4;

        /* Points to the last 4 bytes of the payload as it sits in memory now. */
        register ULONG * p_src = (ULONG *) (packet_ptr->nx_packet_prepend_ptr + data_len_32byte_multiple + NX_ETHERNET_SIZE);
        /* Points to the new address where last 4 bytes of payload will be stored. */
        register ULONG * p_dst = (ULONG *) (packet_ptr->nx_packet_prepend_ptr + data_len_32byte_multiple + NX_ETHERNET_SIZE + NX_PADDING_SIZE);
        /* Points to the start of the payload as it sits in memory now. */
        register ULONG * p_end = (ULONG *) (packet_ptr->nx_packet_prepend_ptr + NX_ETHERNET_SIZE);

        /* Transfer 32 bytes per iteration. Address was moved to 32 byte boundary previously. */
        do
        {
            *p_dst = *p_src;
            p_dst--;
            p_src--;
            *p_dst = *p_src;
            p_dst--;
            p_src--;
            *p_dst = *p_src;
            p_dst--;
            p_src--;
            *p_dst = *p_src;
            p_dst--;
            p_src--;
            *p_dst = *p_src;
            p_dst--;
            p_src--;
            *p_dst = *p_src;
            p_dst--;
            p_src--;
            *p_dst = *p_src;
            p_dst--;
            p_src--;
            *p_dst = *p_src;
            p_dst--;
            p_src--;
        } while (p_end < p_src);

    }

    /** Clean off the Ethernet header.  */
    packet_ptr->nx_packet_prepend_ptr =  packet_ptr->nx_packet_prepend_ptr + NX_ETHERNET_SIZE + NX_PADDING_SIZE;

    /** Adjust the packet length.  */
    packet_ptr->nx_packet_length =  packet_ptr->nx_packet_length - NX_ETHERNET_SIZE - NX_PADDING_SIZE;

    /** Route the incoming packet according to its Ethernet type.  */
#ifdef FEATURE_NX_IPV6
    if (packet_type == NX_ETHERNET_IP || packet_type == NX_ETHERNET_IPV6)
#else
    if (packet_type == NX_ETHERNET_IP)
#endif
    {
        _nx_ip_packet_deferred_receive(nx_rec_ptr->ip_ptr, packet_ptr);
    }
    else if (packet_type == NX_ETHERNET_ARP)
    {
        _nx_arp_packet_deferred_receive(nx_rec_ptr->ip_ptr, packet_ptr);
    }
    else if (packet_type == NX_ETHERNET_RARP)
    {
        _nx_rarp_packet_deferred_receive(nx_rec_ptr->ip_ptr, packet_ptr);
    }
    else
    {
        /** Call the callback for unsupported packet type, if defined. */
        if ((NULL != nx_rec_ptr->p_callback_rec) && (NULL != nx_rec_ptr->p_callback_rec->nx_ether_unknown_packet_receive_callback))
        {
            nx_rec_ptr->p_callback_rec->nx_ether_unknown_packet_receive_callback(packet_ptr, packet_type);
        }
        else
        {
            /** If Ethernet header id invalid, release the packet.  */
            nx_packet_release(packet_ptr);
        }
    }
    return ;
}

/*******************************************************************************************************************//**
 * @brief   nx_driver_deferred_processing
 *
 * This function processing the deferred action within the context of the IP thread.
 *
 * @param[in]  driver_req_ptr : Driver command from the IP thread
 * @param[in]  nx_rec_ptr     : Pointer to Ethernet record structure
***********************************************************************************************************************/
static void nx_driver_deferred_processing(NX_IP_DRIVER *driver_req_ptr, NX_REC *nx_rec_ptr)
{
    /** Check if we need to poll PHY status.  */
    if (nx_rec_ptr->nx_driver_phy_polling_requested == (UINT) NX_TRUE)
    {
        /** If so, clear the polling flag.  */
        nx_rec_ptr->nx_driver_phy_polling_requested = (UINT) NX_FALSE;

        nx_driver_event_handler(nx_rec_ptr);
    }

    /** Mark request as successful.  */
    driver_req_ptr->nx_ip_driver_status =  (UINT) NX_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief   Retrieve the API version number.
 *
 * @retval  SSP_SUCCESS        Successful return.
 * @retval  SSP_ERR_ASSERTION  The parameter p_version is NULL.
***********************************************************************************************************************/
ssp_err_t nx_ethernet_version_get(ssp_version_t * const p_version)
{
#if (1 == SF_EL_NX_CFG_PARAM_CHECKING_ENABLE)
    /** Verify parameters are valid */
    SSP_ASSERT(NULL != p_version);
#endif
    /** Return the version number */
    p_version->version_id =  s_sf_el_nx_version.version_id;

    return SSP_SUCCESS;
}

/***********************************************************************//**
 * @} (end addtogroup NX_SYNERGY_PORT)
 **************************************************************************/
