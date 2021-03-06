/*
    Logitech G25 controller on STM32F407
    Copyright (C) 2017, markjones112358.

    EMAIL: markjones112358@gmail.com

    This piece of code is FREE SOFTWARE and is released
    under the Apache License, Version 2.0 (the "License");
*/

/*
    USB-HID Gamepad for ChibiOS/RT
    Copyright (C) 2014, +inf Wenzheng Xu.

    EMAIL: wx330@nyu.edu

    This piece of code is FREE SOFTWARE and is released
    under the Apache License, Version 2.0 (the "License");
*/

/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#define ADC_GRP1_NUM_CHANNELS   3
#define ADC_GRP1_BUF_DEPTH      1

#include "ch.h"
#include "hal.h"
#include "adccfg.h"

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

// uint16_t data_brake;
// uint16_t data_angle_arr[STEER_AVG];
// uint16_t data_adjust;
// uint32_t adc_count;


/*
 * ADC streaming callback.
 */
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
  (void)adcp;
  (void)n;
  data_brake =  buffer[0] / 16;
  //data_angle =  buffer[1];
  data_angle_arr[adc_count % STEER_AVG] = buffer[1] * 32;
  data_adjust = 255 - (buffer[2] / 16);
  adc_count++;
}

static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;
}

/*
 * ADC conversion group.
 * Mode:        Continuous, 16 samples of 8 channels, SW triggered.
 * Channels:    IN11, IN12, IN11, IN12, IN11, IN12, Sensor, VRef.
 */
// static const ADCConversionGroup adcgrpcfg1 = {
//   TRUE,
//   ADC_GRP1_NUM_CHANNELS,
//   adccallback,
//   adcerrorcallback,
//   0,                        /* CR1 */
//   ADC_CR2_SWSTART,          /* CR2 */
//   ADC_SMPR1_SMP_AN12(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3),
//   0,                        /* SMPR2 */
//   ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
//   0,
//   ADC_SQR3_SQ6_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ5_N(ADC_CHANNEL_IN11) |
//   ADC_SQR3_SQ4_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN11) |
//   ADC_SQR3_SQ2_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)
// };

static const ADCConversionGroup adcgrpcfg1 = {
  TRUE,
  ADC_GRP1_NUM_CHANNELS,
  adccallback,
  adcerrorcallback,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN10(ADC_SAMPLE_3),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ3_N(ADC_CHANNEL_IN12) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11) | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)
};

void myADCinit(void){

   palSetGroupMode(GPIOC, PAL_PORT_BIT(0) | PAL_PORT_BIT(1) | PAL_PORT_BIT(2),0, PAL_MODE_INPUT_ANALOG);
  /*
   * Activates the ADC1 driver and the temperature sensor.
   */


   adcStart(&ADCD1, NULL);


  /*
   * Starts an ADC continuous conversion.
   */
   adcStartConversion(&ADCD1, &adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);

   chThdSleepMilliseconds(1000);
   adc_count = 0;
}
