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

/***********************************************************************************************************************
 * File Name    : ether_phy.c
 * Description  : Ethernet PHY device driver for KSZ8091RNB and KSZ8081RNB (Micrel, Inc.)
 **********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"
/* Only for KSZ8081RNB and KSZ8091RNB PHY chips */
#if defined (BSP_BOARD_PHY_KSZ8091) || defined(BSP_BOARD_PHY_KSZ8081)
/*LDRA_ANALYSIS */
#include "ether_phy.h"
#include "framework/instances/sf_el_nx.h"
/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_MII_MDI_BIT_SHIFT (3U)
#define ETHER_MII_MDI_BIT_MASK  (1U << 3)
#define ETHER_MII_MDO_HIGH  (1U << 2)
#define ETHER_MII_MDO_LOW   (0U << 2)
#define ETHER_MII_MDD_WRITE (1U << 1)
#define ETHER_MII_MDD_READ  (0U << 1)
#define ETHER_MII_MDC_HIGH  (1U << 0)
#define ETHER_MII_MDC_LOW   (0U << 0)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static phy_record_t phy_record[2] = {{0,0},{0,0}};
static void phy_write (uint32_t ether_channel, uint16_t reg_addr, uint16_t data);

static uint16_t phy_read (uint32_t ether_channel, uint16_t reg_addr);

static void phy_preamble (uint32_t channel);

static void phy_reg_set (uint32_t ether_channel, uint16_t reg_addr, int32_t option);

static void phy_reg_read (uint32_t ether_channel, uint16_t * pdata);

static void phy_reg_write (uint32_t ether_channel, uint16_t data);

static void phy_trans_1to0 (uint32_t ether_channel);

static void phy_mii_write0 (uint32_t ether_channel);

static void phy_mii_write1 (uint32_t ether_channel);

static void phy_idle (uint32_t ether_channel);

/***********************************************************************************************************************
 Global Variables
 **********************************************************************************************************************/

/***********************************************************************//**
 * @ingroup SF_Library
 * @defgroup ETHER_PHY  NetX Synergy Port PHY Driver
 *
 * @brief Interface between SF_EL_NX Ethernet framework and PHY driver
 *
 * @{
 **************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief bsp_ethernet_phy_init - Initialize  Ethernet PHY device.
 * @param[in]  channel       Ethernet channel number
 * @retval  R_PHY_OK       PHY device is initialized successfully
 * @retval  R_PHY_ERROR    PHY device is not initialized successfully or Validation of PHY chip fails.
 * @note    Refer HW manual for valid channels for a target MCU
***********************************************************************************************************************/
int16_t bsp_ethernet_phy_init (uint32_t channel)
{
    uint16_t reg;
    uint32_t count = 0;

    /** Read the PHY Identifier register and compare read value with the PHY chip OUI identifier number */
    reg = phy_read(channel, PHY_REG_IDENTIFIER1);
    if (!(reg & PHY_IDENTIFIER1_OUI))
    {
        return R_PHY_ERROR;
    }

    phy_record[channel].preamble_length = R_PHY_NORMAL_PREAMBLE;

    /** Resets PHY device */
    phy_write(channel, PHY_REG_CONTROL, PHY_CONTROL_RESET);

    /** Waits the reset completion, PHY_CONTROL_RESET bit is self-cleared after 1 is written to it. */
    do
    {
        reg = phy_read(channel, PHY_REG_CONTROL);
        if (count >= ETHER_CFG_PHY_DELAY_RESET)
        {
            return R_PHY_ERROR;
        }
        count++;
    } while (reg & PHY_CONTROL_RESET);

#ifdef BSP_BOARD_PHY_KSZ8091
    /**
     * When MICREL_KSZ8091RNB of the Micrel, Inc. is used,
     * the pin that outputs the state of LINK is used combinedly with ACTIVITY in default.
     * The setting of the pin is changed so that only the state of LINK is output.
     * Set Clock Mode to 50MHz
     */
#ifdef BSP_BOARD_S7G2_DK
    if((uint8_t)0 == bsp_s7g2_dk_board_rev)
    {
        reg  = phy_read(channel, PHY_REG_PHY_CONTROL_2);
        reg &= (uint16_t) (~(PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_MASK | PHY_REG_PHY_CONTROL2_LED_MODE_MASK));
        reg |= (PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_50MHZ | PHY_REG_PHY_CONTROL2_LED_MODE_LINK0_ACTIVITY1);
        phy_write(channel, PHY_REG_PHY_CONTROL_2, reg);
    }
#else
    reg  = phy_read(channel, PHY_REG_PHY_CONTROL_2);
    reg &= (uint16_t) (~(PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_MASK | PHY_REG_PHY_CONTROL2_LED_MODE_MASK));
    reg |= (PHY_REG_PHY_CONTROL2_RMMI_REF_CLOCK_SELECT_50MHZ | PHY_REG_PHY_CONTROL2_LED_MODE_LINK0_ACTIVITY1);
    phy_write(channel, PHY_REG_PHY_CONTROL_2, reg);
#endif
#endif

    /** Sets Duplex Mode as Full-duplex */
    reg  = phy_read(channel, PHY_REG_CONTROL);
    reg |= PHY_CONTROL_FULL_DUPLEX;
    phy_write(channel, PHY_REG_CONTROL, reg);

    phy_record[channel].preamble_length = R_PHY_SUPPRESSED_PREAMBLE;

    return R_PHY_OK;
} /* End of function bsp_ethernet_phy_init() */

