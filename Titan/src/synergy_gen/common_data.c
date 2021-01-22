/* generated common source file - do not edit */
#include "common_data.h"

#define UX_HOST_INITIALIZE
#define USB_HOST_STORAGE_CLASS_REGISTER

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
{ .media_type = SDMMC_MEDIA_TYPE_CARD, .bus_width = SDMMC_BUS_WIDTH_1_BIT, .channel = 1, },
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
#if (1) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer1) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_ELC_SOFTWARE_EVENT_1)
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr, DMAC, INT, 1);
#endif
#endif
dmac_instance_ctrl_t g_transfer1_ctrl;
transfer_info_t g_transfer1_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .irq = TRANSFER_IRQ_EACH,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_BLOCK,
  .p_dest = (void *) NULL,
  .p_src = (void const *) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_on_dmac_cfg_t g_transfer1_extend =
{ .channel = 1, .offset_byte = 0, };
const transfer_cfg_t g_transfer1_cfg =
{ .p_info = &g_transfer1_info, .activation_source = ELC_EVENT_ELC_SOFTWARE_EVENT_1, .auto_enable = false, .p_callback =
          NULL,
  .p_context = &g_transfer1, .irq_ipl = (1), .p_extend = &g_transfer1_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{ .p_ctrl = &g_transfer1_ctrl, .p_cfg = &g_transfer1_cfg, .p_api = &g_transfer_on_dmac };
#if (1) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer0) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_ELC_SOFTWARE_EVENT_0)
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr, DMAC, INT, 0);
#endif
#endif
dmac_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .irq = TRANSFER_IRQ_EACH,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_BLOCK,
  .p_dest = (void *) NULL,
  .p_src = (void const *) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_on_dmac_cfg_t g_transfer0_extend =
{ .channel = 0, .offset_byte = 0, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .activation_source = ELC_EVENT_ELC_SOFTWARE_EVENT_0, .auto_enable = false, .p_callback =
          NULL,
  .p_context = &g_transfer0, .irq_ipl = (1), .p_extend = &g_transfer0_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dmac };
/***********************************************************************************************************************
 * Registers Interrupt Vector for USBHS Controller.
 **********************************************************************************************************************/
#if (SF_EL_UX_HCD_CFG_HS_IRQ_IPL != BSP_IRQ_DISABLED)
/* USBHS ISR vector registering. */
#if !defined(SSP_SUPPRESS_ISR_g_sf_el_ux_hcd_hs_0) && !defined(SSP_SUPPRESS_ISR_USB)
SSP_VECTOR_DEFINE_UNIT(usbhs_usb_int_resume_isr, USB, HS, USB_INT_RESUME, 0);
#endif
#endif

#undef SYNERGY_NOT_DEFINED
#define SYNERGY_NOT_DEFINED (1)
/***********************************************************************************************************************
 * The definition of wrapper interface for USBX Synergy Port HCD Driver.
 **********************************************************************************************************************/
static UINT g_sf_el_ux_hcd_hs_0_initialize(UX_HCD *hcd)
{
#if ((SYNERGY_NOT_DEFINED != g_transfer0) && (SYNERGY_NOT_DEFINED != g_transfer1))
    /* DMA support */
    UX_HCD_SYNERGY_TRANSFER hcd_transfer;
    hcd_transfer.ux_synergy_transfer_tx = (transfer_instance_t *) &g_transfer0;
    hcd_transfer.ux_synergy_transfer_rx = (transfer_instance_t *) &g_transfer1;
    return (UINT) ux_hcd_synergy_initialize_transfer_support (hcd, (UX_HCD_SYNERGY_TRANSFER *) &hcd_transfer);
#else
    /* Non DMA support */
    return (UINT)ux_hcd_synergy_initialize(hcd);
#endif
} /* End of function g_sf_el_ux_hcd_hs_0_initialize() */
#undef SYNERGY_NOT_DEFINED
/***********************************************************************************************************************
 * USB CDC-ACM Interface Descriptor for FS mode g_usb_interface_desc_cdcacm_0
 **********************************************************************************************************************/
#if defined(__GNUC__)
static volatile const unsigned char g_usb_interface_desc_cdcacm_0_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_interface_desc_fs") BSP_ALIGN_VARIABLE_V2(1)
#else /* __ICCARM__ */
#pragma section = ".usb_interface_desc_fs" 1
__root static const unsigned char g_usb_interface_desc_cdcacm_0_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_interface_desc_fs")
#endif
=
{
    /****************************************************************
     * Interface Association Descriptor(IAD)                        *
     ****************************************************************/
    0x08, /* 0 bLength */
    UX_INTERFACE_ASSOCIATION_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bFirstInterface */
    0x02, /* 3 bInterfaceCount */
    0x02, /* 4 bFunctionClass : Communication */
    0x02, /* 5 bFunctionSubClass : Abstract Control Model */
    0x00, /* 6 bFunctionProtocol : Standard or enhanced AT Command set protocol */
    0x00, /* 7 iFunction : String descriptor index */
    /****************************************************************
     * Communication Class Interface Descriptor                     *
     ****************************************************************/
    0x09, /* 0 bLength */
    UX_INTERFACE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bInterfaceNumber */
    0x00, /* 3 bAlternateSetting  : Alternate for SetInterface Request */
    0x01, /* 4 bNumEndpoints      : 1 Endpoint for Interface#1 */
    0x02, /* 5 bInterfaceClass    : Communications Interface Class(0x2) */
    0x02, /* 6 bInterfaceSubClass : Abstract Control Model(0x2) */
    0x01, /* 7 bInterfaceProtocol : Common AT command(0x01) */
    0x00, /* 8 iInterface Index */
    /****************************************************************
     * Header Functional Descriptor                                 *
     ****************************************************************/
    0x05, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType : CS_INTERFACE(24h) */
    0x00, /* 2 bDescriptorSubtype : Header Functional Descriptor(0x0) */
    0x10, /* 3 bcdCDC Number  0x0110 == 1.10 */
    0x01, /* 4 bcdCDC */
    /****************************************************************
     * Abstract Control Management Functional Functional Descriptor *
     ****************************************************************/
    0x04, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType : CS_INTERFACE(24h) */
    0x02, /* 2 bDescriptorSubtype : Abstract Control Management Functional Descriptor(0x2) */
    0x06, /* 3 bmCapabilities (Supports SendBreak, GetLineCoding, SetControlLineState, GetLineCoding) */
    /****************************************************************
     * Union Functional Descriptor                                  *
     ****************************************************************/
    0x05, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType : CS_INTERFACE(24h) */
    0x06, /* 2 bDescriptorSubtype : Union Functional Descriptor(0x6) */
    0x00, /* 3 bMasterInterface */
    0x01, /* 4 bSubordinateInterface0 */
    /****************************************************************
     * Call Management Functional Descriptor                        *
     ****************************************************************/
    0x05, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType */
    0x01, /* 2 bDescriptorSubtype : Call Management Functional Descriptor(0x1) */
    0x03, /* 3 bmCapabilities */
    0x01, /* 4 bDataInterface */
    /****************************************************************
     * CDC-ACM Endpoint descriptor (Interrupt) for Interface#0      *
     ****************************************************************/
    0x07, /* 0 bLength */
    UX_ENDPOINT_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (UX_ENDPOINT_IN | 3), /* 2 bEndpointAddress */
    UX_INTERRUPT_ENDPOINT, /* 3 bmAttributes  */
    0x08, /* 4 wMaxPacketSize */
    0x00, /* 5 wMaxPacketSize */
    0x0F, /* 6 bInterval */
    /****************************************************************
     * CDC-ACM Data Class Interface Descriptor                      *
     ****************************************************************/
    0x09, /* 0 bLength */
    UX_INTERFACE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x01, /* 2 bInterfaceNumber */
    0x00, /* 3 bAlternateSetting  : Alternate for SetInterface Request */
    0x02, /* 4 bNumEndpoints      : 2 Endpoints for Interface#0 */
    0x0A, /* 5 bInterfaceClass    : Data Interface Class(0xA) */
    0x00, /* 6 bInterfaceSubClass : Abstract Control Model */
    0x00, /* 7 bInterfaceProtocol : No class specific protocol required */
    0x00, /* 8 iInterface Index   : String descriptor index */
    /****************************************************************
     * CDC-ACM Endpoint Descriptor (Bulk-Out)  for Interface#1      *
     ****************************************************************/
    0x07, /* 0 bLength */
    UX_ENDPOINT_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (UX_ENDPOINT_OUT | 1), /* 2 bEndpointAddress */
    UX_BULK_ENDPOINT, /* 3 bmAttributes  */
    0x40, /* 4 wMaxPacketSize */
    0x00, /* 5 wMaxPacketSize */
    0x00, /* 6 bInterval */
    /****************************************************************
     * CDC-ACM Endpoint Descriptor (Bulk-In) for Interface#1        *
     ****************************************************************/
    0x07, /* 0 bLength */
    UX_ENDPOINT_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (UX_ENDPOINT_IN | 2), /* 2 bEndpointAddress */
    UX_BULK_ENDPOINT, /* 3 bmAttributes  */
    0x40, /* 4 wMaxPacketSize */
    0x00, /* 5 wMaxPacketSize */
    0x00, /* 6 bInterval */
};

