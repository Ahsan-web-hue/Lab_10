/*
 * Timer_HAL.h
 *
 *  Created on: 14-Jul-2026
 *      Author: ABU MUSA TRADERS
 */

#ifndef INC_TIMER_HAL_H_
#define INC_TIMER_HAL_H_

#include "main.h"
#include "tim.h"
#include <stdint.h>

#define TIME_COUNT 4

class Time
{
public:
    Time(TIM_TypeDef *Instance, TIM_HandleTypeDef *htim);
    virtual ~Time();

    virtual HAL_StatusTypeDef start(void);
    virtual HAL_StatusTypeDef stop(void);

    uint32_t read(void);
    virtual void reset(void);

protected:
    TIM_TypeDef *_Instance;
    TIM_HandleTypeDef *_htim;

private:
    static bool is_init[TIME_COUNT];
    void init(void);
};

#endif /* INC_TIMER_HAL_H_ */
