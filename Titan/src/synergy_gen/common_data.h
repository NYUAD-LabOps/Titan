/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_dmac.h"
#include "r_transfer_api.h"
#include "r_sdmmc.h"
#include "r_sdmmc_api.h"
#include "sf_block_media_sdmmc.h"
#include "sf_block_media_api.h"
#include "sf_el_fx.h"
#include "fx_api.h"
#include "fx_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_fmi.h"
#include "r_fmi_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer5;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
/** SDMMC on SDMMC Instance. */
extern const sdmmc_instance_t g_sdmmc0;
#ifndef NULL
void NULL(sdmmc_callback_args_t *p_args);
#endif
/** Block Media on SDMMC Instance */
extern sf_block_media_instance_t g_sf_block_media_sdmmc0;
extern sf_el_fx_t g_sf_el_fx0_cfg;
#ifndef NULL
void NULL(sf_el_fx_callback_args_t *p_args);
#endif
void fx_common_init0(void);
extern FX_MEDIA g_fx_media0;

void g_fx_media0_err_callback(void *p_instance, void *p_data);
ssp_err_t fx_media_init0_format(void);
uint32_t fx_media_init0_open(void);
void fx_media_init0(void);
/** ELC Instance */
extern const elc_instance_t g_elc;
/** FMI on FMI Instance. */
extern const fmi_instance_t g_fmi;
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
void g_common_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMON_DATA_H_ */
