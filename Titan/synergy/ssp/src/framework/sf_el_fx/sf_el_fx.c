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
 * File Name    : sf_el_fx.c
 * Description  : FileX File System I/O
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup SF_EL_FX
 * @{
 **********************************************************************************************************************/

/** SSP FileX Support. */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Include necessary system files.  */

#include "fx_api.h"
#include "bsp_api.h"
#include "sf_el_fx.h"
#include "sf_el_fx_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_EL_FX_55AA_SIGNATURE_OFFSET  (0x1FEU)
#define SF_EL_FX_BOOT_SIGNATURE_VALUE   (0xAA55U)
#define SF_EL_FX_JUMP_INST_OFFSET       (0U)
#define SF_EL_FX_JUMP_INST_SET1_BYTE0   (0xE9U)
#define SF_EL_FX_JUMP_INST_SET2_BYTE0   (0xEBU)
#define SF_EL_FX_JUMP_INST_SET2_BYTE2   (0x90U)
#define SF_EL_FX_FAT16_SECTORS_PER_FAT  (0x16U)
#define SF_EL_FX_FAT32_SECTORS_PER_FAT  (0x24U)
#define SF_EL_FX_FAT16_SECTOR_COUNT     (0x13U)
#define SF_EL_FX_FAT32_SECTOR_COUNT     (0x20U)
#define SF_EL_FX_PARTITION0_OFFSET      (0x1C6U)
#define SF_EL_FX_PARTITION0_TYPE_OFFSET (0x1C2U)
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static ssp_err_t check_partition_offset(uint8_t * p_sector, uint32_t * p_partition_offset);
static ssp_err_t check_fat_boot_record (uint8_t * p_sector, uint32_t * p_partition_offset);
static ssp_err_t sf_el_fx_driver_release_sectors_handler(sf_block_media_instance_t * p_block_media,
                                                         uint32_t                    num_sectors,
                                                         uint32_t                    start_sector);
static ssp_err_t sf_el_fx_driver_init_handler(sf_block_media_instance_t * p_block_media,
                                              uint32_t                  * write_protect,
                                              uint32_t                  * requires_sector_release);
static ssp_err_t sf_el_fx_driver_boot_read_handler(sf_block_media_instance_t * p_block_media,
                                                   uint8_t                   * p_buffer,
                                                   uint32_t                  * driver_status);
static ssp_err_t sf_el_fx_driver_request_handler(FX_MEDIA * p_fx_media);
/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/


/** The file system relies on the media to be formatted prior to creating directories and files
 *  The sector size and sector count will change depending on the media type and size.
 *
 *  The File Allocation Table (FAT) starts after the reserved sectors in the media. The FAT area is
 *  basically an array of 12-bit, 16-bit, or 32-bit entries that determine if that cluster is
 *  allocated or part of a chain of clusters comprising a subdirectory or a file.
 *  The size of each FAT entry is determined by the number of clusters that need to be represented.
 *  If the number of clusters (derived from the total sectors divided by the sectors per cluster)
 *  is less than 4,086, 12-bit FAT entries are used. If the total number of clusters is greater
 *  than 4,086 and less than or equal to 65,525, 16-bit FAT entries are used. Otherwise, if the
 *  total number of clusters is greater than 65,525, 32-bit FAT entries are used.
 */

/*******************************************************************************************************************//**
 * @brief  Access Block Media device functions open, close, read, write and control.
 *
 * The SF_EL_FX_BlockDriver function is called from the FileX file system driver and issues requests to a Block Media
 * device through the Synergy Block Media Interface. Uses block media driver for accesses.
 * @param[in,out] p_fx_media                        FileX media control block. All information about each open media
 *                                                  device are maintained in by the FX_MEDIA data type.
 *                                                  The I/O driver communicates the success or failure of the request
 *                                                  through the fx_media_driver_status member of FX_MEDIA
 *                                                  (p_fx_media->fx_media_driver_status).
 *                                                  Possible values are documented in the FileX User Guide.
 * @retval                                          none.
 *
 * @return                                          Nothing, but updates FileX media control block.This function calls
 *                                                      * sf_el_fx_driver_request_handler
 **********************************************************************************************************************/

void  SF_EL_FX_BlockDriver (FX_MEDIA * p_fx_media)
{
    ssp_err_t ret_val = SSP_SUCCESS;

    /** - Initialize FileX I/O status to error. It will change to FX_SUCCESS unless an operation fails. */
    p_fx_media->fx_media_driver_status = (UINT)FX_IO_ERROR;

#if  (SF_EL_FX_CFG_PARAM_CHECKING_ENABLE)
    /** - Validate FX_MEDIA::fx_media_driver_info and update FX_MEDIA::fx_media_driver_status to FX_PTR_ERROR in-case of
     *    any failure. */
    if (NULL == p_fx_media->fx_media_driver_info)
    {
        p_fx_media->fx_media_driver_status = (UINT)FX_PTR_ERROR;
        return;
    }
#endif

#if  (SF_EL_FX_CFG_PARAM_CHECKING_ENABLE)
    /** - Validate sf_el_fx_t::p_lower_lvl_block_media and update FX_MEDIA::fx_media_driver_status to FX_PTR_ERROR
     *    in-case of any failure. */
    if (NULL == ((sf_el_fx_t *) p_fx_media->fx_media_driver_info)->p_lower_lvl_block_media)
    {
        p_fx_media->fx_media_driver_status = (UINT)FX_PTR_ERROR;
        return;
    }
#endif

    /** - Pass control to sf_el_fx_driver_request_handler, which is responsible for setting
     *    FX_MEDIA::fx_media_driver_status in-case of any failure. */
    ret_val = sf_el_fx_driver_request_handler(p_fx_media);

    if (SSP_SUCCESS == ret_val)
    {
        /**  *
         *  Update FX_MEDIA::fx_media_driver_status to FX_SUCCESS on successful driver request. */
        p_fx_media->fx_media_driver_status = (UINT)FX_SUCCESS;
    }
}


/*******************************************************************************************************************//**
 * @} (end addtogroup SF_EL_FX)
 **********************************************************************************************************************/

/******************************************************************************************************************//**
 * @brief Checks if the sector passed is a valid boot record or master boot record. If not valid boot record
 * returns the offset to the partitions boot record.
 * @param[in]   p_sector                    Pointer to sector.
 * @param[out]  p_partition_offset          Sector number of found partition table. 0 if boot sector.
 * @retval      SSP_SUCCESS                 Found boot record or partition offset.
 * @retval      SSP_ERR_MEDIA_OPEN_FAILED   Not a valid boot record or partition table.
 *********************************************************************************************************************/
static ssp_err_t check_partition_offset (uint8_t * p_sector, uint32_t * p_partition_offset)
{
    ssp_err_t ret;
    uint32_t offset;
    uint16_t check_AA55_1FE;
    UCHAR * p_temp = (UCHAR *) p_sector;

    /** - Check for FAT boot record. */
    ret = check_fat_boot_record(p_sector, p_partition_offset);

    /** - Check for master boot record. */
    if (SSP_SUCCESS != ret)
    {
        check_AA55_1FE = (uint16_t)_fx_utility_16_unsigned_read(&p_temp[SF_EL_FX_55AA_SIGNATURE_OFFSET]);

        /**  *
         *  Read the offset to partition 0. */
        offset = (uint32_t)_fx_utility_32_unsigned_read(&p_temp[SF_EL_FX_PARTITION0_OFFSET]);

        /**  *
         *  Check for partition type is set and the offset is not zero and boot record signature for valid MBR then
         *  report the partition offset. */
        if (((offset != 0U) && (p_sector[SF_EL_FX_PARTITION0_TYPE_OFFSET] != 0U)) &&
                           (SF_EL_FX_BOOT_SIGNATURE_VALUE == check_AA55_1FE))
        {
            *p_partition_offset = offset;
            return SSP_SUCCESS;
        }
    }

    return ret;
}

/******************************************************************************************************************//**
 * @brief Checks if the sector passed is a valid boot record. If not returns error.
 * @param[in]   p_sector                    Pointer to sector
 * @param[out]  p_partition_offset          Sector number of found partition table. 0 if boot sector.
 * @retval      SSP_SUCCESS                 Found boot record or partition offset
 * @retval      SSP_ERR_MEDIA_OPEN_FAILED   Not a valid boot record or partition table
 *********************************************************************************************************************/
static ssp_err_t check_fat_boot_record (uint8_t * p_sector, uint32_t * p_partition_offset)
{
    bool has_jump_instruction;
    uint32_t fat16_sectors_per_fat;
    uint32_t fat16_sector_count;
    uint32_t fat32_sectors_per_fat;
    uint32_t fat32_sector_count;

    uint32_t has_sectors_per_fat;
    uint32_t has_sector_count;
    uint16_t sector_size = 0;

    uint16_t check_AA55_sector_end;
    uint16_t check_AA55_1FE;

    UCHAR * p_temp = (UCHAR *) p_sector;

    /** - Check for a valid jump instruction. */
    has_jump_instruction = (SF_EL_FX_JUMP_INST_SET1_BYTE0 == p_sector[SF_EL_FX_JUMP_INST_OFFSET]) ||
            ((SF_EL_FX_JUMP_INST_SET2_BYTE0 == p_sector[SF_EL_FX_JUMP_INST_OFFSET]) &&
            (SF_EL_FX_JUMP_INST_SET2_BYTE2 == p_sector[SF_EL_FX_JUMP_INST_OFFSET+2]));

    /** - Check for a non zero sectors per FAT. */
    fat16_sectors_per_fat = (uint32_t)_fx_utility_16_unsigned_read(&p_temp[SF_EL_FX_FAT16_SECTORS_PER_FAT]);
    fat32_sectors_per_fat = (uint32_t)_fx_utility_32_unsigned_read(&p_temp[SF_EL_FX_FAT32_SECTORS_PER_FAT]);
    has_sectors_per_fat = fat16_sectors_per_fat | fat32_sectors_per_fat;

    /** - Check for a non zero sector count. */
    fat16_sector_count = (uint32_t)_fx_utility_16_unsigned_read(&p_temp[SF_EL_FX_FAT16_SECTOR_COUNT]);
    fat32_sector_count = (uint32_t)_fx_utility_32_unsigned_read(&p_temp[SF_EL_FX_FAT32_SECTOR_COUNT]);
    has_sector_count = fat16_sector_count | fat32_sector_count;

    /** - Check for jump instruction and sectors per fat and sector count are not 0
     *    which indicate FAT Boot record. */
    if (has_jump_instruction && has_sector_count && has_sectors_per_fat)
    {
        /**  *
         *  Get sector size. */
        sector_size = (uint16_t)p_sector[FX_BYTES_SECTOR + 1];
        sector_size = (uint16_t)((sector_size << 8) | (p_sector[FX_BYTES_SECTOR]));

        /**  *
         *  Check the sector size value. */
        if (0U != (sector_size % 32U))
        {
            return SSP_ERR_MEDIA_OPEN_FAILED;
        }

        /**  *
         *  Check Boot record signature to make sure the buffer is valid then return valid boot record. */
        check_AA55_sector_end = (uint16_t)_fx_utility_16_unsigned_read(&p_temp[sector_size-2]);
        check_AA55_1FE = (uint16_t)_fx_utility_16_unsigned_read(&p_temp[SF_EL_FX_55AA_SIGNATURE_OFFSET]);
        if ((SF_EL_FX_BOOT_SIGNATURE_VALUE == check_AA55_sector_end) ||
        		  (SF_EL_FX_BOOT_SIGNATURE_VALUE == check_AA55_1FE))
        {
            * p_partition_offset = 0U;
            return SSP_SUCCESS;
        }
    }
    else
    {
        return SSP_ERR_MEDIA_OPEN_FAILED;
    }

    return SSP_SUCCESS;
}

/******************************************************************************************************************//**
 * @brief      Handles the block driver release sectors request.
 *
 * @param      p_block_media   The block media pointer
 * @param[in]  num_sectors     The number of sectors to release
 * @param[in]  start_sector    The start sector number
 *
 * @retval     SSP_SUCCESS         Driver release sectors request completed successfully
 * @return                         See @ref Common_Error_Codes or lower level drivers for other possible return
 *                                 codes. This function calls
 *                                        * sf_block_media_api_t::ioctl
 *********************************************************************************************************************/
static ssp_err_t sf_el_fx_driver_release_sectors_handler(sf_block_media_instance_t * p_block_media,
                                                         uint32_t                    num_sectors,
                                                         uint32_t                    start_sector)
{
    ssp_err_t ret_val = SSP_SUCCESS;

    /** Release one sector at a time unless either requested number of sectors release or error detected while releasing
     * a sector. */
    while ((num_sectors) && (SSP_SUCCESS == ret_val))
    {
        ret_val = p_block_media->p_api->ioctl(p_block_media->p_ctrl,
                                              SSP_COMMAND_CTRL_SECTOR_RELEASE,
                                              (void *) (&start_sector));
        start_sector++;
        num_sectors--;
    }
    return ret_val;
}


/******************************************************************************************************************//**
 * @brief      Handles the block driver initialization request.
 *
 * @param      p_block_media       The block media pointer
 * @param      write_protect       The write protect value pointer
 * @param      free_sector_update  The free sector update value pointer
 *
 * @retval     SSP_SUCCESS         Driver initialization request completed successfully
 * @return                         See @ref Common_Error_Codes or lower level drivers for other possible return
 *                                 codes. This function calls
 *                                        * sf_block_media_api_t::open
 *                                        * sf_block_media_api_t::ioctl
 *********************************************************************************************************************/
static ssp_err_t sf_el_fx_driver_init_handler(sf_block_media_instance_t * p_block_media,
                                              uint32_t                  * write_protect,
                                              uint32_t                  * free_sector_update)
{
    ssp_err_t ret_val                 = SSP_SUCCESS;
    uint32_t requires_sector_release = 0U;

    /** - Open the block media. */
    ret_val = p_block_media->p_api->open(p_block_media->p_ctrl, p_block_media->p_cfg);
    if (SSP_SUCCESS != ret_val)
    {
        return ret_val;
    }

    /** - Get write protection status. */
    ret_val = p_block_media->p_api->ioctl(p_block_media->p_ctrl,
                                          SSP_COMMAND_GET_WRITE_PROTECTED,
                                          write_protect);
    if (SSP_SUCCESS != ret_val)
    {
        return ret_val;
    }

    /** - Get sector release require status and update FX_MEDIA::fx_media_driver_free_sector_update to FX_TRUE if sector
     *  release supported by the block media driver. */
    ret_val = p_block_media->p_api->ioctl(p_block_media->p_ctrl,
                                          SSP_COMMAND_GET_SECTOR_RELEASE,
                                          (void *) &requires_sector_release);
    if ((SSP_SUCCESS == ret_val) && (0U != requires_sector_release))
    {
        *free_sector_update = (uint32_t) FX_TRUE;
    }

    return ret_val;
}


/******************************************************************************************************************//**
 * @brief      Handles the block driver boot read request.
 *
 * @param      p_block_media   The block media pointer
 * @param      p_buffer        The buffer pointer
 * @param      driver_status   The driver status
 *
 * @retval     SSP_SUCCESS                 Driver boot read request completed successfully
 * @retval     SSP_ERR_MEDIA_OPEN_FAILED   Not a valid boot record or partition table
 *
 * @return                         See @ref Common_Error_Codes or lower level drivers for other possible return
 *                                 codes. This function calls
 *                                        * sf_block_media_api_t::read
 *                                        * check_partition_offset
 *********************************************************************************************************************/
static ssp_err_t sf_el_fx_driver_boot_read_handler(sf_block_media_instance_t * p_block_media,
                                                   uint8_t                   * p_buffer,
                                                   uint32_t                  * driver_status)
{
    ssp_err_t ret_val = SSP_SUCCESS;
    uint32_t partition_offset = 0U;

    /**  Read the boot record and return to the caller.  */
    /**  - Read the first non hidden sector. */
    ret_val = p_block_media->p_api->read(p_block_media->p_ctrl, p_buffer,
                                         0, 1);
    if(SSP_SUCCESS != ret_val)
    {
        return ret_val;
    }

    /**  - Check the partition offset to determine if the current sector is the boot record or partition table. */
    ret_val = check_partition_offset(p_buffer, &partition_offset);

    if (partition_offset > 0U)
    {
        /**   *
         *   Read the sector at the offset indicated in the partition table. */
        ret_val = p_block_media->p_api->read(p_block_media->p_ctrl, p_buffer,
                                             partition_offset, 1);
        if(SSP_SUCCESS != ret_val)
        {
            return ret_val;
        }

        /**    *
         *    Check the partition offset to confirm it is the boot record. */
        ret_val = check_partition_offset(p_buffer, &partition_offset);
    }

    /**  - check_partition_offset failed or the current buffer is not the boot sector */
    if((ret_val != SSP_SUCCESS) || (partition_offset != 0U))
    {
        *driver_status = (uint32_t) FX_MEDIA_INVALID;
        return ret_val == SSP_SUCCESS ? SSP_ERR_MEDIA_OPEN_FAILED : ret_val;
    }
    return ret_val;
}


/******************************************************************************************************************//**
 * @brief      Handles the FileX driver request
 * @param      p_fx_media      The pointer to FileX media
 *
 * @retval     SSP_SUCCESS                 Driver request fulfilled.
 * @retval     SSP_ERR_MEDIA_OPEN_FAILED   Not a valid boot record or partition table
 * @retval     SSP_ERR_UNSUPPORTED         Requested FileX command is not supported
 *
 * @return                     See @ref Common_Error_Codes or lower level drivers for other possible return
 *                             codes. This function calls
 *                              * sf_block_media_api_t::read
 *                              * sf_block_media_api_t::write
 *                              * sf_block_media_api_t::close
 *                              * sf_el_fx_driver_release_sectors_handler
 *                              * sf_el_fx_driver_init_handler
 *                              * sf_el_fx_driver_boot_read_handler
 *********************************************************************************************************************/
static ssp_err_t sf_el_fx_driver_request_handler(FX_MEDIA * p_fx_media)
{
    ssp_err_t                   ret_val       = SSP_SUCCESS;
    sf_block_media_instance_t * p_block_media = (sf_block_media_instance_t *) ((sf_el_fx_t *)
                                                                                   p_fx_media->fx_media_driver_info)
                                                    ->p_lower_lvl_block_media;

    /** Process the driver request specified in the media control block.  */
    switch (p_fx_media->fx_media_driver_request)
    {
        /** - FX_DRIVER_READ \n
         *  FileX reads one or more sectors into memory by issuing a read request to the I/O driver.
         *
         */
        case (UINT)FX_DRIVER_READ:
        {
            ret_val = p_block_media->p_api->read(p_block_media->p_ctrl,
                                                 p_fx_media->fx_media_driver_buffer,
                                                 p_fx_media->fx_media_driver_logical_sector +
                                                     p_fx_media->fx_media_hidden_sectors,
                                                 p_fx_media->fx_media_driver_sectors);
            break;
        }

        /** - FX_DRIVER_WRITE \n
         *  FileX writes one or more sectors to the physical media by issuing a write request to the I/O driver.
         *
         */
        case (UINT)FX_DRIVER_WRITE:
        {
            ret_val = p_block_media->p_api->write(p_block_media->p_ctrl,
                                                  p_fx_media->fx_media_driver_buffer,
                                                  p_fx_media->fx_media_driver_logical_sector +
                                                      p_fx_media->fx_media_hidden_sectors,
                                                  p_fx_media->fx_media_driver_sectors);
            break;
        }

        /** - FX_DRIVER_FLUSH \n
         *   FileX flushes all sectors currently in the driver's sector cache to the physical media by issuing a flush
         *   request to the I/O driver. Synergy drivers do not currently cache sectors.
         *    - Command not currently supported in available Synergy modules. Return driver success.
         */
        case (UINT)FX_DRIVER_FLUSH:
        {
            break;
        }

        /** - FX_DRIVER_RELEASE_SECTORS \n
         *  If previously selected by the driver during initialization, FileX informs the driver whenever one or more
         *  consecutive sectors become free.
         *
         */
        case (UINT)FX_DRIVER_RELEASE_SECTORS:
        {
            /* fx_media_driver_free_sector_update is set when FileX sends the FX_DRIVER_INIT request. FileX only
             * sends the FX_DRIVER_RELEASE_SECTORS request if fx_media_driver_free_sector_update is FX_TRUE
             */
            ret_val = sf_el_fx_driver_release_sectors_handler(p_block_media,
                                                              (uint32_t) p_fx_media->fx_media_driver_sectors,
                                                              (uint32_t) p_fx_media->fx_media_driver_logical_sector);
            break;
        }

        /** - FX_DRIVER_ABORT \n
         *  FileX informs the driver to abort all further physical I/O activity with the physical media by issuing an
         *  abort request to the I/O driver. The driver should not perform any I/O again until it is re-initialized.
         */
        case (UINT)FX_DRIVER_ABORT:
        {
            /**  *
            * Close/un-initialize device. */
            ret_val = p_block_media->p_api->close(p_block_media->p_ctrl);
            break;
        }

        /** - FX_DRIVER_INIT \n
         *  Although the actual driver initialization processing is application specific, it usually consists of data
         *  structure initialization and possibly some preliminary hardware initialization. This request is the first
         *  made by FileX and is done from within the fx_media_open service. If media write protection is detected,
         *  the driver fx_media_driver_write_protect member of FX_MEDIA should be set to FX_TRUE.
         */
        case (UINT)FX_DRIVER_INIT:
        {
            ret_val = sf_el_fx_driver_init_handler(p_block_media,
                                                   (uint32_t *) &p_fx_media->fx_media_driver_write_protect,
                                                   (uint32_t *) &p_fx_media->fx_media_driver_free_sector_update);
            break;
        }

        /** - FX_DRIVER_UNINIT \n
         *  FileX uses the uninit command to close the media.
         */
        case (UINT)FX_DRIVER_UNINIT:
        {
            /**  *
            * Close/un-initialize device. */
            ret_val = p_block_media->p_api->close(p_block_media->p_ctrl);
            break;
        }

        /** - FX_DRIVER_BOOT_READ \n
         *  Instead of using a standard read request, FileX makes a specific request to read the media's boot
         *  sector.
         */
        case (UINT)FX_DRIVER_BOOT_READ:
        {
            ret_val = sf_el_fx_driver_boot_read_handler(p_block_media,
                                                        p_fx_media->fx_media_driver_buffer,
                                                        (uint32_t *) &p_fx_media->fx_media_driver_status);
            break;
        }

        /** - FX_DRIVER_BOOT_WRITE \n
         *  Instead of using a standard write request, FileX makes a specific request to write the media's boot
         *  sector.
         */
        case (UINT)FX_DRIVER_BOOT_WRITE:
        {
            /**     *
             *     Write the boot record and return to the caller. */

            ret_val = p_block_media->p_api->write(p_block_media->p_ctrl,
                                                  p_fx_media->fx_media_driver_buffer,
                                                  p_fx_media->fx_media_hidden_sectors,
                                                  1);
            break;
        }

        /** - default \n
         *  Invalid driver request. */
        default:
        {
            p_fx_media->fx_media_driver_status = (UINT)FX_IO_ERROR;
            ret_val = SSP_ERR_UNSUPPORTED;
            break;
        }
    }
    return ret_val;
}
