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
/**********************************************************************************************************************
* File Name    : sf_el_fx.h
* Description  : SF_EL_FX
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_EL_FX FX_IO Framework
 * @brief FileX adaptation layer for block media device drivers.
 *
 * SF_EL_FX FileX I/O is a single entry function which adapts FileX to Renesas Synergy block media device drivers.
 *
 * @section SF_EL_FX_API_SUMMARY Summary
 * SF_EL_FX Has no API file.
 *
 * @{
***********************************************************************************************************************/

#ifndef SF_EL_FX_API_H
#define SF_EL_FX_API_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Include driver API and ThreadX API */
#include "bsp_api.h"
#include "fx_api.h"
#include "sf_block_media_api.h"

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
SSP_HEADER

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Version of the API defined in this file */
#define SF_EL_FX_API_VERSION_MAJOR   (1)
#define SF_EL_FX_API_VERSION_MINOR   (0)

/** Version of code that implements the API defined in this file */
#define SF_EL_FX_CODE_VERSION_MAJOR  (1U)
#define SF_EL_FX_CODE_VERSION_MINOR  (11U)


/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Block Media Control Block Type */
typedef struct st_sf_el_fx
{
    sf_block_media_instance_t  * p_lower_lvl_block_media;       ///< Lower level block media pointer
    void                       * p_extend;
} sf_el_fx_t;

/**********************************************************************************************************************
 * Public Functions
 **********************************************************************************************************************/

void SF_EL_FX_BlockDriver (FX_MEDIA * media_ptr);

/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
SSP_FOOTER

#endif //SF_EL_FX_API_H

/*******************************************************************************************************************//**
 * @} (end defgroup SF_EL_FX)
***********************************************************************************************************************/