#if defined(UX_DCD_SYNERY_USE_USBHS)
/***********************************************************************************************************************
 * USB CDC-ACM Interface Descriptor for HS mode g_usb_interface_desc_cdcacm_0
 **********************************************************************************************************************/
#if defined(__GNUC__)
static volatile const unsigned char g_usb_interface_desc_cdcacm_0_high_speed[] BSP_PLACE_IN_SECTION_V2(".usb_interface_desc_hs") BSP_ALIGN_VARIABLE_V2(1)
#else /* __ICCARM__ */
#pragma section = ".usb_interface_desc_hs" 1
__root static const unsigned char g_usb_interface_desc_cdcacm_0_high_speed[] BSP_PLACE_IN_SECTION_V2(".usb_interface_desc_hs")
#endif
=
{
    /****************************************************************
     * Interface Association Descriptor(IAD)                        *
     ****************************************************************/
    0x08, /* 0 bLength */
    UX_INTERFACE_ASSOCIATION_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bFirstInterface */
    0x02, /* 3 bInterfaceCount */
    0x02, /* 4 bFunctionClass : Communication */
    0x02, /* 5 bFunctionSubClass : Abstract Control Model */
    0x00, /* 6 bFunctionProtocol : Standard or enhanced AT Command set protocol */
    0x00, /* 7 iFunction : String descriptor index */
    /****************************************************************
     * Communication Class Interface Descriptor                     *
     ****************************************************************/
    0x09, /* 0 bLength */
    UX_INTERFACE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bInterfaceNumber */
    0x00, /* 3 bAlternateSetting  : Alternate for SetInterface Request */
    0x01, /* 4 bNumEndpoints      : 1 Endpoint for Interface#1 */
    0x02, /* 5 bInterfaceClass    : Communications Interface Class(0x2) */
    0x02, /* 6 bInterfaceSubClass : Abstract Control Model(0x2) */
    0x01, /* 7 bInterfaceProtocol : Common AT command(0x01) */
    0x00, /* 8 iInterface Index */
    /****************************************************************
     * Header Functional Descriptor                                 *
     ****************************************************************/
    0x05, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType : CS_INTERFACE(24h) */
    0x00, /* 2 bDescriptorSubtype : Header Functional Descriptor(0x0) */
    0x10, /* 3 bcdCDC Number  0x0110 == 1.10 */
    0x01, /* 4 bcdCDC */
    /****************************************************************
     * Abstract Control Management Functional Functional Descriptor *
     ****************************************************************/
    0x04, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType : CS_INTERFACE(24h) */
    0x02, /* 2 bDescriptorSubtype : Abstract Control Management Functional Descriptor(0x2) */
    0x06, /* 3 bmCapabilities (Supports SendBreak, GetLineCoding, SetControlLineState, GetLineCoding) */
    /****************************************************************
     * Union Functional Descriptor                                  *
     ****************************************************************/
    0x05, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType : CS_INTERFACE(24h) */
    0x06, /* 2 bDescriptorSubtype : Union Functional Descriptor(0x6) */
    0x00, /* 3 bMasterInterface */
    0x01, /* 4 bSubordinateInterface0 */
    /****************************************************************
     * Call Management Functional Descriptor                        *
     ****************************************************************/
    0x05, /* 0 bFunctionLength */
    0x24, /* 1 bDescriptorType */
    0x01, /* 2 bDescriptorSubtype : Call Management Functional Descriptor(0x1) */
    0x03, /* 3 bmCapabilities */
    0x01, /* 4 bDataInterface */
    /****************************************************************
     * CDC-ACM Endpoint descriptor (Interrupt) for Interface#0      *
     ****************************************************************/
    0x07, /* 0 bLength */
    UX_ENDPOINT_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (UX_ENDPOINT_IN | 3), /* 2 bEndpointAddress */
    UX_INTERRUPT_ENDPOINT, /* 3 bmAttributes  */
    0x08, /* 4 wMaxPacketSize */
    0x00, /* 5 wMaxPacketSize */
    0x0F, /* 6 bInterval */
    /****************************************************************
     * CDC-ACM Data Class Interface Descriptor                      *
     ****************************************************************/
    0x09, /* 0 bLength */
    UX_INTERFACE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x01, /* 2 bInterfaceNumber */
    0x00, /* 3 bAlternateSetting  : Alternate for SetInterface Request */
    0x02, /* 4 bNumEndpoints      : 2 Endpoints for Interface#0 */
    0x0A, /* 5 bInterfaceClass    : Data Interface Class(0xA) */
    0x00, /* 6 bInterfaceSubClass : Abstract Control Model */
    0x00, /* 7 bInterfaceProtocol : No class specific protocol required */
    0x00, /* 8 iInterface Index   : String descriptor index */
    /****************************************************************
     * CDC-ACM Endpoint Descriptor (Bulk-Out)  for Interface#1      *
     ****************************************************************/
    0x07, /* 0 bLength */
    UX_ENDPOINT_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (UX_ENDPOINT_OUT | 1), /* 2 bEndpointAddress */
    UX_BULK_ENDPOINT, /* 3 bmAttributes  */
    0x00, /* 4 wMaxPacketSize */
    0x02, /* 5 wMaxPacketSize */
    0x00, /* 6 bInterval */
    /****************************************************************
     * CDC-ACM Endpoint Descriptor (Bulk-In) for Interface#1        *
     ****************************************************************/
    0x07, /* 0 bLength */
    UX_ENDPOINT_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (UX_ENDPOINT_IN | 2), /* 2 bEndpointAddress */
    UX_BULK_ENDPOINT, /* 3 bmAttributes  */
    0x00, /* 4 wMaxPacketSize */
    0x02, /* 5 wMaxPacketSize */
    0x00, /* 6 bInterval */
};
#endif

