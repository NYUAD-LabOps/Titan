/* generated common source file - do not edit */
#include "common_data.h"
#if (12) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer5) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_SDHIMMC1_DMA_REQ)
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr, DMAC, INT, 4);
#endif
#endif
dmac_instance_ctrl_t g_transfer5_ctrl;
transfer_info_t g_transfer5_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .irq = TRANSFER_IRQ_EACH,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_on_dmac_cfg_t g_transfer5_extend =
{ .channel = 4, .offset_byte = 0, };
const transfer_cfg_t g_transfer5_cfg =
{ .p_info = &g_transfer5_info,
  .activation_source = ELC_EVENT_SDHIMMC1_DMA_REQ,
  .auto_enable = false,
  .p_callback = NULL,
  .p_context = &g_transfer5,
  .irq_ipl = (12),
  .p_extend = &g_transfer5_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer5 =
{ .p_ctrl = &g_transfer5_ctrl, .p_cfg = &g_transfer5_cfg, .p_api = &g_transfer_on_dmac };
#if (12) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_sdmmc0) && !defined(SSP_SUPPRESS_ISR_SDMMC1)
SSP_VECTOR_DEFINE_CHAN(sdhimmc_accs_isr, SDHIMMC, ACCS, 1);
#endif
#endif
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_sdmmc0) && !defined(SSP_SUPPRESS_ISR_SDMMC1)
SSP_VECTOR_DEFINE_CHAN(sdhimmc_card_isr, SDHIMMC, CARD, 1);
#endif
#endif
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_sdmmc0) && !defined(SSP_SUPPRESS_ISR_SDMMC1)
SSP_VECTOR_DEFINE_CHAN(sdhimmc_dma_req_isr, SDHIMMC, DMA_REQ, 1);
#endif
#endif
sdmmc_instance_ctrl_t g_sdmmc0_ctrl;

/** SDMMC extended configuration */
const sdmmc_extended_cfg_t g_sdmmc0_ext_cfg_t =
{ .block_size = 512, .card_detect = SDMMC_CARD_DETECT_NONE, .write_protect = SDMMC_WRITE_PROTECT_NONE, };

sdmmc_cfg_t g_sdmmc0_cfg =
{ .hw =
{ .media_type = SDMMC_MEDIA_TYPE_CARD, .bus_width = SDMMC_BUS_WIDTH_4_BITS, .channel = 1, },
  .p_callback = NULL,

  .p_extend = (void*) &g_sdmmc0_ext_cfg_t,
  .p_lower_lvl_transfer = &g_transfer5,

  .access_ipl = (12),
  .sdio_ipl = BSP_IRQ_DISABLED, .card_ipl = (BSP_IRQ_DISABLED), .dma_req_ipl = (BSP_IRQ_DISABLED), };
/* Instance structure to use this module. */
const sdmmc_instance_t g_sdmmc0 =
{ .p_ctrl = &g_sdmmc0_ctrl, .p_cfg = &g_sdmmc0_cfg, .p_api = &g_sdmmc_on_sdmmc };
static const sf_block_media_on_sdmmc_cfg_t g_sf_block_media_sdmmc0_block_media_cfg =
{ .p_lower_lvl_sdmmc = &g_sdmmc0, };
static sf_block_media_sdmmc_instance_ctrl_t g_sf_block_media_sdmmc0_ctrl;
static sf_block_media_cfg_t g_sf_block_media_sdmmc0_cfg =
{ .block_size = 512, .p_extend = &g_sf_block_media_sdmmc0_block_media_cfg };

sf_block_media_instance_t g_sf_block_media_sdmmc0 =
{ .p_ctrl = &g_sf_block_media_sdmmc0_ctrl, .p_cfg = &g_sf_block_media_sdmmc0_cfg, .p_api = &g_sf_block_media_on_sdmmc };
#define g_sf_el_fx0_total_partition       0U

#if (g_sf_el_fx0_total_partition > 1U)
            sf_el_fx_media_partition_data_info_t g_sf_el_fx0_partition_data_info[g_sf_el_fx0_total_partition];
            #endif

sf_el_fx_instance_ctrl_t g_sf_el_fx0_ctrl;