/*******************************************************************************************************************//**
 * @brief Sets Auto-Negotiation advertisement and starts auto-negotiation.
 * @param[in]  channel       Ethernet channel number
 * @param[in]  pause       Using state of pause frames
***********************************************************************************************************************/
void bsp_ethernet_phy_start_autonegotiate (uint32_t channel, uint8_t pause)
{
    /* Sets local ability */
    if (ETHER_FLAG_OFF == pause)
    {
        /* When pause frame is not used */
        phy_record[channel].local_advertise = ((((PHY_AN_ADVERTISEMENT_100F    |
                                         PHY_AN_ADVERTISEMENT_100H)   |
                                         PHY_AN_ADVERTISEMENT_10F)    |
                                         PHY_AN_ADVERTISEMENT_10H)    |
                                         PHY_AN_ADVERTISEMENT_SELECTOR);
    }
    else
    {
        /* When pause frame is used */
        phy_record[channel].local_advertise  = ((((((PHY_AN_ADVERTISEMENT_ASYMP   |
                                           PHY_AN_ADVERTISEMENT_PAUSE)  |
                                           PHY_AN_ADVERTISEMENT_100F)   |
                                           PHY_AN_ADVERTISEMENT_100H)   |
                                           PHY_AN_ADVERTISEMENT_10F)    |
                                           PHY_AN_ADVERTISEMENT_10H)    |
                                           PHY_AN_ADVERTISEMENT_SELECTOR);
    }

    /* Configures what the PHY and the Ethernet controller on this board supports */
    phy_write(channel, PHY_REG_AN_ADVERTISEMENT, phy_record[channel].local_advertise );
    phy_write(channel, PHY_REG_CONTROL, (PHY_CONTROL_AN_ENABLE | PHY_CONTROL_AN_RESTART));

} /* End of function bsp_ethernet_phy_start_autonegotiate() */

