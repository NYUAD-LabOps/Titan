/* generated common source file - do not edit */
#include "common_data.h"

#ifndef NX_DISABLE_IPV6
#ifndef FILL_NXD_IPV6_ADDRESS
#define FILL_NXD_IPV6_ADDRESS(ipv6,f0,f1,f2,f3,f4,f5,f6,f7) do { \
                                                                       ipv6.nxd_ip_address.v6[0] = (((uint32_t)f0 << 16) & 0xFFFF0000) | ((uint32_t)f1 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_address.v6[1] = (((uint32_t)f2 << 16) & 0xFFFF0000) | ((uint32_t)f3 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_address.v6[2] = (((uint32_t)f4 << 16) & 0xFFFF0000) | ((uint32_t)f5 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_address.v6[3] = (((uint32_t)f6 << 16) & 0xFFFF0000) | ((uint32_t)f7 & 0x0000FFFF);\
                                                                       ipv6.nxd_ip_version       = NX_IP_VERSION_V6;\
                                                                   } while(0);
#endif /* FILL_NXD_IPV6_ADDRESS */
#endif
#if (12) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer5) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_ADC0_COMPARE_MATCH)
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
  .p_dest = (void *) NULL,
  .p_src = (void const *) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_on_dmac_cfg_t g_transfer5_extend =
{ .channel = 4, .offset_byte = 0, };
const transfer_cfg_t g_transfer5_cfg =
{ .p_info = &g_transfer5_info, .activation_source = ELC_EVENT_ADC0_COMPARE_MATCH, .auto_enable = false, .p_callback =
          NULL,
  .p_context = &g_transfer5, .irq_ipl = (12), .p_extend = &g_transfer5_extend, };
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

  .p_extend = (void *) &g_sdmmc0_ext_cfg_t,
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
sf_el_fx_t g_sf_el_fx0_cfg =
{ .p_lower_lvl_block_media = &g_sf_block_media_sdmmc0 };
NX_REC nx_record1;
static NX_CALLBACK_REC g_sf_el_nx_callbacks =
{ .nx_ether_unknown_packet_receive_callback = NULL, .nx_ether_mac_address_change_callback = NULL, };
static sf_el_nx_cfg_t sf_el_nx1_cfg =
{ .channel = 1, .nx_mac_address =
{ .nx_mac_address_h = SF_EL_NX_CFG_ENET1_MAC_H, .nx_mac_address_l = SF_EL_NX_CFG_ENET1_MAC_L },
  .p_callback_rec = &g_sf_el_nx_callbacks, .etherc_ptr = R_ETHERC1, .edmac_ptr = R_EDMAC1, };
#if SF_EL_NX_CFG_IRQ_IPL != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_sf_el_nx) && !defined(SSP_SUPPRESS_ISR_EDMAC1)
SSP_VECTOR_DEFINE_CHAN(edmac_eint_isr, EDMAC, EINT, 1);
#endif
#endif

void nx_ether_driver_eth1(NX_IP_DRIVER *driver_req_ptr)
{
    nx_ether_driver (driver_req_ptr, &nx_record1, &sf_el_nx1_cfg);
}

/** Make user given name point to correct driver entry point. */
VOID (*g_sf_el_nx)(NX_IP_DRIVER *driver_req_ptr) = nx_ether_driver_eth1;
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void nx_common_init0(void)
 **********************************************************************************************************************/
void nx_common_init0(void)
{
    /** Initialize the NetX Duo system. */
    nx_system_initialize ();
}
NX_PACKET_POOL g_packet_pool0;
uint8_t g_packet_pool0_pool_memory[(16 * (1568 + sizeof(NX_PACKET)))];
#if defined(__ICCARM__)
#define g_packet_pool0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_packet_pool0_err_callback  = g_packet_pool0_err_callback_internal
#elif defined(__GNUC__)
#define g_packet_pool0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_packet_pool0_err_callback_internal")))
#endif
void g_packet_pool0_err_callback(void *p_instance, void *p_data)
g_packet_pool0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_packet_pool0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_packet_pool0_err_callback_internal(void *p_instance, void *p_data);
void g_packet_pool0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void packet_pool_init0(void)
 **********************************************************************************************************************/
void packet_pool_init0(void)
{
    UINT g_packet_pool0_err;
    /* Create Client packet pool. */
    g_packet_pool0_err = nx_packet_pool_create (&g_packet_pool0, "g_packet_pool0 Packet Pool", 1568,
                                                &g_packet_pool0_pool_memory[0], (16 * (1568 + sizeof(NX_PACKET))));
    if (NX_SUCCESS != g_packet_pool0_err)
    {
        g_packet_pool0_err_callback ((void *) &g_packet_pool0, &g_packet_pool0_err);
    }
}
NX_IP g_ip0;
#ifndef NX_DISABLE_IPV6
UINT g_ip0_interface_index = 0;
UINT g_ip0_address_index;
NXD_ADDRESS g_ip0_global_ipv6_address;
NXD_ADDRESS g_ip0_local_ipv6_address;
#endif            
uint8_t g_ip0_stack_memory[2048] BSP_PLACE_IN_SECTION_V2(".stack.g_ip0") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
#if 1 == 1                       // Check for ARP is enabled
#if (0 == 0)    // Check for ARP cache storage units is in bytes
#define    NX_ARP_CACHE_SIZE    (520)
#else
#define    NX_ARP_CACHE_SIZE    (520 * sizeof(NX_ARP))
#endif
uint8_t g_ip0_arp_cache_memory[NX_ARP_CACHE_SIZE] BSP_ALIGN_VARIABLE(4);
#endif
ULONG g_ip0_actual_status;

#ifndef NULL
#define NULL_DEFINE
void NULL(struct NX_IP_STRUCT *ip_ptr, UINT interface_index, UINT link_up);
#endif            
#if defined(__ICCARM__)
#define g_ip0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_ip0_err_callback  = g_ip0_err_callback_internal
#elif defined(__GNUC__)
#define g_ip0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_ip0_err_callback_internal")))
#endif
void g_ip0_err_callback(void *p_instance, void *p_data)
g_ip0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function
 *             with the prototype below.
 *             - void g_ip0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_ip0_err_callback_internal(void *p_instance, void *p_data);
void g_ip0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ip_init0(void)
 **********************************************************************************************************************/
void ip_init0(void)
{
    UINT g_ip0_err;
#ifndef NX_DISABLE_IPV6
    FILL_NXD_IPV6_ADDRESS(g_ip0_global_ipv6_address, 0x2001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1);
    FILL_NXD_IPV6_ADDRESS(g_ip0_local_ipv6_address, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

#endif
    /* Create an IP instance. */
    g_ip0_err = nx_ip_create (&g_ip0, "g_ip0 IP Instance", IP_ADDRESS (192, 168, 10, 181),
                              IP_ADDRESS (255, 255, 255, 0), &g_packet_pool0, g_sf_el_nx, &g_ip0_stack_memory[0], 2048,
                              0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#define SYNERGY_NOT_DEFINED     (0xFFFFFFFF)
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_arp_enable (&g_ip0, &g_ip0_arp_cache_memory[0], NX_ARP_CACHE_SIZE);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != SYNERGY_NOT_DEFINED)
    g_ip0_err = nx_rarp_enable(&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback((void *)&g_ip0,&g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_tcp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_udp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_icmp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != 1)
    g_ip0_err = nx_igmp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#endif
#if (SYNERGY_NOT_DEFINED != SYNERGY_NOT_DEFINED)
    g_ip0_err = nx_ip_fragment_enable(&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback((void *)&g_ip0,&g_ip0_err);
    }
#endif            
#undef SYNERGY_NOT_DEFINED

#ifndef NX_DISABLE_IPV6
    /** Here's where IPv6 is enabled. */
    g_ip0_err = nxd_ipv6_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
    g_ip0_err = nxd_icmp_enable (&g_ip0);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
    /* Wait for link to be initialized so MAC address is valid. */
    /** Wait for init to finish. */
    g_ip0_err = nx_ip_interface_status_check (&g_ip0, 0, NX_IP_INITIALIZE_DONE, &g_ip0_actual_status, NX_WAIT_FOREVER);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
    /** Setting link local address */
    if (0x0
            == (g_ip0_local_ipv6_address.nxd_ip_address.v6[0] | g_ip0_local_ipv6_address.nxd_ip_address.v6[1]
                    | g_ip0_local_ipv6_address.nxd_ip_address.v6[2] | g_ip0_local_ipv6_address.nxd_ip_address.v6[3]))
    {
        g_ip0_err = nxd_ipv6_address_set (&g_ip0, g_ip0_interface_index, NX_NULL, 10, NX_NULL);
    }
    else
    {
        g_ip0_err = nxd_ipv6_address_set (&g_ip0, g_ip0_interface_index, &g_ip0_local_ipv6_address, 10,
                                          &g_ip0_address_index);
    }
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
    if (0x0
            != (g_ip0_global_ipv6_address.nxd_ip_address.v6[0] | g_ip0_global_ipv6_address.nxd_ip_address.v6[1]
                    | g_ip0_global_ipv6_address.nxd_ip_address.v6[2] | g_ip0_global_ipv6_address.nxd_ip_address.v6[3]))
    {
        g_ip0_err = nxd_ipv6_address_set (&g_ip0, g_ip0_interface_index, &g_ip0_global_ipv6_address, 64,
                                          &g_ip0_address_index);
    }
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#endif

#ifdef NULL_DEFINE
    g_ip0_err = nx_ip_link_status_change_notify_set (&g_ip0, NULL);
    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback ((void *) &g_ip0, &g_ip0_err);
    }
#endif

    /* Gateway IP Address */
#define IP_VALID(a,b,c,d)     (a|b|c|d)
#if IP_VALID(0,0,0,0)
    g_ip0_err = nx_ip_gateway_address_set(&g_ip0,
            IP_ADDRESS(0,0,0,0));

    if (NX_SUCCESS != g_ip0_err)
    {
        g_ip0_err_callback((void *)&g_ip0,&g_ip0_err);
    }
#endif         
#undef IP_VALID

}
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
ssp_err_t SF_EL_FX_Get_MEDIA_Info(sf_block_media_instance_t *p_block_media, uint32_t *sector_size,
        uint32_t *sector_count);
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
    ssp_err_t error = SF_EL_FX_Get_MEDIA_Info (g_sf_el_fx0_cfg.p_lower_lvl_block_media, &sector_size, &sector_count);

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
    fx_ret_val = fx_media_format (&g_fx_media0, // Pointer to FileX media control block.
            SF_EL_FX_BlockDriver, // Driver entry
            &g_sf_el_fx0_cfg, // Pointer to Block Media Driver
            g_media_memory_g_fx_media0, // Media buffer pointer
            sizeof(g_media_memory_g_fx_media0), // Media buffer size
            (CHAR *) "Volume 1", // Volume Name
            1, // Number of FATs
            256, // Directory Entries
            0, // Hidden sectors
            sector_count, // Total sectors - Hidden Sectors
            sector_size, // Sector size
            1, // Sectors per cluster
            1, // Heads
            1); // Sectors per track

    if (FX_SUCCESS != fx_ret_val)
    {
        return SSP_ERR_MEDIA_FORMAT_FAILED;
    }

    return SSP_SUCCESS;
}

uint32_t fx_media_init0_open(void)
{
    return fx_media_open (&g_fx_media0, (CHAR *) "g_fx_media0", SF_EL_FX_BlockDriver, &g_sf_el_fx0_cfg,
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
        g_fx_media0_err_callback ((void *) &g_fx_media0, &err_open);
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
ssp_err_t SF_EL_FX_Get_MEDIA_Info(sf_block_media_instance_t *p_block_media, uint32_t *sector_size,
        uint32_t *sector_count)
{
    ssp_err_t ret_val = SSP_SUCCESS;

    ret_val = p_block_media->p_api->open (p_block_media->p_ctrl, p_block_media->p_cfg);
    if (ret_val != SSP_SUCCESS)
    {
        return ret_val;
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

    /* Close driver.  */
    ret_val = p_block_media->p_api->close (p_block_media->p_ctrl);
    return ret_val;
}
void g_common_init(void)
{
    /** Call initialization function if user has selected to do so. */
#if (1)
    nx_common_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    packet_pool_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (0)
    ip_init0();
#endif
    /** Call initialization function if user has selected to do so. */
#if FX_COMMON_INITIALIZE
    fx_common_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if SF_EL_FX_AUTO_INIT_g_fx_media0
    fx_media_init0();
#endif
}