/* Size of this USB Interface Descriptor */
#define USB_IFD_SIZE_0x00       (sizeof(g_usb_interface_desc_cdcacm_0_full_speed))
/* Number of Interface this USB Interface Descriptor has */
#define USB_IFD_NUM_0x00        (2)
#ifndef USB_IFD_NUM_0
#define USB_IFD_NUM_0  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_0 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_1
#define USB_IFD_NUM_1  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_1 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_2
#define USB_IFD_NUM_2  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_2 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_3
#define USB_IFD_NUM_3  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_3 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_4
#define USB_IFD_NUM_4  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_4 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_5
#define USB_IFD_NUM_5  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_5 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_6
#define USB_IFD_NUM_6  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_6 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_7
#define USB_IFD_NUM_7  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_7 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_8
#define USB_IFD_NUM_8  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_8 USB_IFD_SIZE_0x00
#else
#ifndef USB_IFD_NUM_9
#define USB_IFD_NUM_9  USB_IFD_NUM_0x00
#define USB_IFD_SIZE_9 USB_IFD_SIZE_0x00
#endif 
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#if defined(__ICCARM__)
#define ux_v2_err_callback_WEAK_ATTRIBUTE
#pragma weak ux_v2_err_callback  = ux_v2_err_callback_internal
#elif defined(__GNUC__)
#define ux_v2_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("ux_v2_err_callback_internal")))
#endif
void ux_v2_err_callback(void *p_instance, void *p_data)
ux_v2_err_callback_WEAK_ATTRIBUTE;
/* Static memory pool allocation used by USBX system. */
static CHAR g_ux_pool_memory[65536];
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void ux_v2_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void ux_v2_err_callback_internal(void *p_instance, void *p_data);
void ux_v2_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

#ifdef UX_HOST_INITIALIZE
/* Function prototype for the function to notify a USB event from the USBX Host system. */
static UINT ux_system_host_change_function(ULONG event, UX_HOST_CLASS *host_class, VOID *instance);

#ifdef USB_HOST_STORAGE_CLASS_REGISTER

/* A semaphore for synchronizing to the USB media storage mount timing. */
static TX_SEMAPHORE ux_host_storage_semaphore_insertion;

/* Temporary storage place for the pointer to a USBX Host Mass Storage Class Instance. */
static UX_HOST_CLASS_STORAGE *g_ux_new_host_storage_instance;

/* Function prototype for the function to register the USBX Host Class Mass Storage. */
static void ux_host_stack_class_register_storage(void);

/* Function prototype for the function to notify changes happened to the USBX Host Class Mass Storage. */
static UINT ux_system_host_storage_change_function(ULONG event, VOID *instance);

/*******************************************************************************************************************//**
 * @brief      This is the function to register the USBX Host Class Mass Storage.
 **********************************************************************************************************************/
static void ux_host_stack_class_register_storage(void)
{
    UINT status;
    status = ux_host_stack_class_register (_ux_system_host_class_storage_name, ux_host_class_storage_entry);
    if (UX_SUCCESS != status)
    {
        ux_v2_err_callback (NULL, &status);
    }

    /* Create semaphores used for the USB Mass Storage Media Initialization. */
    status = tx_semaphore_create (&ux_host_storage_semaphore_insertion, "ux_host_storage_semaphore_insertion", 0);
    if (TX_SUCCESS != status)
    {
        ux_v2_err_callback (NULL, &status);
    }
}

/*******************************************************************************************************************//**
 * @brief      This is the function to notify changes happened to the USBX Host Class Mass Storage.
 * @param[in]  event      Event code from happened to the USBX Host system.
 * @param[in]  instance   Pointer to a USBX Host class instance, which occurs a event.
 * @retval     USBX error code
 **********************************************************************************************************************/
static UINT ux_system_host_storage_change_function(ULONG event, VOID *instance)
{
    if (UX_DEVICE_INSERTION == event) /* Check if there is a device insertion. */
    {
        g_ux_new_host_storage_instance = instance;

        /* Put the semaphore for a USBX Mass Storage Media insertion. */
        tx_semaphore_put (&ux_host_storage_semaphore_insertion);
    }
    else if (UX_DEVICE_REMOVAL == event) /* Check if there is a device removal. */
    {
        g_ux_new_host_storage_instance = NULL;
    }
    return UX_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      This is the function to get the FileX Media Control Block for a USB Mass Storage device.
 * @param[in]      new_instance     Pointer to a USBX Host Mass Storage Class instance.
 * @param[in/out]  p_storage_media  Pointer of the pointer to save an instance of the USBX Host Mass Storage Media.
 * @param[in/out]  p_fx_media       Pointer of the pointer to save an instance of FileX Media Control Block.
 * @retval     UX_HOST_CLASS_INSTANCE_UNKNOWN  The instance provided was not for a valid Mass Storage device.
 **********************************************************************************************************************/
UINT ux_system_host_storage_fx_media_get(UX_HOST_CLASS_STORAGE *instance, UX_HOST_CLASS_STORAGE_MEDIA **p_storage_media,
        FX_MEDIA **p_fx_media)
{
    UINT error = UX_SUCCESS;
    UX_HOST_CLASS *p_host_class;
    UX_HOST_CLASS_STORAGE_MEDIA *p_storage_media_local;
    FX_MEDIA *p_fx_media_local;
    int index;

    /** Get the USBX Host Mass Storage Class. */
    ux_host_stack_class_get (_ux_system_host_class_storage_name, &p_host_class);

    /** Get the pointer to a media attached to the class container for USB Host Mass Storage. */
    p_storage_media_local = (UX_HOST_CLASS_STORAGE_MEDIA *) p_host_class->ux_host_class_media;

    /** Get the pointer to a FileX Media Control Block. */
    for (index = 0; index < UX_HOST_CLASS_STORAGE_MAX_MEDIA; index++)
    {
        p_fx_media_local = &p_storage_media_local->ux_host_class_storage_media;
        if (p_fx_media_local->fx_media_driver_info != instance)
        {
            /* None of FileX Media Control Blocks matched to this USBX Host Mass Storage Instance. */
            p_storage_media_local++;
        }
        else
        {
            /* Found a FileX Media Control Block used for this USBX Host Mass Storage Instance. */
            break;
        }
    }
    if (UX_HOST_CLASS_STORAGE_MAX_MEDIA == index)
    {
        error = UX_HOST_CLASS_INSTANCE_UNKNOWN;
    }
    else
    {
        *p_storage_media = p_storage_media_local;
        *p_fx_media = p_fx_media_local;
    }

    return error;
}
#endif /* USB_HOST_STORAGE_CLASS_REGISTER */

/*******************************************************************************************************************//**
 * @brief      This is the function to notify a USB event from the USBX Host system.
 * @param[in]  event     Event code from happened to the USBX Host system.
 * @param[in]  usb_class Pointer to a USBX Host class, which occurs a event.
 * @param[in]  instance  Pointer to a USBX Host class instance, which occurs a event.
 * @retval     USBX error code
 **********************************************************************************************************************/
static UINT ux_system_host_change_function(ULONG event, UX_HOST_CLASS *host_class, VOID *instance)
{
    UINT status = UX_SUCCESS;
    SSP_PARAMETER_NOT_USED (event);
    SSP_PARAMETER_NOT_USED (host_class);
    SSP_PARAMETER_NOT_USED (instance);

#if !defined(usb_host_plug_event_notification)
    /* Call user function back for USBX Host Class event notification. */
    status = usb_host_plug_event_notification (event, host_class, instance);
    if (UX_SUCCESS != status)
    {
        return status;
    }
#endif

#ifdef USB_HOST_STORAGE_CLASS_REGISTER
    /* Check the class container if it is for a USBX Host Mass Storage class. */
    if (UX_SUCCESS
            == _ux_utility_memory_compare (_ux_system_host_class_storage_name, host_class,
                                           _ux_utility_string_length_get (_ux_system_host_class_storage_name)))
    {
        status = ux_system_host_storage_change_function (event, instance);
    }
#endif
    return status;
}
#endif

#ifdef USB_HOST_HID_CLASS_REGISTER
/* Function prototype to register USBX Host HID Clients to the USBX Host system. */
static void ux_host_class_hid_clients_register(void);
#endif

/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_common_init0(void)
 **********************************************************************************************************************/
void ux_common_init0(void)
{
    UINT status_ux_init;

    /** Initialize the USBX system. */
    status_ux_init = ux_system_initialize ((CHAR *) g_ux_pool_memory, 65536, UX_NULL, 0);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback (NULL, &status_ux_init);
    }

#ifdef UX_HOST_INITIALIZE
    /** Initialize the USBX Host stack. */
    status_ux_init = ux_host_stack_initialize (ux_system_host_change_function);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback (NULL, &status_ux_init);
    }

    /** Register USB Host classes. */
