///*
// * ADC_HAL.h
// *
// *  Created on: 11-Jul-2026
// *      Author: ABU MUSA TRADERS
// */
//
//#ifndef INC_ADC_HAL_H_
//#define INC_ADC_HAL_H_
//
//#include "main.h"
//#include "adc.h"
//#include <vector>
//#include "ISR_List.h"
//#define ADC_COUNT 1
//
//class ADC
//{
//public:
//    ADC(ADC_TypeDef *Instance, ADC_HandleTypeDef *hadc);
//    ~ADC();
//
//    void add_channels(uint32_t channel, uint32_t SampleTime);
//    uint16_t read(uint8_t channel, uint16_t timeout = 1000);
//
//private:
//    ADC_HandleTypeDef *_hadc;
//    ADC_TypeDef *_instance;
//
//    static bool is_init[ADC_COUNT];
//
//    void init();
//    HAL_StatusTypeDef set_channel(uint8_t channel);
//
//    std::vector<ADC_ChannelConfTypeDef> channels;
//};
/////ADC DMA START FORM HERE
//
////class ADC_DMA{
////public:
////	ADC_DMA(ADC_TypeDef *Instance,ADC_HandleTypeDef *hadc,uint16_t samples_per_channel);
////	~ADC_DMA(void);
////
////	HAL_StatusTypeDef start(void);
////	HAL_StatusTypeDef stop(void);
////
////
////private:
////	static ISR<ADC_DMA> ISR_List;
////	uint16_t _sample_per_channel;
////	uint16_t *_samples;
////	uint16_t _buffer_length;
////
////
////
////};
//
//
//#endif /* INC_ADC_HAL_H_ */
/*
 * ADC_HAL.h
 *
 *  Created on: 11-Jul-2026
 *      Author: ABU MUSA TRADERS
 */

#ifndef INC_ADC_HAL_H_
#define INC_ADC_HAL_H_

#include "main.h"
#include "adc.h"
#include <vector>
#include "ISR_List.h"
#include"CircularBuffer.h"
#include<stdlib.h>

#define ADC_COUNT 1

class ADC
{
public:
    ADC(ADC_TypeDef *Instance, ADC_HandleTypeDef *hadc);
    virtual ~ADC();

    void add_channels(uint32_t channel, uint32_t SampleTime);
    uint16_t read(uint8_t channel, uint16_t timeout = 1000);

protected:                      // <-- Changed from private
    ADC_HandleTypeDef *_hadc;
    ADC_TypeDef *_instance;

    std::vector<ADC_ChannelConfTypeDef> channels;

private:
    static bool is_init[ADC_COUNT];

    void init();
    HAL_StatusTypeDef set_channel(uint8_t channel);
};

/*==================== ADC DMA ====================*/

class ADC_DMA : public ADC
{
public:
    ADC_DMA(ADC_TypeDef *Instance,
            ADC_HandleTypeDef *hadc,
            uint16_t samples_per_channel);

    virtual ~ADC_DMA();

    HAL_StatusTypeDef start(void);
    HAL_StatusTypeDef stop(void);
    HAL_StatusTypeDef add_channels(uint8_t channels);
    HAL_StatusTypeDef read(uint8_t channel,uint16_t *value);

private:
    static ISR<ADC_DMA> ISR_List;

    uint16_t _samples_per_channel;
    uint16_t *_samples;
    uint32_t _buffer_length;
    uint8_t _channel_count;
    HAL_StatusTypeDef add_channel(void);
    std::vector<CircularBuffer<uint16_t>*> _channel_data;

    static void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
    static void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);

    void copy_half(void);
    void copy_full(void);

};

#endif /* INC_ADC_HAL_H_ */