/*******************************************************************************************************************//**
 * @brief Gets capabilities of an Ethernet PHY device
 * @param[in]  channel       Ethernet channel number
 * @param[in]  p_line_speed_duplex    A pointer to the location of both the line speed and the duplex
 * @param[in]  p_local_pause         A pointer to the location to store the local pause bits
 * @param[in]  p_partner_pause       A pointer to the location to store the partner pause bits
 * @retval  R_PHY_OK       Got information successfully
 * @retval  R_PHY_ERROR    PHY device is yet to be initialized
 * @note Validating parameters is not required by this function as it by design is only called by sf_el_nx framework
 *       with valid parameters.
***********************************************************************************************************************/
int16_t bsp_ethernet_phy_get_autonegotiate (uint32_t channel,
                               uint16_t * p_line_speed_duplex,
                               uint16_t * p_local_pause,
                               uint16_t * p_partner_pause)
{
    uint16_t reg;

    /** Reads the status register. Because reading the first time shows the previous state, the Link status bit should
     *  be read twice.
     */
    reg = phy_read(channel, PHY_REG_STATUS);
    reg = phy_read(channel, PHY_REG_STATUS);

    /** Checks the link status */
    if (!(reg & PHY_STATUS_LINK_UP))
    {
        return R_PHY_ERROR;
    }

    /** Check the auto-negotiation status */
    if (!(reg & PHY_STATUS_AN_COMPLETE))
    {
        return R_PHY_ERROR;
    }

    /** Gets local pause capability */
    if (phy_record[channel].local_advertise  & PHY_AN_ADVERTISEMENT_PAUSE)
    {
        *p_local_pause |= ETHER_PHY_PAUSE_SYMMETRIC;
    }

    if (phy_record[channel].local_advertise  & PHY_AN_ADVERTISEMENT_ASYMP)
    {
        *p_local_pause |= ETHER_PHY_PAUSE_ASYMMETRIC;
    }

    /* Gets the link partner response */
    reg = phy_read(channel, PHY_REG_AN_LINK_PARTNER);

    /* Gets partner pause capability */
    if (PHY_AN_LINK_PARTNER_PAUSE == (reg & PHY_AN_LINK_PARTNER_PAUSE))
    {
        *p_partner_pause = ETHER_PHY_PAUSE_SYMMETRIC;
    }

    if (PHY_AN_LINK_PARTNER_ASYMP == (reg & PHY_AN_LINK_PARTNER_ASYMP))
    {
        *p_partner_pause |= ETHER_PHY_PAUSE_ASYMMETRIC;
    }

    /* Gets the line speed and the duplex */
    if (reg & PHY_AN_LINK_PARTNER_10H)
    {
        *p_line_speed_duplex = PHY_LINK_10H;
    }

    if (reg & PHY_AN_LINK_PARTNER_10F)
    {
        *p_line_speed_duplex = PHY_LINK_10F;
    }

    if (reg & PHY_AN_LINK_PARTNER_100H)
    {
        *p_line_speed_duplex = PHY_LINK_100H;
    }

    if (reg & PHY_AN_LINK_PARTNER_100F)
    {
        *p_line_speed_duplex = PHY_LINK_100F;
    }

    return R_PHY_OK;
} /* End of function bsp_ethernet_phy_get_autonegotiate() */

/*******************************************************************************************************************//**
 * @brief bsp_ethernet_phy_get_link_status - Returns the status of the physical link.
 * @param[in]  channel     Ethernet channel number
 * @retval  R_PHY_OK       PHY device is initialized successfully
 * @retval  R_PHY_ERROR    PHY device is not initialized successfully
***********************************************************************************************************************/
int16_t bsp_ethernet_phy_get_link_status (uint32_t channel)
{
    uint16_t reg;

    reg = phy_read(channel, PHY_REG_STATUS);
    if (!(reg & PHY_STATUS_LINK_UP))
    {
        return R_PHY_ERROR;
    }
    return R_PHY_OK;
} /* End of function bsp_ethernet_phy_get_link_status() */

/***********************************************************************//**
 * @} (end defgroup ETHER_PHY)
 **************************************************************************/

/*******************************************************************************************************************//**
 * @brief Writes data to a PHY register
 * @param[in]  channel       Ethernet channel number
 * @param[in]  reg_addr       Address of the PHY register
 * @param[in]  data        Data to write
***********************************************************************************************************************/
static void phy_write (uint32_t channel, uint16_t reg_addr, uint16_t data)
{
    /**
     * The value is read from the PHY register by the frame format of MII Management Interface provided
     * for by Table 22-12 of 22.2.4.5 of IEEE 802.3-2008_section2.
     */
    phy_preamble(channel);                         /* write PRE */
    phy_reg_set(channel, reg_addr, PHY_MII_WRITE); /* write ST, OP, PHAYD, REGAD, TA */
    phy_trans_1to0(channel);
    phy_reg_write(channel, data);
    phy_idle(channel);

} /* End of function phy_write() */