/** SF_EL_FX interface configuration */
const sf_el_fx_config_t g_sf_el_fx0_config =
{
#if (g_sf_el_fx0_total_partition > 1U) 
                .p_partition_data        = (sf_el_fx_media_partition_data_info_t *)g_sf_el_fx0_partition_data_info, 
            #else 
  .p_partition_data = NULL,
#endif 
  .p_lower_lvl_block_media = &g_sf_block_media_sdmmc0,
  .p_context = &g_sf_el_fx0_cfg, .p_extend = NULL, .total_partitions = g_sf_el_fx0_total_partition,
#if (g_sf_el_fx0_total_partition > 1U) 
                .p_callback              = NULL, 
            #else 
  .p_callback = NULL,
#endif 
        };

/* Instance structure to use this module. */
sf_el_fx_t g_sf_el_fx0_cfg =
{ .p_ctrl = &g_sf_el_fx0_ctrl, .p_config = &g_sf_el_fx0_config, };
#define FX_COMMON_INITIALIZE (1)
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void fx_common_init0(void)
 **********************************************************************************************************************/
void fx_common_init0(void)
{
    /** Initialize the FileX system. */
    fx_system_initialize ();
}
#if defined(__ICCARM__)
            #define g_fx_media0_err_callback_WEAK_ATTRIBUTE
            #pragma weak g_fx_media0_err_callback  = g_fx_media0_err_callback_internal
            #elif defined(__GNUC__)
            #define g_fx_media0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_fx_media0_err_callback_internal")))
            #endif
void g_fx_media0_err_callback(void *p_instance, void *p_data)
g_fx_media0_err_callback_WEAK_ATTRIBUTE;
#define SF_EL_FX_FORMAT_MEDIA_ENABLE_g_fx_media0 (0)
#define SF_EL_FX_FORMAT_FULL_MEDIA_g_fx_media0 (1)
#define SF_EL_FX_AUTO_INIT_g_fx_media0 (0)
ssp_err_t SF_EL_FX_Get_MEDIA_Info(sf_el_fx_ctrl_t *const p_api_ctrl, sf_el_fx_config_t const *const p_config,
        uint32_t *sector_size, uint32_t *sector_count);
FX_MEDIA g_fx_media0;
uint8_t g_media_memory_g_fx_media0[512];
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_fx_media0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_fx_media0_err_callback_internal(void *p_instance, void *p_data);
void g_fx_media0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

ssp_err_t fx_media_init0_format(void)
{
    uint32_t fx_ret_val = FX_SUCCESS;

    uint32_t sector_size = 512;
    uint32_t sector_count = 3751936;

#if SF_EL_FX_FORMAT_FULL_MEDIA_g_fx_media0
    ssp_err_t error = SF_EL_FX_Get_MEDIA_Info (g_sf_el_fx0_cfg.p_ctrl, g_sf_el_fx0_cfg.p_config, &sector_size,
                                               &sector_count);

    if ((error != SSP_SUCCESS) || (sector_count <= 0))
    {
        return SSP_ERR_MEDIA_FORMAT_FAILED;
    }

    sector_count -= 0;
#endif

    if (sizeof(g_media_memory_g_fx_media0) < 512)
    {
        return SSP_ERR_MEDIA_FORMAT_FAILED;
    }

    /* Format media.  */
#ifdef FX_ENABLE_EXFAT
                fx_ret_val = fx_media_exFAT_format(&g_fx_media0, // Pointer to FileX media control block.
                                             SF_EL_FX_BlockDriver, // Driver entry
                                             &g_sf_el_fx0_cfg, // Pointer to Block Media Driver
                                             g_media_memory_g_fx_media0, // Media buffer pointer
                                             sizeof(g_media_memory_g_fx_media0), // Media buffer size
                                             (CHAR *)"Volume 1", // Volume Name
                                             1, // Number of FATs
                                             0, // Hidden sectors
                                             sector_count, // Total sectors - Hidden Sectors
                                             sector_size, // Sector size
                                             1, // Sectors per cluster
                                             12345, // Volume Serial Number
                                             128); // Boundary unit
#else
    fx_ret_val = fx_media_format (&g_fx_media0, // Pointer to FileX media control block.
            SF_EL_FX_BlockDriver, // Driver entry
            &g_sf_el_fx0_cfg, // Pointer to Block Media Driver
            g_media_memory_g_fx_media0, // Media buffer pointer
            sizeof(g_media_memory_g_fx_media0), // Media buffer size
            (CHAR*) "Volume 1", // Volume Name
            1, // Number of FATs
            256, // Directory Entries
            0, // Hidden sectors
            sector_count, // Total sectors - Hidden Sectors
            sector_size, // Sector size
            1, // Sectors per cluster
            1, // Heads
            1); // Sectors per track
#endif
    if (FX_SUCCESS != fx_ret_val)
    {
        return SSP_ERR_MEDIA_FORMAT_FAILED;
    }

    return SSP_SUCCESS;
}

