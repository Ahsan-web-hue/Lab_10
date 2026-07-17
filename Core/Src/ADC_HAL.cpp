/*
 * ADC_HAL.cpp
 *
 *  Created on: 11-Jul-2026
 *      Author: ABU MUSA TRADERS
 */

#include "ADC_HAL.h"
ISR<ADC_DMA> ADC_DMA::ISR_List;

bool ADC::is_init[ADC_COUNT] = {false};


ADC::ADC(ADC_TypeDef *Instance, ADC_HandleTypeDef *hadc)
    : _hadc(hadc), _instance(Instance)
{
    init();
}

ADC::~ADC()
{
    channels.clear();
}

void ADC::init()
{
    if (_instance == ADC1)
    {
        if (is_init[0])
            return;

        is_init[0] = true;

        MX_ADC1_Init();
    }
}

void ADC::add_channels(uint32_t channel, uint32_t SampleTime)
{
    ADC_ChannelConfTypeDef sConfig;

    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = SampleTime;

    channels.push_back(sConfig);
}

HAL_StatusTypeDef ADC::set_channel(uint8_t channel)
{
    if (channel >= channels.size())
    {
        return HAL_ERROR;
    }

    return HAL_ADC_ConfigChannel(_hadc, &channels[channel]);
}

uint16_t ADC::read(uint8_t channel, uint16_t timeout)
{
    uint16_t value = 0;

    HAL_StatusTypeDef ret = set_channel(channel);

    if (ret != HAL_OK)
    {
        return value;
    }

    HAL_ADC_Start(_hadc);

    ret = HAL_ADC_PollForConversion(_hadc, timeout);

    if (ret == HAL_OK)
    {
        value = HAL_ADC_GetValue(_hadc);
    }

    HAL_ADC_Stop(_hadc);

    return value;
}



ADC_DMA::ADC_DMA(ADC_TypeDef *Instance,
                 ADC_HandleTypeDef *hadc,
                 uint16_t samples_per_channel)
    : ADC(Instance, hadc),
      _samples_per_channel(samples_per_channel),
      _samples(nullptr),
      _buffer_length(0),
	  _channel_count(0)
{
#if USE_HAL_REGISTER_CALLBACKS!=1
//#error""
#endif
	_hadc->ConvCpltCallback=HAL_ADC_ConvCpltCallback;
	_hadc->ConvHalfCpltCallback=HAL_ADC_ConvHalfCpltCallback;

	_channel_data.clear();
    ISR_List.add(this);
}
ADC_DMA::~ADC_DMA()
{
	stop();
    ISR_List.remove(this);

    if (_samples != NULL)
    {
        free(_samples);
    }
    for(uint16_t i=0;i<_channel_data.size();i++){
    	if(_channel_data[i]==NULL){
    		continue;
    	}
    	delete _channel_data[i];
    }
}
HAL_StatusTypeDef ADC_DMA::start(void)
{
    if (_samples == nullptr){
        return HAL_ERROR;
    }
    if(_samples==0){
    	return HAL_ERROR;
    }

    return HAL_ADC_Start_DMA(
        _hadc,
        (uint32_t *)_samples,
        _buffer_length);
}

HAL_StatusTypeDef ADC_DMA::stop(void)
{
    return HAL_ADC_Stop_DMA(_hadc);
}

HAL_StatusTypeDef ADC_DMA::read(uint8_t channel,uint16_t *value){
	if(_channel_data[channel]->count()==0){
  		return HAL_ERROR;
	}
	_channel_data[channel]->pull(value, 1);
	return HAL_OK;

}
HAL_StatusTypeDef ADC_DMA::add_channels(uint8_t channels){
	HAL_StatusTypeDef ret = HAL_ERROR;
	for(uint16_t i=0;i<_channel_data.size();i++){
		ret=add_channel();
		if(ret!=HAL_OK){
			return ret;
		}
	}
	return ret;
}
HAL_StatusTypeDef ADC_DMA::add_channel(void)
{
    // Increment channel count
    _channel_count++;

    // Total number of samples in DMA buffer
    _buffer_length = _samples_per_channel * _channel_count;

    // Reallocate memory for all samples
    uint16_t *samples = (uint16_t *)realloc(
        _samples,
        _buffer_length * sizeof(uint16_t));

    // Check allocation failure
    if (samples == nullptr)
    {
        _channel_count--;

        _buffer_length = _samples_per_channel * _channel_count;

        return HAL_ERROR;
    }

    // Save new buffer pointer
    _samples = samples;

    // Create circular buffer for this channel
    _channel_data.push_back(
        new CircularBuffer<uint16_t>(_samples_per_channel));

    return HAL_OK;
}

void ADC_DMA::HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    for (uint16_t i = 0; i < ISR_List.size(); i++)
    {
        ADC_DMA *handle = ISR_List.get(i);

        if (handle->_hadc == hadc)
        {
            handle->copy_full();
        }
    }
}
void ADC_DMA::HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    for (uint16_t i = 0; i < ISR_List.size(); i++)
    {
        ADC_DMA *handle = ISR_List.get(i);

        if (handle->_hadc == hadc)
        {
            handle->copy_half();
        }
    }
}

void ADC_DMA::copy_half(void)
{
    for (uint32_t i = 0; i < (_buffer_length / 2); i += _channel_data.size())
    {
        for (uint32_t j = 0; j < _channel_data.size(); j++)
        {
            _channel_data[j]->put(&_samples[i + j], 1);
        }
    }
}

void ADC_DMA::copy_full(void)
{
    for (uint32_t i = (_buffer_length / 2);
         i < _buffer_length;
         i += _channel_data.size())
    {
        for (uint32_t j = 0; j < _channel_data.size(); j++)
        {
            _channel_data[j]->put(&_samples[i + j], 1);
        }
    }
}
