/*******************************************************************************************************************//**
 * @brief Reads data from a PHY register
 * @param[in]  channel       Ethernet channel number
 * @param[in]  reg_addr       Address of the PHY register
 * @retval  uint16_t Data read through MII Management Interface
***********************************************************************************************************************/
static uint16_t phy_read (uint32_t channel, uint16_t reg_addr)
{
    uint16_t data = 0U;

    /*
     * The value is read from the PHY register by the frame format of MII Management Interface provided
     * for by Table 22-12 of 22.2.4.5 of IEEE 802.3-2008_section2.
     */
    phy_preamble(channel);
    phy_reg_set(channel, reg_addr, PHY_MII_READ);
    phy_idle(channel);
    phy_reg_read(channel, &data);
    phy_idle(channel);

    return (data);
} /* End of function phy_read() */

/*******************************************************************************************************************//**
 * @brief Creates preamble. As preliminary preparation for access to the PHY module register, "1" is output via the MII
 *   management interface.
 * @param[in]  channel       Ethernet channel number
***********************************************************************************************************************/
static void phy_preamble (uint32_t channel)
{
    /**
     * The processing of PRE (preamble) about the frame format of MII Management Interface which is provided by
     * "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    uint16_t i = phy_record[channel].preamble_length;

    while (i > (uint16_t)0)
    {
        i--;
        phy_mii_write1(channel);
    }
} /* End of function phy_preamble() */

/*******************************************************************************************************************//**
 * @brief Sets a PHY device to read or write mode
 * @param[in]  channel       Ethernet channel number
 * @param[in]  reg_addr      Address of the PHY register
 * @param[in]  option        mode (PHY_MII_READ|PHY_MII_WRITE)
***********************************************************************************************************************/
static void phy_reg_set (uint32_t channel, uint16_t reg_addr, int32_t option)
{
    uint16_t data;

    data = PHY_MII_ST;          /* ST code      */

    if (PHY_MII_READ == option)
    {
        data |= PHY_MII_READ;   /* OP code(RD)  */
    }
    else
    {
        data |= PHY_MII_WRITE;  /* OP code(WT)  */
    }

    data |= ((PHY_MII_ADDRESS & 0x001FU) << 7);     /* Phy address  */
    data = (uint16_t) (data | ((reg_addr & 0x001FU) << 2));            /* Reg Address  */

    /* Send MII Management Frame */
    int32_t i = 14;
    while (i > 0)
    {
        i--;
        if (0 == (data & 0x8000))
        {
            phy_mii_write0(channel);
        }
        else
        {
            phy_mii_write1(channel);
        }
        data = (uint16_t) (data << 1U);
    }
} /* End of function phy_reg_set() */

/*******************************************************************************************************************//**
 * @brief Switches data bus so MII interface can drive data for write operation
 * @param[in]  channel       Ethernet channel number
*************************************************************************************************i********/
static void phy_trans_1to0 (uint32_t channel)
{
    /**
     * The processing of TA (turnaround) about writing of the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    phy_mii_write1(channel);
    phy_mii_write0(channel);
} /* End of function phy_trans_1to0() */

/*******************************************************************************************************************//**
 * @brief Writes to PHY register through MII interface
 * @param[in]  channel       Ethernet channel number
 * @param[in]  data           Value to write
***********************************************************************************************************************/
static void phy_reg_write (uint32_t channel, uint16_t data)
{
    /**
     * The processing of DATA (data) about writing of the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    int32_t i = 16;
    while (i > 0)
    {
        i--;
        if (0U == (data & 0x8000U))
        {
            phy_mii_write0(channel);
        }
        else
        {
            phy_mii_write1(channel);
        }
        data = (uint16_t) (data << 1U);
    }
} /* End of function phy_reg_write() */

/*******************************************************************************************************************//**
 * @brief Outputs 0 to the MII interface
 * @param[in]  channel       Ethernet channel number
***********************************************************************************************************************/
static void phy_mii_write0 (uint32_t channel)
{
    int32_t i;
    volatile uint32_t * pir;

    if (0U == channel)
    {
        pir = &(R_ETHERC0->PIR);
    }
    else
    {
        pir = &(R_ETHERC1->PIR);
    }

    /*
     * The processing of one bit about frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     * The data that 0 is output.
     */
    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_LOW | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_LOW;
    }

    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_LOW | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_HIGH;
    }

    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_LOW | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_HIGH;
    }

    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_LOW | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_LOW;
    }
} /* End of function phy_mii_write0() */

