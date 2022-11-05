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

/**********************************************************************************************************************
 * File Name    : sf_el_nx.h
 * Description  : Interface file between the sf_el_nx module and the PHY driver.
 *                Include this file while implementing the new BSP file for PHY other than on-board PHY chip.
 **********************************************************************************************************************/

#ifndef SF_EL_NX_H
#define SF_EL_NX_H

#include "bsp_api.h"

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
SSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup NX_SYNERGY_PORT
 * @defgroup SF_EL_NX Interface file between SF_EL_NX and PHY driver
 * @{
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* PHY return definitions */
#define R_PHY_OK    (int16_t)(0)   ///< PHY device is initialized successfully
#define R_PHY_ERROR (int16_t)(-1)  ///< PHY device is not initialized successfully

#define ETHER_FLAG_OFF         (0U)
#define ETHER_FLAG_ON          (1U)
#define ETHER_FLAG_ON_LINK_ON  (3U)
#define ETHER_FLAG_ON_LINK_OFF (2U)

#define R_PHY_NORMAL_PREAMBLE      0x20U  ///< Standard preamble length (32bit 1's )
#define R_PHY_SUPPRESSED_PREAMBLE  0x01U  ///< Preamble suppression value

#define LINK_STAT_MASK         (1U << 2)
#define PHY_MII_ST             (1U << 14)
#define PHY_MII_READ           (0x2U << 12) ///< PHY read OP code
#define PHY_MII_WRITE          (0x1U << 12) ///< PHY write OP code
#define PHY_MII_ADDRESS        (0x0000U)   /* @note Configures PHY address 3b'000 due to PE-HMI and DKS7G2 boards
                                                  does not configure PHY address */

/* Standard PHY Registers */
#define PHY_REG_CONTROL          (0x0000U)  ///< Basic Control register
#define PHY_REG_STATUS           (0x0001U)  ///< Basic Status register
#define PHY_REG_IDENTIFIER1      (0x0002U)  ///< PHY Identifier 1 register
#define PHY_REG_IDENTIFIER2      (0x0003U)  ///< PHY Identifier 2 register
#define PHY_REG_AN_ADVERTISEMENT (0x0004U)  ///< Auto-Negotiation Advertisement register
#define PHY_REG_AN_LINK_PARTNER  (0x0005U)  ///< Auto-Negotiation Link Partner Ability register
#define PHY_REG_AN_EXPANSION     (0x0006U)  ///< Auto-Negotiation Expansion register

/* Basic Mode Control Register Bit Definitions */
#define PHY_CONTROL_RESET       (1U << 15)
#define PHY_CONTROL_LOOPBACK    (1U << 14)
#define PHY_CONTROL_100_MBPS    (1U << 13)
#define PHY_CONTROL_AN_ENABLE   (1U << 12)
#define PHY_CONTROL_POWER_DOWN  (1U << 11)
#define PHY_CONTROL_ISOLATE     (1U << 10)
#define PHY_CONTROL_AN_RESTART  (1U << 9)
#define PHY_CONTROL_FULL_DUPLEX (1U << 8)
#define PHY_CONTROL_COLLISION   (1U << 7)

/* Basic Mode Status Register Bit Definitions */
#define PHY_STATUS_100_T4        (1U << 15)
#define PHY_STATUS_100F          (1U << 14)
#define PHY_STATUS_100H          (1U << 13)
#define PHY_STATUS_10F           (1U << 12)
#define PHY_STATUS_10H           (1U << 11)
#define PHY_STATUS_AN_COMPLETE   (1U << 5)
#define PHY_STATUS_RM_FAULT      (1U << 4)
#define PHY_STATUS_AN_ABILITY    (1U << 3)
#define PHY_STATUS_LINK_UP       (1U << 2)
#define PHY_STATUS_JABBER        (1U << 1)
#define PHY_STATUS_EX_CAPABILITY (1U << 0)

/* Auto Negotiation Advertisement Bit Definitions */
#define PHY_AN_ADVERTISEMENT_NEXT_PAGE (1U << 15)
#define PHY_AN_ADVERTISEMENT_RM_FAULT  (1U << 13)
#define PHY_AN_ADVERTISEMENT_ASYMP     (1U << 11)
#define PHY_AN_ADVERTISEMENT_PAUSE     (1U << 10)
#define PHY_AN_ADVERTISEMENT_100_T4    (1U << 9)
#define PHY_AN_ADVERTISEMENT_100F      (1U << 8)
#define PHY_AN_ADVERTISEMENT_100H      (1U << 7)
#define PHY_AN_ADVERTISEMENT_10F       (1U << 6)
#define PHY_AN_ADVERTISEMENT_10H       (1U << 5)
#define PHY_AN_ADVERTISEMENT_SELECTOR  (1U << 0)

/* Auto Negotiate Link Partner Ability Bit Definitions */
#define PHY_AN_LINK_PARTNER_NEXT_PAGE ((uint16_t) (1U << 15))
#define PHY_AN_LINK_PARTNER_ACK       ((uint16_t) (1U << 14))
#define PHY_AN_LINK_PARTNER_RM_FAULT  ((uint16_t) (1U << 13))
#define PHY_AN_LINK_PARTNER_ASYMP     ((uint16_t) (1U << 11))
#define PHY_AN_LINK_PARTNER_PAUSE     ((uint16_t) (1U << 10))
#define PHY_AN_LINK_PARTNER_100_T4    ((uint16_t) (1U << 9))
#define PHY_AN_LINK_PARTNER_100F      ((uint16_t) (1U << 8))
#define PHY_AN_LINK_PARTNER_100H      ((uint16_t) (1U << 7))
#define PHY_AN_LINK_PARTNER_10F       ((uint16_t) (1U << 6))
#define PHY_AN_LINK_PARTNER_10H       ((uint16_t) (1U << 5))
#define PHY_AN_LINK_PARTNER_SELECTOR  ((uint16_t) (1U << 0))

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Standard PHY speed and duplex operation mode */
typedef enum e_linkstat
{
    PHY_NO_LINK = 0,    ///< Link not established
    PHY_LINK_10H,       ///< 10Mbps Half duplex
    PHY_LINK_10F,       ///< 10Mbps Full duplex
    PHY_LINK_100H,      ///< 100Mbps Half duplex
    PHY_LINK_100F       ///< 100Mbps Full duplex
} linkstat_t;

/** Standard PHY data structure */
typedef struct st_phy_record {
    uint16_t preamble_length; ///< Preamble length
    uint16_t local_advertise; ///< The capabilities of the local link as PHY data 
} phy_record_t;

/***********************************************************************//**
 * @} (end defgroup SF_EL_NX)
 **************************************************************************/
/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief bsp_ethernet_phy_init - Initialize  Ethernet PHY device.
 * @param[in]  channel     Ethernet channel number
 * @retval  R_PHY_OK       PHY device is initialized successfully
 * @retval  R_PHY_ERROR    PHY device is not initialized successfully
***********************************************************************************************************************/
int16_t bsp_ethernet_phy_init (uint32_t ether_channel);

/*******************************************************************************************************************//**
 * @brief Gets capabilities of an Ethernet PHY device
 * @param[in]  channel               Ethernet channel number
 * @param[in]  p_line_speed_duplex   A pointer to the location of both the line speed and the duplex
 * @param[in]  p_local_pause         A pointer to the location to store the local pause bits
 * @param[in]  p_partner_pause       A pointer to the location to store the partner pause bits
 * @retval  R_PHY_OK                 Got information successfully
 * @retval  R_PHY_ERROR              PHY device is yet to be initialized
***********************************************************************************************************************/
int16_t bsp_ethernet_phy_get_autonegotiate (uint32_t ether_channel,
                               uint16_t * pline_speed_duplex,
                               uint16_t * plocal_pause,
                               uint16_t * ppartner_pause);

/*******************************************************************************************************************//**
 * @brief Sets Auto-Negotiation advertisement and starts auto-negotiation.
 * @param[in]  channel       Ethernet channel number
 * @param[in]  pause         Using state of pause frames
***********************************************************************************************************************/
void bsp_ethernet_phy_start_autonegotiate (uint32_t ether_channel, uint8_t pause);

/*******************************************************************************************************************//**
 * @brief bsp_ethernet_phy_get_link_status - Returns the status of the physical link.
 * @param[in]  channel     Ethernet channel number
 * @retval  R_PHY_OK       PHY device is initialized successfully
 * @retval  R_PHY_ERROR    PHY device is not initialized successfully
***********************************************************************************************************************/
int16_t bsp_ethernet_phy_get_link_status (uint32_t ether_channel);

/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
SSP_FOOTER

#endif /* SF_EL_NX_H */
