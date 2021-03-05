#include "Management.h"
#include <Titan.h>
#include <math.h>

#define THERMISTORPIN A5
// resistance at 25 degrees C
#define THERMISTORNOMINAL 100000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 80000

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
    double R1 = SERIESRESISTOR;
    double R2log, R2, T;
    double c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
    ioport_level_t level;
    ssp_err_t err;

    double tempSet = 100.0;

    while (machineGlobalsBlock->globalsInit != 1)
    {
        tx_thread_sleep (500);
    }

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

    double steinhart, Rtherm;

    ///The Management thread will be responsible for the build plate, enclosure, and tool chamber.
    while (1)
    {

        //    for (i = 0; i < 10; i++)
        //    {
        //        g_adc1.p_api->read (g_adc1.p_ctrl, ADC_REG_CHANNEL_2, &adc_data);
        //        voltage = 3.3;
        //        voltage = voltage / 4095.0;
        //        voltage *= adc_data;
        //        tmpTemp = (voltage - 1.25) / .005;
        //        temp += tmpTemp;
        //        //            printf ("\nADC102: %f", voltage);
        //        tx_thread_sleep (10);
        //    }

        g_adc0.p_api->read (g_adc0.p_ctrl, ADC_REG_CHANNEL_2, &adc_data);
        voltage = 3.3;
        voltage = voltage / 4095.0;
        voltage *= adc_data;

        if (voltage < 3.3)
        {

//            R2 = R1 * ((3.3 / voltage) - 1.0);
//
//            steinhart = R2 / THERMISTORNOMINAL;     // (R/Ro)
//            steinhart = log (steinhart);                  // ln(R/Ro)
//            steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
//            steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
//            steinhart = 1.0 / steinhart;                 // Invert
//            steinhart -= 273.15;
            T = (voltage - 1.25) / 0.005;
//        R2 = R1 * ((3.3 / voltage) - 1.0);
//        R2log = log (R2);
//        T = (1.0 / (c1 + c2 * R2log + c3 * R2log * R2log * R2log));
//        T = T - 273.15;
//        T = (T * 9.0) / 5.0 + 32.0;

//            tmpTemp = (voltage - 1.25) / .005;
//            tempRead = tmpTemp;

            //    temp += tmpTemp;

            //    toolBlock->tempRead = temp / 10.0;

            if (1)
            {
                if (1)
                    printf ("\nTemperature Set: %f", tempSet);
                if (1)
                    printf ("\nTemperature Read: %f", T);
                printf ("\nVoltage Read: %f", voltage);
            }

            if (T < tempSet)
            {
                err = g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_04, IOPORT_LEVEL_HIGH); //heater

                //    err = g_ioport.p_api->pinWrite (IOPORT_PORT_03_PIN_14, IOPORT_LEVEL_LOW);
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //fan
                if (1)
                {
                    if (1)
                        printf ("\nHeating...");
                }
            }
            else if (T > tempSet)
            {
                err = g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_04, IOPORT_LEVEL_LOW); //heater

                //    err = g_ioport.p_api->pinWrite (IOPORT_PORT_03_PIN_14, IOPORT_LEVEL_LOW);
//                err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_06, IOPORT_LEVEL_HIGH); //fan
                if (1)
                {
                    if (1)
                        printf ("\nCooling...");
                }
            }
        } else{
            printf ("\nVoltage out of range. Cooling...");
            err = g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_04, IOPORT_LEVEL_LOW);
        }
        tx_thread_sleep (500);
//        tx_thread_suspend (tx_thread_identify ());
    }
}
