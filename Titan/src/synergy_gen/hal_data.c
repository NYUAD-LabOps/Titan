/* generated HAL source file - do not edit */
#include "hal_data.h"
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_external_irqZ) && !defined(SSP_SUPPRESS_ISR_ICU8)
SSP_VECTOR_DEFINE(icu_irq_isr, ICU, IRQ8);
#endif
#endif
static icu_instance_ctrl_t g_external_irqZ_ctrl;
static const external_irq_cfg_t g_external_irqZ_cfg =
{ .channel = 8,
  .trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = ext_irqZ_callback,
  .p_context = &g_external_irqZ,
  .p_extend = NULL,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irqZ =
{ .p_ctrl = &g_external_irqZ_ctrl, .p_cfg = &g_external_irqZ_cfg, .p_api = &g_external_irq_on_icu };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_external_irqY) && !defined(SSP_SUPPRESS_ISR_ICU7)
SSP_VECTOR_DEFINE(icu_irq_isr, ICU, IRQ7);
#endif
#endif
static icu_instance_ctrl_t g_external_irqY_ctrl;
static const external_irq_cfg_t g_external_irqY_cfg =
{ .channel = 7,
  .trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = ext_irqY_callback,
  .p_context = &g_external_irqY,
  .p_extend = NULL,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irqY =
{ .p_ctrl = &g_external_irqY_ctrl, .p_cfg = &g_external_irqY_cfg, .p_api = &g_external_irq_on_icu };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_external_irqX) && !defined(SSP_SUPPRESS_ISR_ICU12)
SSP_VECTOR_DEFINE(icu_irq_isr, ICU, IRQ12);
#endif
#endif
static icu_instance_ctrl_t g_external_irqX_ctrl;
static const external_irq_cfg_t g_external_irqX_cfg =
{ .channel = 12,
  .trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .autostart = true,
  .p_callback = ext_irqX_callback,
  .p_context = &g_external_irqX,
  .p_extend = NULL,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irqX =
{ .p_ctrl = &g_external_irqX_ctrl, .p_cfg = &g_external_irqX_cfg, .p_api = &g_external_irq_on_icu };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timerT) && !defined(SSP_SUPPRESS_ISR_GPT7)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 7);
#endif
#endif
static gpt_instance_ctrl_t g_timerT_ctrl;
static const timer_on_gpt_cfg_t g_timerT_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timerT_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 7,
  .autostart = false,
  .p_callback = gpt_T_callback,
  .p_context = &g_timerT,
  .p_extend = &g_timerT_extend,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const timer_instance_t g_timerT =
{ .p_ctrl = &g_timerT_ctrl, .p_cfg = &g_timerT_cfg, .p_api = &g_timer_on_gpt };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timerC) && !defined(SSP_SUPPRESS_ISR_GPT5)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 5);
#endif
#endif
static gpt_instance_ctrl_t g_timerC_ctrl;
static const timer_on_gpt_cfg_t g_timerC_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timerC_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 5,
  .autostart = false,
  .p_callback = gpt_C_callback,
  .p_context = &g_timerC,
  .p_extend = &g_timerC_extend,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const timer_instance_t g_timerC =
{ .p_ctrl = &g_timerC_ctrl, .p_cfg = &g_timerC_cfg, .p_api = &g_timer_on_gpt };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timerA) && !defined(SSP_SUPPRESS_ISR_GPT4)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 4);
#endif
#endif
static gpt_instance_ctrl_t g_timerA_ctrl;
static const timer_on_gpt_cfg_t g_timerA_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timerA_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 4,
  .autostart = false,
  .p_callback = gpt_A_callback,
  .p_context = &g_timerA,
  .p_extend = &g_timerA_extend,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const timer_instance_t g_timerA =
{ .p_ctrl = &g_timerA_ctrl, .p_cfg = &g_timerA_cfg, .p_api = &g_timer_on_gpt };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timerZ) && !defined(SSP_SUPPRESS_ISR_GPT3)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 3);
#endif
#endif
static gpt_instance_ctrl_t g_timerZ_ctrl;
static const timer_on_gpt_cfg_t g_timerZ_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timerZ_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 3,
  .autostart = false,
  .p_callback = gpt_Z_callback,
  .p_context = &g_timerZ,
  .p_extend = &g_timerZ_extend,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const timer_instance_t g_timerZ =
{ .p_ctrl = &g_timerZ_ctrl, .p_cfg = &g_timerZ_cfg, .p_api = &g_timer_on_gpt };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timerY) && !defined(SSP_SUPPRESS_ISR_GPT2)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 2);
#endif
#endif
static gpt_instance_ctrl_t g_timerY_ctrl;
static const timer_on_gpt_cfg_t g_timerY_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timerY_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 2,
  .autostart = false,
  .p_callback = gpt_Y_callback,
  .p_context = &g_timerY,
  .p_extend = &g_timerY_extend,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const timer_instance_t g_timerY =
{ .p_ctrl = &g_timerY_ctrl, .p_cfg = &g_timerY_cfg, .p_api = &g_timer_on_gpt };
#if (0) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timerX) && !defined(SSP_SUPPRESS_ISR_GPT1)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 1);
#endif
#endif
static gpt_instance_ctrl_t g_timerX_ctrl;
static const timer_on_gpt_cfg_t g_timerX_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timerX_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  .period = 10,
  .unit = TIMER_UNIT_PERIOD_MSEC,
  .duty_cycle = 50,
  .duty_cycle_unit = TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 1,
  .autostart = false,
  .p_callback = gpt_X_callback,
  .p_context = &g_timerX,
  .p_extend = &g_timerX_extend,
  .irq_ipl = (0), };
