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

/***********************************************************************************************************************
 * File Name    : ether_phy.h
 * Description  : Ethernet PHY device driver header file for Micrel KSZ8091RNB and KSZ8081RNB PHY chips
 **********************************************************************************************************************/


#ifndef ETHER_PHY_H
#define ETHER_PHY_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
SSP_HEADER
/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @addtogroup ETHER_PHY NetX Synergy Port PHY Driver
 * @{
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/* Define the access timing of MII/RMII register used for vendor specific PHY(KSZ8091RNB and KSZ8081RNB) */
#ifndef ETHER_CFG_PHY_MII_WAIT
#define ETHER_CFG_PHY_MII_WAIT (6)             /* Please define the value of 2 or more */
#endif

/* Define the waiting time for reset completion of PHY-LSI used for vendor specific PHY(KSZ8091RNB and KSZ8081RNB) */
#define ETHER_CFG_PHY_DELAY_RESET (0x00020000L)

/* Define the waiting time for link-up completion of PHY-LSI used for vendor specific PHY(KSZ8091RNB and KSZ8081RNB) */
#define ETHER_CFG_PHY_LINKUP_WAIT (700)

/* Vendor Specific PHY Registers */
/* KSZ8091RNB */
#define PHY_REG_PHY_CONTROL_2 (0x001FU)

/* MICREL KSZ8091RNB PHY Control2  Bit Definitions */
#define PHY_REG_PHY_CONTROL2_LED_MODE_MASK                 (3U << 4)
#define PHY_REG_PHY_CONTROL2_LED_MODE_LINK0_ACTIVITY1      (1U << 4)
#define PHY_REG_PHY_CONTROL2_LED_MODE_LINKACTIVITY0_SPEED1 (0 << 4)

#define PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_MASK    (1U << 7)
#define PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_25MHZ   (0U << 7)
#define PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_50MHZ   (1U << 7)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef enum e_ether_pause
{
    ETHER_PHY_PAUSE_NONE       = 0,
    ETHER_PHY_PAUSE_SYMMETRIC  = 1,
    ETHER_PHY_PAUSE_ASYMMETRIC = 2,
} ether_phy_pause_t;


/***********************************************************************//**
 * @} (end defgroup ETHER_PHY)
 **************************************************************************/
/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/

/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
SSP_FOOTER

#endif // ETHER_PHY_H
