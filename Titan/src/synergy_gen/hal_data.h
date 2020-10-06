/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;
#ifndef getTemp_callback
void getTemp_callback(timer_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* HAL_DATA_H_ */
