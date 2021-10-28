#include "Management.h"
#include <Titan.h>
#include <math.h>

#define PRINTF 0

///The Management thread is responsible for maintaining the build environment and related hardware.
/// This includes the automatic tool changer, heated build plate, air handler, etc...

/* Management entry function */
void Management_entry(void)
{
    int i;
    int debtmp = 0;
    adc_data_size_t adc_data;
    double voltage;
    double tmpTemp, temp;
    double tempRead;
    int ThermistorPin = 0;
    int Vo;
    double T, buildPlateT;
    ioport_level_t level;
    ssp_err_t err;

    double tempSet = 260.0;

    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }

    err = g_timer6.p_api->open (g_timer6.p_ctrl, g_timer6.p_cfg);
    err = g_timer6.p_api->start(g_timer6.p_ctrl);
    err = g_timer6.p_api->periodSet (g_timer6.p_ctrl, 1, TIMER_UNIT_PERIOD_SEC);
    err = g_timer6.p_api->dutyCycleSet (g_timer6.p_ctrl, 20, TIMER_PWM_UNIT_PERCENT, 0);

    UINT status;
    ///ADC setup goes here
    /// Initialize ADC Driver
    status = g_adc0.p_api->open (g_adc0.p_ctrl, g_adc0.p_cfg);
    if (status == SSP_SUCCESS)
    {
        if (1)
            printf ("\nadc0 open success.");
    }

    //Configure ADC Channel
    status = g_adc0.p_api->scanCfg (g_adc0.p_ctrl, g_adc0.p_channel_cfg);
    if (status == SSP_SUCCESS)
    {
        if (1)
            printf ("\nadc0 scanCfg success.");
    }

    //Start the ADC scan
    status = g_adc0.p_api->scanStart (g_adc0.p_ctrl);
    if (status == SSP_SUCCESS)
    {
        if (1)
            printf ("\nadc0 scan start success.");
    }

    temp = 0;

    ///The Management thread will be responsible for the build plate, enclosure, and tool changer.
    while (1)
    {

        g_adc0.p_api->read (g_adc0.p_ctrl, ADC_REG_CHANNEL_0, &adc_data);
        voltage = 3.3;
        voltage = voltage / 4095.0;
        voltage *= adc_data;

        ///Extruder
        if (voltage < 3.3)
        {
            T = (voltage - 1.25) / 0.005;

            if (PRINTF)
            {

                printf ("\nTemperature Set: %f", tempSet);

                printf ("\nTemperature Read: %f", T);
                printf ("\nVoltage Read: %f", voltage);
            }

            if (T < tempSet && T > 20.0)
            {
                err = g_ioport.p_api->pinWrite (IOPORT_PORT_04_PIN_15, IOPORT_LEVEL_LOW); //heater
                g_timer6.p_api->dutyCycleSet (g_timer6.p_ctrl, 20, TIMER_PWM_UNIT_PERCENT, 0);
                //    err = g_ioport.p_api->pinWrite (IOPORT_PORT_03_PIN_14, IOPORT_LEVEL_LOW);
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //fan
                if (PRINTF)
                {

                    printf ("\nHeating...");
                }
            }
            else if (T > tempSet)
            {
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_04_PIN_15, IOPORT_LEVEL_HIGH); //heater

                g_timer6.p_api->dutyCycleSet (g_timer6.p_ctrl, 90, TIMER_PWM_UNIT_PERCENT, 0);

                //    err = g_ioport.p_api->pinWrite (IOPORT_PORT_03_PIN_14, IOPORT_LEVEL_LOW);
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //fan
                if (PRINTF)
                {
                    printf ("\nCooling...");
                }
            }
            else
            {
                ///Temperature out of range. Shut down.
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_04_PIN_15, IOPORT_LEVEL_HIGH);
                g_timer6.p_api->dutyCycleSet (g_timer6.p_ctrl, 90, TIMER_PWM_UNIT_PERCENT, 0);
            }
        }
        else
        {
//            printf ("\nVoltage out of range. Cooling...");
//            err = g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_04, IOPORT_LEVEL_LOW);
            g_timer6.p_api->dutyCycleSet (g_timer6.p_ctrl, 90, TIMER_PWM_UNIT_PERCENT, 0);
        }

        ///Build Plate
//        g_adc0.p_api->read (g_adc0.p_ctrl, ADC_REG_CHANNEL_2, &adc_data);
//        voltage = 3.3;
//        voltage = voltage / 4095.0;
//        voltage *= adc_data;
//
//        if (voltage < 3.3)
//        {
//            buildPlateT = (voltage - 1.25) / 0.005;
//
//            if (PRINTF)
//            {
//
//                printf ("\nBed Set: %f", bedTempSet);
//
//                printf ("\nBed Read: %f", buildPlateT);
//                printf ("\nVoltage Read: %f", voltage);
//            }
//
//            if (buildPlateT < bedTempSet)
//            {
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_03, IOPORT_LEVEL_HIGH); //heater
//
//                //    err = g_ioport.p_api->pinWrite (IOPORT_PORT_03_PIN_14, IOPORT_LEVEL_LOW);
//                //                err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //fan
//                if (PRINTF)
//                {
//
//                    printf ("\nHeating...");
//                }
//            }
//            else if (buildPlateT > bedTempSet)
//            {
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_03, IOPORT_LEVEL_LOW); //heater
//
//                //    err = g_ioport.p_api->pinWrite (IOPORT_PORT_03_PIN_14, IOPORT_LEVEL_LOW);
//                //                err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //fan
//                if (PRINTF)
//                {
//                    printf ("\nCooling...");
//                }
//            }
//        }
//        else
//        {
//            //            printf ("\nVoltage out of range. Cooling...");
//            err = g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_03, IOPORT_LEVEL_LOW);
//        }

        tx_thread_sleep (1000);
//        tx_thread_suspend (tx_thread_identify ());
    }
}
