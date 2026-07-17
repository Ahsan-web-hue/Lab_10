/*
 * ADC_DMA.h
 *
 *  Created on: 12-Jul-2026
 *      Author: ABU MUSA TRADERS
 */

#ifndef INC_ADC_DMA_H_
#define INC_ADC_DMA_H_
#include"DMA_HAL.h"
#include"CircularBuffer.h"
class ADC_DMA{
public:
	ADC_DMA(ADC_TypeDef *Instance,ADC_HandleTypeDef *hadc);
	~ADC_DMA(void);
private:
	static ISR<ADC_DMA> ISR_List;


};



#endif /* INC_ADC_DMA_H_ */
