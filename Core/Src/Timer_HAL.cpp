/*
 * Timer_HAL.cpp
 *
 *  Created on: 14-Jul-2026
 *      Author: ABU MUSA TRADERS
 */

#include "Timer_HAL.h"

bool Time::is_init[TIME_COUNT] = {false};

Time::Time(TIM_TypeDef *Instance, TIM_HandleTypeDef *htim)
    : _Instance(Instance), _htim(htim)
{
    init();
}

Time::~Time()
{
    if (_Instance == TIM3)
    {
        is_init[2] = false;
        HAL_TIM_Base_MspDeInit(_htim);
    }
}

void Time::init(void)
{
    if (_Instance == TIM3)
    {
        if (is_init[2])
        {
            return;
        }

        is_init[2] = true;
        MX_TIM3_Init();
    }
}

HAL_StatusTypeDef Time::start(void)
{
    return HAL_TIM_Base_Start(_htim);
}

HAL_StatusTypeDef Time::stop(void)
{
    return HAL_TIM_Base_Stop(_htim);
}

void Time::reset(void)
{
    __HAL_TIM_SET_COUNTER(_htim, 0);
}

uint32_t Time::read(void)
{
    return __HAL_TIM_GET_COUNTER(_htim);
}