/* Instance structure to use this module. */
const timer_instance_t g_timerX =
{ .p_ctrl = &g_timerX_ctrl, .p_cfg = &g_timerX_cfg, .p_api = &g_timer_on_gpt };
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timer6) && !defined(SSP_SUPPRESS_ISR_GPT6)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 6);
#endif
#endif
static gpt_instance_ctrl_t g_timer6_ctrl;
static const timer_on_gpt_cfg_t g_timer6_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timer6_cfg =
{ .mode = TIMER_MODE_PWM, .period = 1, .unit = TIMER_UNIT_PERIOD_SEC, .duty_cycle = 20, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 6, .autostart = false, .p_callback = NULL, .p_context = &g_timer6, .p_extend = &g_timer6_extend, .irq_ipl =
          (BSP_IRQ_DISABLED), };
/* Instance structure to use this module. */
const timer_instance_t g_timer6 =
{ .p_ctrl = &g_timer6_ctrl, .p_cfg = &g_timer6_cfg, .p_api = &g_timer_on_gpt };
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_adc0) && !defined(SSP_SUPPRESS_ISR_ADC0)
SSP_VECTOR_DEFINE_CHAN(adc_scan_end_isr, ADC, SCAN_END, 0);
#endif
#endif
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_adc0) && !defined(SSP_SUPPRESS_ISR_ADC0)
SSP_VECTOR_DEFINE_CHAN(adc_scan_end_b_isr, ADC, SCAN_END_B, 0);
#endif
#endif
adc_instance_ctrl_t g_adc0_ctrl;
const adc_cfg_t g_adc0_cfg =
{ .unit = 0,
  .mode = ADC_MODE_CONTINUOUS_SCAN,
  .resolution = ADC_RESOLUTION_12_BIT,
  .alignment = ADC_ALIGNMENT_RIGHT,
  .add_average_count = ADC_ADD_AVERAGE_FOUR,
  .clearing = ADC_CLEAR_AFTER_READ_ON,
  .trigger = ADC_TRIGGER_SOFTWARE,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC,
  .p_callback = NULL,
  .p_context = &g_adc0,
  .scan_end_ipl = (BSP_IRQ_DISABLED),
  .scan_end_b_ipl = (BSP_IRQ_DISABLED),
  .calib_adc_skip = false,
  .voltage_ref = ADC_EXTERNAL_VOLTAGE,
  .over_current = OVER_CURRENT_DETECTION_ENABLE,
  .pga0 = PGA_DISABLE,
  .pga1 = PGA_DISABLE,
  .pga2 = PGA_DISABLE, };
const adc_channel_cfg_t g_adc0_channel_cfg =
        { .scan_mask = (uint32_t) (
                ((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) ADC_MASK_CHANNEL_1) | ((uint64_t) ADC_MASK_CHANNEL_2)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) ADC_MASK_CHANNEL_6)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                        | ((uint64_t) 0) | ((uint64_t) 0) | (0)),
          /** Group B channel mask is right shifted by 32 at the end to form the proper mask */
          .scan_mask_group_b = (uint32_t) (
                  (((uint64_t) ADC_MASK_CHANNEL_0) | ((uint64_t) ADC_MASK_CHANNEL_1) | ((uint64_t) ADC_MASK_CHANNEL_2)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) ADC_MASK_CHANNEL_6)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0) | ((uint64_t) 0)
                          | ((uint64_t) 0) | ((uint64_t) 0) | (0)) >> 32),
          .priority_group_a = ADC_GROUP_A_PRIORITY_OFF, .add_mask = (uint32_t) (
                  (ADC_MASK_CHANNEL_0) | (0) | (ADC_MASK_CHANNEL_2) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)
                          | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0) | (0)
                          | (0) | (0) | (0) | (0)),
          .sample_hold_mask = (uint32_t) ((0) | (0) | (0)), .sample_hold_states = 24, };
/* Instance structure to use this module. */
const adc_instance_t g_adc0 =
{ .p_ctrl = &g_adc0_ctrl, .p_cfg = &g_adc0_cfg, .p_channel_cfg = &g_adc0_channel_cfg, .p_api = &g_adc_on_adc };
#if (BSP_IRQ_DISABLED) != BSP_IRQ_DISABLED
#if !defined(SSP_SUPPRESS_ISR_g_timer0) && !defined(SSP_SUPPRESS_ISR_GPT0)
SSP_VECTOR_DEFINE_CHAN(gpt_counter_overflow_isr, GPT, COUNTER_OVERFLOW, 0);
#endif
#endif
static gpt_instance_ctrl_t g_timer0_ctrl;
static const timer_on_gpt_cfg_t g_timer0_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .shortest_pwm_signal = GPT_SHORTEST_LEVEL_OFF, };
static const timer_cfg_t g_timer0_cfg =
{ .mode = TIMER_MODE_PWM, .period = 15, .unit = TIMER_UNIT_PERIOD_SEC, .duty_cycle = 33, .duty_cycle_unit =
          TIMER_PWM_UNIT_PERCENT,
  .channel = 0, .autostart = false, .p_callback = NULL, .p_context = &g_timer0, .p_extend = &g_timer0_extend, .irq_ipl =
          (BSP_IRQ_DISABLED), };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_gpt };
void g_hal_init(void)
{
    g_common_init ();
}