uint32_t fx_media_init0_open(void)
{
    return fx_media_open (&g_fx_media0, (CHAR*) "g_fx_media0", SF_EL_FX_BlockDriver, &g_sf_el_fx0_cfg,
                          g_media_memory_g_fx_media0, sizeof(g_media_memory_g_fx_media0));
}

/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void fx_media_init0(void)
 **********************************************************************************************************************/
void fx_media_init0(void)
{
#if SF_EL_FX_FORMAT_MEDIA_ENABLE_g_fx_media0

                ssp_err_t err_format = fx_media_init0_format();
                    
                if(err_format != SSP_SUCCESS)
                {
                    g_fx_media0_err_callback((void *)&g_fx_media0,&err_format);
                }

                #endif

    uint32_t err_open = fx_media_init0_open ();

    if (err_open != FX_SUCCESS)
    {
        g_fx_media0_err_callback ((void*) &g_fx_media0, &err_open);
    }
}
const elc_instance_t g_elc =
{ .p_api = &g_elc_on_elc, .p_cfg = NULL };
/* Instance structure to use this module. */
const fmi_instance_t g_fmi =
{ .p_api = &g_fmi_on_fmi };
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
const cgc_instance_t g_cgc =
{ .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
ssp_err_t SF_EL_FX_Get_MEDIA_Info(sf_el_fx_ctrl_t *const p_api_ctrl, sf_el_fx_config_t const *const p_config,
        uint32_t *sector_size, uint32_t *sector_count)
{
    sf_el_fx_instance_ctrl_t *p_ctrl = (sf_el_fx_instance_ctrl_t*) p_api_ctrl;
    sf_block_media_instance_t *p_block_media = (sf_block_media_instance_t*) p_config->p_lower_lvl_block_media;
    ssp_err_t ret_val = SSP_SUCCESS;

    if (SF_EL_FX_PARTITION_GLOBAL_OPEN != p_ctrl->media_info.global_open.status)
    {
        ret_val = p_block_media->p_api->open (p_block_media->p_ctrl, p_block_media->p_cfg);
        if (ret_val != SSP_SUCCESS)
        {
            return ret_val;
        }
    }

    /* Get actual sector size from media. */
    ret_val = p_block_media->p_api->ioctl (p_block_media->p_ctrl, SSP_COMMAND_GET_SECTOR_SIZE, sector_size);
    if (ret_val != SSP_SUCCESS)
    {
        return ret_val;
    }

    /* Get actual sector count from media. */
    ret_val = p_block_media->p_api->ioctl (p_block_media->p_ctrl, SSP_COMMAND_GET_SECTOR_COUNT, sector_count);
    if (ret_val != SSP_SUCCESS)
    {
        return ret_val;
    }

    if (SF_EL_FX_PARTITION_GLOBAL_OPEN != p_ctrl->media_info.global_open.status)
    {
        /* Close driver.  */
        ret_val = p_block_media->p_api->close (p_block_media->p_ctrl);
    }

    return ret_val;
}
void g_common_init(void)
{
    /** Call initialization function if user has selected to do so. */
#if FX_COMMON_INITIALIZE
    fx_common_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if SF_EL_FX_AUTO_INIT_g_fx_media0
                fx_media_init0();
            #endif
}