#ifdef USB_HOST_HUB_CLASS_REGISTER
    status_ux_init = ux_host_stack_class_register(_ux_system_host_class_hub_name, ux_host_class_hub_entry);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback(NULL,&status_ux_init);
    }
#endif

#ifdef USB_HOST_CDC_ACM_CLASS_REGISTER
    status_ux_init = ux_host_stack_class_register(_ux_system_host_class_cdc_acm_name, ux_host_class_cdc_acm_entry);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback(NULL,&status_ux_init);
    }
#endif

#ifdef USB_HOST_HID_CLASS_REGISTER
    status_ux_init = ux_host_stack_class_register(_ux_system_host_class_hid_name, ux_host_class_hid_entry);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback(NULL,&status_ux_init);
    }

    ux_host_class_hid_clients_register ();
#endif

#ifdef USB_HOST_STORAGE_CLASS_REGISTER
    ux_host_stack_class_register_storage ();
#endif

#ifdef USB_HOST_VIDEO_CLASS_REGISTER
    status_ux_init = ux_host_stack_class_register(_ux_system_host_class_video_name, ux_host_class_video_entry);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback(NULL,&status_ux_init);
    }
#endif

#ifdef USB_HOST_AUDIO_CLASS_REGISTER
    status_ux_init = ux_host_stack_class_register(_ux_system_host_class_audio_name, ux_host_class_audio_entry);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback(NULL,&status_ux_init);
    }
#endif

#ifdef USB_HOST_PRINTER_CLASS_REGISTER
    status_ux_init = ux_host_stack_class_register(_ux_system_host_class_printer_name, _ux_host_class_printer_entry);
    if (UX_SUCCESS != status_ux_init)
    {
        ux_v2_err_callback(NULL,&status_ux_init);
    }
#endif

#endif /* UX_HOST_INITIALIZE */
}
#if defined(__ICCARM__)
#define g_ux_host_0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_ux_host_0_err_callback  = g_ux_host_0_err_callback_internal
#elif defined(__GNUC__)
#define g_ux_host_0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_ux_host_0_err_callback_internal")))
#endif
void g_ux_host_0_err_callback(void *p_instance, void *p_data)
g_ux_host_0_err_callback_WEAK_ATTRIBUTE;
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void g_ux_host_0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_ux_host_0_err_callback_internal(void *p_instance, void *p_data);
void g_ux_host_0_err_callback_internal(void *p_instance, void *p_data)
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
 *            - void ux_host_init0(void)
 **********************************************************************************************************************/
void ux_host_init0(void)
{
    /** Register a USB host controller. */
    UINT status_g_ux_host_0 = ux_host_stack_hcd_register ((UCHAR *) "g_sf_el_ux_hcd_hs_0",
                                                          g_sf_el_ux_hcd_hs_0_initialize, R_USBHS_BASE,
                                                          UX_SYNERGY_CONTROLLER);
    if (UX_SUCCESS != status_g_ux_host_0)
    {
        g_ux_host_0_err_callback (NULL, &status_g_ux_host_0);
    }
}
/* Pointer to a USBX Host Mass Storage Class Instance */
UX_HOST_CLASS_STORAGE *g_ux_host_class_storage0;
#if (1) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer3) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_USBFS_FIFO_1)
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr, DMAC, INT, 3);
#endif
#endif
dmac_instance_ctrl_t g_transfer3_ctrl;
transfer_info_t g_transfer3_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .irq = TRANSFER_IRQ_EACH,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_BLOCK,
  .p_dest = (void *) NULL,
  .p_src = (void const *) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_on_dmac_cfg_t g_transfer3_extend =
{ .channel = 3, .offset_byte = 0, };
const transfer_cfg_t g_transfer3_cfg =
{ .p_info = &g_transfer3_info,
  .activation_source = ELC_EVENT_USBFS_FIFO_1,
  .auto_enable = false,
  .p_callback = NULL,
  .p_context = &g_transfer3,
  .irq_ipl = (1),
  .p_extend = &g_transfer3_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer3 =
{ .p_ctrl = &g_transfer3_ctrl, .p_cfg = &g_transfer3_cfg, .p_api = &g_transfer_on_dmac };
#if (1) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_transfer2) && !defined(SSP_SUPPRESS_ISR_DMACELC_EVENT_USBFS_FIFO_0)
SSP_VECTOR_DEFINE_CHAN(dmac_int_isr, DMAC, INT, 2);
#endif
#endif
dmac_instance_ctrl_t g_transfer2_ctrl;
transfer_info_t g_transfer2_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .irq = TRANSFER_IRQ_EACH,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .size = TRANSFER_SIZE_1_BYTE,
  .mode = TRANSFER_MODE_BLOCK,
  .p_dest = (void *) NULL,
  .p_src = (void const *) NULL,
  .num_blocks = 0,
  .length = 0, };