/*******************************************************************************************************************//**
 * @brief Outputs 1 to the MII interface
 * @param[in]  channel       Ethernet channel number
***********************************************************************************************************************/
static void phy_mii_write1 (uint32_t channel)
{
    int32_t i;
    volatile uint32_t * pir;

    if(0 == channel)
    {
        pir = &(R_ETHERC0->PIR);
    }
    else
    {
        pir = &(R_ETHERC1->PIR);
    }

    /*
     * The processing of one bit about frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     * The data that 1 is output.
     */
    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_HIGH | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_LOW;
    }

    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_HIGH | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_HIGH;
    }

    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_HIGH | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_HIGH;
    }

    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDO_HIGH | ETHER_MII_MDD_WRITE | ETHER_MII_MDC_LOW;
    }
} /* End of function phy_mii_write1() */

/*******************************************************************************************************************//**
 * @brief Reads data from a PHY register
 * @param[in]  channel     Ethernet channel number
 * @param[in]  pdata       Read data through MII Management Interface
***********************************************************************************************************************/
static void phy_reg_read (uint32_t channel, uint16_t * pdata)
{
    int32_t  j;
    uint16_t reg_data;
    volatile uint32_t * pir;

    if(0 == channel)
    {
        pir = &(R_ETHERC0->PIR);
    }
    else
    {
        pir = &(R_ETHERC1->PIR);
    }

    /**
     * The processing of DATA (data) about reading of the frame format of MII Management Interface which is
     * provided by "Table 22-12" of "22.2.4.5" of "IEEE 802.3-2008_section2".
     */
    reg_data  = 0U;
    int32_t i = 16;
    while (i > 0)
    {
        i--;
        /** Toggles clock */
        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--)
        {
            *pir = ETHER_MII_MDD_READ | ETHER_MII_MDC_LOW;
        }
        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--)
        {
            *pir = ETHER_MII_MDD_READ | ETHER_MII_MDC_HIGH;
        }

        reg_data = (uint16_t) (reg_data << 1U);

        /** Reads MDI bit */
        reg_data  |= (uint16_t) ((*pir & ETHER_MII_MDI_BIT_MASK) >> ETHER_MII_MDI_BIT_SHIFT);

        /** Toggles clock */
        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--)
        {
            *pir = ETHER_MII_MDD_READ | ETHER_MII_MDC_HIGH;
        }
        for (j = ETHER_CFG_PHY_MII_WAIT; j > 0; j--)
        {
            *pir = ETHER_MII_MDD_READ | ETHER_MII_MDC_LOW;
        }
    }
    (*pdata) = reg_data;
} /* End of function phy_reg_read() */

/*******************************************************************************************************************//**
 * @brief Performs bus release so that PHY can drive data for read operation
 * @param[in]  channel       Ethernet channel number
***********************************************************************************************************************/
static void phy_idle (uint32_t channel)
{
    int32_t i;
    volatile uint32_t * pir;

    if (0U == channel)
    {
        pir = &(R_ETHERC0->PIR);
    }
    else
    {
        pir = &(R_ETHERC1->PIR);
    }

    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDC_LOW;
    }
    for (i = ETHER_CFG_PHY_MII_WAIT * 2; i > 0; i--)
    {
        *pir = ETHER_MII_MDC_HIGH;
    }
    for (i = ETHER_CFG_PHY_MII_WAIT; i > 0; i--)
    {
        *pir = ETHER_MII_MDC_LOW;
    }
} /* End of function phy_idle() */
#endif
