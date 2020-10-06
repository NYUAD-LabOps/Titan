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

/**************************************************************************
 * File: nx_renesas.h
 *
 *  Includes all the ethernet device driver related header files in one 
 *  place. This file will be only one that needs to be included into any
 *  ethernet device driver code file.
 *
 *  It may need to be changed to add or delete features.
 **************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup NX_SYNERGY_PORT
 * @{
***********************************************************************************************************************/

#ifndef     NX_RENESAS_H
#define     NX_RENESAS_H


#ifdef   __cplusplus
/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {
#endif

#include "sf_el_nx_cfg.h"

/** Version of the API defined in this file */
#define SF_EL_NX_API_VERSION_MAJOR (1U)
#define SF_EL_NX_API_VERSION_MINOR (3U)

/** Version of code that implements the API defined in this file */
#define SF_EL_NX_CODE_VERSION_MAJOR (1U)
#define SF_EL_NX_CODE_VERSION_MINOR (12U)

/** Determine if the driver uses IP deferred processing or direct ISR processing.  */
#define  MAX_ENET_INSTANCES  2

/** Netx packets queued before dropping */
#define  TX_QUEUE_DEPTH    10
#define  NX_PADDING_SIZE   2U

/** Max Ethernet packet size (14+ 1500 +2) */
#ifndef NX_ENET_MAX_MTU
#define NX_ENET_MAX_MTU     1518U
#endif

#define NX_NOT_INITIALIZED       0x0000U
/** Device is Initialized */
#define NX_INITIALIZE_DONE       0x0002U
/** Device is enabled*/
#define NX_LINK_ENABLED          0x0004U

#define NX_CANNOT_INITSTORAGE    0x0001U
#define NX_CANNOT_INITHARDWARE   0x0002U

#define NX_DRIVER_EVENT_POLLING_INTERVAL_SHORT   500U
#define NX_DRIVER_EVENT_POLLING_INTERVAL_LONG    1000U

#define NX_BACKOFF_RELEASE(p)   \
{   \
    (p) -> nx_packet_prepend_ptr =  (p) -> nx_packet_prepend_ptr + NX_ETHERNET_SIZE;  \
    (p) -> nx_packet_length =  (p) -> nx_packet_length - NX_ETHERNET_SIZE;    \
    nx_packet_transmit_release((p)); \
}
    
/** Descriptor structure */
typedef struct EMAC_BD_STRUCT
{
    uint32_t            bd_status;         ///< Status - 32 bits
    uint16_t            bd_rxdatalength;   ///< data length
    uint16_t            bd_bufsize;        ///< Buffer size
    uint8_t            *bd_buffer_ptr;     ///< Buffer pointer
    NX_PACKET          *bd_nx_packet;      ///< Padding used to associate descriptor with nx_packet
} EMAC_BD;

/** MAC Address structure */
typedef struct st_nx_mac_address
{
    ULONG nx_mac_address_h;
    ULONG nx_mac_address_l;
} nx_mac_address_t;

/** SF_EL_NX callback record structure */
typedef struct NX_CALLBACK_REC_STRUCT
{
    void (*nx_ether_unknown_packet_receive_callback) (NX_PACKET *packet_ptr, USHORT packet_type);
    void (*nx_ether_mac_address_change_callback) (nx_mac_address_t *p_mac_config);
}NX_CALLBACK_REC;

/** sf_el_nx configuration structure is similar to SSP configuration structure.
 * This is collection of parameters for this module that is required for an instance initialization.
 */
typedef struct st_sf_el_nx_cfg
{
    uint16_t          channel;
    nx_mac_address_t  nx_mac_address;
    R_ETHERC0_Type  * etherc_ptr;
    R_EDMAC0_Type   * edmac_ptr;
    NX_CALLBACK_REC * p_callback_rec;
}sf_el_nx_cfg_t;

/** Bits in the status word of the tx / rx descriptors */
#define DESCRIPTOR_FLAG_ACTIVE      0x80000000U  /* TACT/RACT - Tx/Rx descriptor active */
#define DESCRIPTOR_FLAG_LAST        0x40000000U  /* TDLE/RDLE - Tx/RX descriptor list last */
#define FRAME_POSITION_HEAD_END     0x30000000U  /* Define Frame Position bits */
#define FRAME_POSITION_HEAD         0x20000000U  /* Define Frame Position -- Head */
#define FRAME_POSITION_END          0x10000000U  /* Define Frame Position -- End */
#define FRAME_POSITION_MID          0x00000000U  /* Define Frame Position -- Middle */


#define FE                          0x08000000U  /* TFE - Tx Frame Error */
                                    /* or RFE - Rx Frame Error */


/* RFS - bit 26-0, bit 9, 7, 4-0 are used for error status */
#define RMAF                        0x00800000U

/* defines for the EESR bit values, same bit values can be used for the EESIPR */

#define EESR_TWB    0x40000000U  /* Write back complete */
#define EESR_TABT   0x04000000U  /* Transmit Abort */
#define EESR_RABT   0x02000000U  /* Receive Abort */
#define EESR_RFCOF  0x01000000U  /* receive frame counter overflow */
#define EESR_ADE    0x00800000U  /* Adress error */
#define EESR_ECI    0x00400000U  /* EtherC States Register interrupt */
#define EESR_TC     0x00200000U  /* Tx complete */
#define EESR_TDE    0x00100000U  /* Tx Descriptor Exhausted */
#define EESR_TFUF   0x00080000U  /* Tx FIFO Underflow */
#define EESR_FR     0x00040000U  /* Frame Received */
#define EESR_RDE    0x00020000U  /* Rx Descriptor Exhausted */
#define EESR_RFOF   0x00010000U  /* Rx FIFO overflow */
#define EESR_CND    0x00000800U  /* Carrier not detected */
#define EESR_DLC    0x00000400U  /* Detect Loss of Carrier */
#define EESR_CD     0x00000200U  /* Collision Detect */
#define EESR_TRO    0x00000100U  /* Tx retry over */
#define EESR_RMAF   0x00000080U  /* Receive Multicast Addresss Frame */
#define EESR_RRF    0x00000010U  /* Receive Residual bit frame */
#define EESR_RTLF   0x00000008U  /* Receive Too-Long Frame */
#define EESR_RTSF   0x00000004U  /* Receive too-short Frame */
#define EESR_PRE    0x00000002U  /* PHY-LSI Receive Error */
#define EESR_CERF   0x00000001U  /* CRC Error on Receive Frame */

#define RFS_BITS    0x000003FFU

/* EESIPR error in packets */
#define EESIPR_PKTERRORS ( EESR_RRF | EESR_RTLF | EESR_RTSF | EESR_CERF)
#define EESIPR_BUFERRORS (EESR_RFCOF | EESR_TDE | EESR_TFUF | EESR_RDE | EESR_RFOF)   
#define EESIPR_COMERRORS (EESR_CND |EESR_DLC | EESR_CD | EESR_TRO | EESR_PRE)

#define EESIPR_ALLERRORS (EESIPR_PKTERRORS | EESIPR_BUFERRORS | EESIPR_COMERRORS)

/* current value of EESIPR */
#define EESIPR_CURSETTING (EESR_TC|EESR_FR)

/** Driver record structure is similar to SSP control structure that describes an open
 * instance of sf_el_nx module.
 */
typedef struct NX_REC_STRUCT
{
    UINT         nx_state ;   ///< state of this driver
    NX_IP        *ip_ptr;     ///< NetX IP structure handling this controller
    NX_INTERFACE *ether_interface_ptr;
    uint16_t     channel;     ///< Channel associated with this record

    /** Allocate space for the BDs.  Build one extra BD there because
       the BDs must be 16 byte aligned. */
    UCHAR        rx_bd_space[sizeof(EMAC_BD) * (NUM_RX_DESC + 1)];
    UCHAR        tx_bd_space[sizeof(EMAC_BD) * (NUM_TX_DESC + 1)];

    EMAC_BD     *driver_rx_bd;             ///< Pointer to Receive BD
    EMAC_BD     *driver_tx_bd;             ///< Pointer to Transmit BD
    
    ULONG       driver_tx_bd_index;        ///< Transmit BD buffer index
    ULONG       driver_tx_bd_in_use;       ///< Number of transmit BD in use
    ULONG       driver_tx_release_index;   ///< Index of Transmit buffer index to release
    ULONG       driver_rx_bd_index;        ///< Receive BD buffer index

    
    UINT         driver_packets_queued;       ///< [not used]
    NX_PACKET   *driver_tx_packet_queue;      ///< [not used]
    NX_PACKET   *driver_tx_packet_queue_end;  ///< [not used]
    
    R_ETHERC0_Type *etherc_ptr;     ///< Pointer to ETHERC register base address
    R_EDMAC0_Type  *edmac_ptr;      ///< Pointer to EDMAC register base address

    TX_TIMER    driver_event_polling_timer;

    UINT        link_established;    ///< link status flag

    UINT        nx_driver_phy_polling_requested;  ///< Polling flag

    NX_CALLBACK_REC * p_callback_rec;   ///< pointer to the user callback function
    IRQn_Type   irq;                    ///< Interrupt priority number
}   NX_REC;

/***********************************************************************************************************************
 * External function prototypes
 ***********************************************************************************************************************/
void nx_ether_interrupt(NX_REC *nx_rec_ptr);
void enet_hw_enable_interrupt(NX_REC* nx_rec_ptr);
UINT nx_synergy_ethernet_init(NX_REC * nx_rec_ptr, sf_el_nx_cfg_t * sf_el_nx_cfg_ptr, bool hw_padding);
UINT nx_synergy_ethernet_deinit(NX_REC * nx_rec_ptr, sf_el_nx_cfg_t * sf_el_nx_cfg_ptr);
void nx_driver_event_handler(NX_REC* nx_rec_ptr);
ssp_err_t nx_ether_driver(NX_IP_DRIVER *driver_req_ptr, NX_REC * nx_rec_ptr, sf_el_nx_cfg_t * sf_el_nx_cfg_ptr);
ssp_err_t nx_ether_custom_packet_send( NX_PACKET_POOL *pool_ptr,
                                     NX_REC * nx_record_ptr,
                                     UCHAR  * data,
                                     UINT length,
                                     USHORT ether_type,
                                     nx_mac_address_t dest_mac_address);
/** Determine if a C++ compiler is being used.  If so, complete the standard C conditional started above.  */
#ifdef   __cplusplus
        }
#endif

#endif      

/*******************************************************************************************************************//**
 * @} (end defgroup NX_SYNERGY_PORT)
***********************************************************************************************************************/