const transfer_on_dmac_cfg_t g_transfer2_extend =
{ .channel = 2, .offset_byte = 0, };
const transfer_cfg_t g_transfer2_cfg =
{ .p_info = &g_transfer2_info,
  .activation_source = ELC_EVENT_USBFS_FIFO_0,
  .auto_enable = false,
  .p_callback = NULL,
  .p_context = &g_transfer2,
  .irq_ipl = (1),
  .p_extend = &g_transfer2_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer2 =
{ .p_ctrl = &g_transfer2_ctrl, .p_cfg = &g_transfer2_cfg, .p_api = &g_transfer_on_dmac };
#if defined(__ICCARM__)
#define g_sf_el_ux_dcd_fs_0_err_callback_WEAK_ATTRIBUTE
#pragma weak g_sf_el_ux_dcd_fs_0_err_callback  = g_sf_el_ux_dcd_fs_0_err_callback_internal
#elif defined(__GNUC__)
#define g_sf_el_ux_dcd_fs_0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_sf_el_ux_dcd_fs_0_err_callback_internal")))
#endif
void g_sf_el_ux_dcd_fs_0_err_callback(void *p_instance, void *p_data)
g_sf_el_ux_dcd_fs_0_err_callback_WEAK_ATTRIBUTE;
#if (SF_EL_UX_CFG_FS_IRQ_IPL != BSP_IRQ_DISABLED)
/* USBFS ISR vector registering. */
#if !defined(SSP_SUPPRESS_ISR_g_sf_el_ux_dcd_fs_0) && !defined(SSP_SUPPRESS_ISR_USB)
SSP_VECTOR_DEFINE_UNIT(usbfs_int_isr, USB, FS, INT, 0);
#endif
#endif

/* Prototype function for USBX DCD Initializer. */
void ux_dcd_initialize(void);

#undef SYNERGY_NOT_DEFINED
#define SYNERGY_NOT_DEFINED (1)
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void g_sf_el_ux_dcd_fs_0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_sf_el_ux_dcd_fs_0_err_callback_internal(void *p_instance, void *p_data);
void g_sf_el_ux_dcd_fs_0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}
#if ((SYNERGY_NOT_DEFINED != g_transfer2) && (SYNERGY_NOT_DEFINED != g_transfer3))
/***********************************************************************************************************************
 * The definition of wrapper interface for USBX Synergy Port DCD Driver to get a transfer module instance.
 **********************************************************************************************************************/
static UINT g_sf_el_ux_dcd_fs_0_initialize_transfer_support(ULONG dcd_io)
{
    UX_DCD_SYNERGY_TRANSFER dcd_transfer;
    dcd_transfer.ux_synergy_transfer_tx = (transfer_instance_t *) &g_transfer2;
    dcd_transfer.ux_synergy_transfer_rx = (transfer_instance_t *) &g_transfer3;
    return (UINT) ux_dcd_synergy_initialize_transfer_support (dcd_io, (UX_DCD_SYNERGY_TRANSFER *) &dcd_transfer);
} /* End of function g_sf_el_ux_dcd_fs_0_initialize_transfer_support() */
#endif

/***********************************************************************************************************************
 * Initializes USBX Device Controller Driver.
 **********************************************************************************************************************/
void ux_dcd_initialize(void)
{
    UINT status;
    /* Initializes the USB device controller, enabling DMA transfer if transfer module instances are given. */
#if ((SYNERGY_NOT_DEFINED == g_transfer2) || (SYNERGY_NOT_DEFINED == g_transfer3))
    status = (UINT)ux_dcd_synergy_initialize(R_USBFS_BASE);
#else
    status = g_sf_el_ux_dcd_fs_0_initialize_transfer_support (R_USBFS_BASE);
#endif
#undef SYNERGY_NOT_DEFINED
    if (UX_SUCCESS != status)
    {
        g_sf_el_ux_dcd_fs_0_err_callback (NULL, &status);
    }
} /* End of function ux_dcd_initialize() */
#if defined(__ICCARM__)
#define ux_device_err_callback_WEAK_ATTRIBUTE
#pragma weak ux_device_err_callback  = ux_device_err_callback_internal
#elif defined(__GNUC__)
#define ux_device_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("ux_device_err_callback_internal")))
#endif
void ux_device_err_callback(void *p_instance, void *p_data)
ux_device_err_callback_WEAK_ATTRIBUTE;
/* Definition for the special linker section for USB */
#if defined(__GNUC__)
extern uint32_t __usb_dev_descriptor_start_fs;
extern uint32_t __usb_descriptor_end_fs;
#if defined(UX_DCD_SYNERY_USE_USBHS)
extern uint32_t __usb_dev_descriptor_start_hs;
extern uint32_t __usb_descriptor_end_hs;
#endif
#endif

/* Counter to calculate number of Interfaces */
static uint8_t g_usbx_number_of_interface_count = 0;

/***********************************************************************************************************************
 * USB Device Descriptor for FS mode
 **********************************************************************************************************************/
#if defined(__GNUC__)
static volatile const unsigned char device_framework_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_device_desc_fs") BSP_ALIGN_VARIABLE_V2(1)
#else /* __ICCARM__ */
#pragma section = ".usb_device_desc_fs" 1
__root static const unsigned char device_framework_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_device_desc_fs")
#endif
=
{
    0x12, /* 0 bLength */
    UX_DEVICE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bcdUSB BCD(2.0) */
    0x02, /* 3 bcdUSB */
    0x02, /* 4 bDeviceClass    : Device Class */
#if (0x02 == 0x00) || (0x02 == 0x02) || (0x02 == 0x03) || (0x02 == 0x08)
    0x00, /* 5 bDeviceSubClass : none */
    0x00, /* 6 bDeviceProtocol : none */
#elif (0x02 == 0xEF)
    0x02, /* 5 bDeviceSubClass : Common Class(0x02) */
    0x01, /* 6 bDeviceProtocol :Interface Association Descriptor(IAD) */
#elif (0x02 == 0xFF)
    0x00, /* 5 bDeviceSubClass : none */
    0x00, /* 6 bDeviceProtocol : none */
#endif
    0x40, /* 7 bMaxPacketSize0 */
    (uint8_t)(0x045B), /* 8 idVendor */
    (uint8_t)(0x045B >> 8), /* 9 idVendor */
    (uint8_t)(0x0000), /* 10 idProduct */
    (uint8_t)(0x0000 >> 8), /* 11 idProduct */
    (uint8_t)(0x0000), /* 12 bcdDevice */
    (uint8_t)(0x0000 >> 8), /* 13 bcdDevice */
    0x00, /* 14 iManufacturer */
    0x00, /* 15 iProduct */
    0x00, /* 16 iSerialNumber */
    0x01, /* 17 bNumConfigurations */
};

#if defined(UX_DCD_SYNERY_USE_USBHS)
/***********************************************************************************************************************
 * USB Device Descriptor for HS Mode
 **********************************************************************************************************************/
#if defined(__GNUC__)
static volatile const unsigned char device_framework_high_speed[] BSP_PLACE_IN_SECTION_V2(".usb_device_desc_hs") BSP_ALIGN_VARIABLE_V2(1)
#else /* __ICCARM__ */
#pragma section = ".usb_device_desc_hs" 1
__root static const unsigned char device_framework_high_speed[] BSP_PLACE_IN_SECTION_V2(".usb_device_desc_hs")
#endif
=
{
    0x12, /* 0 bLength */
    UX_DEVICE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bcdUSB BCD(2.0) */
    0x02, /* 3 bcdUSB */
    0x02, /* 4 bDeviceClass    : Device Class */
#if (0x02 == 0x00) || (0x02 == 0x02) || (0x02 == 0x03) || (0x02 == 0x08)
    0x00, /* 5 bDeviceSubClass : none */
    0x00, /* 6 bDeviceProtocol : none */
#elif (0x02 == 0xEF) /* Composite device */
    0x02, /* 5 bDeviceSubClass : Common Class(0x02) */
    0x01, /* 6 bDeviceProtocol :Interface Association Descriptor(IAD) */
#elif (0x02 == 0xFF) /* Vendor specific device */
    0x00, /* 5 bDeviceSubClass : none */
    0x00, /* 6 bDeviceProtocol : none */
#endif
    0x40, /* 7 bMaxPacketSize0 */
    (uint8_t)(0x045B), /* 8 idVendor */
    (uint8_t)(0x045B >> 8), /* 9 idVendor */
    (uint8_t)(0x0000), /* 10 idProduct */
    (uint8_t)(0x0000 >> 8), /* 11 idProduct */
    (uint8_t)(0x0000), /* 12 bcdDevice */
    (uint8_t)(0x0000 >> 8), /* 13 bcdDevice */
    0x00, /* 14 iManufacturer */
    0x00, /* 15 iProduct */
    0x00, /* 16 iSerialNumber */
    0x01, /* 17 bNumConfigurations */
    /****************************************************************
     * Device qualifier descriptor                                  *
     ****************************************************************/
    0x0a, /* 0 bLength */
    UX_DEVICE_QUALIFIER_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bcdUSB BCD(2.0) */
    0x02, /* 3 bcdUSB */
    0x02, /* 4 bDeviceClass    : Device Class */
#if (0x02 == 0x00) || (0x02 == 0x02) || (0x02 == 0x03) || (0x02 == 0x08)
    0x00, /* 5 bDeviceSubClass : none */
    0x00, /* 6 bDeviceProtocol : none */
#elif (0x02 == 0xEF)
    0x02, /* 5 bDeviceSubClass : Common Class(0x02) */
    0x00, /* 6 bDeviceProtocol : none */
#elif (0x02 == 0xFF)
    0x00, /* 5 bDeviceSubClass : none */
    0x00, /* 6 bDeviceProtocol : none */
#endif
    0x40, /* 7 bMaxPacketSize0 */
    0x01, /* 8 bNumConfigs : 1 */
    0x00, /* 9 Reserve, fixed to 0 */
};
#endif

/***********************************************************************************************************************
 * Configuration Value Auto-calculation for USB Configuration Descriptor
 **********************************************************************************************************************/
#ifndef USB_IFD_NUM_0
#define USB_IFD_NUM_0  (0)
#define USB_IFD_SIZE_0 (0)
#endif
#ifndef USB_IFD_NUM_1
#define USB_IFD_NUM_1  (0)
#define USB_IFD_SIZE_1 (0)
#endif
#ifndef USB_IFD_NUM_2
#define USB_IFD_NUM_2  (0)
#define USB_IFD_SIZE_2 (0)
#endif
#ifndef USB_IFD_NUM_3
#define USB_IFD_NUM_3  (0)
#define USB_IFD_SIZE_3 (0)
#endif
#ifndef USB_IFD_NUM_4
#define USB_IFD_NUM_4  (0)
#define USB_IFD_SIZE_4 (0)
#endif
#ifndef USB_IFD_NUM_5
#define USB_IFD_NUM_5  (0)
#define USB_IFD_SIZE_5 (0)
#endif
#ifndef USB_IFD_NUM_6
#define USB_IFD_NUM_6  (0)
#define USB_IFD_SIZE_6 (0)
#endif
#ifndef USB_IFD_NUM_7
#define USB_IFD_NUM_7  (0)
#define USB_IFD_SIZE_7 (0)
#endif
#ifndef USB_IFD_NUM_8
#define USB_IFD_NUM_8  (0)
#define USB_IFD_SIZE_8 (0)
#endif
#ifndef USB_IFD_NUM_9
#define USB_IFD_NUM_9  (0)
#define USB_IFD_SIZE_9 (0)
#endif

#define USB_NUMBER_OF_INTERFACE USB_IFD_NUM_0 + USB_IFD_NUM_1 + USB_IFD_NUM_2 + USB_IFD_NUM_3 + USB_IFD_NUM_4 + USB_IFD_NUM_5 + USB_IFD_NUM_6 + USB_IFD_NUM_7 + USB_IFD_NUM_8 + USB_IFD_NUM_9
#define USB_TOTAL_LENGTH        USB_IFD_SIZE_0 + USB_IFD_SIZE_1 + USB_IFD_SIZE_2 + USB_IFD_SIZE_3 + USB_IFD_SIZE_4 + USB_IFD_SIZE_5 + USB_IFD_SIZE_6 + USB_IFD_SIZE_7 + USB_IFD_SIZE_8 + USB_IFD_SIZE_9

#if (0 != 0x00)
#define USB_CFG_DESCRIPTOR_bNumInterfaces     (0x00)
#else
#define USB_CFG_DESCRIPTOR_bNumInterfaces     (USB_NUMBER_OF_INTERFACE)
#endif
#if (0 != 0x00)
#define USB_CFG_DESCRIPTOR_wTotalLength       (0x00)
#else
#define USB_CFG_DESCRIPTOR_wTotalLength       (0x09 + USB_TOTAL_LENGTH)
#endif

/***********************************************************************************************************************
 * Consolidated USB Device Descriptor Framework on RAM for IAR build
 **********************************************************************************************************************/
#ifdef __ICCARM__
/* Memory area to complete USB device Descriptor Framework. */
static uint8_t device_framework_full_speed_ram[0x12 + USB_CFG_DESCRIPTOR_wTotalLength];
#if defined(UX_DCD_SYNERY_USE_USBHS)
static uint8_t device_framework_high_speed_ram[0x12 + 0x0a + USB_CFG_DESCRIPTOR_wTotalLength];
#endif
#endif

/***********************************************************************************************************************
 * USB Configuration Descriptor for FS mode
 **********************************************************************************************************************/
#if defined(__GNUC__)
static volatile const unsigned char ux_cfg_descriptor_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_config_desc_fs") BSP_ALIGN_VARIABLE_V2(1)
#else /* __ICCARM__ */
#pragma section = ".usb_config_desc_fs" 1
__root static const unsigned char ux_cfg_descriptor_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_config_desc_fs")
#endif
=
{
    0x09, /* 0 bLength */
    UX_CONFIGURATION_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (uint8_t)(USB_CFG_DESCRIPTOR_wTotalLength), /* 2 wTotalLength : This will be calculated at run-time. */
    (uint8_t)(USB_CFG_DESCRIPTOR_wTotalLength >> 8), /* 3 wTotalLength : This will be calculated at run-time. */
    (uint8_t)(USB_CFG_DESCRIPTOR_bNumInterfaces), /* 4 bNumInterfaces */
    0x01, /* 5 bConfigurationValue : Fixed to 1 since only one configuration is supported. */
    0x00, /* 6 iConfiguration */
    0x80 | (1 << 6) | (0 << 5), /* 7 bmAttributes */
    50, /* 8 bMaxPower */
};

#if defined(UX_DCD_SYNERY_USE_USBHS)
/***********************************************************************************************************************
 * USB Configuration Descriptor for HS mode
 **********************************************************************************************************************/
#if defined(__GNUC__)
static volatile const unsigned char ux_cfg_descriptor_high_speed[] BSP_PLACE_IN_SECTION_V2(".usb_config_desc_hs") BSP_ALIGN_VARIABLE_V2(1)
#else /* __ICCARM__ */
#pragma section = ".usb_config_desc_hs" 1
__root static const unsigned char ux_cfg_descriptor_high_speed[] BSP_PLACE_IN_SECTION_V2(".usb_config_desc_hs")
#endif
=
{
    0x09, /* 0 bLength */
    UX_CONFIGURATION_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    (uint8_t)(USB_CFG_DESCRIPTOR_wTotalLength), /* 2 wTotalLength : This will be calculated at run-time. */
    (uint8_t)(USB_CFG_DESCRIPTOR_wTotalLength >> 8), /* 3 wTotalLength : This will be calculated at run-time. */
    (uint8_t)(USB_CFG_DESCRIPTOR_bNumInterfaces), /* 4 bNumInterfaces */
    0x01, /* 5 bConfigurationValue : Fixed to 1 since only one configuration is supported. */
    0x00, /* 6 iConfiguration */
    0x80 | (1 << 6) | (0 << 5), /* 7 bmAttributes */
    50, /* 8 bMaxPower */
};
#endif

/***********************************************************************************************************************
 * USB Language Framework (Default setting for US English)
 **********************************************************************************************************************/
#if(0 == 0) /* Create USB Language Framework with default setting if not defined by user. */
static volatile const UCHAR language_id_framework_default[] =
{ (uint8_t) (0x0409), /* Supported Language Code */
  (uint8_t) (0x0409 >> 8) /* US English as the default */
};
#endif

/***********************************************************************************************************************
 * Function to get size of USB String Descriptor
 **********************************************************************************************************************/
#if (0)
extern const UCHAR NULL[];
static ULONG ux_device_string_descriptor_size_get(void)
{
    ULONG size = 0;
    UCHAR * ptr = (UCHAR *)NULL;
    if(NULL != ptr)
    {
        for(INT i = 0; i < 0; i++)
        {
            ptr = ptr + 3; /* bLength at byte offset 3 */
            /* Counts bLength + Language code(2bytes) + bLength(1byte) */
            size = size + *ptr + 4;
            ptr = ptr + (*ptr) + 1;
        }
    }
    return size;
}
#endif

#if (0)
extern const UCHAR NULL[];
#endif
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void ux_device_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void ux_device_err_callback_internal(void *p_instance, void *p_data);
void ux_device_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

#ifdef __ICCARM__

/*******************************************************************************************************************//**
 * @brief      This function is called during USBX device initialization to remove any padding inserted by the compiler.
 **********************************************************************************************************************/
void ux_device_remove_compiler_padding(unsigned char * p_device_framework, UINT length)
{
    INT remaining, count = 0;
    unsigned char *index, *temp;
    remaining = length;
    index = p_device_framework;

    /* Parse the device framework and remove any padding inserted  by the compiler.  */
    while(index < (p_device_framework + length))

    {
        temp = index;

        if(*temp == 0)
        {
            /* Remove, if any padding inserted by the compiler. */
            count = 0;
            while( (*temp == 0 ) && (temp < (p_device_framework + length)) )
            {
                temp++;
                count++;
            }

            remaining -= count;
            if(remaining > 0)
            {
                memcpy(index, temp, remaining);
                count = 0;
            }

            else
            {
                index = p_device_framework + length;
            }
        }

        else
        {
            /* Jump to next offset of the descriptors. */
            remaining = remaining - (*index);
            index = index + (*index);

        }
    }
}

#endif
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_device_init0(void)
 **********************************************************************************************************************/
void ux_device_init0(void)
{

    UINT status_ux_device_init;

    /** Calculate the size of USBX String Framework. */
    ULONG string_framework_size;
    UCHAR *p_string_framework;

#if(0) /* Check if the number of index in USBX String Framework is greater than zero. */
    {
        p_string_framework = (UCHAR *)NULL;
        string_framework_size = ux_device_string_descriptor_size_get();
    }
#else
    {
        p_string_framework = NULL;
        string_framework_size = 0;
    }
#endif

    /** Calculate the size of USB Language Framework. */
    ULONG language_framework_size;
    UCHAR *p_language_framework;

#if(0) /* Check if the number of index in USB Language Framework is greater than zero. */
    {
        p_language_framework = (UCHAR *)NULL;
        language_framework_size = 0 * 2;
    }
#else
    {
        p_language_framework = (UCHAR *) language_id_framework_default;
        language_framework_size = 2;
    }
#endif

    /** Initialize the USB Device stack. */
#ifdef __GNUC__
    status_ux_device_init = ux_device_stack_initialize(
#if defined(UX_DCD_SYNERY_USE_USBHS)
            (UCHAR *)device_framework_high_speed, ((uint32_t)&__usb_descriptor_end_hs - (uint32_t)&__usb_dev_descriptor_start_hs),
#else /* If USB DCD runs in FS mode, USB Device Descriptor for HS mode is not required. */
            NULL, 0,
#endif
            (UCHAR *)device_framework_full_speed, ((uint32_t)&__usb_descriptor_end_fs - (uint32_t)&__usb_dev_descriptor_start_fs),
            p_string_framework, string_framework_size,
            p_language_framework, language_framework_size, NULL);
#else /* __ICCARM__ */

    uint8_t *p_device_framework;
    unsigned char *start, *end, interface_desc_fs_hs[0x12 + 0x0a + USB_CFG_DESCRIPTOR_wTotalLength + 0x14];
    UINT length = 0;
#if defined(UX_DCD_SYNERY_USE_USBHS)
    start = (unsigned char *)(__section_begin (".usb_interface_desc_hs"));
    end = (unsigned char *)(__section_end (".usb_interface_desc_hs"));
    length = abs(end - start);
    memset(interface_desc_fs_hs, 0, sizeof(interface_desc_fs_hs));
    p_device_framework = (uint8_t *)interface_desc_fs_hs;
    /* Creates Device Descriptor Frameworks copying Descriptors from ROM. */
    memcpy (p_device_framework, device_framework_high_speed, sizeof(device_framework_high_speed));
    p_device_framework = p_device_framework + sizeof(device_framework_high_speed);
    /* Copy Configuration Descriptor */
    memcpy (p_device_framework, ux_cfg_descriptor_high_speed, sizeof(ux_cfg_descriptor_high_speed));
    p_device_framework = p_device_framework + sizeof(ux_cfg_descriptor_high_speed);
    /* Copy Interface Descriptors */
    memcpy (p_device_framework, (uint8_t *) __section_begin (".usb_interface_desc_hs"), length);
    ux_device_remove_compiler_padding(p_device_framework, length);
    /* Copy device frame work descriptor after removal of padding. */
    memcpy(device_framework_high_speed_ram, interface_desc_fs_hs, sizeof(device_framework_high_speed_ram));
#endif

    start = (unsigned char *) (__section_begin (".usb_interface_desc_fs"));
    end = (unsigned char *) (__section_end (".usb_interface_desc_fs"));
    length = abs (end - start);
    memset (interface_desc_fs_hs, 0, sizeof(interface_desc_fs_hs));
    p_device_framework = (uint8_t *) interface_desc_fs_hs;
    /* Creates Device Descriptor Frameworks copying Descriptors from ROM. */
    memcpy (p_device_framework, device_framework_full_speed, sizeof(device_framework_full_speed));
    p_device_framework = p_device_framework + sizeof(device_framework_full_speed);
    /* Copy Configuration Descriptor */
    memcpy (p_device_framework, ux_cfg_descriptor_full_speed, sizeof(ux_cfg_descriptor_full_speed));
    p_device_framework = p_device_framework + sizeof(ux_cfg_descriptor_full_speed);
    /* Copy Interface Descriptors */
    memcpy (p_device_framework, (uint8_t *) __section_begin (".usb_interface_desc_fs"), length);
    ux_device_remove_compiler_padding (p_device_framework, length);
    /* Copy device framework descriptor after removal of padding. */
    memcpy (device_framework_full_speed_ram, interface_desc_fs_hs, sizeof(device_framework_full_speed_ram));

    status_ux_device_init = ux_device_stack_initialize (
#if defined(UX_DCD_SYNERY_USE_USBHS)
            (UCHAR *)device_framework_high_speed_ram, (0x12 + 0x0a + USB_CFG_DESCRIPTOR_wTotalLength),
#else /* If USB DCD runs in FS mode, USB Device Descriptor for HS mode is not required. */
            NULL,
            0,
#endif
            (UCHAR *) device_framework_full_speed_ram,
            (0x12 + USB_CFG_DESCRIPTOR_wTotalLength), p_string_framework, string_framework_size, p_language_framework,
            language_framework_size, NULL);
#endif
    if (UX_SUCCESS != status_ux_device_init)
    {
        ux_device_err_callback (NULL, &status_ux_device_init);
    }

}
/***********************************************************************************************************************
 * Function prototypes for USBX CDC-ACM
 **********************************************************************************************************************/
static void g_ux_device_class_cdc_acm1_setup(void);

/***********************************************************************************************************************
 * USBX CDC-ACM Parameter Setup Function.
 **********************************************************************************************************************/
static UX_SLAVE_CLASS_CDC_ACM_PARAMETER g_ux_device_class_cdc_acm1_parameter;
void g_ux_device_class_cdc_acm1_setup(void)
{
    g_ux_device_class_cdc_acm1_parameter.ux_slave_class_cdc_acm_instance_activate = ux_cdc_device1_instance_activate;
    g_ux_device_class_cdc_acm1_parameter.ux_slave_class_cdc_acm_instance_deactivate =
            ux_cdc_device1_instance_deactivate;
    /* Initializes the device cdc class. */
    ux_device_stack_class_register (_ux_system_slave_class_cdc_acm_name, ux_device_class_cdc_acm_entry, 1, 0x00,
                                    (VOID *) &g_ux_device_class_cdc_acm1_parameter);

    /* Counts up the number of Interfaces. */
    g_usbx_number_of_interface_count = (uint8_t) (g_usbx_number_of_interface_count + 2);
}
/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_device_class_cdc_acm_init1(void)
 **********************************************************************************************************************/
void ux_device_class_cdc_acm_init1(void)
{
    /* Setups USB CDC-ACM Class to create the instance */
    g_ux_device_class_cdc_acm1_setup ();

    /* USB DCD initialization (Only executed once after all the class registration completed). */
    if (g_usbx_number_of_interface_count == USB_NUMBER_OF_INTERFACE)
    {
        /* Initializes USBX Device Control Driver */
        ux_dcd_initialize ();
    }
}
/*******************************************************************************************************************//**
 * @brief     Initialize the USB Device stack and register the class into slave stack
 **********************************************************************************************************************/
void g_ux_device_class_cdc_acm1_ux_device_open_init(void)
{
    /* Initialize the USB Device stack */
    ux_device_init0 ();

    /* Register the class into slave stack */
    ux_device_class_cdc_acm_init1 ();
}
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
                              1);
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
/* A pointer to FileX Media Control Block for a USB flash device. */
FX_MEDIA *g_fx_media1_ptr;

/* Pointer to a USBX Host Mass Storage Class Media. */
UX_HOST_CLASS_STORAGE_MEDIA *g_ux_host_class_storage0_media;

#if defined(__ICCARM__)
#define g_fx_media1_err_callback_failed_to_get_fx_media_WEAK_ATTRIBUTE
#pragma weak g_fx_media1_err_callback_failed_to_get_fx_media  = g_fx_media1_err_callback_failed_to_get_fx_media_internal
#elif defined(__GNUC__)
#define g_fx_media1_err_callback_failed_to_get_fx_media_WEAK_ATTRIBUTE __attribute__ ((weak, alias("g_fx_media1_err_callback_failed_to_get_fx_media_internal")))
#endif
void g_fx_media1_err_callback_failed_to_get_fx_media(void *p_instance, void *p_data)
g_fx_media1_err_callback_failed_to_get_fx_media_WEAK_ATTRIBUTE;

/* Function prototype for the weak initialization error function. */
void g_fx_media1_err_callback_failed_to_get_fx_media_internal(void *p_instance, void *p_data);

/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user function
 *             with the prototype below.
 *             - void g_fx_media1_err_callback_failed_to_get_fx_media(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance  Not used.
 * @param[in]  p_data      Not used.
 **********************************************************************************************************************/
void g_fx_media1_err_callback_failed_to_get_fx_media_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED (p_instance);
    SSP_PARAMETER_NOT_USED (p_data);

    /** A FileX media control block for a USB mass storage device was not found. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

/*******************************************************************************************************************//**
 * @brief      This is the function to get g_fx_media1 for a USB Mass Storage device.
 **********************************************************************************************************************/
void fx_media_init_function1(void)
{
    /** Suspend the thread until a USB Mass Storage Device inserted. */
    UINT err_g_fx_media1_semaphore_get = tx_semaphore_get (&ux_host_storage_semaphore_insertion, 1000);
    if (TX_SUCCESS != err_g_fx_media1_semaphore_get)
    {
        g_fx_media1_err_callback_failed_to_get_fx_media (&ux_host_storage_semaphore_insertion,
                                                         &err_g_fx_media1_semaphore_get);
    }

    /** Initialize the FileX Media Control Block g_fx_media1 for a USB Mass Storage device. */
    /** Get the pointer to a USBX Host Mass Storage Instance. */
    g_ux_host_class_storage0 = g_ux_new_host_storage_instance;

    /** Initialize the FileX Media Control Block g_fx_media0 for a USB Mass Storage device. */
    UINT err_g_fx_media1_get = ux_system_host_storage_fx_media_get (g_ux_new_host_storage_instance,
                                                                    &g_ux_host_class_storage0_media, &g_fx_media1_ptr);
    if (UX_SUCCESS != err_g_fx_media1_get)
    {
        g_fx_media1_err_callback_failed_to_get_fx_media (&g_fx_media1_ptr, &err_g_fx_media1_get);
    }
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
    ux_common_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    ux_host_init0 ();
#endif

    /** Call initialization function if user has selected to do so. */
#if (1)
    ux_device_init0 ();
#endif
    /** Call initialization function if user has selected to do so. */
#if (1)
    ux_device_class_cdc_acm_init1 ();
#endif
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
#if (0)
    /** Call FileX Media Control Block initialization function. Can call at later time as well. */
    fx_media_init_function1();
#endif
    /** Call initialization function if user has selected to do so. */
#if SF_EL_FX_AUTO_INIT_g_fx_media0
    fx_media_init0();
#endif
}
